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
#include "abstractnetworklayer.h"
#include "abstractprovider.h"


namespace Joschy {


class AbstractNetworkLayerPrivate
{
public:
    AbstractNetworkLayerPrivate(AbstractNetworkLayer *layer)
        : q(layer)
    {}


    AbstractProvider *provider;


private:
    AbstractNetworkLayer *q;


};


AbstractNetworkLayer::AbstractNetworkLayer(QObject *parent)
    : Joschy::Plugin(parent), d(new AbstractNetworkLayerPrivate(this))
{


}


AbstractNetworkLayer::~AbstractNetworkLayer()
{

    JOSCHY_DEBUG();
    delete d;

}


Joschy::AbstractProvider *AbstractNetworkLayer::provider() const
{

    return d->provider;

}


void AbstractNetworkLayer::freeId(const QString &id)
{

    removeUniqueId(id);

}


void AbstractNetworkLayer::cancelJob(const QString &id)
{

    cancel(id);

}


void AbstractNetworkLayer::setProvider(Joschy::AbstractProvider *provider)
{

    d->provider = provider;

}


void AbstractNetworkLayer::emitUploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent,
                                              qlonglong bytesTotal)
{

    d->provider->emitUploadProgress(id, speed, bytesSent, bytesTotal);

}


} // namespace Joschy

#include "abstractnetworklayer.moc"
