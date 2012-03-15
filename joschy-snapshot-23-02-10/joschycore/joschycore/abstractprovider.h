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

#ifndef ABSTRACTPROVIDER_H
#define ABSTRACTPROVIDER_H


// own
#include "plugin.h"
#include "joschy_global.h"
#include "video.h"
#include "actionreply.h"

// Qt
#include <QtCore/QVariantMap>
#include <QtCore/QPair>


namespace Joschy {


class AbstractProviderPrivate;
class AbstractNetworkLayer;


/**
 * @class AbstractProvider joschycore/abstractprovider.h
 *
 * @short The base class for all providers
 **/
class JOSCHY_EXPORT AbstractProvider : public Joschy::Plugin
{
    Q_OBJECT
    friend class AbstractNetworkLayer;
    friend class Manager;


public:
    /**
     * Creates a new AbstractProvider object.
     * @param parent the parent QObject
     **/
    explicit AbstractProvider(QObject *parent = 0);
    virtual ~AbstractProvider();

    /**
     * @return Returns a list of all valid categorys
     * @see categorysChanged
     **/
    virtual QStringList categorys() const = 0;
    /**
     * @return Returns true if login is authenticated; otherwise returns false.
     * @see authenticate
     **/
    virtual bool isAuthenticated(const QString &login) const = 0;

    /**
     * authenticates login
     *
     * Possible ErrorTypes in reply:
     * - #Joschy::Plugin::InvalidArgumentError < empty login/password
     *
     * @return Returns an Joschy::ActionReply
     * @see isAuthenticated
     **/
    virtual Joschy::ActionReply authenticate(const QString &login, const QString &password) = 0;
    /**
     * Starts an upload
     *
     * Possible ErrorTypes in reply:
     * - #Joschy::Plugin::NotAuthenticatedError < login is not authenticated
     * - #Joschy::Plugin::FileNotFoundError < video file not found
     * - #Joschy::Plugin::CannotOpenError < cannot open the video file
     *
     * @param video the video to upload
     * @param login the login
     * @return Returns an Joschy::ActionReply
     * @see authenticate, uploadFinished, error, uploadProgress
     **/
    virtual Joschy::ActionReply upload(const QString &login, Joschy::Video *video) = 0;
    /**
     * Starts a search
     * @verbatim
        QHash<QString, QVariant> data;
       @endverbatim
     * Required parameters:\n
     *\n
     * @verbatim
        data["Key"] = "dogs"
       @endverbatim
     *\n
     *\n
     * Optional parameters:\n
     * @verbatim
        data["Max"] = 25; // max videos in result
        data["Start"] = 5; // start index
        data["Category"] = "Tech"; // the category
        data["Author"] = "Obama"; // the author
       @endverbatim
     * @note Wich optional parameters are supported depends on the provider
     * Possible ErrorTypes in reply:\n
     * - #Joschy::Plugin::InvalidArgumentError < an argument was missing or invalid
     *
     * @param data The search data
     * @return Returns an Joschy::ActionReply
     * @see error, searchFinished
     **/
    virtual Joschy::ActionReply search(const QHash<QString, QVariant> &data) = 0;
    /**
     * Starts a thumbnail update
     *
     * Possible ErrorTypes in reply:
     * - #Joschy::Plugin::InvalidArgumentError < invalid thumbnail url
     *
     * @param video the video (thumbnailUrl must be set)
     * @param thumbnailDir the local directory were the thumbnail should be stored
     * @return Returns an Joschy::ActionReply
     * @see error, thumbnailUpdated
     **/
    virtual Joschy::ActionReply updateThumbnail(const Joschy::Video &video, const QString &thumbnailDir) = 0;
    /**
     * Cancels an action
     * if succesfull the provider sends an error with id and error = Joschy::Plugin::ActionCanceledError
     * @param id the id of the job
     * @see error
     **/
    void cancel(const QString &id);
    virtual void init();


public slots:
    /**
     * @internal called by AbstractNetworkLayer
     * @param id the id of the job
     * @param data the data QByteArray data["Reply"] the reply, int data["Status"] the http status code,
     * QString data["ContentType"] the content type
     * @param errorType the error type if one has occurred
     * @param errorString an error text if available
     **/
    virtual void jobFinished(const QString &id,
                             const QVariantMap &data,
                             const Joschy::Plugin::ErrorType &errorType = Joschy::Plugin::NoError,
                             const QString &errorString = QString()) = 0;



private:
    AbstractProviderPrivate *d;

    /**
     * @internal
     **/
    void setNetworkLayer(Joschy::AbstractNetworkLayer *layer);


private slots:
    /**
     * @internal
     **/
    void emitUploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent, qlonglong bytesTotal);


protected:
    /**
     * @internal
     **/
    Joschy::AbstractNetworkLayer *layer() const;
    /**
     * TODO
     **/
    QVariant load(const QString &key);
    /**
     * TODO
     **/
    void save(const QString &key, const QVariant &data);


signals:
    /**
     * An error occurred
     * @param id the id of the action
     * @param type the error type
     * @param text the error text if available
     **/
    void error(const QString &id, const Joschy::Plugin::ErrorType &type, const QString &text);
    /**
     * The authentication with id was successfull
     * @param id the id
     **/
    void authenticated(const QString &id);
    /**
     * An upload was successfull
     * @param id the id
     * @param video the video
     **/
    void uploadFinished(const QString &id, Joschy::Video video);
    /**
     * The search with id was successfull
     * @param id the id
     * @param videos all found videos
     **/
    void searchFinished(const QString &id, QList<Joschy::Video> videos);
    /**
     * The thumbnail update with id was successfull
     * @param id the id
     * @note The thumbnail is located at: thumbnailDir/video->thumbnail()
     **/
    void thumbnailUpdated(const QString &id);
    /**
     * Upload progress informations
     * @param id the id
     * @param speed the upload speed in bytes/s or -1
     * @param bytesSent the number of bytes uploaded or -1
     * @param bytesTotal the total number of bytes to be uploaded or -1
     **/
    void uploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent, qlonglong bytesTotal);
    /**
     * TODO
     * @param categorys The new categorys
     **/
    void categorysChanged(const QStringList &categorys);


};


} // namespace Joschy


#endif // ABSTRACTPROVIDER_H
