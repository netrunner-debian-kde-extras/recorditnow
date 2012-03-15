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

#ifndef YOUTUBEPROVIDER_H
#define YOUTUBEPROVIDER_H


// own
#include "responseparser.h"

// Joschy
#include <joschycore/abstractprovider.h>
#include <joschycore/scheduler.h>

// Qt
#include <QtCore/QMultiHash>
#include <QtCore/QtPlugin>
#include <QtCore/QMutex>


namespace Joschy {


class AbstractJob;
class PostFile;
class YouTubeParserThread;
class JOSCHY_EXPORT YouTubeProvider : public Joschy::AbstractProvider
{
    Q_OBJECT


public:
    explicit YouTubeProvider(QObject *parent = 0);
    ~YouTubeProvider();

    QStringList categorys() const;
    bool isAuthenticated(const QString &login) const;

    Joschy::ActionReply authenticate(const QString &login, const QString &password);
    Joschy::ActionReply upload(const QString &login, Joschy::Video *video);
    Joschy::ActionReply search(const QHash<QString, QVariant> &data);
    Joschy::ActionReply updateThumbnail(const Joschy::Video &video, const QString &thumbnailDir);

    void init();


private:
    QHash<QString, QString> m_tokens;
    QHash<QString, ResponseParser::Type> m_actions;
    QHash<QString, Joschy::PostFile*> m_postFiles;
    QHash<QString, QString> m_thumbnails;
    QList<Joschy::AbstractJob*> m_parser;
    QHash<QString, QString> m_categorys;

    void updateCategorys();


private slots:
    void parserFinished(Joschy::AbstractJob *job);


protected slots:
    void jobFinished(const QString &id,
                     const QVariantMap &data,
                     const Joschy::Plugin::ErrorType &errorType = Joschy::Plugin::NoError,
                     const QString &errorString = QString());


};


} // namespace Joschy


Q_EXPORT_PLUGIN2(joschyyoutube, Joschy::YouTubeProvider);


#endif // YOUTUBEPROVIDER_H
