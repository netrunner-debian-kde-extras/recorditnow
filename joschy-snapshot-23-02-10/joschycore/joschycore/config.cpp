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
#include "config.h"

// Qt
#include <QtCore/QSettings>


namespace Joschy {


class ConfigPrivate : public QSharedData {
public:
    ConfigPrivate(Config *cfg)
        : q(cfg)
    {}


    static bool titleSort(const Joschy::Video &video1, const Joschy::Video &video2)
    {
        return video1.title().toLower() < video2.title().toLower();
    }
    static bool categorySort(const Joschy::Video &video1, const Joschy::Video &video2)
    {
        return video1.category().toLower() < video2.category().toLower();
    }
    static bool descriptionSort(const Joschy::Video &video1, const Joschy::Video &video2)
    {
        return video1.description().toLower() < video2.description().toLower();
    }
    static bool publishedSort(const Joschy::Video &video1, const Joschy::Video &video2)
    {
        return video1.published() < video2.published();
    }
    static bool durationSort(const Joschy::Video &video1, const Joschy::Video &video2)
    {
        return video1.duration() < video2.duration();
    }
    static bool ratingSort(const Joschy::Video &video1, const Joschy::Video &video2)
    {
        return video1.rating() < video2.rating();
    }
    static bool viewCountSort(const Joschy::Video &video1, const Joschy::Video &video2)
    {
        return video1.viewCount() < video2.viewCount();
    }


    QString group;
    QString file;


private:
    Config *q;


};


Config::Config(const QString &group, const QString &file)
    : d(new ConfigPrivate(this))
{

    d->group = group;
    d->file = file;

}


Config::Config(const Joschy::Config &copy)
    : d(copy.d)
{


}


Config::Config()
    : d(new ConfigPrivate(this))
{


}


Config::~Config()
{



}


Joschy::Config &Config::operator=(const Joschy::Config &rhs)
{

    d = rhs.d;
    return *this;

}


bool Config::operator==(const Joschy::Config &rhs) const
{

    return d == rhs.d;

}


bool Config::operator!=(const Joschy::Config &rhs) const
{

    return d != rhs.d;

}


bool Config::operator<(const Joschy::Config &rhs) const
{

    return d < rhs.d;

}


bool Config::operator>(const Joschy::Config &rhs) const
{

    return d > rhs.d;

}


bool Config::isValid() const
{

    return (!d->file.isEmpty() && !d->group.isEmpty());

}


QString Config::group() const
{

    return d->group;

}


QString Config::file() const
{

    return d->file;

}


QList<Joschy::Video> Config::loadVideos() const
{

    QList<Joschy::Video> videos;
    loadVideos(&videos);
    return videos;

}


void Config::setGroup(const QString &group)
{

    d->group = group;

}


void Config::setFile(const QString &file)
{

    d->file = file;

}


QString Config::createKey(const QString &k, const int &id) const
{

    QString key = k;
    key.append(' '+QString::number(id));
    return key;

}


void Config::saveVideos(const QList<Joschy::Video> &videos)
{

    if (!isValid()) {
        JOSCHY_DEBUG() << "Invalid cfg:" << d->file << d->group;
        return;
    }

    if (videos.isEmpty()) {
        return;
    }

    QSettings settings(d->file, QSettings::IniFormat);
    settings.beginGroup(d->group);

    int count = 0;
    foreach (const Joschy::Video &video, videos) {
        foreach (const QString &propertyKey, video.propertys()) {
            settings.setValue(createKey(propertyKey, count), video.property(propertyKey));
        }
        settings.setValue(createKey("PropertyKeys", count), video.propertys());
        count++;
    }
    settings.setValue("VideoCount", count);

    settings.endGroup();
    settings.sync();

}


void Config::loadVideos(QList<Joschy::Video> *videos) const
{

    if (!isValid()) {
        JOSCHY_DEBUG() << "Invalid cfg:" << d->file << d->group;
        return;
    }

    QSettings settings(d->file, QSettings::IniFormat);
    settings.beginGroup(d->group);

    const int count = settings.value("VideoCount", 0).toInt();
    for (int i = 0; i < count; i++) {
        Joschy::Video video;
        const QStringList keys = settings.value(createKey("PropertyKeys", i), QStringList()).toStringList();
        foreach (const QString &propertyKey, keys) {
            video.setProperty(propertyKey, settings.value(createKey(propertyKey, i)));
        }
        videos->append(video);
    }

    settings.endGroup();

}


QList<Joschy::Video> Config::sortVideos(const QList<Joschy::Video> &videos,
                                        const Joschy::Config::SortType &type)
{

    QList<Joschy::Video> list = videos;
    sortVideos(&list, type);
    return list;

}


void Config::sortVideos(QList<Joschy::Video> *videos, const Joschy::Config::SortType &type)
{

    switch (type) {
    case Config::SortByTitleType: qStableSort(videos->begin(), videos->end(), ConfigPrivate::titleSort); break;
    case Config::SortByDescriptionType: qStableSort(videos->begin(), videos->end(), ConfigPrivate::descriptionSort); break;
    case Config::SortByPublishedType: qStableSort(videos->begin(), videos->end(), ConfigPrivate::publishedSort); break;
    case Config::SortByDurationType: qStableSort(videos->begin(), videos->end(), ConfigPrivate::durationSort); break;
    case Config::SortByRatingType: qStableSort(videos->begin(), videos->end(), ConfigPrivate::ratingSort); break;
    case Config::SortByViewCountType: qStableSort(videos->begin(), videos->end(), ConfigPrivate::viewCountSort); break;
    case Config::SortByCategoryType: qStableSort(videos->begin(), videos->end(), ConfigPrivate::categorySort); break;
    }

}




} // namespace Joschy

