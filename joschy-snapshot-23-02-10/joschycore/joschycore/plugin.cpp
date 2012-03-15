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
#include "plugin.h"
#include "pluginmanager_p.h"

// Qt
#include <QtCore/QUuid>
#include <QtCore/QMutex>


namespace Joschy {


class PluginPrivate {
public:
    PluginPrivate(Plugin *plugin)
        : q(plugin)
    {}


    QStringList uniqueIds;
    PluginManager *pluginManager;
    QMutex mutex;


private:
    Plugin  *q;


};


Plugin::Plugin(QObject *parent)
    : QObject(parent), d(new PluginPrivate(this))
{


}


QString Plugin::addUniqueId()
{

    d->mutex.lock();

    bool unique = false;
    while (!unique) {
        const QString id = QUuid::createUuid().toString();

        unique = true;
        foreach (Joschy::Plugin *plugin, d->pluginManager->m_loadedPlugins) {
            if (plugin->d->uniqueIds.contains(id)) {
                unique = false;
                break;
            }
        }
        if (unique) {
            d->uniqueIds.append(id);

            d->mutex.unlock();

            return id;
        }
    }

    return QString(); // warning

}


void Plugin::removeUniqueId(const QString &id)
{

    d->uniqueIds.removeAll(id);

}


void Plugin::setPluginManager(PluginManager *manager)
{

    d->pluginManager = manager;

}



} // namespace Joschy


#include "plugin.moc"
