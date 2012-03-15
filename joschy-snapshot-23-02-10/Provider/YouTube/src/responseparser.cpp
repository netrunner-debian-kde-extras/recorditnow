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
#include "responseparser.h"

// Joschy
#include <joschycore/joschy_global.h>

// Qt
#include <QtXml/QXmlStreamReader>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>
#include <QTimer>

namespace Joschy {


ResponseParser::ResponseParser(const Type &type, const QString &id, const QVariantMap &data, QObject *parent)
    : Joschy::AbstractResponseParser(type, id, data, parent)
{


}


Joschy::Video ResponseParser::getVideo() const
{

    if (!m_videos.isEmpty()) {
        return m_videos.first();
    } else {
        return Joschy::Video();
    }

}


QList<Joschy::Video> ResponseParser::getVideos() const
{

    return m_videos;

}


QString ResponseParser::token() const
{

    return m_token;

}


QString ResponseParser::login() const
{

    return m_login;

}


QByteArray ResponseParser::image() const
{

    return m_image;

}


QHash<QString, QString> ResponseParser::getCategorys() const
{

    return m_categorys;

}


void ResponseParser::parseLogin(const QVariantMap &data)
{

    const int code = data.value("Status").toInt();

    if (code == 200) { // success
        parseAuth(data.value("Reply").toByteArray());
    } else if (code == 403) { // failure with an explanatory error code
        parseErrorReply(data.value("Reply").toString());
    } else {
        parseAdditionalErrorCodes(code);
        if (!error()) {
            setError(true);
            setErrorType(Plugin::UnknownError);
        }
    }

}


void ResponseParser::parseSearch(const QVariantMap &data)
{

    const int code = data.value("Status").toInt();

    if (code == 200) { // success
        parseXML(data.value("Reply").toByteArray());
    } else if (code == 400) { // bad request
        setError(true);
        setErrorType(Plugin::InvalidArgumentError);
    } else {
        parseAdditionalErrorCodes(code);
        if (!error()) {
            setError(true);
            setErrorType(Plugin::UnknownError);
        }
    }

}


void ResponseParser::parseUpload(const QVariantMap &data)
{

    const int code = data.value("Status").toInt();

    if (code == 201) { // success
        parseXML(data.value("Reply").toByteArray());
    } else if (code == 400) { // bad request
        setError(true);
        setErrorType(Plugin::InvalidArgumentError);
    } else {
        parseAdditionalErrorCodes(code);
        if (!error()) {
            setError(true);
            setErrorType(Plugin::UnknownError);
        }
    }

}


void ResponseParser::parseThumbnail(const QVariantMap &data)
{

    const int code = data.value("Status").toInt();

    if (code == 200) {
        m_image = data.value("Reply").toByteArray();
    } else {
        parseAdditionalErrorCodes(code);
        if (!error()) {
            setError(true);
            setErrorType(Plugin::UnknownError);
        }
    }

}


void ResponseParser::parseCategorys(const QVariantMap &data)
{

    QXmlStreamReader reader(data.value("Reply").toByteArray());
    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.name() == QLatin1String("category") && reader.isStartElement()) {
            bool deprecated = false;
            const QString term = reader.attributes().value("term").toString();
            const QString label = reader.attributes().value("label").toString();
            while (true) {
                if (reader.name() == QLatin1String("category") && reader.isEndElement()) {
                    break;
                }

                if (reader.name() == QLatin1String("deprecated")) {
                    deprecated = true;
                    break;
                }
                reader.readNext();
            }

            if (!deprecated) {
                m_categorys[term] = label;
            }
        }
    }

    if (!error()) {
        setError(reader.hasError());
    }

    if (error()) {
        setErrorType(Plugin::UnknownError);
    }

    JOSCHY_DEBUG() << "Categorys:" << m_categorys;

}


void ResponseParser::parseXML(const QByteArray &response)
{

    QXmlStreamReader reader(response);
    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.error()) {
            JOSCHY_DEBUG() << "reader error...";
            break;
        }

        if (reader.isStartElement() && reader.name() == "entry") {
            JOSCHY_DEBUG() << "entry found....";
            readEntry(&reader);
        } else if (reader.isStartElement() && reader.name() == "errors") {
            JOSCHY_DEBUG() << "error found";
            readErrors(&reader);
        }
    }

    if (!error()) {
        setError(reader.hasError());
    }

    if (error()) {
        setErrorType(Plugin::UnknownError);
    }

    JOSCHY_DEBUG() << "Found:" << m_videos.size() << "Videos" << "Error:" << error();


}


void ResponseParser::readEntry(QXmlStreamReader *reader)
{

    Joschy::Video video;
    while (true) {
        reader->readNext();
        if (reader->name() == "entry" && reader->isEndElement()) {
            break;
        }

        if (reader->error()) {
            JOSCHY_DEBUG() << "reader error.....";
            break;
        }


        if (reader->isStartElement()) {
            if (reader->name() == "link" &&
                reader->attributes().value("rel").toString() == "alternate" &&
                reader->attributes().value("type").toString() == "text/html")
            { // URL
                const QUrl url = reader->attributes().value("href").toString();
                video.setUrl(url);
            } else if (reader->name() == "author") { // Author
                reader->readNext();
                if (reader->name() == "name") {
                    const QString author = reader->readElementText();
                    video.setAuthor(author);
                }
            } else if (reader->name() == "published") { // Published
               const QDateTime published = QDateTime::fromString(reader->readElementText(), Qt::ISODate);
               video.setPublished(published);
            } else if (reader->namespaceUri() == "http://gdata.youtube.com/schemas/2007" &&
                       reader->name() == "statistics") { // Statistics
                const qlonglong viewCount = reader->attributes().value("viewCount").toString().toLongLong();
                video.setViewCount(viewCount);
                const int favoriteCount = reader->attributes().value("favoriteCount").toString().toInt();
            } else if (reader->namespaceUri() == "http://search.yahoo.com/mrss/" &&
                       reader->name() == "group") { // media group
                while (true) {
                    reader->readNext();

                    if (reader->isEndElement() && reader->name() == "group") {
                        break;
                    }

                    if (reader->error()) {
                        JOSCHY_DEBUG() << "reader error in media group....";
                        break;
                    }

                    if (reader->isStartElement()) {
                        if (reader->name() == "thumbnail") { // Thumbnail
                            const QUrl url(reader->attributes().value("url").toString());
                            video.setThumbnailUrl(url);
                        } else if (reader->name() == "title") { // Title
                            const QString title = reader->readElementText();
                            video.setTitle(title);
                        } else if (reader->name() == "description") { // Description
                            const QString description = reader->readElementText();
                            video.setDescription(description);
                        } else if (reader->name() == "duration") { // Duration
                            const qlonglong duration = reader->attributes().value("seconds").toString().toLongLong();
                            video.setDuration(duration);
                        } else if (reader->name() == "keywords") { // Keywords
                            QStringList keywords;
                            foreach (const QString &keyword, reader->readElementText().split(',')) {
                                keywords.append(keyword.trimmed());
                            }
                            video.setKeywords(keywords);
                        } else if (reader->name() == "category") { // Category
                            const QString category = reader->readElementText();
                            video.setCategory(category);
                        }
                    } else {
                       // JOSCHY_DEBUG("Unkown/Unused media element:" << reader->name();
                    }
                }
            } else if (reader->name() == "rating") { // rating
                const double rating = reader->attributes().value("average").toString().toDouble();
                video.setRating(rating);
                const qlonglong raters = reader->attributes().value("numRaters").toString().toLongLong();
                video.setRaters(raters);
            } else if (reader->name() == "updated") { // Updated
                const QDateTime updated = QDateTime::fromString(reader->readElementText(), Qt::ISODate);
                video.setUpdated(updated);
            } else {
               // JOSCHY_DEBUG("Unkown/Unused element:" << reader->name();
            }
        }
    }
    m_videos.append(video);

}


void ResponseParser::readErrors(QXmlStreamReader *reader)
{

    while (!reader->atEnd()) {
        reader->readNext();

        if (reader->isEndElement() && reader->name() == "errors") {
            break;
        }

        if (reader->isStartElement() && reader->name() == "error") {
            reader->readNext();
            while (reader->name() != "error") {
                if (reader->name() == "domain") {
                    const QString domain = reader->readElementText();
                    JOSCHY_DEBUG() << "Domain:" << domain;
                } else if (reader->name() == "code") {
                    const QString code = reader->readElementText();
                    JOSCHY_DEBUG() << "Code:" << code;
                    if (code == QLatin1String("required")) {
                        setErrorType(Plugin::InvalidArgumentError);
                    }
                } else if (reader->name() == "location") {
                    const QString location = reader->readElementText();
                    JOSCHY_DEBUG() << "Location:" << location;
                }
                reader->readNext();
            }
        }
    }

}


void ResponseParser::parseAuth(const QByteArray &response)
{

    if (response.startsWith("Auth=")) {
        const QString reply = response.trimmed();
        const QRegExp loginRX("YouTubeUser=.*");
        const QRegExp tokenRX("Auth=.*YouTubeUser=");

        loginRX.indexIn(reply);
        tokenRX.indexIn(reply);

        QString login = loginRX.cap();
        login = login.trimmed();
        login.remove(0, 12); // YouTubeUser=
        login.remove(tokenRX);

        QString token = tokenRX.cap();
        token.remove(loginRX);
        token = token.trimmed();
        token.remove(0, 5); // Auth=

        m_login = login;
        m_token = token;
    } else {
        setError(true);
        setErrorType(Joschy::Plugin::UnknownError);
    }

}


void ResponseParser::parseError(const QByteArray &response)
{

    const QRegExp errorRX("=.*$");
    errorRX.indexIn(response);

    setErrorString(errorRX.cap().remove(0, 1));
    setError(true);
    setErrorType(Plugin::UnknownError); // TODO

}


void ResponseParser::parseErrorReply(const QString &reply)
{

    setError(true);
    const QRegExp errorRX("^.*=");

    QString error = reply;
    error = error.remove(errorRX).trimmed();
    if (error == "BadAuthentication") {
        setErrorType(Plugin::InvalidArgumentError);
    } else if (error == "NotVerified") {
        setErrorType(Plugin::NotVerifiedError);
    } else if (error == "TermsNotAgreed") {
        setErrorType(Plugin::TermsNotAgreedError);
    } else if (error == "CaptchaRequired") {
        setErrorType(Plugin::ServiceError);
    } else if (error == "Unknown") {
        setErrorType(Plugin::UnknownError);
    } else if (error == "AccountDeleted") {
        setErrorType(Plugin::AccountDeletedError);
    } else if (error == "AccountDisabled") {
        setErrorType(Plugin::AccountDisabledError);
    } else if (error == "ServiceDisabled") {
        setErrorType(Plugin::ServiceDisabledError);
    } else if (error == "ServiceUnavailable") {
        setErrorType(Plugin::ServiceUnavailableError);
    } else {
        setErrorType(Plugin::UnknownError);
    }

}


void ResponseParser::parseAdditionalErrorCodes(const int &code)
{

    if (code == 401) {
        // (Not authorized) A 401 response code indicates that a request
        // did not contain an Authorization header, that the format of
        // the Authorization header was invalid, or that the authentication
        // token supplied in the header was invalid.
        setError(true);
        setErrorType(Plugin::NotAuthenticatedError);
    } else if (code == 403) {
        // 403 (Forbidden) - A 403 response code indicates that you have
        // submitted a request that is not properly authenticated for the
        // requested operation.
        setError(true);
        setErrorType(Plugin::InternalError);
    } else if (code == 500) {
        // (Internal error) - A 500 response code indicates that YouTube
        // experienced an error handling a request. You could retry the
        // request at a later time.
        setError(true);
        setErrorType(Plugin::ServiceError);
    } else if (code == 501) {
        // (Not implemented) - A 501 response code indicates that you have tried
        // to execute an unsupported operation, such as retrieving a list of ratings
        // or updating a complaint.
        setError(true);
        setErrorType(Plugin::InternalError);
    } else if (code == 503) {
        // (Service unavailable) - A 503 response code indicates that the YouTube Data
        // API service can not be reached. You could retry your request at a later time.
        setError(true);
        setErrorType(Plugin::ServiceUnavailableError);
    }

}


void ResponseParser::startJob()
{

    switch(type()) {
    case ResponseParser::SearchType: parseSearch(data()); break;
    case ResponseParser::AuthenticationType: parseLogin(data()); break;
    case ResponseParser::UploadType: parseUpload(data()); break;
    case ResponseParser::UpdateThumbnailType: parseThumbnail(data()); break;
    case ResponseParser::UpdateCategorysType: parseCategorys(data()); break;
    default: break;
    }

    jobDone();

}


bool ResponseParser::cancelJob()
{

    return false;

}


} // namespace Joschy


#include "responseparser.moc"

