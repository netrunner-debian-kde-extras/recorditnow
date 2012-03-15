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

#ifndef POSTFILE_H
#define POSTFILE_H


// own
#include "joschy_global.h"

// Qt
#include <QtCore/QFile>



namespace Joschy {


class PostFilePrivate;


/**
 * @class PostFile joschycore/postfile.h
 *
 * @short A QFile with post data
 **/
class JOSCHY_EXPORT PostFile : public QFile
{
    Q_OBJECT


public:
    /**
     * Creates a new PostFile object.
     * @param file the video file
     **/
    explicit PostFile(const QString &file);
    ~PostFile();

    /**
     * @internal
     **/
    qint64 bytesAvailable() const;
    /**
     * @return Returns the file size + the post data size
     **/
    qint64 size () const;

    /**
     * Sets the post data.
     * Example:
     * \verbatim
        firstData
        <file data>
        secondData
      \endverbatim
     * @param firstData Post data
     * @param secondData Post data
     **/
    void setData(const QByteArray &firstData, const QByteArray &secondData);


private:
    PostFilePrivate *d;


protected:
    /**
     * @internal
     **/
    qint64 readData(char *data, qint64 maxSize);


};


} // namespace Joschy


#endif // POSTFILE_H
