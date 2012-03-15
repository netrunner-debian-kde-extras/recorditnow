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
#include "manager.h"
#include "device.h"

// KDE
#include <kdebug.h>

// Qt
#include <QtCore/QFile>
#include <QtCore/QDir>

// C
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>

// Solid
#include <solid/device.h>
#include <solid/deviceinterface.h>
#include <solid/genericinterface.h>
#include <solid/predicate.h>


namespace KeyMon {



Manager::Manager(QObject *parent)
    : QObject(parent)
{



}


Manager::~Manager()
{



}


QList<KeyMon::DeviceInfo> Manager::getInputDeviceList()
{

    QList<KeyMon::DeviceInfo> list;
    foreach (const Solid::Device &device, Solid::Device::allDevices()) {

        bool found = false;
        KeyMon::DeviceInfo info;
        const Solid::GenericInterface *interface = device.as<Solid::GenericInterface>();

        if (interface && interface->isValid()) {
            foreach (const QString &cap, interface->property("info.capabilities").toStringList()) {
                if (cap == QLatin1String("input.mouse")) {
                    if (interface->property("input.x11_driver").toString() != QLatin1String("evdev")) {
                        break;
                    }
                    info.file = interface->property("input.device").toString();
                    info.uuid = device.udi();
                    info.name = device.product();
                    info.icon = device.icon();
                    info.type = KeyMon::DeviceInfo::MouseType;
                    found = true;
                    break;

                } else if (cap == QLatin1String("input.keyboard")) {
                    if (interface->property("input.x11_driver").toString() != QLatin1String("evdev")) {
                        break;
                    }
                    info.file = interface->property("input.device").toString();
                    info.uuid = device.udi();
                    info.name = device.product();
                    info.icon = device.icon();
                    info.type = KeyMon::DeviceInfo::KeyboardType;
                    found = true;
                    break;
                }
            }

            if (found) {
                kDebug() << "Found input device:" << info.name;
                list.append(info);
            }


        }
    }
    return list;

}


QString Manager::fileForDevice(const KeyMon::DeviceInfo &info)
{

    Solid::Device device = Solid::Device(info.uuid);
    if (device.isValid()) {
        const Solid::GenericInterface *interface = device.as<Solid::GenericInterface>();
        if (interface && interface->isValid()) {
            return interface->property("input.device").toString();
        } else {
            kWarning() << "Invalid interface!";
            return QString();
        }
    }
    kWarning() << "Device not found...";
    return QString();

}


}; // Namespace KeyMon


#include "manager.moc"

