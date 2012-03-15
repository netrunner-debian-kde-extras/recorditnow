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

#ifndef ACTIONREPLY_H
#define ACTIONREPLY_H


// own
#include "joschy_global.h"
#include "plugin.h"

// Qt
#include <QtCore/QString>
#include <QtCore/QExplicitlySharedDataPointer>


namespace Joschy {


class ActionReplyPrivate;


/**
 * @class ActionReply joschycore/actionreply.h
 *
 * @short Class that encapsulates a reply coming from the AbstractProvider after executing an action.
 *
 * An instance of ActionReply is returned every time you call a function from an AbstractProvider.
 *
 * Every ActionReply contains an unique id of the job or an error and some informations.
 **/
class JOSCHY_EXPORT ActionReply
{


public:
    /**
     * Constructs a copy of copy.
     * @param copy the copy
     * @see operator=
     **/
    ActionReply(const Joschy::ActionReply &copy);
    /**
     * Creates a new ActionReply.
     * error() = false and errorType = Joschy::Plugin::NoError
     **/
    ActionReply();
    ~ActionReply();

    /**
     * Assigns rhs to this ActionReply and returns a reference to this ActionReply.
     **/
    Joschy::ActionReply &operator=(const Joschy::ActionReply &rhs);
    /**
     * Returns true if ActionReply rhs is equal to this ActionReply; otherwise returns false.
     **/
    bool operator==(const Joschy::ActionReply &rhs) const;
    /**
     * Returns true if this ActionReply is not equal to ActionReply rhs; otherwise returns false.
     **/
    bool operator!=(const Joschy::ActionReply &rhs) const;
    bool operator<(const Joschy::ActionReply &rhs) const;
    bool operator>(const Joschy::ActionReply &rhs) const;

    /**
     * @return Returns the unique job id or an empty string if an error occured.
     * @see setId
     **/
    QString id() const;
    /**
     * @return Returns the errorType.
     * @see setErrorType, setErrorString, errorString
     **/
    Joschy::Plugin::ErrorType errorType() const;
    /**
     * @return Returns true if an error occured; otherwise returns false.
     **/
    bool error() const;
    /**
     * @return Returns an error message or an empty string.
     * @see setErrorString, error
     **/
    QString errorString() const;

    /**
     * Sets the job id to id.
     * @param id the id.
     * @see id
     **/
    void setId(const QString &id);
    /**
     * Sets the error type
     * @param type the error type.
     * @see errorType, error
     **/
    void setErrorType(const Joschy::Plugin::ErrorType &type);
    /**
     * Sets the error message.
     * @param errorString the error message.
     * @see errorString, error, errorType
     **/
    void setErrorString(const QString &errorString);


private:
    QExplicitlySharedDataPointer<ActionReplyPrivate> d;


};


} // namespace Joschy


#endif // ACTIONREPLY_H
