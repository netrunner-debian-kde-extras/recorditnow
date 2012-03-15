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

#ifndef VIDEO_H
#define VIDEO_H


// own
#include "joschy_global.h"

// Qt
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include <QtCore/QExplicitlySharedDataPointer>


namespace Joschy {


class VideoPrivate;


/**
 * @class Video joschycore/video.h
 *
 * @short A video
 **/
class JOSCHY_EXPORT Video
{


public:
    /**
     * Creates a new Video object.
     **/
    Video();
    /**
     * Constructs a copy of copy.
     **/
    Video(const Joschy::Video &copy);
    ~Video();

    /**
     * Assigns rhs to this Video and returns a reference to this Video.
     **/
    Joschy::Video &operator=(const Joschy::Video &rhs);
    bool operator==(const Joschy::Video &rhs) const;
    bool operator!=(const Joschy::Video &rhs) const;
    bool operator<(const Joschy::Video &rhs) const;
    bool operator>(const Joschy::Video &rhs) const;


    /**
     * @return Returns the title of the video
     * @see setTitle
     **/
    QString title() const;
    /**
     * @return Returns the description of the video
     * @see setDescription
     **/
    QString description() const;
    /**
     * @return Returns the url to the video
     * @see setUrl
     **/
    QUrl url() const;
    /**
     * @return Returns the category of the video
     * @see setCategory
     **/
    QString category() const;
    /**
     * @return Returns the keywords of the video
     * @see setKeywords
     **/
    QStringList keywords() const;
    /**
     * @return Returns the author of the video
     * @see setAuthor
     **/
    QString author() const;
    /**
     * @return Returns the publish date of the video
     * @see setPublished
     **/
    QDateTime published() const;
    QDateTime updated() const;
    /**
     * @return Returns the rating of the video
     * @see setRating
     **/
    double rating() const;
    /**
     * @return Returns the number of raters of the video
     * @see setRaters
     **/
    qlonglong raters() const;
    /**
     * @return Returns the view count of the video
     * @see setViewCount
     **/
    qlonglong viewCount() const;
    /**
     * @return Returns the thumbnail url of the video
     * @see setThumbnailUrl
     **/
    QUrl thumbnailUrl() const;
    /**
     * @return Returns the duration of the video
     * @see setDuration
     **/
    qlonglong duration() const;
    /**
     * @return Returns the thumbnail name of the video
     **/
    QString thumbnail() const;
    /**
     * @return Returns the property specified by name
     * @see setProperty
     **/
    QVariant property(const QString &name) const;
    /**
     * @return Returns all property names
     * @see setProperty, property
     **/
    QStringList propertys() const;


    /**
     * Sets the title.
     * @param title The new title
     * @see title
     **/
    void setTitle(const QString &title);
    /**
     * Sets the description.
     * @param description The new description
     * @see description
     **/
    void setDescription(const QString &description);
    /**
     * Sets the url.
     * @param url The new url
     * @see url
     **/
    void setUrl(const QUrl &url);
    /**
     * Sets the category.
     * @param category The new category
     * @see category
     **/
    void setCategory(const QString &category);
    /**
     * Sets the keywords.
     * @param keywords The new keywords
     * @see keywords
     **/
    void setKeywords(const QStringList &keywords);
    /**
     * Sets the author.
     * @param author The new author
     * @see author
     **/
    void setAuthor(const QString &author);
    /**
     * Sets the published date.
     * @param dateTime The new date
     * @see published
     **/
    void setPublished(const QDateTime &dateTime);
    /**
     * Sets the last updated date.
     * @param dateTime The new date
     * @see updated
     **/
    void setUpdated(const QDateTime &dateTime);
    /**
     * Sets the rating.
     * @param rating The new rating
     * @see rating
     **/
    void setRating(const double &rating);
    /**
     * Sets the number of raters.
     * @param raters The new number of raters
     * @see raters
     **/
    void setRaters(const qlonglong &raters);
    /**
     * Sets the view count.
     * @param viewCount The new view count
     * @see viewCount
     **/
    void setViewCount(const qlonglong &viewCount);
    /**
     * Sets the thumbnail url.
     * @param thumbnail The new url
     * @see thumbnailUrl
     **/
    void setThumbnailUrl(const QUrl &thumbnail);
    /**
     * Sets the duration.
     * @param duration The new duration
     * @see duration
     **/
    void setDuration(const qlonglong &duration);
    /**
     * Sets a prperty.
     * @param name The property name
     * @param value The value
     * @see property, propertys
     **/
    void setProperty(const QString &name, const QVariant &value);


private:
    QExplicitlySharedDataPointer<VideoPrivate> d;


};


} // namespace Joschy


#endif // VIDEO_H
