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

#ifndef KEYMONMANAGER_H
#define KEYMONMANAGER_H


// own
#include "keymon/event.h"
#include"keymon/deviceinfo.h"

// KDE
#include <kauth.h>

// Qt
#include <QtCore/QObject>



using namespace KAuth;
class KeyMonManager : public QObject
{
    Q_OBJECT
    friend class KeyMonManagerSingleton;


public:
    ~KeyMonManager();
    static KeyMonManager *self();
    static QList<KeyMon::DeviceInfo> getInputDeviceList();
    static QString fileForDevice(const KeyMon::DeviceInfo &info);


    QString error() const;
    bool start(const QStringList &devs);
    void stop();


private:
    static KeyMonManager *m_self;
    bool m_started;
    QString m_error;

    explicit KeyMonManager(QObject *parent = 0);

    QString parseError(const int &errorCode);


private slots:
    void progressStep(const QVariantMap &data);
    void actionPerformed(const ActionReply &reply);
    void actionStarted();


signals:
    void keyEvent(const KeyMon::Event &event);
    void stopped();
    void started();


};


#endif // KEYMONMANAGER_H
