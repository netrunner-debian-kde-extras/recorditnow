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

#ifndef ABSTRACTNETWORKLAYER_H
#define ABSTRACTNETWORKLAYER_H


// own
#include "plugin.h"
#include "joschy_global.h"

// Qt
#include <QtCore/QHash>
#include <QtCore/QUrl>


namespace Joschy {


class AbstractProvider;
class AbstractNetworkLayerPrivate;
typedef QHash<QByteArray, QByteArray> Header;

/**
 * @class AbstractNetworkLayer joschycore/abstractnetworklayer.h
 *
 * @short The base class for all layers
 **/
class JOSCHY_EXPORT AbstractNetworkLayer : public Joschy::Plugin
{
    Q_OBJECT
    friend class AbstractProvider;
    friend class Manager;


public:
    /**
     * Creates a new AbstractNetworkLayer object.
     * @param parent the parent QObject
     **/
    explicit AbstractNetworkLayer(QObject *parent = 0);
    virtual ~AbstractNetworkLayer();

    /**
     * @reimp
     * Sends the contents of the data byte array and header to the destination specified by url.
     * @see get, Joschy::AbstractProvider::jobFinished
     * @note call provider()->jobFinished() if the job is finished
     * @return Retuns an unique job id
     **/
    virtual QString post(const QUrl &url, const Header &header, const QByteArray &data) = 0;
    /**
     * @reimp
     * Sends the contents of the device and header to the destination specified by url.
     * @note use this for big files (10Mb+)
     * @note call provider()->jobFinished() if the job is finished
     * @see get, Joschy::PostFile, Joschy::AbstractProvider::jobFinished
     * @return Retuns an unique job id
     **/
    virtual QString post(const QUrl &url, const Header &header, QIODevice *device) = 0;
    /**
     * @reimp
     * Posts a request to obtain the contents of the target url.
     * @note call provider()->jobFinished() if the job is finished
     * @see post, Joschy::AbstractProvider::jobFinished
     * @return Retuns an unique job id
     **/
    virtual QString get(const QUrl &url, const QHash<QByteArray, QByteArray> &header = Header()) = 0;

    /**
     * @return Returns the provider plugin
     **/
    Joschy::AbstractProvider *provider() const;

    /**
     * Called by the provider plugin.
     **/
    void freeId(const QString &id);


private:
    AbstractNetworkLayerPrivate *d;

    /**
     * @internal
     **/
    void cancelJob(const QString &id);
    /**
     * @internal
     **/
    void setProvider(Joschy::AbstractProvider *provider);


protected slots:
    /**
     * Call this function to emit upload progress informations
     * @param id the id of the job
     * @param speed the upload speed in bytes or -1
     * @param bytesSent the number of bytes uploaded or -1
     * @param bytesTotal the total number of bytes to be uploaded or -1
     **/
    void emitUploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent, qlonglong bytesTotal);
    /**
     * @reimp
     * Cancel a job
     * @param id the id of the job
     * @note This _must_ always stop the upload/download
     * @see post, get
     **/
    virtual void cancel(const QString &id) = 0;


};


} // namespace Joschy


#endif // ABSTRACTNETWORKLAYER_H
