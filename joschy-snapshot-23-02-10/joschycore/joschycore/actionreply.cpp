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
#include "actionreply.h"


namespace Joschy {


class ActionReplyPrivate: public QSharedData
{
public:
    ActionReplyPrivate(ActionReply *reply)
        : errorType(Joschy::Plugin::NoError),
        q(reply)
    {}


    ~ActionReplyPrivate()
    {}


    QString id;
    QString errorString;
    Joschy::Plugin::ErrorType errorType;


private:
    ActionReply *q;


};



ActionReply::ActionReply(const Joschy::ActionReply &copy)
    : d(copy.d)
{


}


ActionReply::ActionReply()
    : d(new ActionReplyPrivate(this))
{


}


ActionReply::~ActionReply()
{



}


Joschy::ActionReply &ActionReply::operator=(const Joschy::ActionReply &rhs)
{

    d = rhs.d;
    return *this;

}


bool ActionReply::operator==(const Joschy::ActionReply &rhs) const
{

    return d == rhs.d;

}


bool ActionReply::operator!=(const Joschy::ActionReply &rhs) const
{

    return d != rhs.d;

}


bool ActionReply::operator<(const Joschy::ActionReply &rhs) const
{

    return d < rhs.d;

}


bool ActionReply::operator>(const Joschy::ActionReply &rhs) const
{

    return d > rhs.d;

}


QString ActionReply::id() const
{

    return d->id;

}


Joschy::Plugin::ErrorType ActionReply::errorType() const
{

    return d->errorType;

}


bool ActionReply::error() const
{

    return d->errorType != Joschy::Plugin::NoError;

}


QString ActionReply::errorString() const
{

    return d->errorString;

}


void ActionReply::setId(const QString &id)
{

    d->id = id;

}


void ActionReply::setErrorType(const Joschy::Plugin::ErrorType &type)
{

    d->errorType = type;

}


void ActionReply::setErrorString(const QString &errorString)
{

    d->errorString = errorString;

}


} // namespace Joschy
