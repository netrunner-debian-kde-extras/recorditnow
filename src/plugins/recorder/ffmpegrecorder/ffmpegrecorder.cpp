/***************************************************************************
 *   Copyright (C) 2010 by Kai Dombrowe <just89@gmx.de>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/


// own
#include "ffmpegrecorder.h"
#include <recorditnow_ffmpegrecorder.h>

// KDE
#include <kplugininfo.h>
#include <kprocess.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <kdebug.h>

// Qt
#include <QtGui/QWidget>
#include <QtCore/QDir>
#include <QtGui/QX11Info>

// C
#include <sys/types.h>
#include <signal.h>

// X
#include <X11/Xlib.h>


K_PLUGIN_FACTORY(myFactory, registerPlugin<FfmpegRecorder>();)
K_EXPORT_PLUGIN(myFactory("ffmpeg_recorder"))
FfmpegRecorder::FfmpegRecorder(QObject *parent, const QVariantList &args)
    : AbstractRecorder(parent, args)
{

    m_recorder = 0;
    m_paused = false;

}


FfmpegRecorder::~FfmpegRecorder()
{

    clean();

}


void FfmpegRecorder::record(const AbstractRecorder::Data &d)
{

    const QString ffmpeg = KGlobal::dirs()->findExe("ffmpeg");
    if (ffmpeg.isEmpty()) {
        emit error(i18n("Cannot find ffmpeg!\n"
                        "Please install ffmpeg or use another plugin."));
        return;
    }

    Settings::self()->readConfig();

    m_data = d;
    m_tmpFile = getTemporaryFile(d.workDir);

    QRect geometry = d.geometry;
    if (d.winId != -1) {
        XWindowAttributes attributes;
        if (!XGetWindowAttributes(QX11Info::display(), d.winId, &attributes)) {
            emit error(i18n("Invalid window!"));
            return;

        }
        geometry = QRect(attributes.x, attributes.y, attributes.width, attributes.height);
    }


    m_tmpFile = unique(m_tmpFile.append(QString(".%1").arg(Settings::format())));

    QStringList args;
    if (!Settings::useCustomCommand()) {
        // qscale must be > 0.0 and <= 255
        double videoQuality;

        if (Settings::videoQuality() <= 10) {
            videoQuality = 20;
        } else if (Settings::videoQuality() <= 20) {
            videoQuality = 15;
        } else if (Settings::videoQuality() <= 30) {
            videoQuality = 10;
        } else if (Settings::videoQuality() <= 40) {
            videoQuality = 5;
        } else if (Settings::videoQuality() <= 50) {
            videoQuality = 4;
        } else if (Settings::videoQuality() <= 60) {
            videoQuality = 3;
        } else if (Settings::videoQuality() <= 70) {
            videoQuality = 2;
        } else if (Settings::videoQuality() <= 80) {
            videoQuality = 1;
        } else if (Settings::videoQuality() <= 90) {
            videoQuality = 0.5;
        } else if (Settings::videoQuality() <= 100) {
            videoQuality = 0.1;
        } else {
            videoQuality = 2;
        }

        args << "-f" << "x11grab" << "-qscale" << QString::number(videoQuality) << "-r" << QString::number(d.fps);
        args << "-s" << QString("%1x%2").arg(geometry.width()).arg(geometry.height());
        args << "-i" << DisplayString(QX11Info::display())+QString("+%1,%2").arg(geometry.x()).arg(geometry.y());
        args << "-s" << QString("%1x%2").arg(geometry.width()).arg(geometry.height());
        args << m_tmpFile;
        //ffmpeg -f x11grab -qscale 2 -r 20 -s 1440x900 -i :0.0 -s 1440x900 x11grab.avi
    } else {

        QString command = Settings::customCommand();
        if (!command.contains("%{output}")) {
            emit error(i18n("Invalid command!"));
            return;
        }

        command.prepend("-f x11Grab ");
        command.replace("%{output}", m_tmpFile);
        command.replace("%{x}", QString::number(geometry.x()));
        command.replace("%{y}", QString::number(geometry.y()));
        command.replace("%{width}", QString::number(geometry.width()));
        command.replace("%{height}", QString::number(geometry.height()));
        command.replace("%{fps}", QString::number(d.fps));
        command.replace("%{XServer}", DisplayString(QX11Info::display()));

        args = command.split(' ');
    }

    // create/start
    m_recorder = new KProcess(this);
    m_recorder->setOutputChannelMode(KProcess::MergedChannels);

    connect(m_recorder, SIGNAL(readyReadStandardOutput()), this, SLOT(newRecorderOutput()));
    connect(m_recorder, SIGNAL(finished(int)), this, SLOT(recorderFinished(int)));

    kDebug() << "starting:" << ffmpeg << "with:" << args;

    m_recorder->setProgram(ffmpeg, args);

    setState(Record);
    emit status(i18n("Capturing!"));

    m_recorder->start();

}


void FfmpegRecorder::pause()
{

    if (!m_recorder) {
        return;
    }

    if (!m_paused) {
        emit status(i18n("Paused!"));
        kill(m_recorder->pid(), SIGSTOP);
        m_paused = true;
    } else {
        emit status(i18n("Capturing!"));
        kill(m_recorder->pid(), SIGCONT);
        m_paused = false;
    }

}


void FfmpegRecorder::stop()
{
    if (m_recorder) {
        m_recorder->write("q\n");
        m_paused = false;
    }

}


void FfmpegRecorder::newRecorderOutput()
{

    if (!m_recorder) {
        return;
    }

    QString output = QString(m_recorder->readAllStandardOutput()).trimmed();

    if (output.isEmpty()) {
        return;
    }

    foreach (QString line, output.split('\n')) { // krazy:exclude=foreach
        line = line.trimmed();
        kDebug() << "New Line:" << line;

        // frame=   40 fps=  9 q=0.0 Lsize=     585kB time=2.00 bitrate=2397.5kbits/s dup=36 drop=0
        QString text;

        const QRegExp frameRX("frame.*fps.*");
        if (frameRX.exactMatch(line)) {
            QString frames = line;
            frames.remove(QRegExp(".*frame="));
            frames.remove(QRegExp("fps=.*"));
            frames = frames.trimmed();
            text.append(i18nc("Video frames", "Frames=%1", frames));
            text.append(' ');
        }
        const QRegExp sizeRX(".*size.*time.*");
        if (sizeRX.exactMatch(line)) {
            QString size = line;
            size.remove(QRegExp(".*size="));
            size.remove(QRegExp("time.*"));
            size = size.trimmed();
            size.remove("kB");

            bool ok = false;
            double bytes = size.toDouble(&ok)*1024;
            if (ok) {
                size = KGlobal::locale()->formatByteSize(bytes);
                text.append(i18nc("Video size", "Size=%1", size));
                text.append(' ');
            } else {
                kWarning() << "toDouble() failed:" << size;
            }
        }

        QRegExp timeRX(".*time.*bitrate.*");
        if (timeRX.exactMatch(line)) {
            QString time = line;
            time.remove(QRegExp(".*time="));
            time.remove(QRegExp("bitrate.*"));
            time = time.trimmed();
            time.remove(QRegExp("\\..*"));

            bool ok = false;
            unsigned long msecs = time.toLong(&ok)*1000;
            if (ok) {
                time = KGlobal::locale()->formatDuration(msecs);
                text.append(i18nc("Video duration", "Duration=%1", time));
                text.append(' ');
            } else {
                kWarning() << "toLong() failed:" << time;
            }
        }

        QRegExp bitrateRX(".*bitrate.*kbits/s.*");
        if (bitrateRX.exactMatch(line)) {
            QString bitrate = line;
            bitrate.remove(QRegExp(".*bitrate="));
            bitrate.remove(QRegExp("kbits/s.*"));
            bitrate = bitrate.trimmed();
            text.append(i18nc("Video bitrate", "Bitrate=%1 kbits/s", bitrate));
        }

        if (!text.isEmpty()) {
            emit status(text);
        }
    }

}


void FfmpegRecorder::clean()
{

    if (m_recorder) {
        m_recorder->disconnect(this);
        if (m_recorder->state() != KProcess::NotRunning) {
            m_recorder->terminate();
        }
        m_recorder->waitForFinished(-1);
        m_recorder->deleteLater();
        m_recorder = 0;
    }

}


void FfmpegRecorder::moveToDestination()
{

    m_moveId = move(m_tmpFile, m_data.outputFile);
    if (m_moveId.isEmpty()) {
        emit error(i18n("Internal error!"));
    }

}


void FfmpegRecorder::recorderFinished(const int &ret)
{

    const KProcess::ExitStatus status = m_recorder->exitStatus();
    kDebug() << "status:" << status << "ret:" << ret;

    if (status == KProcess::CrashExit || ret != 0) {
        clean();
        emit finished(Crash);
        return;
    }

    QFile outputFile(m_data.outputFile);
    if (outputFile.exists()) {
        if (m_data.overwrite) {
            m_removeId = remove(m_data.outputFile);
            return;
        } else {
            m_data.outputFile = unique(m_data.outputFile);
            emit outputFileChanged(m_data.outputFile);
        }
    }
    moveToDestination();

}


void FfmpegRecorder::jobFinished(const QString &id, const QString &errorString)
{

    if (!errorString.isEmpty()) {
        emit error(errorString);
        return;
    }

    if (id == m_moveId) {
        emit finished(Normal);
    } else if (id == m_removeId) {
        moveToDestination();
    }

}


#include "ffmpegrecorder.moc"
