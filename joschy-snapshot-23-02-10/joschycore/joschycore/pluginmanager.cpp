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
#include "pluginmanager_p.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>


namespace Joschy {


PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{

    updatePluginList();

}


QList<Joschy::PluginInfo> PluginManager::availableProvider() const
{

    QList<Joschy::PluginInfo> list;
    foreach (const PluginInfo &info, m_pluginList) {
        if (info.type() == Joschy::PluginInfo::ProviderType) {
            list.append(info);
        }
    }
    return list;

}


QList<Joschy::PluginInfo> PluginManager::availableNetworkLayer() const
{

    QList<Joschy::PluginInfo> list;
    foreach (const PluginInfo &info, m_pluginList) {
        if (info.type() == Joschy::PluginInfo::NetworkLayerType) {
            list.append(info);
        }
    }
    return list;

}


void PluginManager::unloadPlugin(Joschy::Plugin *plugin)
{

    if (m_loadedPlugins.contains(plugin)) {
        m_loadedPlugins.removeAll(plugin);
        delete plugin;
    }

}


void PluginManager::updatePluginList()
{

    m_pluginList.clear();

    JOSCHY_DEBUG() << "load plugin list...";
    const QStringList filters = QStringList() << "*.plugin";
    foreach (const QString &pluginDir, Joschy::PluginInfo::pluginDirs()) {
        JOSCHY_DEBUG() << "PluginDir:" << pluginDir;
        QDir dir(pluginDir);
        foreach (const QString &file, dir.entryList(filters, QDir::Files)) {
            m_pluginList.append(PluginInfo(dir.absoluteFilePath(file)));
        }
    }
    JOSCHY_DEBUG() << "Done: " << m_pluginList.size();

}


Joschy::Plugin *PluginManager::loadPluginInternal(const Joschy::PluginInfo &info)
{

    JOSCHY_DEBUG() << "load:" << info.library();
    QPluginLoader loader(info.library());
    QObject *instance = loader.instance();
    if (instance) {
        JOSCHY_DEBUG() << "success";
        Joschy::Plugin *plugin = qobject_cast<Joschy::Plugin*>(instance);
        m_loadedPlugins.append(plugin);
        plugin->setPluginManager(this);
        return plugin;
    } else {
        JOSCHY_DEBUG() << "error:" << loader.errorString();
        return 0;
    }

}


} // namespace Joschy


#include "pluginmanager_p.moc"
