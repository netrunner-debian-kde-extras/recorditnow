/***************************************************************************
 *   Copyright (C) 2009 by Kai Dombrowe <just89@gmx.de>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef RECORDMYDESKTOPRECORDER_H
#define RECORDMYDESKTOPRECORDER_H


// own
#include "abstractrecorder.h"

// Qt
#include <QtCore/QVariantList>
#include <QtCore/QPointer>


class KProcess;
class RecordMyDesktopRecorder : public AbstractRecorder
{
    Q_OBJECT


public:
    explicit RecordMyDesktopRecorder(QObject *parent = 0, const QVariantList &args = QVariantList());
    ~RecordMyDesktopRecorder();

    void record(const AbstractRecorder::Data &d);
    void pause();
    void stop();


private:
    struct RecordMyDesktopData {
        QString outputFile;
        QString tmpFile;
        bool paused;
        bool overwrite;
    };

    KProcess *m_recorder;
    RecordMyDesktopData m_data;
    QString m_moveId;
    QString m_removeId;

    void clean();
    void moveToDestination();
   // bool remove(const QString &file);
   // bool move(const QString &from, const QString &to);


private slots:
    void newRecorderOutput();
    void recorderFinished(const int & ret);


protected slots:
    void jobFinished(const QString &id, const QString &errorString);


};


#endif // RECORDMYDESKTOPRECORDER_H
