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
#include "abstractresponseparser.h"

// Qt
#include <QtCore/QMutex>


namespace Joschy {


QMutex mutex;
class AbstractResponseParserPrivate
{
public:
    AbstractResponseParserPrivate(AbstractResponseParser *parser)
        : q(parser)
    {}


    QString id;
    AbstractResponseParser::Type type;
    QVariantMap data;
    bool error;
    Joschy::Plugin::ErrorType errorType;
    QString errorString;


private:
    AbstractResponseParser *q;


};


AbstractResponseParser::AbstractResponseParser(const Type &type, const QString &id,
    const QVariantMap &data, QObject *parent)
        : Joschy::AbstractJob(parent),
        d(new AbstractResponseParserPrivate(this))
{

    d->type = type;
    d->id = id;
    d->data = data;
    d->error = false;
    d->errorType = Joschy::Plugin::NoError;

}


AbstractResponseParser::~AbstractResponseParser()
{

    mutex.lock();
    delete d;
    d = 0;
    mutex.unlock();

}


Joschy::AbstractResponseParser::Type AbstractResponseParser::type() const
{

    return d->type;

}


QString AbstractResponseParser::id() const
{

    return d->id;

}


QVariantMap AbstractResponseParser::data() const
{

    return d->data;

}


bool AbstractResponseParser::error() const
{

    return d->error;

}


Joschy::Plugin::ErrorType AbstractResponseParser::errorType() const
{

    return d->errorType;

}


QString AbstractResponseParser::errorString() const
{

    return d->errorString;

}


void AbstractResponseParser::setError(const bool &error)
{

    d->error = error;

}


void AbstractResponseParser::setErrorType(const Joschy::Plugin::ErrorType &type)
{

    d->errorType = type;

}


void AbstractResponseParser::setErrorString(const QString &errorString)
{

    d->errorString = errorString;

}


} // namespace Joschy


#include "abstractresponseparser.moc"
