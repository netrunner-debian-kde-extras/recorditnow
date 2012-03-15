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
#include "recorditnowplugin.h"

// KDE
#include <kstandarddirs.h>
#include <kio/jobclasses.h>
#include <kio/job.h>
#include <kdebug.h>
#include <klocalizedstring.h>

// Qt
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QUuid>


RecordItNowPlugin::RecordItNowPlugin(QObject *parent)
    : QObject(parent)
{



}


RecordItNowPlugin::~RecordItNowPlugin()
{



}


QString RecordItNowPlugin::getUniqueId()
{

    QString id = QUuid::createUuid().toString();
    while (m_uniqueIds.contains(id)) {
        id = QUuid::createUuid().toString();
    }

    m_uniqueIds.append(id);
    return id;

}


void RecordItNowPlugin::removeUniqueId(const QString &id)
{

    m_uniqueIds.removeAll(id);

}


void RecordItNowPlugin::jobFinishedInternal(KJob *job)
{

    QString id = m_jobs.value(job);

    m_jobs.remove(job);

    QString errorString;
    if (job->error()) {
        errorString = job->errorString();
        if (errorString.isEmpty()) {
            errorString = i18n("Unkown error!");
        }
    }

    removeUniqueId(id);
    jobFinished(id, errorString);

}


QString RecordItNowPlugin::move(const QString &from ,const QString &to)
{

    if (!QFile::exists(from)) {
        return QString();
    }

    KIO::FileCopyJob *job = KIO::file_move(KUrl(from), KUrl(to), -1, KIO::HideProgressInfo);
    connect(job, SIGNAL(finished(KJob*)), this, SLOT(jobFinishedInternal(KJob*)));

    job->setAutoDelete(true);

    m_jobs[job] = getUniqueId();

    return m_jobs.value(job);

}


QString RecordItNowPlugin::remove(const QString &file)
{

    KIO::SimpleJob *job = KIO::file_delete(KUrl(file), KIO::HideProgressInfo);
    connect(job, SIGNAL(finished(KJob*)), this, SLOT(jobFinishedInternal(KJob*)));

    job->setAutoDelete(true);

    m_jobs[job] = getUniqueId();

    return m_jobs.value(job);

}


QString RecordItNowPlugin::getTemporaryFile(const QString &workDir) const
{

    QString tmpDir = workDir;
    if (tmpDir.isEmpty()) {
        tmpDir = KGlobal::dirs()->locateLocal("tmp", "");
    }

    if (tmpDir.isEmpty()) {
        tmpDir = QDir::homePath();
    }

    if (!tmpDir.endsWith('/')) {
        tmpDir.append('/');
    }
    QString path = (tmpDir+"recorditnow_tmp");

    path = unique(path);

    return path;

}


QString RecordItNowPlugin::unique(const QString &file) const
{

    QString result = file;
    const QRegExp rx("-[0-9]+$");
    const QRegExp frx("\\..{3}$");

    frx.indexIn(result);
    const QString format = frx.cap();
    result.remove(frx);

    while (QFile::exists(result+format)) {
        rx.indexIn(result);
        if (!rx.cap().isEmpty()) {
            int number = rx.cap().remove(0, 1).toInt();
            number++;
            result.remove(rx);
            result += '-';
            result += QString::number(number);
        } else {
            result += "-0";
        }
    }
    result += format;

    return result;

}


void RecordItNowPlugin::jobFinished(const QString &id, const QString &errorString)
{

    Q_UNUSED(id);
    Q_UNUSED(errorString);

}


#include "recorditnowplugin.moc"

