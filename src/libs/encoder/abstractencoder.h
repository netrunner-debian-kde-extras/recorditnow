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

#ifndef ABSTRACTENCODER_H
#define ABSTRACTENCODER_H


// own
#include "recorditnowplugin.h"

// KDE
#include <kdemacros.h>

// Qt
#include <QtCore/QObject>
#include <QtCore/QVariantList>


class KDE_EXPORT AbstractEncoder : public RecordItNowPlugin
{
    Q_OBJECT


public:
    enum ExitStatus {
        Normal = 0,
        Crash = 1
    };

    struct Data {
        QString file;
        QString workDir;
        bool overwrite;
    };

    explicit AbstractEncoder(QObject *parent = 0, const QVariantList &args = QVariantList());
    ~AbstractEncoder();

    virtual void encode(const Data &d) = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;


signals:
    void status(const QString &text);
    void error(const QString &text);
    void outputFileChanged(const QString &newFile);
    void finished(const AbstractEncoder::ExitStatus &status);


};


#endif // ABSTRACTRECORDER_H
