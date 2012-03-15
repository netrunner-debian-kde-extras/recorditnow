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

#ifndef MANAGER_H
#define MANAGER_H


// own
#include "joschy_global.h"
#include "plugininfo.h"
#include "plugin.h"

// Qt
#include <QtCore/QObject>


namespace Joschy {


class ManagerPrivate;
class AbstractProvider;
class PluginManager;


/**
 * @class Manager joschycore/manager.h
 *
 * @short Class to load/unload providers and to manage some settings.
 *
 *
 * Load an provider:
 *
 * @verbatim
    QList<Joschy::PluginInfo> providerList = Joschy::Manager::self()->availableProvider();
    QList<Joschy::PluginInfo> layerList =  Joschy::Manager::self()->availableNetworkLayer();

    if (providerList.isEmpty() || layerList.isEmpty()) {
         return;
    }

    Joschy::AbstractProvider *provider = Joschy::Manager::self()->createProvider(providerList.first(), layerList.first());
    if (!provider) {
        return;
    }

    connect(provider, SIGNAL(authenticated(QString)), this, SLOT(authenticated(QString)));
    connect(provider, SIGNAL(error(QString,Joschy::Plugin::ErrorType,QString)), this,
            SLOT(error(QString,Joschy::Plugin::ErrorType,QString)));
    connect(provider, SIGNAL(uploadFinished(QString,Joschy::Video)), this,
            SLOT(uploadFinished(QString,Joschy::Video)));
    connect(provider, SIGNAL(searchFinished(QString,QList<Joschy::Video>)), this,
            SLOT(searchFinished(QString,QList<Joschy::Video>)));
    connect(provider, SIGNAL(thumbnailUpdated(QString)), this,
            SLOT(thumbnailUpdateFinished(QString)));
    connect(provider, SIGNAL(uploadProgress(QString,qlonglong,qlonglong,qlonglong)), this,
            SLOT(uploadProgress(QString,qlonglong,qlonglong,qlonglong)));
   @endverbatim
 *
 *
 * Auth:
 *
 * @verbatim
    Joschy::ActionReply reply = provider->authenticate("login", "password");
    if (reply.error()) {
        qDebug() << "Error:" << reply.errorType() << reply.errorString();
    } else {
        qDebug() << "Auth started:" << reply.id();
    }
   @endverbatim
 *
 *
 * Upload:
 *
 * @verbatim
    Joschy::Video video;
    video.setTitle("My Title");
    video.setDescription("My description");
    video.setCategory("Tech");
    video.setKeywords(QStringList() << "KDE" << "Qt");
    video.setUrl(QUrl("/home/joschy/my_video.ogv"));

    Joschy::ActionReply reply = provider->upload("user", &video);
    if (reply.error()) {
        qDebug() << "Error:" << reply.errorType() << reply.errorString();
    } else {
        qDebug() << "Upload started:" << reply.id();
    }
   @endverbatim
 *
 *
 * Search:
 *
 * @verbatim
    QHash<QString, QVariant> data;
    data["Key"] = "kde4";
    data["Max"] = 30;
    reply = provider->search(data);
    if (reply.error()) {
        qDebug() << "Error:" << reply.errorType() << reply.errorString();
    } else {
        qDebug() << "Search started:" << reply.id();
    }
    @endverbatim
 *
 *
 * Update a thumbnail:
 *
 * @verbatim
    QString thumbnailDir("/tmp/joschynails");
    Joschy::ActionReply reply = provider->updateThumbnail(video, thumbnailDir);
    if (!reply.error()) {
       qDebug() << "thumbnail update started:" << reply.id();
    } else {
       qDebug() << reply.errorType() << reply.errorString();
    }
   @endverbatim
 *
 *
**/
class JOSCHY_EXPORT Manager: public QObject
{
    Q_OBJECT
    friend class ManagerSingleton;


public:
    /**
     * @return Returns the current Scheduler mode
     * @see setSchedulerMode
    **/
    Joschy::Plugin::SchedulerMode schedulerMode() const;
    /**
     * @return Returns all available provider
    **/
    QList<Joschy::PluginInfo> availableProvider() const;
    /**
     * @return Returns all available Network layer
    **/
    QList<Joschy::PluginInfo> availableNetworkLayer() const;
    /**
     * Singleton accessor
     **/
    static Manager *self();
    /**
     * Creates an Provider instance with the given layer
     * @note Do not delete the object. Use unloadProvider
     * @return the Provider or 0 if the provider or the layer could not be loaded
     * @see unloadProvider
     **/
    Joschy::AbstractProvider *createProvider(const QString &provider, const QString &layer);
    /**
     * @see createProvider
     **/
    Joschy::AbstractProvider *createProvider(const Joschy::PluginInfo &providerInfo,
                                             const Joschy::PluginInfo &layerInfo);
    /**
     * Unloads a provider instance.
     **/
    void unloadProvider(Joschy::AbstractProvider *provider);
    /**
     * Sets the Scheduler mode.
     * @param mode the new mode
     * @param delay the delay of msecs to wait before the next job is started (only on SlowSchedulerMode)
     * @see schedulerMode
    **/
    void setSchedulerMode(const Joschy::Plugin::SchedulerMode &mode, const int &delay = 300);


private:
    /**
     * @internal
     **/
    explicit Manager(QObject *parent = 0);
    ~Manager();

    ManagerPrivate *d;


protected:
    /**
     * @internal
     **/
    Joschy::AbstractProvider *loadProvider(const QString &provider, const QString &layer);
    /**
     * @internal
     **/
    Joschy::AbstractProvider *loadProvider(const Joschy::PluginInfo &provider,
                                           const Joschy::PluginInfo &layer);



};


} // namespace Joschy


#endif // MANAGER_H
