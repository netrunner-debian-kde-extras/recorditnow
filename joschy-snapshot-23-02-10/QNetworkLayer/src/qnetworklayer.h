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

#ifndef QNETWORKLAYER_H
#define QNETWORKLAYER_H


// Joschy
#include <joschycore/abstractnetworklayer.h>

// Qt
#include <QtCore/QtPlugin>


class QNetworkAccessManager;
class QNetworkReply;
namespace Joschy {


class AbstractJob;
class TransferJob;
class JOSCHY_EXPORT QNetworkLayer : public Joschy::AbstractNetworkLayer
{
    Q_OBJECT


public:
    explicit QNetworkLayer(QObject *parent = 0);
    ~QNetworkLayer();

    QString post(const QUrl &url, const Header &header, const QByteArray &data);
    QString post(const QUrl &url, const Header &header, QIODevice *device);
    QString get(const QUrl &url, const Header &header = Header());


private:
    QNetworkAccessManager *m_networkManager;
    QHash<AbstractJob*, QString> m_jobs;


private slots:
    void jobFinished(Joschy::AbstractJob *job);


protected:
    void cancel(const QString &id);


};


} // namespace Joschy


Q_EXPORT_PLUGIN2(joschyqnetworklayer, Joschy::QNetworkLayer);


#endif // QNETWORKLAYER_H
