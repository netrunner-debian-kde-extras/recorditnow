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

#ifndef ABSTRACTRESPONSEPARSER_H
#define ABSTRACTRESPONSEPARSER_H

// own
#include "joschy_global.h"
#include "abstractjob.h"
#include "plugin.h"

// Qt
#include <QtCore/QObject>
#include <QtCore/QVariantMap>


namespace Joschy {


class AbstractResponseParserPrivate;


/**
 * @class AbstractResponseParser joschycore/abstractresponseparser.h
 *
 * @short Useful class for response parser
 **/
class JOSCHY_EXPORT AbstractResponseParser : public Joschy::AbstractJob
{
    Q_OBJECT


public:
    enum Type {
        AuthenticationType = 0,
        UploadType = 1,
        SearchType = 2,
        UpdateThumbnailType = 3,
        UpdateCategorysType = 4,
        UserType = 500
    };

    explicit AbstractResponseParser(const Type &type, const QString &id, const QVariantMap &data,
                                    QObject *parent);
    virtual ~AbstractResponseParser();

    Joschy::AbstractResponseParser::Type type() const;
    QString id() const;
    QVariantMap data() const;
    bool error() const;
    Joschy::Plugin::ErrorType errorType() const;
    QString errorString() const;


private:
    AbstractResponseParserPrivate *d;


protected:
    void setError(const bool &error);
    void setErrorType(const Joschy::Plugin::ErrorType &type);
    void setErrorString(const QString &errorString);


};


} // namespace Joschy


#endif // ABSTRACTRESPONSEPARSER_H
