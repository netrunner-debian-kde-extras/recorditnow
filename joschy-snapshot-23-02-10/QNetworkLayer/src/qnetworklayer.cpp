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
#include "qnetworklayer.h"
#include "transferjob.h"

// Joschy
#include <joschycore/abstractprovider.h>
#include <joschycore/scheduler.h>

// Qt
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>


#define JOSCHY_CLASS "Joschy::QNetworkLayer"
namespace Joschy {


QNetworkLayer::QNetworkLayer(QObject *parent)
    :Joschy::AbstractNetworkLayer(parent)
{

    m_networkManager = new QNetworkAccessManager(this);

}


QNetworkLayer::~QNetworkLayer()
{

    JOSCHY_DEBUG();
    delete m_networkManager;

}


QString QNetworkLayer::post(const QUrl &url, const Header &header, const QByteArray &data)
{

    QNetworkRequest request;
    request.setUrl(url);

    QHashIterator<QByteArray, QByteArray> it(header);
    while (it.hasNext()) {
        it.next();
        request.setRawHeader(it.key(), it.value());
    }

    const QString id = addUniqueId();

    TransferJob::TransferData d;
    d.manager = m_networkManager;
    d.type = TransferJob::NormalPostType;
    d.request = request;
    d.data = data;
    d.id = id;

    TransferJob *job = new TransferJob(this, d);
    connect(job, SIGNAL(uploadProgress(QString,qlonglong,qlonglong,qlonglong)), this,
            SLOT(emitUploadProgress(QString,qlonglong,qlonglong,qlonglong)));
    connect(job, SIGNAL(finished(Joschy::AbstractJob*)), this, SLOT(jobFinished(Joschy::AbstractJob*)));
    m_jobs[job] = id;

    Joschy::Scheduler::schedule(job);

    return  id;

}


QString QNetworkLayer::post(const QUrl &url, const Header &header, QIODevice *device)
{

    QNetworkRequest request;
    request.setUrl(url);

    QHashIterator<QByteArray, QByteArray> it(header);
    while (it.hasNext()) {
        it.next();
        request.setRawHeader(it.key(), it.value());
    }

    const QString id = addUniqueId();

    TransferJob::TransferData data;
    data.manager = m_networkManager;
    data.type = TransferJob::FilePostType;
    data.request = request;
    data.device = device;
    data.id = id;

    TransferJob *job = new TransferJob(this, data);
    connect(job, SIGNAL(uploadProgress(QString,qlonglong,qlonglong,qlonglong)), this,
            SLOT(emitUploadProgress(QString,qlonglong,qlonglong,qlonglong)));
    connect(job, SIGNAL(finished(Joschy::AbstractJob*)), this, SLOT(jobFinished(Joschy::AbstractJob*)));
    m_jobs[job] = id;

    Joschy::Scheduler::schedule(job);

    return  id;

}


QString QNetworkLayer::get(const QUrl &url, const Header &header)
{

    QNetworkRequest request;
    request.setUrl(url);

    if (!header.isEmpty()) {
        QHashIterator<QByteArray, QByteArray> it(header);
        while (it.hasNext()) {
            it.next();
            request.setRawHeader(it.key(), it.value());
        }
    }

    const QString id = addUniqueId();

    TransferJob::TransferData data;
    data.manager = m_networkManager;
    data.type = TransferJob::GetType;
    data.request = request;
    data.id = id;

    TransferJob *job = new TransferJob(this, data);
    connect(job, SIGNAL(uploadProgress(QString,qlonglong,qlonglong,qlonglong)), this,
            SLOT(emitUploadProgress(QString,qlonglong,qlonglong,qlonglong)));
    connect(job, SIGNAL(finished(Joschy::AbstractJob*)), this, SLOT(jobFinished(Joschy::AbstractJob*)));
    m_jobs[job] = id;

    Joschy::Scheduler::schedule(job);

    return  id;

}


void QNetworkLayer::jobFinished(Joschy::AbstractJob *job)
{

    TransferJob *tJob = static_cast<TransferJob*>(job);
    QNetworkReply *reply = tJob->reply();
    JOSCHY_DEBUG() << "finished:" << reply->error() << reply->errorString();

    QVariantMap data;
    data["Reply"] = reply->readAll();
    data["ContentType"] = reply->header(QNetworkRequest::ContentTypeHeader);
    data["Status"] = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    const QString id = m_jobs.take(job);

    const QString errorString = reply->errorString();
    Joschy::Plugin::ErrorType error = Joschy::Plugin::NoError;
    switch (reply->error()) {
    case QNetworkReply::NoError: break;
    case QNetworkReply::ConnectionRefusedError:
    case QNetworkReply::RemoteHostClosedError:
    case QNetworkReply::HostNotFoundError:
    case QNetworkReply::UnknownNetworkError:
    case QNetworkReply::TimeoutError: error = Joschy::Plugin::NetworkError; break;
    case QNetworkReply::OperationCanceledError: error = Joschy::Plugin::ActionCanceledError; break;
    case QNetworkReply::SslHandshakeFailedError: error = Joschy::Plugin::SslError; break;
    case QNetworkReply::ProxyConnectionRefusedError:
    case QNetworkReply::ProxyConnectionClosedError:
    case QNetworkReply::ProxyNotFoundError:
    case QNetworkReply::ProxyTimeoutError:
    case QNetworkReply::ProxyAuthenticationRequiredError: error = Joschy::Plugin::ProxyError; break;
    default: error = Joschy::Plugin::UnknownError; break;
    }

    if (job->isCanceled()) {
        error = Joschy::Plugin::ActionCanceledError;
    }

    delete job;
    reply->deleteLater();
    
    provider()->jobFinished(id, data, error, errorString);

}


void QNetworkLayer::cancel(const QString &id)
{

    QHashIterator<AbstractJob*, QString> it(m_jobs);
    while (it.hasNext()) {
        it.next();
        if (it.value() == id) {
            static_cast<TransferJob*>(it.key())->cancel();
            break;
        }
    }

}



} // namespace Joschy


#include "qnetworklayer.moc"
