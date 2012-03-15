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
#include "video.h"

// Qt
#include <QtCore/QStringList>
#include <QtCore/QSharedData>
#include <QtCore/QCryptographicHash>


namespace Joschy {


class VideoPrivate : public QSharedData
{
public:
    VideoPrivate(Video *video)
        : q(video)
    {}

    QHash<QString, QVariant> videoData;


private:
    Video *q;


};


Video::Video()
    : d(new VideoPrivate(this))
{


}


Video::Video(const Joschy::Video &copy)
    : d(copy.d)
{


}


Video::~Video()
{


}


Joschy::Video &Video::operator=(const Joschy::Video &rhs)
{

    d = rhs.d;
    return *this;

}


bool Video::operator==(const Joschy::Video &rhs) const
{

    return d == rhs.d;

}


bool Video::operator!=(const Joschy::Video &rhs) const
{

    return d != rhs.d;

}


bool Video::operator<(const Joschy::Video &rhs) const
{

    return d < rhs.d;

}


bool Video::operator>(const Joschy::Video &rhs) const
{

    return d > rhs.d;

}


QString Video::title() const
{

    return d->videoData.value("Title").toString();

}


QString Video::description() const
{

    return d->videoData.value("Description").toString();

}


QUrl Video::url() const
{

    return d->videoData.value("Url").toUrl();

}


QString Video::category() const
{

    return d->videoData.value("Category").toString();

}


QStringList Video::keywords() const
{

    return d->videoData.value("Keywords").toStringList();

}


QString Video::author() const
{

    return d->videoData.value("Author").toString();

}


QDateTime Video::published() const
{

    return d->videoData.value("Published").toDateTime();

}


QDateTime Video::updated() const
{

    return d->videoData.value("Updated").toDateTime();

}


double Video::rating() const
{

    return d->videoData.value("Rating").toDouble();

}


qlonglong Video::raters() const
{

    return d->videoData.value("Raters").toLongLong();

}


qlonglong Video::viewCount() const
{

    return d->videoData.value("ViewCount").toLongLong();

}


QUrl Video::thumbnailUrl() const
{

    return d->videoData.value("ThumbnailUrl").toUrl();

}


qlonglong Video::duration() const
{

    return d->videoData.value("Duration").toLongLong();

}


QString Video::thumbnail() const
{

    const QByteArray data = url().toString().toLatin1();
    return QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

}


QVariant Video::property(const QString &name) const
{

    return d->videoData.value(name);

}


QStringList Video::propertys() const
{

    return d->videoData.keys();

}


void Video::setTitle(const QString &title)
{

    d->videoData["Title"] = title;

}


void Video::setDescription(const QString &description)
{

    d->videoData["Description"] = description;

}


void Video::setUrl(const QUrl &url)
{

    d->videoData["Url"] = url;

}


void Video::setCategory(const QString &category)
{

    d->videoData["Category"] = category;

}


void Video::setKeywords(const QStringList &keywords)
{

    d->videoData["Keywords"] = keywords;

}


void Video::setAuthor(const QString &author)
{

    d->videoData["Author"] = author;

}


void Video::setPublished(const QDateTime &dateTime)
{

    d->videoData["Published"] = dateTime;

}


void Video::setUpdated(const QDateTime &dateTime)
{

    d->videoData["Updated"] = dateTime;

}


void Video::setRating(const double &rating)
{

    d->videoData["Rating"] = rating;

}


void Video::setRaters(const qlonglong &raters)
{

    d->videoData["Raters"] = raters;

}


void Video::setViewCount(const qlonglong &viewCount)
{

    d->videoData["ViewCount"] = viewCount;

}


void Video::setThumbnailUrl(const QUrl &thumbnail)
{

    d->videoData["ThumbnailUrl"] = thumbnail;

}


void Video::setDuration(const qlonglong &duration)
{

    d->videoData["Duration"] = duration;

}


void Video::setProperty(const QString &name, const QVariant &value)
{

    d->videoData[name] = value;

}



} // namespace Joschy


