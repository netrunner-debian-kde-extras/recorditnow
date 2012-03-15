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


// own
#include "keymonmanager.h"
#include "keymon/manager.h"

// KDE
#include <kglobal.h>
#include <kdebug.h>
#include <kauth.h>
#include <klocalizedstring.h>


class KeyMonManagerSingleton
{
    public:
        KeyMonManager self;
};


K_GLOBAL_STATIC(KeyMonManagerSingleton, privateSelf)


KeyMonManager::KeyMonManager(QObject *parent)
    : QObject(parent), m_started(false)
{



}


KeyMonManager::~KeyMonManager()
{

//    stop(); crash

}


KeyMonManager *KeyMonManager::self()
{

    return &privateSelf->self;

}


QList<KeyMon::DeviceInfo> KeyMonManager::getInputDeviceList()
{

    return KeyMon::Manager::getInputDeviceList();

}


QString KeyMonManager::fileForDevice(const KeyMon::DeviceInfo &info)
{

    return KeyMon::Manager::fileForDevice(info);

}


QString KeyMonManager::error() const
{

    return m_error;

}


bool KeyMonManager::start(const QStringList &devs)
{

    if (m_started) {
        kDebug() << "already running...";
        return true;
    }
    kDebug() << "start..." << devs;

    KAuth::Action action("org.kde.recorditnow.helper.watch");
    connect(action.watcher(), SIGNAL(progressStep(QVariantMap)), this,
            SLOT(progressStep(QVariantMap)));
    connect(action.watcher(), SIGNAL(actionPerformed(ActionReply)), this,
            SLOT(actionPerformed(ActionReply)));

    QVariantMap args;
    args["Devs"] = devs;

    action.setArguments(args);
    action.setExecutesAsync(true);

    kDebug() << "valid action:" << action.isValid();

    KAuth::ActionReply reply = action.execute("org.kde.recorditnow.helper");
    if (reply.errorCode() != KAuth::ActionReply::NoError) {
        if (reply.errorCode() == KAuth::ActionReply::UserCancelled) {
            m_error.clear();
            action.watcher()->disconnect(this);
            return false;
        } else {
            action.watcher()->disconnect(this);
            m_error = parseError(reply.errorCode());
            return false;
        }
    }
    m_started = true;

    kDebug() << "started: type:" << reply.type() << "Failed?" << reply.failed() << "Error code:" <<
            reply.errorCode() << "error Description:" << reply.errorDescription();

    kDebug() << "Invalid:" << bool(ActionReply::InvalidActionReply == reply);
    kDebug() << "Denied:" << bool(ActionReply::AuthorizationDeniedReply == reply);
    kDebug() << "Canceled:" << bool(ActionReply::UserCancelledReply == reply);
    kDebug() << "success:" << bool(ActionReply::SuccessReply == reply);

    return true;

}


void KeyMonManager::stop()
{

    if (!m_started) {
        return;
    }
    kDebug() << "stopping...";

    KAuth::Action action("org.kde.recorditnow.helper.watch");
    action.setHelperID("org.kde.recorditnow.helper");
    action.watcher()->disconnect(this);
    action.stop();

    m_started = false;

}


QString KeyMonManager::parseError(const int &errorCode)
{

    KAuth::ActionReply::Error reply = static_cast<KAuth::ActionReply::Error>(errorCode);

    QString error;
    switch (reply) {
    case KAuth::ActionReply::NoResponder:
    case KAuth::ActionReply::NoSuchAction:
    case KAuth::ActionReply::InvalidAction:
    case KAuth::ActionReply::HelperBusy:
    case KAuth::ActionReply::DBusError: error = i18n("An internal error has occurred.\n"
                                                     "Error code: %1\n", errorCode); break;
    case KAuth::ActionReply::AuthorizationDenied: error = i18n("You don't have the authorization to"
                                                               " use this feature."); break;
    default: break;
    }

    return error;

}


void KeyMonManager::progressStep(const QVariantMap &data)
{

    if (data.contains("Started")) {
        emit started();
        return;
    }

    KeyMon::Event event;
    event.key = static_cast<KeyMon::Event::Key>(data["Key"].toInt());
    event.pressed = data["Pressed"].toBool();
    event.keyCode = data.value("KeyCode").toInt();
    event.mouseEvent = data.value("MouseEvent").toBool();

    emit keyEvent(event);

}


void KeyMonManager::actionPerformed(const ActionReply &reply)
{

    if (reply.type() != KAuth::ActionReply::Success) {
        if (reply.type() == KAuth::ActionReply::HelperError) {
            m_error = reply.data().value("ErrorString").toString();
        } else {
            m_error = parseError(reply.errorCode());
        }
    }

    KAuth::Action action("org.kde.recorditnow.helper.watch");
    action.setHelperID("org.kde.recorditnow.helper");
    action.watcher()->disconnect(this);

    kDebug() << "action performed:" << reply.type() << reply.errorCode() << reply.errorDescription();

    m_started = false;
    emit stopped();

}


void KeyMonManager::actionStarted()
{

    emit started();

}

#include "keymonmanager.moc"
