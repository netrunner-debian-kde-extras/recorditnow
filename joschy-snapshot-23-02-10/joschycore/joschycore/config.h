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

#ifndef CONFIG_H
#define CONFIG_H


// own
#include "joschy_global.h"
#include "video.h"

// Qt
#include <QtCore/QString>
#include <QtCore/QExplicitlySharedDataPointer>


namespace Joschy {


class ConfigPrivate;


/**
 * @class Config joschycore/config.h
 *
 * @short Useful class for joschy stuff
 **/
class JOSCHY_EXPORT Config
{


public:
    /**
     * Sort order
     **/
    enum SortType {
        SortByTitleType = 0,           /**< Sorts the videos by title (case insensitive) */
        SortByDescriptionType = 1,     /**< Sorts the videos by description (case insensitive) */
        SortByCategoryType = 2,        /**< Sorts the videos by category (case insensitive) */
        SortByViewCountType = 3,       /**< Sorts the videos by view count */
        SortByRatingType = 4,          /**< Sorts the videos by rating */
        SortByPublishedType = 5,       /**< Sorts the videos by its plublished date */
        SortByDurationType = 6         /**< Sorts the videos by duration */
    };

    /**
     * Constructs a Config object.
     * @param group the group
     * @param file the config file
     **/
    Config(const QString &group, const QString &file);
    /**
     * Constructs a copy of copy.
     **/
    Config(const Joschy::Config &copy);
    /**
     * Constructs a Config object.
     * @note You must set a group and a file!
     * @see setGroup, setFile
     **/
    Config();
    ~Config();

    /**
     * Assigns rhs to this Config and returns a reference to this Config.
     **/
    Joschy::Config &operator=(const Joschy::Config &rhs);
    bool operator==(const Joschy::Config &rhs) const;
    bool operator!=(const Joschy::Config &rhs) const;
    bool operator<(const Joschy::Config &rhs) const;
    bool operator>(const Joschy::Config &rhs) const;
    /**
     * @return Returns true if the config is ready to use; otherwise returns false.
     **/
    bool isValid() const;
    /**
     * @return Returns the config group.
     * @see setGroup
     **/
    QString group() const;
    /**
     * @return Returns the config file.
     * @see setFile
     **/
    QString file() const;

    /**
     * @return Returns all videos in file under group
     * @see saveVideos
     **/
    QList<Joschy::Video> loadVideos() const;
    /**
     * Sorts the list of videos by type
     * @param videos The videos to sort
     * @param type The Sort type
     * @return Returns the sorted list
     * @see sortVideos
     **/
    static QList<Joschy::Video> sortVideos(const QList<Joschy::Video> &videos, const Joschy::Config::SortType &type);
    /**
     * Sets the group to group
     * @see setFile
     **/
    void setGroup(const QString &group);
    /**
     * Sets the config file to file
     * @see setGroup
     **/
    void setFile(const QString &file);
    /**
     * Save all videos in list videos to the file under group
     **/
    void saveVideos(const QList<Joschy::Video> &videos);
    /**
     * @return Returns all videos in file under group
     * @see saveVideos
     **/
    void loadVideos(QList<Joschy::Video> *videos) const;
    /**
     * Sorts the list of videos by type
     * @param videos The videos to sort
     * @param type The Sort type
     * @see sortVideos
     **/
    static void sortVideos(QList<Joschy::Video> *videos, const Joschy::Config::SortType &type);


private:
    QExplicitlySharedDataPointer<ConfigPrivate> d;

    /**
     * @internal
     **/
    QString createKey(const QString &k, const int &id) const;



};


} // namespace Joschy


#endif // CONFIG_H
