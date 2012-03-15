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
#include "postfile.h"

// Qt
#include <QtCore/QFile>


namespace Joschy {


class PostFilePrivate {
public:
    PostFilePrivate(PostFile *file)
        : q(file)
    {}


    void updateSize()
    {

        size = (topData.size()+bottomData.size());

    }


    QByteArray topData;
    QByteArray bottomData;
    qint64 size;


private:
    PostFile *q;


};


PostFile::PostFile(const QString &file)
    : QFile(file), d(new PostFilePrivate(this))
{

    Q_ASSERT(exists());
    if (!open(QIODevice::ReadOnly)) {
        JOSCHY_DEBUG() << error() << errorString();
    }
    d->size = 0;

}


PostFile::~PostFile()
{

    close();
    delete d;

}


qint64 PostFile::bytesAvailable() const
{

    return d->topData.size()+d->bottomData.size()+QFile::bytesAvailable();

}


qint64 PostFile::size() const
{

    return (QFile::size()+d->size);

}


void PostFile::setData(const QByteArray &firstData, const QByteArray &secondData)
{

    d->topData = firstData;
    d->bottomData = secondData;
    d->updateSize();

}


qint64 PostFile::readData(char *data, qint64 maxSize)
{

    qint64 sizeRead = 0;
    qint64 sizeLeft = 0;
    QByteArray dataRead;

    if (!d->topData.isEmpty()) {
        dataRead = d->topData.left(maxSize);
        sizeRead = dataRead.size();
        d->topData.remove(0, sizeRead);
    }

    if (sizeRead < maxSize) {
        if (!QFile::atEnd()) {
            sizeLeft = maxSize-sizeRead;
            QByteArray buff;
            buff.resize(sizeLeft);
            const qint64 bytes = QFile::readData(buff.data(), sizeLeft);
            buff.resize(bytes);

            dataRead.append(buff);
            sizeRead = dataRead.size();
        }
    }

    if (sizeRead < maxSize) {
        if (!d->bottomData.isEmpty()) {
            sizeLeft = maxSize-sizeRead;
            dataRead.append(d->bottomData.left(sizeLeft));
            qint64 r = d->bottomData.left(sizeLeft).size();
            sizeRead = dataRead.size();
            d->bottomData.remove(0, r);
        }
    }
    memcpy(data, dataRead.data(), dataRead.size());
    return dataRead.size();

}


} // namespace Joschy


#include "postfile.moc"
