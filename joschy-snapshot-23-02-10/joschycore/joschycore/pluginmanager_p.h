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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

// own
#include "joschy_global.h"
#include "plugininfo.h"
#include "plugin.h"

// Qt
#include <QtCore/QObject>


namespace Joschy {


class PluginManager : public QObject
{
    Q_OBJECT
    friend class Plugin;


public:
    explicit PluginManager(QObject *parent = 0);

    QList<Joschy::PluginInfo> availableProvider() const;
    QList<Joschy::PluginInfo> availableNetworkLayer() const;

    template<typename T>
    T loadPlugin(const Joschy::PluginInfo &info)
    {

        if (!m_pluginList.contains(info)) {
            return 0;
        }

        Joschy::Plugin *plugin = loadPluginInternal(info);
        if (plugin) {
            return qobject_cast<T>(plugin);
        }
        return 0;

    }

    void unloadPlugin(Joschy::Plugin *plugin);


private:
    QList<PluginInfo> m_pluginList;
    QList<Joschy::Plugin*> m_loadedPlugins;


private slots:
    void updatePluginList();


protected:
    Joschy::Plugin *loadPluginInternal(const Joschy::PluginInfo &info);


};


} // namespace Joschy


#endif // PLUGINMANAGER_H
