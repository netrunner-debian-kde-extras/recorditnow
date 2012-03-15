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
#include <joschycore/joschy_global.h>

// Qt
#include <QtXml/QXmlStreamReader>
#include <QtCore/QDateTime>
#include <QtCore/QStringList>


namespace Joschy {


ResponseParser::ResponseParser(const Type &type, const QString &id, const ActionData &data,
                               QObject *parent)
                                   : Joschy::AbstractResponseParser(type, id, data.data, parent)
{

    m_data = data;

}


ResponseParser::~ResponseParser()
{



}


Joschy::ResponseParser::ActionData ResponseParser::actionData() const
{

    return m_data;

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


QByteArray ResponseParser::image() const
{

    return m_image;

}


QHash<QString, QString> ResponseParser::getCategorys() const
{

    return m_categorys;

}


void ResponseParser::parseSearch(const QVariantMap &data)
{

    const QString contentType = data.value("ContentType").toString();
    const int code = data.value("Status").toInt();

    if (code == 200) { // success
        QXmlStreamReader reader(data.value("Reply").toByteArray());
        while (!reader.atEnd()) {
            reader.readNext();
            if (reader.name() == "item") {
                readEntry(&reader);
            }
        }
    } else { // TODO
        JOSCHY_DEBUG() << "ERROR:" << code << contentType << data.value("Reply");
        setError(true);
        setErrorType(Plugin::UnknownError);
    }

}


void ResponseParser::parseUpload(const QVariantMap &data)
{

    const QString contentType = data.value("ContentType").toString();
    const int code = data.value("Status").toInt();

    if (code == 200) { // success

        Joschy::Video video;
        QXmlStreamReader reader(data.value("Reply").toByteArray());
        while (!reader.atEnd()) {
            reader.readNext();

            if (reader.error()) {
                JOSCHY_DEBUG() << "reader error...";
                break;
            }

            if (reader.isStartElement() && reader.name() == "post_url") {
                video.setUrl(reader.readElementText());
            } else {
                JOSCHY_DEBUG() << reader.name();
            }
        }
        m_videos.append(video);
    } else {  // TODO
        JOSCHY_DEBUG() << "ERROR:" << code << contentType << data.value("Reply");
        setError(true);
        setErrorType(Plugin::UnknownError);
    }

}


void ResponseParser::parseThumbnail(const QVariantMap &data)
{

    const QString contentType = data.value("ContentType").toString();
    const int code = data.value("Status").toInt();

    if (code == 200) {
        m_image = data.value("Reply").toByteArray();
    } else {
        setError(true);
        setErrorType(Plugin::UnknownError);
    }

}


void ResponseParser::parseCategorys(const QVariantMap &data)
{

    QXmlStreamReader reader(data.value("Reply").toByteArray());
    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.name() == QLatin1String("category") && reader.isStartElement()) {
            QString id, name;
            while (true) {
                if (reader.name() == QLatin1String("category") && reader.isEndElement()) {
                    break;
                }
                reader.readNext();

                if (reader.name() == QLatin1String("id")) {
                    id = reader.readElementText();
                } else if (reader.name() == QLatin1String("name")) {
                    name = reader.readElementText();
                }
            }

            if (!name.isEmpty() && !id.isEmpty()) {
                m_categorys[id] = name;
            }
        }
    }

    if (!error()) {
        setError(reader.hasError());
    }

    if (error()) {
        setErrorType(Joschy::Plugin::UnknownError);
    }

}


void ResponseParser::readEntry(QXmlStreamReader *reader)
{

    Joschy::Video video;

    reader->readNext();
    while (reader->readNext() != QXmlStreamReader::Invalid) {
        if (reader->isEndElement()) {
            break;
        } else if (reader->isStartElement()) {
            break;
        }
    }

    while (reader->name() != "item") {
        reader->readNext();

        if (reader->name().isEmpty() || reader->isEndElement()) {
            continue;
        }

        if (reader->name() == "title" && reader->prefix().isEmpty()) {
            video.setTitle(reader->readElementText().trimmed());
            JOSCHY_DEBUG() << "title:" << video.title();
        } else if (reader->name() == "user" && reader->prefix() == "blip") {
            video.setAuthor(reader->readElementText().trimmed());
            JOSCHY_DEBUG() << "Author:" << video.author();
        } else if (reader->name() == "rating" && reader->prefix() == "blip") {
            video.setRating(reader->readElementText().toDouble());
            JOSCHY_DEBUG() << "Rating:" << video.rating();
        } else if (reader->name() == "license" && reader->prefix() == "blip") {
            video.setProperty("License", reader->readElementText().trimmed());
            JOSCHY_DEBUG() << "License:" << video.property("License");
        } else if (reader->name() == "runtime" && reader->prefix() == "blip") {
            video.setDuration(reader->readElementText().toLongLong());
            JOSCHY_DEBUG() << "Duration:" << video.duration();
        } else if (reader->name() == "content" && reader->prefix() == "media") {
            if (reader->attributes().value("isDefault").toString() == "true") {
                video.setProperty("Download", QUrl(reader->attributes().value("url").toString()));
                JOSCHY_DEBUG() << "Download:" << video.property("Download");
            }
        } else if (reader->name() == "player" && reader->prefix() == "media") {
            video.setUrl(QUrl(reader->attributes().value("url").toString()));
            JOSCHY_DEBUG() << "Url:" << video.url();
        } else if (reader->name() == "puredescription" && reader->prefix() == "blip") {
            video.setDescription(reader->readElementText().trimmed());
            JOSCHY_DEBUG() << "Description:" << video.description();
        } else if (reader->name() == "pubDate") {
            QString string = reader->readElementText().remove(" +0000");

            #warning "find a better way!?"
            string.replace("Mon", QDate::shortDayName(1, QDate::DateFormat));
            string.replace("Tue", QDate::shortDayName(2, QDate::DateFormat));
            string.replace("Wed", QDate::shortDayName(3, QDate::DateFormat));
            string.replace("Thu", QDate::shortDayName(4, QDate::DateFormat));
            string.replace("Fri", QDate::shortDayName(5, QDate::DateFormat));
            string.replace("Sat", QDate::shortDayName(6, QDate::DateFormat));
            string.replace("Sun", QDate::shortDayName(7, QDate::DateFormat));

            string.replace("Jan", QDate::shortMonthName(1, QDate::DateFormat));
            string.replace("Feb", QDate::shortMonthName(2, QDate::DateFormat));
            string.replace("Mar", QDate::shortMonthName(3, QDate::DateFormat));
            string.replace("Apr", QDate::shortMonthName(4, QDate::DateFormat));
            string.replace("May", QDate::shortMonthName(5, QDate::DateFormat));
            string.replace("Jun", QDate::shortMonthName(6, QDate::DateFormat));
            string.replace("Jul", QDate::shortMonthName(7, QDate::DateFormat));
            string.replace("Aug", QDate::shortMonthName(8, QDate::DateFormat));
            string.replace("Sep", QDate::shortMonthName(9, QDate::DateFormat));
            string.replace("Oct", QDate::shortMonthName(10, QDate::DateFormat));
            string.replace("Nov", QDate::shortMonthName(11, QDate::DateFormat));
            string.replace("Dec", QDate::shortMonthName(12, QDate::DateFormat));

            video.setPublished(QDateTime::fromString(string, "ddd, dd MMM yyyy HH:mm:ss"));
            JOSCHY_DEBUG() << "string:" << string << "=" << video.published();
        } else if (reader->name() == "thumbnail") {
            video.setThumbnailUrl(QUrl(reader->attributes().value("url").toString()));
            JOSCHY_DEBUG() << "Thumbnail:" << video.thumbnailUrl();
        } else if (reader->name() == "category") {
            if (video.category().isEmpty()) {
                video.setCategory(reader->readElementText());
                JOSCHY_DEBUG() << "Category:" << video.category();
            } else {
                QString keys = video.keywords().join(", ");
                if (!keys.isEmpty()) {
                    keys.append(", ");
                }
                keys.append(reader->readElementText());
                video.setKeywords(keys.split(", "));
            }
        }
    }
    JOSCHY_DEBUG() << "Keywords:" << video.keywords();

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


void ResponseParser::parseError(const QByteArray &response)
{

    const QRegExp errorRX("=.*$");
    errorRX.indexIn(response);

    setErrorString(errorRX.cap().remove(0, 1));
    setError(true);
    setErrorType(Plugin::UnknownError); // TODO

}


void ResponseParser::startJob()
{

    switch(type()) {
    case ResponseParser::SearchType: parseSearch(data()); break;
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


