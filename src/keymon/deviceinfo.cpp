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
#include "deviceinfo.h"


namespace KeyMon {


DeviceInfo::DeviceInfo()
{



}


DeviceInfo::DeviceInfo(const DeviceInfo &copy)
{

    name = copy.name;
    file = copy.file;
    uuid = copy.uuid;
    icon = copy.icon;
    type = copy.type;

}


DeviceInfo::~DeviceInfo()
{


}


QByteArray KeyMon::DeviceInfo::toArray(const QList<KeyMon::DeviceInfo> &list)
{

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << list.size();
    foreach (const KeyMon::DeviceInfo &info, list) {
        stream << info.name << info.file << info.uuid << (int)info.type << info.icon;
    }

    return array;

}


QList<KeyMon::DeviceInfo> DeviceInfo::fromArray(QByteArray &array)
{

    QList<KeyMon::DeviceInfo> list;
    QDataStream stream(&array, QIODevice::ReadOnly);

   int size;
   stream >> size;

   for (int i = 0; i < size; i++) {
       DeviceInfo info;
       int type;
       stream >> info.name >> info.file >> info.uuid >> type >> info.icon;
       info.type = static_cast<KeyMon::DeviceInfo::DeviceType>(type);
       list.append(info);
   }
    return list;

}




} // namespace KeyMon
