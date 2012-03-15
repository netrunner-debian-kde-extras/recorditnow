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
#include "devicehelper.h"
#include "device.h"

// KDE
#include <kdebug.h>
#include <klocalizedstring.h>

// Qt
#include <QtCore/QTimer>
#include <QtCore/QStringList>

// c
#include <unistd.h>


ActionReply DeviceHelper::watch(QVariantMap args)
{

    kDebug() << "watch...";
    QTimer timer(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(500);

    QList<KeyMon::Device*> devs;
    foreach (const QString &dev, args.value("Devs").toStringList()) {
        KeyMon::DeviceInfo info;
        info.uuid = dev;

        KeyMon::Device *device = new KeyMon::Device(this, KeyMon::Manager::fileForDevice(info));
        devs.append(device);
        if (device->error()) {
            kDebug() << "error";
            qDeleteAll(devs);

            KAuth::ActionReply reply = ActionReply::HelperError;

            QVariantMap data;
            data["ErrorString"] = i18n("Could not open device file %1", dev);
            reply.setData(data);

            return reply;
        }
        connect(device, SIGNAL(buttonPressed(KeyMon::Event)), this, SLOT(key(KeyMon::Event)));
        connect(device, SIGNAL(keyPressed(KeyMon::Event)), this, SLOT(key(KeyMon::Event)));
    }

    QVariantMap data;
    data["Started"] = true;
    HelperSupport::progressStep(data);

    m_loop.exec(); // krazy:exclude=crashy

    qDeleteAll(devs);

    return ActionReply::SuccessReply;

}


void DeviceHelper::key(const KeyMon::Event &event)
{

    QVariantMap data;
    data["Key"] = event.key;
    data["KeyCode"] = event.keyCode;
    data["Pressed"] = event.pressed;
    data["MouseEvent"] = event.mouseEvent;
    HelperSupport::progressStep(data);

}


void DeviceHelper::timeout()
{

    if (HelperSupport::isStopped()) {
        m_loop.quit();
    }

}


KDE4_AUTH_HELPER_MAIN("org.kde.recorditnow.helper", DeviceHelper)


#include "devicehelper.moc"
