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
#include "abstractprovider.h"
#include "abstractnetworklayer.h"

// Qt
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QDir>


namespace Joschy {


class AbstractProviderPrivate
{
public:
    AbstractProviderPrivate(AbstractProvider *provider)
        : q(provider)
    {}


    Joschy::AbstractNetworkLayer *layer;


private:
    AbstractProvider *q;


};



AbstractProvider::AbstractProvider(QObject *parent)
    : Joschy::Plugin(parent), d(new AbstractProviderPrivate(this))
{



}


AbstractProvider::~AbstractProvider()
{

    delete d;

}


Joschy::AbstractNetworkLayer *AbstractProvider::layer() const
{

    return d->layer;

}


QVariant AbstractProvider::load(const QString &key)
{

    QSettings settings(this);
    return settings.value(key);

}


void AbstractProvider::save(const QString &key, const QVariant &data)
{

    QSettings settings(this);
    settings.setValue(key, data);
    settings.sync();

}


void AbstractProvider::cancel(const QString &id)
{

    d->layer->cancelJob(id);

}


void AbstractProvider::init()
{


}


void AbstractProvider::setNetworkLayer(Joschy::AbstractNetworkLayer *layer)
{

    d->layer = layer;

}


void AbstractProvider::emitUploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent,
                                          qlonglong bytesTotal)
{

    emit uploadProgress(id, speed, bytesSent, bytesTotal);

}


} // namespace Joschy


#include "abstractprovider.moc"
