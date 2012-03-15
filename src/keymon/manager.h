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


#ifndef MANAGER_H
#define MANAGER_H


// own
#include "deviceinfo.h"

// KDE
#include <kdemacros.h>

// Qt
#include <QtCore/QObject>
#include <QtCore/QList>



namespace KeyMon {


class Device;
class KDE_EXPORT Manager : public QObject
{
    Q_OBJECT


public:
    struct DeviceData
    {
        QString name;
        QString file;
        QString uuid;
    };
    explicit Manager(QObject *parent = 0);
    ~Manager();

    static QList<KeyMon::DeviceInfo> getInputDeviceList();
    static QString fileForDevice(const KeyMon::DeviceInfo &info);


};


}; // Namespace KeyMon



#endif // MANAGER_H
