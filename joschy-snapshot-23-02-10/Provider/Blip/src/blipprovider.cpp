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
#include "blipprovider.h"
#include "responseparser.h"

// Joschy
#include <joschycore/abstractnetworklayer.h>
#include <joschycore/joschy_global.h>
#include <joschycore/video.h>
#include <joschycore/postfile.h>
#include <joschycore/scheduler.h>
#include <joschycore/abstractjob.h>

// Qt
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTimer>


namespace Joschy {


BlipProvider::BlipProvider(QObject *parent)
    : Joschy::AbstractProvider(parent)
{


}


BlipProvider::~BlipProvider()
{

    qDeleteAll(m_parser);

}


QStringList BlipProvider::categorys() const
{
/*
    QStringList list;
    list << "Art" << "Autos & Vehicles" << "Business" << "Citizen Journalism";
    list << "Comedy" << "Conferences and other Events" << "Default Category" << "Documentary";
    list << "Educational" << "Food & Drink" << "Friends" << "Gaming" << "Health";
    list << "Literature" << "Movies and Television" << "Music and Entertainment";
    list << "Personal or Auto-biographical" << "Politics" << "Religion" << "School and Education";
    list << "Science" << "Sports" << "Technology" << "The Environment";
    list << "The Mainstream Media" << "Travel" << "Videoblogging" <<"Web Development and Sites";
*/
    return m_categorys.values();

}



bool BlipProvider::isAuthenticated(const QString &login) const
{

    return m_logins.contains(login);

}


Joschy::ActionReply BlipProvider::authenticate(const QString &login, const QString &password)
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

    m_logins[login] = password;
    reply.setId(addUniqueId());
    m_tmpIds.append(reply.id());

    QTimer::singleShot(100, this, SLOT(finishedAuth()));

    return reply;

}


Joschy::ActionReply BlipProvider::upload(const QString &login, Joschy::Video *video)
{

    Joschy::ActionReply reply;

    if (!isAuthenticated(login)) {
        reply.setErrorType(Plugin::NotAuthenticatedError);
        reply.setErrorString(tr("You need to authenticate first"));
    }

    QFile f(video->url().toString());
    if (!f.exists()) {
        reply.setErrorType(Plugin::FileNotFoundError);
        reply.setErrorString(tr("Video %1: No such file or directory").arg(video->url().toString()));
    }

    if (!f.open(QIODevice::ReadOnly)) {
        reply.setErrorType(Plugin::CannotOpenError);
        reply.setErrorString(f.errorString());
    } else {
        f.close();
    }

    if (reply.error()) {
        return reply;
    }


    const QUrl url("http://blip.tv/file/post");

    QByteArray BOUNDARY = "-----------$$SomeFancyBoundary$$";
    QByteArray CRLF = "\r\n";


    QHash<QString, QString> fields;
    fields["post"] = '1';
    fields["skin"] = "xmlhttprequest";
    fields["file_role"] = "Web";
    fields["item_type"] = "file";

    fields["userlogin"] = login;
    fields["password"] = m_logins.value(login);
    fields["title"] = video->title();
    fields["description"] = video->description();
    fields["topics"] = video->keywords().join(", ");
    fields["categories_id"] = m_categorys.key(video->category());
   // header["license"] = video->getLicenseCode();


    const QString slug = GET_FILE(video->url().toString());
    QByteArray top = CRLF;

    QHashIterator<QString, QString> it(fields);
    while (it.hasNext()) {
        it.next();
        top.append("--"+BOUNDARY);
        top.append(CRLF);
        top.append(QString("Content-Disposition: form-data; name=\"%1\"").arg(it.key()).toLatin1());
        top.append(CRLF);
        top.append(CRLF);
        top.append(it.value().toLatin1());
        top.append(CRLF);
    }

    top.append("--"+BOUNDARY);
    top.append(CRLF);
    top.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\"%2\"").arg(slug).toLatin1());
    top.append(CRLF);
    top.append("Content-Type: application/octet-stream");
    top.append(CRLF);
    top.append(CRLF);

    QByteArray bottom;
    bottom.append(CRLF);
    bottom.append("--" + BOUNDARY + "--");
    bottom.append(CRLF);

    Joschy::PostFile *file = new Joschy::PostFile(video->url().toLocalFile());
    file->setData(top, bottom);

    QHash<QByteArray, QByteArray> header;
    header["Content-type"] = "multipart/form-data; boundary="+BOUNDARY;
    header["content-length"] = QString::number(file->size()).toLatin1();

    const QString id = layer()->post(url, header, file);

    ResponseParser::ActionData data;
    data.postFile = file;
    data.type = ResponseParser::UploadType;
    data.video = *video;
    m_data[id] = data;

    reply.setId(id);

    return reply;

}


Joschy::ActionReply BlipProvider::search(const QHash<QString, QVariant> &data)
{

    Joschy::ActionReply reply;

    const QString term = data.value("Key").toString();
    if (term.isEmpty()) {
        reply.setErrorType(Plugin::InvalidArgumentError);
        return reply;        
    }

    QUrl url("http://www.blip.tv/search/");
    url.addQueryItem("search", term);
    url.addQueryItem("skin", "rss");

    ResponseParser::ActionData aData;
    aData.postFile = 0;
    aData.type = ResponseParser::SearchType;

    const QString id = layer()->get(url);
    m_data[id] = aData;

    reply.setId(id);

    return reply;

}


Joschy::ActionReply BlipProvider::updateThumbnail(const Joschy::Video &video, const QString &thumbnailDir)
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

    ResponseParser::ActionData data;
    data.postFile = 0;
    data.thumbnail = dir+video.thumbnail();
    data.type = ResponseParser::UpdateThumbnailType;
    m_data[id] = data;

    reply.setId(id);

    return reply;

}



void BlipProvider::init()
{

    QVariant var = load("Blip-Categorys");
    QHashIterator<QString, QVariant> it(var.toHash());
    while (it.hasNext()) {
        it.next();
        m_categorys[it.key()] = it.value().toString();
    }

    var = load("Blip-CategoryDate");
    QDate date = var.toDateTime().date();

    if (m_categorys.isEmpty() || date.month() != QDate::currentDate().month()) { // update every month
        JOSCHY_DEBUG() << "update categorys....";
        updateCategorys();
    }

}


void BlipProvider::updateCategorys()
{

    const QUrl url("http://www.blip.tv/?section=categories&cmd=view&skin=api");

    QHash<QByteArray, QByteArray> header;
    header["Accept-Language"] = QLocale::system().name().toLatin1()+";";

    ResponseParser::ActionData data;
    data.postFile = 0;
    data.type = ResponseParser::UpdateCategorysType;

    const QString id = layer()->get(url, header);
    m_data[id] = data;

}


void BlipProvider::parserFinished(Joschy::AbstractJob *job)
{

    Joschy::ResponseParser *parser = static_cast<Joschy::ResponseParser*>(job);


    const ResponseParser::Type type = parser->type();
    const QString id = parser->id();
    const QString errorString = parser->errorString();
    const Plugin::ErrorType errorType = parser->errorType();
    const bool hasError = parser->error();

    if (hasError) {
        emit error(id, errorType, errorString);
    } else {
        switch (type) {
        case ResponseParser::UploadType: {
                Joschy::Video video = parser->getVideo();
                video.setUrl(parser->actionData().video.url());
                emit uploadFinished(id, video);
                break;
            }
        case ResponseParser::SearchType: {
                emit searchFinished(id, parser->getVideos());
                break;
            }
        case ResponseParser::UpdateThumbnailType: {
                const QString thumbnail = parser->actionData().thumbnail;

                QFile file(thumbnail);
                if (!file.open(QIODevice::WriteOnly)) {
                    JOSCHY_DEBUG() << "open failed:" << thumbnail << file.errorString();
                    emit error(id, Plugin::CannotOpenError, file.errorString());
                } else {
                    if (file.write(parser->image()) == -1) {
                        file.close();
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

                save("Blip-Categorys", QVariant(hash));
                save("Blip-CategoryDate", QVariant(QDateTime::currentDateTime()));

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


void BlipProvider::finishedAuth()
{

    foreach (const QString &id, m_tmpIds) {
        emit authenticated(id);
        removeUniqueId(id);
    }
    m_tmpIds.clear();

}


void BlipProvider::jobFinished(const QString &id, const QVariantMap &data,
                               const Joschy::Plugin::ErrorType &errorType,
                               const QString &errorString)
{

    JOSCHY_DEBUG() << "job finished";
    JOSCHY_DEBUG() << "Reply:" << data.value("Reply").toString();
    JOSCHY_DEBUG() << "Content type:" << data.value("ContentType").toString();
    JOSCHY_DEBUG() << "Status:" << data.value("Status").toString();
    JOSCHY_DEBUG() << "error" << bool(errorType != Plugin::NoError) << errorType;


    ResponseParser::ActionData actionData = m_data[id];
    m_data.remove(id);


    if (actionData.postFile) {
        delete actionData.postFile;
        actionData.postFile = 0;
    }

    if (data.value("Canceled").toBool() || errorType == Joschy::Plugin::ActionCanceledError) {
        emit error(id, Plugin::ActionCanceledError, QString());
        return;
    }
/*
    if (errorType != Joschy::Plugin::NoError) {
        emit error(id, errorType, errorString);
        return;
    }
*/
    actionData.data = data;

    Joschy::ResponseParser *parser = new Joschy::ResponseParser(actionData.type, id, actionData, this);
    connect(parser, SIGNAL(finished(Joschy::AbstractJob*)), this, SLOT(parserFinished(Joschy::AbstractJob*)));

    m_parser.append(parser);
    Joschy::Scheduler::schedule(parser);

}



} // namespace Joschy


#include "blipprovider.moc"
