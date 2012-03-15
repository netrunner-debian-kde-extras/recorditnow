/***************************************************************************
 *   Copyright (C) 2009 by Kai Dombrowe <just89@gmx.de>                    *
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
#include "encodermanager.h"
#include "recorditnowpluginmanager.h"

// KDE
#include <kdebug.h>


EncoderManager::EncoderManager(QObject *parent, RecordItNowPluginManager *manager)
    : QObject(parent), m_manager(manager)
{



}


EncoderManager::~EncoderManager()
{

    clean();

}


QList<EncoderData> EncoderManager::getEncoder() const
{

    QList<EncoderData> list;
    foreach (const KPluginInfo &info, m_manager->getEncoderList()) {
        if (info.isPluginEnabled()) {
            EncoderData data;
            data.first = info.name();
            data.second = KIcon(info.icon());
            list.append(data);
        }
    }
    return list;

}


void EncoderManager::startEncode(const QString &encoder, const AbstractEncoder::Data &data)
{

    if (m_encoder) {
        m_manager->unloadPlugin(m_encoder);
    }

    m_encoder = static_cast<AbstractEncoder*>(m_manager->loadPlugin(encoder));

    if (!m_encoder) {
        encoderError(i18n("Cannot load Encoder %1.", encoder));
        return;
    }

    connect(m_encoder, SIGNAL(status(QString)), this, SIGNAL(status(QString)));
    connect(m_encoder, SIGNAL(error(QString)), this, SLOT(encoderError(QString)));
    connect(m_encoder, SIGNAL(outputFileChanged(QString)), this, SIGNAL(fileChanged(QString)));
    connect(m_encoder, SIGNAL(finished(AbstractEncoder::ExitStatus)), this,
            SLOT(encoderFinished(AbstractEncoder::ExitStatus)));

    m_encoder->encode(data);

}


void EncoderManager::pauseOrContinue()
{

    if (m_encoder) {
        m_encoder->pause();
    }

}


void EncoderManager::stop()
{

    if (m_encoder) {
        m_encoder->stop();
    }

}


void EncoderManager::clean()
{

    if (m_encoder) {
        m_manager->unloadPlugin(m_encoder);
        m_encoder = 0;
    }

}


void EncoderManager::encoderError(const QString &error)
{

    emit finished(error);
    clean();

}


void EncoderManager::encoderFinished(const AbstractEncoder::ExitStatus &status)
{

    QString error;
    if (status == AbstractEncoder::Crash) {
        error = i18n("The encoder has crashed!");
    }
    emit finished(error);
    clean();

}


#include "encodermanager.moc"
