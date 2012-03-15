/***************************************************************************
 *   Copyright (C) 2010 by Kai Dombrowe <just89@gmx.de>                    *
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

#ifndef FFMPEGRECORDER_H
#define FFMPEGRECORDER_H


// own
#include "abstractrecorder.h"

// Qt
#include <QtCore/QVariantList>
#include <QtCore/QPointer>


class KProcess;
class FfmpegRecorder : public AbstractRecorder
{
    Q_OBJECT


public:
    explicit FfmpegRecorder(QObject *parent = 0, const QVariantList &args = QVariantList());
    ~FfmpegRecorder();

    void record(const AbstractRecorder::Data &d);
    void pause();
    void stop();


private:
    KProcess *m_recorder;
    AbstractRecorder::Data m_data;
    QString m_tmpFile;
    bool m_paused;
    QString m_removeId;
    QString m_moveId;

    void clean();
    void moveToDestination();


private slots:
    void newRecorderOutput();
    void recorderFinished(const int & ret);


protected slots:
    void jobFinished(const QString &id, const QString &errorString);


};


#endif // RECORDMYDESKTOPRECORDER_H
