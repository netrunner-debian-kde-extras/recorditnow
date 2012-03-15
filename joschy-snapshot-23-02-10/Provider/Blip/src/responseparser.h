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

#ifndef BLIPRESPONSEPARSER_H
#define BLIPRESPONSEPARSER_H


// Joschy
#include <joschycore/abstractresponseparser.h>
#include <joschycore/video.h>
#include <joschycore/plugin.h>

// Qt
#include <QtCore/QByteArray>


class QXmlStreamReader;
namespace Joschy {


class PostFile;
class ResponseParser: public Joschy::AbstractResponseParser
{


public:
    struct ActionData {
        QString thumbnail;
        Joschy::PostFile *postFile;
        QVariantMap data;
        Joschy::Video video;
        ResponseParser::Type type;
    };
    explicit ResponseParser(const Type &type, const QString &id, const ActionData &data, QObject *parent);
    ~ResponseParser();

    Joschy::ResponseParser::ActionData actionData() const;
    Joschy::Video getVideo() const;
    QList<Joschy::Video> getVideos() const;
    QByteArray image() const;
    QHash<QString, QString> getCategorys() const;

    void parseSearch(const QVariantMap &data);
    void parseUpload(const QVariantMap &data);
    void parseThumbnail(const QVariantMap &data);
    void parseCategorys(const QVariantMap &data);


private:
    QList<Joschy::Video> m_videos;
    QByteArray m_image;
    ActionData m_data;
    QHash<QString, QString> m_categorys;


    void readEntry(QXmlStreamReader *reader);
    void readErrors(QXmlStreamReader *reader);
    void parseError(const QByteArray &response);


protected:
    void startJob();
    bool cancelJob();


};


} // namespace Joschy



#endif // BLIPRESPONSEPARSER_H
