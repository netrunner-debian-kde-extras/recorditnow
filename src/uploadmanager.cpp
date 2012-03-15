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


// own
#include "uploadmanager.h"
#include "recorditnowpluginmanager.h"

// KDE
#include <kdebug.h>

// Qt
#include <QtCore/QDir>

UploadManager::UploadManager(QObject *parent, RecordItNowPluginManager *manager)
    : QObject(parent), m_manager(manager)
{



}


UploadManager::~UploadManager()
{

    clean();

}


QList<UploadData> UploadManager::getUploader() const
{

    QList<UploadData> list;
    foreach (const KPluginInfo &info, m_manager->getUploaderList()) {
        if (info.isPluginEnabled()) {
            UploadData data;
            data.first = info.name();
            data.second = KIcon(info.icon());
            list.append(data);
        }
    }
    return list;

}


void UploadManager::startUpload(const QString &uploader, const QString &file, QWidget *parent)
{

    if (m_uploader) {
        m_manager->unloadPlugin(m_uploader);
    }

    m_uploader = static_cast<AbstractUploader*>(m_manager->loadPlugin(uploader));

    if (!m_uploader) {
        emit finished(i18n("Cannot load upload plugin \"%1\".", uploader));
        return;
    }

    connect(m_uploader, SIGNAL(finished(QString)), this, SLOT(uploaderFinished(QString)));
    connect(m_uploader, SIGNAL(status(QString)), this, SIGNAL(status(QString)));

    m_uploader->show(file, parent);

}


void UploadManager::stop()
{

    if (m_uploader) {
        m_uploader->cancel();
    }

}


void UploadManager::clean()
{

    if (m_uploader) {
        m_uploader->disconnect();
        m_manager->unloadPlugin(m_uploader);
        m_uploader = 0;
    }

}


void UploadManager::uploaderFinished(const QString &error)
{

    emit finished(error);
    clean();

}


#include "uploadmanager.moc"

