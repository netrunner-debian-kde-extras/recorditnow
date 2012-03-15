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
#include "mencoderencoder.h"
#include <recorditnow_mencoder.h>

// KDE
#include <klocalizedstring.h>
#include <kplugininfo.h>
#include <kdebug.h>
#include <kprocess.h>
#include <kstandarddirs.h>

// Qt
#include <QtCore/QDir>

// C
#include <sys/types.h>
#include <signal.h>


static const QStringList formats = QStringList() << "avi" << "flv" << "wmv" << "mkv";


K_PLUGIN_FACTORY(myFactory, registerPlugin<MencoderEncoder>();)
K_EXPORT_PLUGIN(myFactory("mencoder_encoder"))
MencoderEncoder::MencoderEncoder(QObject *parent, const QVariantList &args)
    : AbstractEncoder(parent, args)
{

    m_mencoder = 0;
    m_stopped = false;

}



MencoderEncoder::~MencoderEncoder()
{

    if (m_mencoder) {
        delete m_mencoder;
    }

}


void MencoderEncoder::encode(const Data &d)
{

    emit status(i18n("Starting mencoder!"));

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


void MencoderEncoder::pause()
{

    if (!m_paused) {
        emit status(i18n("Paused!"));
        kill(m_mencoder->pid(), SIGSTOP);
        m_paused = true;
    } else {
        emit status(i18n("Encoding started!"));
        kill(m_mencoder->pid(), SIGCONT);
        m_paused = false;
    }

}


void MencoderEncoder::stop()
{

    if (m_mencoder) {
        m_stopped = true;
        kill(m_mencoder->pid(), SIGINT);
        m_paused = false;
    }

}


void MencoderEncoder::prepare()
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

    m_outputFile += format;
    bool start = true;

    if (!m_data.overwrite) {
        m_outputFile = unique(m_outputFile);
    } else {
        if (QFile::exists(m_outputFile)) {
            start = false;
            m_currentId = remove(m_outputFile);
        }
    }

    emit outputFileChanged(m_outputFile); // update gui
    m_outputFile.remove(format);

    if (start) {
        startMencoder();
    }

}


void MencoderEncoder::startMencoder()
{


    // args
    m_command = m_command.arg(m_tmpFile).arg(m_outputFile);
    const QStringList args = m_command.split(' ');
    kDebug() << "command:" << m_command;

    // exe
    const QString exe = KGlobal::dirs()->findExe("mencoder");
    if (exe.isEmpty()) {
        emit error(i18n("Cannot find mencoder!\n"
                        "Please install mencoder or use another plugin."));
        return;
    }

    Q_ASSERT(!m_mencoder);

    m_mencoder = new KProcess(this);
    m_mencoder->setOutputChannelMode(KProcess::MergedChannels);
    m_mencoder->setProgram(exe, args);

    connect(m_mencoder, SIGNAL(finished(int)), this, SLOT(mencoderFinished(int)));
    connect(m_mencoder, SIGNAL(readyReadStandardOutput()), this, SLOT(newMencoderOutput()));

    m_mencoder->start();

}


void MencoderEncoder::finish()
{

    m_mencoder->disconnect(this);
    m_mencoder->deleteLater();
    m_mencoder = 0;

    emit finished(m_status == 0 || m_stopped ? Normal: Crash);

}


void MencoderEncoder::newMencoderOutput()
{

    QString output = m_mencoder->readAllStandardOutput().trimmed();
//"Pos:   6.7s    101f (99%) 33.95fps Trem:   0min   1mb  A-V:0.066 [1228:192]"
    if (output.contains('%')) {
        output.remove(output.indexOf('%'), output.length());
        output.remove(0, output.lastIndexOf('(')+1);
        bool ok;
        output.toInt(&ok);
        if (ok) {
            emit status(i18nc("%1 = progress in %", "Encoding: %1", output+'%'));
        } else {
            kDebug() << "!ok:" << output;
        }
    } else {
        kDebug() << "!parsed:" << output;
    }


}


void MencoderEncoder::mencoderFinished(const int &ret)
{

    m_status = ret;

    if (QFile::exists(m_tmpFile)) {
        m_state = RemoveTmpFile;
        m_currentId = remove(m_tmpFile);
    } else {
        finish();
    }

}


void MencoderEncoder::jobFinished(const QString &id, const QString &errorString)
{

    if (!errorString.isEmpty()) {
        emit error(errorString);
        return;
    }

    if (m_currentId == id) {
        switch (m_state) {
        case WorkDir: prepare(); break;
        case RemoveOutputFile: startMencoder(); break;
        case RemoveTmpFile: finish(); break;
        }
    }

}


#include "mencoderencoder.moc"
