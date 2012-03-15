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
#include "sounddevice.h"

// KDE
#include <kdebug.h>
#include <kstandarddirs.h>

// Qt
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDir>


SoundDevice::SoundDevice()
{



}


QList<SoundDevice> SoundDevice::getDeviceList()
{

    QList<SoundDevice> cards;

    // asound
    QDir asound("/proc/asound");
    if (!asound.exists()) {
        kWarning() << "/proc/asound does not exists.";
        return cards;
    }

    const QRegExp cardRX("^card[0-9]+$");
    const QStringList subDirs = asound.entryList(QStringList(), QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (const QString &dir, subDirs) {
        if (cardRX.exactMatch(dir)) {
            kDebug() << "found card:" << dir;
            cards.append(scanASoundDevice("/proc/asound/"+dir));
        }
    }

    if (!KGlobal::dirs()->findExe("pulseaudio").isEmpty()) {
        SoundDevice dev;
        dev.m_name = "Pulseaudio";
        dev.m_key = "pulse";
        dev.m_icon = "audio-card";
        cards.append(dev);
    }

    return cards;

}


QString SoundDevice::name() const
{

    return m_name;

}


QString SoundDevice::key() const
{

    return m_key;

}


QString SoundDevice::icon() const
{

    return m_icon;

}


QList<SoundDevice> SoundDevice::scanASoundDevice(const QString &dir)
{

    QList<SoundDevice> cards;
    const QRegExp pcmRX("^pcm[0-9]+c$");

    QDir cardDir(dir);
    const QStringList subDirs = cardDir.entryList(QStringList(), QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        if (pcmRX.exactMatch(subDir)) {
            kDebug() << "found pcm:" << subDir;

            QFile info(dir+'/'+subDir+"/info");
            if (!info.exists()) {
                kWarning() << "info does not exists:" << info.fileName();
                continue;
            }

            if (!info.open(QIODevice::ReadOnly|QIODevice::Text)) {
                kWarning() << "open failed:" << info.error() << info.errorString();
                continue;
            }

            QTextStream stream(&info);
            SoundDevice card;

            QString CARD, DEVICE;
            QString line = stream.readLine();
            while (!line.isNull()) {
                if (line.startsWith(QLatin1String("name:"))) {
                    card.m_name = line.remove("name:").trimmed();
                } else if (line.startsWith(QLatin1String("card:"))) {
                    CARD = line.remove("card:").trimmed();
                } else if (line.startsWith(QLatin1String("device:"))) {
                    DEVICE = line.remove("device:").trimmed();
                }
                line = stream.readLine();
            }
            info.close();

            card.m_key = QString("hw:%1,%2").arg(CARD).arg(DEVICE);
            card.m_icon = "audio-card";
            cards.append(card);
        }
    }
    return cards;

}
