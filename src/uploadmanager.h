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

#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H


// own
#include "abstractuploader.h"

// KDE
#include <kicon.h>

// Qt
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QPointer>


typedef QPair<QString, KIcon> UploadData;
class RecordItNowPluginManager;
class UploadManager : public QObject
{
    Q_OBJECT


public:
    UploadManager(QObject *parent, RecordItNowPluginManager *manager);
    ~UploadManager();

    QList<UploadData> getUploader() const;

    void startUpload(const QString &uploader, const QString &file, QWidget *parent);
    void stop();


private:
    RecordItNowPluginManager *m_manager;
    QPointer<AbstractUploader> m_uploader;

    void clean();

private slots:
    void uploaderFinished(const QString &error);


signals:
    void status(const QString &status);
    void finished(const QString &error);


};


#endif // UPLOADMANAGER_H
