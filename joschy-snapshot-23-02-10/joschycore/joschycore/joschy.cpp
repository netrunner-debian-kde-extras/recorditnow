/*
    Copyright (C) 2010  Kai Dombrowe <just89@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


// own
#include "joschy.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>


namespace Joschy {


QStringList pluginDirs()
{

    QStringList dirs;
    dirs.append(JOSCHY_PLUGINDIR);
    dirs.append(QCoreApplication::applicationDirPath()+QDir::separator()+"joschy");

    foreach (const QString &path, QCoreApplication::libraryPaths()) {
        dirs.append(path+QDir::separator()+"joschy");
    }
    dirs.removeDuplicates();

    return dirs;

}



} // namespace Joschy
