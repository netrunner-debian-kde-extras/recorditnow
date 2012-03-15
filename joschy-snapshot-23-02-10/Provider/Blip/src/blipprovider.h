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

#ifndef BLIPPROVIDER_H
#define BLIPPROVIDER_H


// own
#include "responseparser.h"

// Joschy
#include <joschycore/abstractprovider.h>

// Qt
#include <QtCore/QMultiHash>
#include <QtCore/QtPlugin>


namespace Joschy {


class AbstractJob;
class PostFile;
class JOSCHY_EXPORT BlipProvider : public Joschy::AbstractProvider
{
    Q_OBJECT


public:
    explicit BlipProvider(QObject *parent = 0);
    ~BlipProvider();

    QStringList categorys() const;
    bool isAuthenticated(const QString &login) const;

    Joschy::ActionReply authenticate(const QString &login, const QString &password);
    Joschy::ActionReply upload(const QString &login, Joschy::Video *video);
    Joschy::ActionReply search(const QHash<QString, QVariant> &data);
    Joschy::ActionReply updateThumbnail(const Joschy::Video &video, const QString &thumbnailDir);

    void init();


private:
    QHash<QString, ResponseParser::ActionData> m_data;
    QHash<QString, QString> m_logins;
    QStringList m_tmpIds;
    QList<Joschy::AbstractResponseParser*> m_parser;
    QHash<QString, QString> m_categorys;

    void updateCategorys();


private slots:
    void parserFinished(Joschy::AbstractJob *job);
    void finishedAuth();


protected slots:
    void jobFinished(const QString &id,
                     const QVariantMap &data,
                     const Joschy::Plugin::ErrorType &errorType = Joschy::Plugin::NoError,
                     const QString &errorString = QString());


};


} // namespace Joschy


Q_EXPORT_PLUGIN2(joschyblip, Joschy::BlipProvider);


#endif // BLIPPROVIDER_H
