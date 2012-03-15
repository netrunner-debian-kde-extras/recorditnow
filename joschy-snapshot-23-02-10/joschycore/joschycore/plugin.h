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

#ifndef PLUGIN_H
#define PLUGIN_H


// own
#include "joschy_global.h"

// Qt
#include <QtCore/QObject>
#include <QtCore/QStringList>


/**
 * @mainpage The JoschyCore Library
 *
 * Dokumentation for clients:
 *
 * Joschy::Manager
 *
 *
 * Dokumentation for plugins:
 *
 * Joschy::AbstractProvider \n
 * Joschy::AbstractNetworkLayer
 *
 **/



namespace Joschy {


class PluginManager;
class PluginPrivate;

/**
 * @class Plugin joschycore/plugin.h
 *
 * @short The base class for all plugins
 **/
class JOSCHY_EXPORT Plugin : public QObject
{
    Q_OBJECT
    friend class PluginManager;


public:
    /**
     * Error types
     **/
    enum ErrorType {
        NoError = -1,                   /**< There was no error */
        UnknownError = 0,               /**< An unknown error has occurred */
        ActionCanceledError = 1,        /**< The action was canceled */
        InvalidArgumentError = 2,       /**< Invalid argument */
        NotAuthenticatedError = 3,      /**< You are not authenticated */
        FileNotFoundError = 4,          /**< A file was not found */
        PermissionDeniedError = 5,      /**< Access denied */
        ThumbnailUpdateFailedError = 6, /**< The thumbnail update failed */
        NotVerifiedError = 7,           /**< The account is not verified */
        TermsNotAgreedError = 8,        /**< The terms are not agreed */
        AccountDeletedError = 10,       /**< The account was deleted */
        AccountDisabledError = 11,      /**< The account is disabled */
        ServiceDisabledError = 12,      /**< The service is permanently unavailable */
        ServiceUnavailableError = 13,   /**< The service is temporary unavailable */
        CannotOpenError = 14,           /**< A file could not be opened */
        InternalError = 15,             /**< An internal error has occurred */
        ServiceError = 16,              /**< An service error has occurred */
        NetworkError = 17,              /**< An network error has occurred */
        SslError = 18,                  /**< An ssl error has occurred */
        ProxyError = 19                 /**< An proxy error has occurred */
    };

    /**
     * Scheduler modes
     **/
    enum SchedulerMode {
        SlowSchedulerMode = 0,     /**< Same as Normal but the scheduler waits some time after each job */
        NormalSchedulerMode = 1,   /**< Start one job after another */
        FastSchedulerMode = 2      /**< Run as many jobs as possible */
    };

    /**
     * Creates a new Plugin object.
     * @param parent the parent QObject
     **/
    explicit Plugin(QObject *parent = 0);


private:
    PluginPrivate *d;


protected:
    /**
     * @return An unique id.
     * @see removeUniqueId
     **/
    QString addUniqueId();

    /**
     * Frees the id
     * @param id the id
     * @see addUniqueId
     **/
    void removeUniqueId(const QString &id);
    /**
     * @internal
     **/
    void setPluginManager(PluginManager *manager);


};


} // namespace Joschy


#endif // PLUGIN_H
