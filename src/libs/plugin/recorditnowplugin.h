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

#ifndef RECORDITNOWPLUGIN_H
#define RECORDITNOWPLUGIN_H


// KDE
#include <kdemacros.h>

// Qt
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>


class KJob;
class KDE_EXPORT RecordItNowPlugin : public QObject
{
    Q_OBJECT


public:
    RecordItNowPlugin(QObject *parent = 0);
    ~RecordItNowPlugin();


private:
    QHash<KJob*, QString> m_jobs;
    QStringList m_uniqueIds;

    QString getUniqueId();
    void removeUniqueId(const QString &id);


private slots:
    void jobFinishedInternal(KJob *job);


protected:
    QString move(const QString &from ,const QString &to);
    QString remove(const QString &file);
    QString getTemporaryFile(const QString &workDir) const;
    QString unique(const QString &file) const;

    virtual void jobFinished(const QString &id, const QString &errorString);


};


#endif // RECORDITNOWPLUGIN_H
