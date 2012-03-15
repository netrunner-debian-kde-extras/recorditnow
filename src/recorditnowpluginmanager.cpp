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
#include "recorditnowpluginmanager.h"
#include "libs/recorder/abstractrecorder.h"
#include "libs/encoder/abstractencoder.h"
#include "libs/upload/abstractuploader.h"

// KDE
#include <kservice.h>
#include <kservicetypetrader.h>
#include <kdebug.h>
#include <kconfig.h>
#include <kconfiggroup.h>


RecordItNowPluginManager::RecordItNowPluginManager(QObject *parent)
    : QObject(parent)
{


}


RecordItNowPluginManager::~RecordItNowPluginManager()
{

    clear();

}


void RecordItNowPluginManager::init()
{

    loadPluginList();

}


RecordItNowPlugin *RecordItNowPluginManager::loadPlugin(const QString &name)
{

    QHashIterator<KPluginInfo, RecordItNowPlugin*> it(m_plugins);
    KPluginInfo info;
    while (it.hasNext()) {
        it.next();
        if (it.key().name().toLower() == name.toLower()) {
            if (it.value()) { // already loaded?
                return it.value();
            } else { // get info and load
                info = it.key();
                break;
            }
        }
    }
    if (!info.isValid()) {
        return 0;
    }

    kDebug() << "load plugin:" << name;

    KService::Ptr service = info.service();
    KPluginLoader loader(service->library());
    KPluginFactory *factory = loader.factory();

    if (!factory) {
        kWarning() << "KPluginFactory could not load the plugin:" << service->library() <<
        "Reason:" << loader.errorString();
        return 0;
    }

    RecordItNowPlugin *plugin = factory->create<RecordItNowPlugin>(this);
    delete factory;
    if (!plugin) {
        kWarning() << "factory::create<>() failed " << service->library();
        return 0;
    }

    return (m_plugins[info] = plugin);

}


void RecordItNowPluginManager::unloadPlugin(RecordItNowPlugin *plugin)
{

    QHashIterator<KPluginInfo, RecordItNowPlugin*> it(m_plugins);
    while (it.hasNext()) {
        it.next();
        if (it.value() && it.value() == plugin) {
            kDebug() << "unload plugin:" << it.key().name();
            it.value()->deleteLater();
            m_plugins[it.key()] = 0;
            return;
        }
    }
    kWarning() << "plugin not found!"; // should never happen
    delete plugin; // but play save

}


QList<KPluginInfo> RecordItNowPluginManager::getList(const QString &category) const
{

    QHashIterator<KPluginInfo, RecordItNowPlugin*> it(m_plugins);
    QList<KPluginInfo> infoList;

    while (it.hasNext()) {
        it.next();
        if (it.key().category() == category) {
            infoList.append(it.key());
        }
    }

    return infoList;

}


QList<KPluginInfo> RecordItNowPluginManager::getRecorderList() const
{

    return getList("Recorder");

}


QList<KPluginInfo> RecordItNowPluginManager::getEncoderList() const
{

    return getList("Encoder");

}


void RecordItNowPluginManager::clear()
{

    QHashIterator<KPluginInfo, RecordItNowPlugin*> it(m_plugins);
    while (it.hasNext()) {
        it.next();
        if (it.value()) {
            it.value()->deleteLater();
        }
    }
    m_plugins.clear();

}


void RecordItNowPluginManager::loadPluginList()
{

    clear();

    loadInfos("RecordItNowRecorder");
    if (m_plugins.isEmpty()) {
        printf("*********************************\n");
        printf("Please run \"kbuildsycoca4\".\n");
        printf("*********************************\n");
    }
    loadInfos("RecordItNowEncoder");

    emit pluginsChanged();

}


void RecordItNowPluginManager::loadInfos(const QString &type)
{

    KConfig cfg("recorditnowrc");
    KConfigGroup pCfg(&cfg, "Plugins");

    KService::List offers = KServiceTypeTrader::self()->query(type);
    KService::List::const_iterator iter;

    for (iter = offers.begin(); iter < offers.end(); ++iter) {
        KService::Ptr service = *iter;
        KPluginInfo info(service);

        if (!info.isValid()) {
            kWarning() << "invalid plugin info:" << service->entryPath();
            continue;
        } else {
            info.setConfig(pCfg);
            info.load(pCfg);
            m_plugins[info] = 0;
        }
    }

}


#include "recorditnowpluginmanager.moc"
