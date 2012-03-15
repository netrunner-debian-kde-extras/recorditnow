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
#include "ffmpegencoder.h"
#include <recorditnow_ffmpeg.h>

// KDE
#include <klocalizedstring.h>
#include <kplugininfo.h>
#include <kdebug.h>
#include <kprocess.h>
#include <kstandarddirs.h>

// Qt
#include <QtCore/QDir>
#include <QtCore/QTime>

// C
#include <sys/types.h>
#include <signal.h>


static const QStringList formats = QStringList() << "avi" << "flv" << "wmv" << "mpeg" << "mkv";


K_PLUGIN_FACTORY(myFactory, registerPlugin<FfmpegEncoder>();)
K_EXPORT_PLUGIN(myFactory("ffmpeg_encoder"))
FfmpegEncoder::FfmpegEncoder(QObject *parent, const QVariantList &args)
    : AbstractEncoder(parent, args)
{

    m_duration = -1;
    m_ffmpeg = 0;
    m_stopped = false;

}



FfmpegEncoder::~FfmpegEncoder()
{

    if (m_ffmpeg) {
        delete m_ffmpeg;
    }

}


void FfmpegEncoder::encode(const Data &d)
{

    emit status(i18n("Starting ffmpeg!"));

    m_data = d;

    // check input file
    if (!QFile::exists(d.file)) { // should never happen
        emit error(i18nc("%1 = file", "%1 no such file!", d.file));
        return;
    }

    // reload cfg
    Settings::self()->readConfig();

    m_outputFile = d.file;
    m_tmpFile = getTemporaryFile(d.workDir);


    // move to wokdir
    m_state = WorkDir;
    m_currentId = move(d.file, m_tmpFile);

}


void FfmpegEncoder::pause()
{

    if (!m_paused) {
        emit status(i18n("Paused!"));
        kill(m_ffmpeg->pid(), SIGSTOP);
        m_paused = true;
    } else {
        emit status(i18n("Encoding started!"));
        kill(m_ffmpeg->pid(), SIGCONT);
        m_paused = false;
    }

}


void FfmpegEncoder::stop()
{

    if (m_ffmpeg) {
        m_stopped = true;
        kill(m_ffmpeg->pid(), SIGINT);
        m_paused = false;
    }

}


void FfmpegEncoder::prepare()
{

    // remove format
    if (m_outputFile.length() > 4 && m_outputFile[m_outputFile.length()-4] == '.') {
        m_outputFile.remove(m_outputFile.length()-4, 4);
    }


    // set output file + args
    m_command = Settings::command();
    if (!m_command.contains("%1") || !m_command.contains("%2")) {
        emit error(i18n("Input/output file is missing."));
        return;
    }

    QString format = m_command.mid(m_command.indexOf("%2"));
    format.remove("%2");
    format.remove(QRegExp(" .*"));

    bool start = true;
    m_outputFile += format;
    if (!m_data.overwrite) {
        m_outputFile = unique(m_outputFile);
    } else {
        if (QFile::exists(m_outputFile)) {
            m_state = RemoveOutputFile;
            m_currentId = remove(m_outputFile);
            start = false;
        }
    }

    emit outputFileChanged(m_outputFile); // update gui
    m_outputFile.remove(format);

    if (start) {
        startFfmpeg();
    }

}


void FfmpegEncoder::startFfmpeg()
{

    // args
    m_command = m_command.arg(m_tmpFile).arg(m_outputFile);
    const QStringList args = m_command.split(' ');


    // exe
    const QString exe = KGlobal::dirs()->findExe("ffmpeg");
    if (exe.isEmpty()) {
        emit error(i18n("Cannot find ffmpeg!\n"
                        "Please install ffmpeg or use another plugin."));
        return;
    }

    Q_ASSERT(!m_ffmpeg);

    m_ffmpeg = new KProcess(this);
    m_ffmpeg->setOutputChannelMode(KProcess::MergedChannels);
    m_ffmpeg->setProgram(exe, args);

    connect(m_ffmpeg, SIGNAL(finished(int)), this, SLOT(ffmpegFinished(int)));
    connect(m_ffmpeg, SIGNAL(readyReadStandardOutput()), this, SLOT(newFfmpegOutput()));

    m_ffmpeg->start();

}


void FfmpegEncoder::finish()
{

    m_ffmpeg->disconnect(this);
    m_ffmpeg->deleteLater();
    m_ffmpeg = 0;

    emit finished(m_status == 0 || m_stopped ? Normal: Crash);

}


void FfmpegEncoder::newFfmpegOutput()
{

    QString output = m_ffmpeg->readAllStandardOutput().trimmed();
    if (output.contains(QRegExp("[0-9][0-9]:[0-9][0-9]:[0-9][0-9]"))) {
        output.remove(QRegExp(".*Duration:"));
        output.remove(output.indexOf(','), output.length());
        output = output.trimmed();
        if (output.contains('.')) {
            output.remove(output.indexOf('.'), output.length());
        }
        
        QTime time = QTime::fromString(output, "hh:mm:ss");
        m_duration = time.second();
        m_duration += (time.minute()*60);
        m_duration += (time.hour()*60*60);
        return;
    } else if (output.contains("time=") && m_duration > 0) {
        output.remove(QRegExp(".*time="));
        output = output.trimmed();
        output.remove(QRegExp(" .*"));
        if (output.contains('.')) {
            output.remove(output.indexOf('.'), output.length());
        }
        bool ok;
        const int time = output.toInt(&ok);
        if (!ok) {
            return;
        }
        const QString progress = QString::number((time*100)/m_duration);
        emit status(i18nc("%1 = progress in %", "Encoding: %1", progress+'%'));
        return;
    }
    kDebug() << "!parsed:" << output;

}


void FfmpegEncoder::ffmpegFinished(const int &ret)
{

    m_status = ret;

    if (QFile::exists(m_tmpFile)) {
        m_state = RemoveTmpFile;
        m_currentId = remove(m_tmpFile);
    } else {
        finish();
    }

}


void FfmpegEncoder::jobFinished(const QString &id, const QString &errorString)
{

    if (!errorString.isEmpty()) {
        emit error(errorString);
        return;
    }

    if (m_currentId == id) {
        switch (m_state) {
        case WorkDir: prepare(); break;
        case RemoveOutputFile: startFfmpeg(); break;
        case RemoveTmpFile: finish(); break;
        }
    }

}


#include "ffmpegencoder.moc"

