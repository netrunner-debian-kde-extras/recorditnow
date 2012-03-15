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
#include "youtubeprovider.h"
#include "responseparser.h"

// Joschy
#include <joschycore/abstractnetworklayer.h>
#include <joschycore/joschy_global.h>
#include <joschycore/video.h>
#include <joschycore/postfile.h>
#include <joschycore/abstractjob.h>

// Qt
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtCore/QDir>




#define DEV_KEY "AI39si4PPp_RmxGSVs4cHH93rcG2e9vSRQU1vC0L3sfuy_ZHmtaAWZOdvSfBjmow3YSZfrerx"\
                "jhsZGX0brUrdSLr5qvNchxeiQ"


namespace Joschy {


YouTubeProvider::YouTubeProvider(QObject *parent)
    :Joschy::AbstractProvider(parent)
{


}


YouTubeProvider::~YouTubeProvider()
{

    qDeleteAll(m_parser);

}


QStringList YouTubeProvider::categorys() const
{

/*    QStringList list;
    list << "Autos" << "Comedy" << "Education" << "Entertainment" << "Film";
    list << "Games" << "Howto" << "Music" << "News" << "Nonprofit";
    list << "People" << "Animals" << "Tech" << "Sports" << "Travel";

    return list;
*/
    return m_categorys.values();

}


bool YouTubeProvider::isAuthenticated(const QString &login) const
{

    return m_tokens.contains(login);

}


Joschy::ActionReply YouTubeProvider::authenticate(const QString &login, const QString &password)
{

    QString errorString;
    if (login.isEmpty()) {
        errorString = tr("Empty login");
    }

    if (password.isEmpty()) {
        errorString = tr("Empty password");
    }

    Joschy::ActionReply reply;
    if (!errorString.isEmpty()) {
        reply.setErrorType(Plugin::InvalidArgumentError);
        reply.setErrorString(errorString);
        return reply;
    }

    const QUrl url("https://www.google.com/youtube/accounts/ClientLogin");

    QHash<QByteArray, QByteArray> meta;
    meta.insert("Content-Type", "application/x-www-form-urlencoded");
    meta.insert("errorPage", "false");
    meta.insert("cookies", "none");

    const QByteArray data = "Email="+login.toLatin1()+"&Passwd="+password.toLatin1()+"&service="\
                            "youtube&source=libjoschyyoutube";

    const QString id = layer()->post(url, meta, data);
    m_actions.insert(id, ResponseParser::AuthenticationType);

    reply.setId(id);
    return reply;

}


Joschy::ActionReply YouTubeProvider::upload(const QString &login, Joschy::Video *video)
{

    Joschy::ActionReply reply;

    if (!isAuthenticated(login)) {
        reply.setErrorType(Plugin::NotAuthenticatedError);
        reply.setErrorString(tr("You need to authenticate first"));
    }

    QFile file(video->url().toString());
    if (!file.exists()) {
        reply.setErrorType(Plugin::FileNotFoundError);
        reply.setErrorString(tr("Video %1: No such file or directory").arg(video->url().toString()));
    }

    if (!file.open(QIODevice::ReadOnly)) {
        reply.setErrorType(Plugin::CannotOpenError);
        reply.setErrorString(file.errorString());
    }

    if (reply.error()) {
        return reply;
    }


    const bool bigVideo = (file.size() > (1024*1024)*10);
    const QString title = video->title();
    const QString description = video->description();
    const QString category = m_categorys.key(video->category());
    const QString keywords = video->keywords().join(", ");
    const QString slug = file.fileName().right(file.fileName().lastIndexOf(QDir::separator())+1);
    const QUrl url("http://uploads.gdata.youtube.com/feeds/api/users/"+login+"/uploads");
    const QByteArray mime = "application/octet-stream"; // TODO


    const QByteArray CRLF = "\r\n";
    const QByteArray BOUNDARY = "f93dcbA3";
    QString XML = "<?xml version=\"1.0\"?>"+CRLF+\
                     "<entry xmlns=\"http://www.w3.org/2005/Atom\""+CRLF+\
                       "xmlns:media=\"http://search.yahoo.com/mrss/\""+CRLF+\
                       "xmlns:yt=\"http://gdata.youtube.com/schemas/2007\">"+CRLF+\
                       "<media:group>"+CRLF+\
                         "<media:title type=\"plain\">%1</media:title>"+CRLF+\
                         "<media:description type=\"plain\">"+CRLF+\
                           "%2."+CRLF+\
                         "</media:description>"+CRLF+\
                         "<media:category"+CRLF+\
                           "scheme=\"http://gdata.youtube.com/schemas/2007/categories.cat\">%3"+CRLF+\
                         "</media:category>"+CRLF+\
                         "<media:keywords>%4</media:keywords>"+CRLF+\
                       "</media:group>"+CRLF+\
                     "</entry>";

    XML = XML.arg(title).arg(description).arg(category).arg(keywords);

    QByteArray postData;
    postData.append("--"+BOUNDARY);
    postData.append(CRLF);
    postData.append("Content-Type: application/atom+xml; charset=UTF-8");
    postData.append(CRLF);
    postData.append(CRLF);
    postData.append(XML.toLatin1());
    postData.append(CRLF);
    postData.append("--"+BOUNDARY);
    postData.append(CRLF);
    postData.append("Content-Type: "+mime);
    postData.append(CRLF);
    postData.append("Content-Transfer-Encoding: binary");
    postData.append(CRLF);
    postData.append(CRLF);

    QByteArray bottom = CRLF;
    bottom.append("--"+BOUNDARY+"--");
    bottom.append(CRLF);

    if (!bigVideo) {
        postData.append(file.readAll());
        postData.append(bottom);
    }
    file.close();

    QHash<QByteArray, QByteArray> header;
    header["Authorization"] = "GoogleLogin auth="+m_tokens[login].toLatin1();
    header["GData-Version"] = "2";
    header["X-GData-Key"] = "key="+QString(DEV_KEY).toLatin1();
    header["Connection"] = "close";
    header["Slug"] = slug.toLatin1();
    header["Content-Type"] = "multipart/related; boundary=\""+BOUNDARY+"\"";

    QString id;
    if (!bigVideo) {
        header["Content-Length"] = QString::number(postData.size()).toLatin1();

        id = layer()->post(url, header, postData);
    } else {
        Joschy::PostFile *data = new Joschy::PostFile(file.fileName());
        data->setData(postData, bottom);

        header["Content-Length"] = QString::number(data->size()).toLatin1();

        id = layer()->post(url, header, data);
        m_postFiles[id] = data;
    }
    m_actions.insert(id, ResponseParser::UploadType);
    reply.setId(id);

    return reply;

}


Joschy::ActionReply YouTubeProvider::search(const QHash<QString, QVariant> &data)
{

    Joschy::ActionReply reply;

    int start = 1;
    if (data.contains("Start")) {
        start = data.value("Start").toInt();
    }

    int max = 25;
    if (data.contains("Max")) {
        max = data.value("Max").toInt();
    }

    const QString key = data.value("Key").toString();
    const QString author = data.value("Author").toString();
    const QString category = data.value("Category").toString();

    if (start < 1 || max > 50 || key.isEmpty()) {
        reply.setErrorType(Plugin::InvalidArgumentError);
        reply.setErrorString(tr("Invalid argument"));
        return reply;
    }

    QUrl url("http://gdata.youtube.com/feeds/api/videos");
    url.addQueryItem("q", key);
    url.addQueryItem("start-index", QString::number(start));
    url.addQueryItem("max-results", QString::number(max));

    if (!author.isEmpty()) {
        url.addQueryItem("author", author);
    }

    if (!category.isEmpty()) {
        url.addQueryItem("category", category);
    }


    JOSCHY_DEBUG() << "url:" << url;

    reply.setId(layer()->get(url));

    m_actions[reply.id()] = ResponseParser::SearchType;

    return reply;

}


Joschy::ActionReply YouTubeProvider::updateThumbnail(const Joschy::Video &video, const QString &thumbnailDir)
{

    Joschy::ActionReply reply;

    if (video.thumbnailUrl().isEmpty()) {
        reply.setErrorType(Plugin::InvalidArgumentError);
        reply.setErrorString(tr("No thumbnail url given"));
        return reply;
    }

    QString dir = QDir::cleanPath(thumbnailDir);
    if (!dir.endsWith(QDir::separator())) {
        dir.append(QDir::separator());
    }

    const QString id = layer()->get(video.thumbnailUrl());
    m_actions[id] = ResponseParser::UpdateThumbnailType;
    m_thumbnails[id] = dir+video.thumbnail();

    reply.setId(id);

    return reply;

}


void YouTubeProvider::init()
{

    QVariant var = load("YouTube-Categorys");
    QHashIterator<QString, QVariant> it(var.toHash());
    while (it.hasNext()) {
        it.next();
        m_categorys[it.key()] = it.value().toString();
    }

    var = load("YouTube-CategoryDate");
    QDate date = var.toDateTime().date();

    if (m_categorys.isEmpty() || date.month() != QDate::currentDate().month()) { // update every month
        JOSCHY_DEBUG() << "update categorys....";
        updateCategorys();
    }

}


void YouTubeProvider::updateCategorys()
{

    const QUrl url("http://gdata.youtube.com/schemas/2007/categories.cat");

    QHash<QByteArray, QByteArray> header;
    header["Accept-Language"] = QLocale::system().name().toLatin1()+";";

    const QString id = layer()->get(url, header);
    m_actions[id] = ResponseParser::UpdateCategorysType;

}


void YouTubeProvider::parserFinished(Joschy::AbstractJob *job)
{

    JOSCHY_DEBUG() << "parser finsihed....";

    Joschy::ResponseParser *parser = static_cast<Joschy::ResponseParser*>(job);
    const QString id = parser->id();
    const ResponseParser::Type type = m_actions.take(id);
    const QString errorString = parser->errorString();
    const Plugin::ErrorType errorType = parser->errorType();
    const bool hasError = parser->error();


    if (hasError) {
        emit error(id, errorType, errorString);
    } else {
        switch (type) {
        case ResponseParser::AuthenticationType: {
                m_tokens.insert(parser->login(), parser->token());
                emit authenticated(id);
                break;
            }
        case ResponseParser::UploadType: {
                emit uploadFinished(id, parser->getVideo());
                break;
            }
        case ResponseParser::SearchType: {
                QList<Joschy::Video> videos = parser->getVideos();
                emit searchFinished(id, videos);
                break;
            }
        case ResponseParser::UpdateThumbnailType: {
                const QString thumbnail = m_thumbnails.take(id);

                QFile file(thumbnail);
                if (!file.open(QIODevice::WriteOnly)) {
                    JOSCHY_DEBUG() << "open failed:" << thumbnail << file.errorString();
                    emit error(id, Plugin::CannotOpenError, file.errorString());
                } else {
                    if (file.write(parser->image()) == -1) {
                        file.close();
                        JOSCHY_DEBUG() << file.error() << file.errorString();
                        emit error(id, Plugin::UnknownError, file.errorString());
                    } else {
                        file.close();
                        emit thumbnailUpdated(id);
                    }
                }
                break;
            }
        case ResponseParser::UpdateCategorysType: {
                m_categorys = parser->getCategorys();

                QHash<QString, QVariant> hash;
                QHashIterator<QString, QString> it(m_categorys);
                while (it.hasNext()) {
                    it.next();
                    hash[it.key()] = it.value();
                }

                save("YouTube-Categorys", QVariant(hash));
                save("YouTube-CategoryDate", QVariant(QDateTime::currentDateTime()));

                emit categorysChanged(categorys());
                break;
            }
        default: break;
        }
    }
    m_parser.removeAll(parser);
    delete parser;
    layer()->freeId(id);

}


void YouTubeProvider::jobFinished(const QString &id, const QVariantMap &data,
                                  const Joschy::Plugin::ErrorType &errorType,
                                  const QString &errorString)
{

    JOSCHY_DEBUG() << "job finished";
    JOSCHY_DEBUG() << "Reply:" << data.value("Reply").toString();
    JOSCHY_DEBUG() << "Content type:" << data.value("ContentType").toString();
    JOSCHY_DEBUG() << "Status:" << data.value("Status").toString();
    JOSCHY_DEBUG() << "error" << bool(errorType != Plugin::NoError);

    if (m_postFiles.contains(id)) {
        Joschy::PostFile *file = m_postFiles.take(id);
        delete file;
    }

    if (data.value("Canceled").toBool() || errorType == Joschy::Plugin::ActionCanceledError) {
        m_actions.remove(id);
        emit error(id, Plugin::ActionCanceledError, QString());
        return;
    }

/*    if (errorType != Joschy::Plugin::NoError) {
        m_actions.remove(id);
        emit error(id, errorType, errorString);
        return;
    }
*/
    Joschy::ResponseParser *parser = new Joschy::ResponseParser(m_actions.value(id), id, data, this);
    connect(parser, SIGNAL(finished(Joschy::AbstractJob*)), this, SLOT(parserFinished(Joschy::AbstractJob*)));

    m_parser.append(parser);
    switch (m_actions.value(id)) {
    case ResponseParser::SearchType: parser->setRunInThread(true); break;
    case ResponseParser::UpdateThumbnailType: parser->setPriority(AbstractJob::LowPriority); break;
    case ResponseParser::UpdateCategorysType: parser->setPriority(AbstractJob::HighPriority); break;
    default: break;
    }

    Joschy::Scheduler::schedule(parser);

}



} // namespace Joschy


#include "youtubeprovider.moc"
