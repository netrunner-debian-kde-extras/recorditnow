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

#ifndef DEVICEINFO_H
#define DEVICEINFO_H


// KDE
#include <kdemacros.h>

// Qt
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QDataStream>


namespace KeyMon {


class KDE_EXPORT DeviceInfo
{


public:
    enum DeviceType {
        MouseType = 0,
        KeyboardType = 1
    };

    DeviceInfo();
    DeviceInfo(const KeyMon::DeviceInfo &copy);
    ~DeviceInfo();

    QString name;
    QString file;
    QString uuid;
    QString icon;
    KeyMon::DeviceInfo::DeviceType type;

    static QByteArray toArray(const QList<KeyMon::DeviceInfo> &list);
    static QList<KeyMon::DeviceInfo> fromArray(QByteArray &array);


};


} // namespace KeyMon



#endif // DEVICEINFO_H
