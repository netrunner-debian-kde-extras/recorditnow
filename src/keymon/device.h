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

#ifndef DEVICE_H
#define DEVICE_H


// own
#include "event.h"

// KDE
#include <kdemacros.h>

// Qt
#include <QtCore/QObject>


class QSocketNotifier;
namespace KeyMon {


class KDE_EXPORT Device : public QObject
{
    Q_OBJECT


public:
    explicit Device(QObject *parent, const QString &file, const bool &mouse = true);
    ~Device();

    bool error() const;


private:
    QSocketNotifier *m_socketNotifier;
    bool m_error;
    bool m_watchMouse;


private slots:
    void readEvents();


signals:
    void buttonPressed(const KeyMon::Event &event);
    void keyPressed(const KeyMon::Event &event);
    void finished();


};


}; // Namespace KeyMon


#endif // DEVICE_H
