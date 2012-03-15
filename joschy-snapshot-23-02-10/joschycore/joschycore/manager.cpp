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
#include "manager.h"
#include "pluginmanager_p.h"
#include "abstractprovider.h"
#include "abstractnetworklayer.h"
#include "scheduler.h"


namespace Joschy {


class ManagerPrivate
{
public:
    ManagerPrivate(Manager *manager)
        : q(manager)
    {

        pluginManager = new Joschy::PluginManager(q);

    }


    ~ManagerPrivate()
    {

        delete pluginManager;

    }


    PluginManager *pluginManager;


private:
    Manager *q;


};


class ManagerSingleton
{
public:
    Manager self;


};


JOSCHY_SINGLETON(ManagerSingleton, privateSelf)


Manager::Manager(QObject *parent)
    : QObject(parent), d(new ManagerPrivate(this))
{

    JOSCHY_DEBUG();

}


Manager::~Manager()
{

    JOSCHY_DEBUG();
    delete d;

}


Joschy::Plugin::SchedulerMode Manager::schedulerMode() const
{

    return Joschy::Scheduler::mode();

}


QList<Joschy::PluginInfo> Manager::availableProvider() const
{

    return d->pluginManager->availableProvider();

}


QList<Joschy::PluginInfo> Manager::availableNetworkLayer() const
{

    return d->pluginManager->availableNetworkLayer();

}


Manager *Manager::self()
{

    return &Joschy::privateSelf->self;

}


Joschy::AbstractProvider *Manager::createProvider(const QString &provider, const QString &layer)
{

    PluginInfo providerInfo;
    PluginInfo layerInfo;

    foreach (const PluginInfo &info, availableProvider()) {
        if (provider == info.name()) {
            providerInfo = info;
            break;
        }
    }

    foreach (const PluginInfo &info, availableNetworkLayer()) {
        if (layer == info.name()) {
            layerInfo = info;
            break;
        }
    }

    return createProvider(providerInfo, layerInfo);

}


Joschy::AbstractProvider *Manager::createProvider(const Joschy::PluginInfo &providerInfo,
                                         const Joschy::PluginInfo &layerInfo)
{

    return loadProvider(providerInfo, layerInfo);

}


void Manager::unloadProvider(Joschy::AbstractProvider *provider)
{

    d->pluginManager->unloadPlugin(provider->layer());
    d->pluginManager->unloadPlugin(provider);

}


void Manager::setSchedulerMode(const Joschy::Plugin::SchedulerMode &mode, const int &delay)
{

    Joschy::Scheduler::setMode(mode, delay);

}


Joschy::AbstractProvider *Manager::loadProvider(const QString &provider, const QString &layer)
{

    PluginInfo providerInfo;
    foreach (const PluginInfo &info, availableProvider()) {
        if (provider == info.name()) {
            providerInfo = info;
            break;
        }
    }
    PluginInfo layerInfo;
    foreach (const PluginInfo &info, availableNetworkLayer()) {
        if (layer == info.name()) {
            layerInfo = info;
            break;
        }
    }
    return loadProvider(providerInfo, layerInfo);

}


Joschy::AbstractProvider *Manager::loadProvider(const Joschy::PluginInfo &provider,
                                       const Joschy::PluginInfo &layer)
{

    if (!provider.isValid() || !layer.isValid()) {
        return 0;
    }

    AbstractProvider *providerPlugin = d->pluginManager->loadPlugin<Joschy::AbstractProvider*>(provider);
    if (!providerPlugin) {
        return 0;
    }

    AbstractNetworkLayer *layerPlugin = d->pluginManager->loadPlugin<Joschy::AbstractNetworkLayer*>(layer);
    if (!layerPlugin) {
        d->pluginManager->unloadPlugin(providerPlugin);
        return 0;
    }
    providerPlugin->setNetworkLayer(layerPlugin);
    layerPlugin->setProvider(providerPlugin);
    providerPlugin->init();

    return providerPlugin;

}



} // namespace Joschy


#include "manager.moc"

