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

#ifndef TRANSFERJOB_H
#define TRANSFERJOB_H


// Joschy
#include <joschycore/abstractjob.h>

// Qt
#include <QtNetwork/QNetworkRequest>


class QNetworkReply;
class QNetworkAccessManager;
namespace Joschy {


class TransferJob : public Joschy::AbstractJob
{

    Q_OBJECT


public:
    enum Type {
        NormalPostType = 0,
        FilePostType = 1,
        GetType = 2
    };

    struct TransferData {
        QString id;
        QNetworkRequest request;
        QByteArray data;
        QIODevice *device;
        TransferJob::Type type;
        QNetworkAccessManager *manager;
    };

    explicit TransferJob(QObject *parent, TransferJob::TransferData data);
    ~TransferJob();

    QNetworkReply *reply() const;


private:
    TransferData m_data;
    QNetworkReply *m_reply;
    QTime m_speed;


private slots:
    void jobFinished(QNetworkReply *reply);
    void progress(qint64 bytesSent, qint64 bytesTotal);


protected:
    void startJob();
    bool cancelJob();


signals:
    void uploadProgress(const QString &id, qlonglong speed, qlonglong bytesSent, qlonglong bytesTotal);


};


} // namespace Joschy


#endif // TRANSFERJOB_H
