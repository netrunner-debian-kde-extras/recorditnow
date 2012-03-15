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

#ifndef RESPONSEPARSER_H
#define RESPONSEPARSER_H


// Joschy
#include <joschycore/video.h>
#include <joschycore/plugin.h>
#include <joschycore/abstractresponseparser.h>

// Qt
#include <QtCore/QByteArray>


class QXmlStreamReader;
namespace Joschy {


class ResponseParser : public Joschy::AbstractResponseParser
{
    Q_OBJECT


public:
    explicit ResponseParser(const Type &type, const QString &id, const QVariantMap &data,
                                    QObject *parent = 0);

    Joschy::Video getVideo() const;
    QList<Joschy::Video> getVideos() const;
    QString token() const;
    QString login() const;
    QByteArray image() const;
    QHash<QString, QString> getCategorys() const;

    void parseLogin(const QVariantMap &data);
    void parseSearch(const QVariantMap &data);
    void parseUpload(const QVariantMap &data);
    void parseThumbnail(const QVariantMap &data);
    void parseCategorys(const QVariantMap &data);


private:
    QString m_login;
    QString m_token;
    QList<Joschy::Video> m_videos;
    QByteArray m_image;
    QHash<QString, QString> m_categorys;


    void parseXML(const QByteArray &response);
    void readEntry(QXmlStreamReader *reader);
    void readErrors(QXmlStreamReader *reader);
    void parseAuth(const QByteArray &response);
    void parseError(const QByteArray &response);
    void parseErrorReply(const QString &reply);
    void parseAdditionalErrorCodes(const int &code);



protected:
    void startJob();
    bool cancelJob();


};


} // namespace Joschy



#endif // RESPONSEPARSER_H
