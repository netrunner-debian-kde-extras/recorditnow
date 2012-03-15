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
#include "transferjob.h"

// Qt
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


namespace Joschy {


TransferJob::TransferJob(QObject *parent, TransferJob::TransferData data)
    : Joschy::AbstractJob(parent), m_data(data), m_reply(0)
{

    connect(m_data.manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(jobFinished(QNetworkReply*)));

}


TransferJob::~TransferJob()
{



}


QNetworkReply *TransferJob::reply() const
{

    return m_reply;

}


void TransferJob::jobFinished(QNetworkReply *reply)
{

    if (m_reply != reply) {
        return;
    }
    jobDone();

}


void TransferJob::progress(qint64 bytesSent, qint64 bytesTotal)
{

    qlonglong speed = -1;
    if (bytesTotal != -1) {
        speed = bytesSent*1000.0/m_speed.elapsed();
    }
    emit uploadProgress(m_data.id, speed, bytesSent, bytesTotal);

}


void TransferJob::startJob()
{

    if (isCanceled()) {
        return;
    }

    switch (m_data.type) {
    case NormalPostType: {
            m_reply = m_data.manager->post(m_data.request, m_data.data);
            break;
        }
    case FilePostType: {
            m_reply = m_data.manager->post(m_data.request, m_data.device);
            break;
        }
    case GetType: m_reply = m_data.manager->get(m_data.request); break;
    default: break;
    }
    connect(m_reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(progress(qint64,qint64)));
    m_speed.start();

}


bool TransferJob::cancelJob()
{

    if (m_reply) {
        m_reply->abort();
    }
    return true;

}


} // namespace Joschy


#include "transferjob.moc"
