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
#include "keyboardkeypage.h"
#include "keymonmanager.h"
#include "../keyboard/keyboardkey.h"

// KDE
#include <kauth.h>
#include <kdebug.h>
#include <kmessagebox.h>

// Qt
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>


KeyboardKeyPage::KeyboardKeyPage(const QString &device, QWidget *parent)
    : QWizardPage(parent)
{

    m_grab = false;
    m_key = -1;
    m_device = device;

    setupUi(this);

    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(KeyMonManager::self(), SIGNAL(keyEvent(KeyMon::Event)), this,
            SLOT(keyEvent(KeyMon::Event)));
    connect(KeyMonManager::self(), SIGNAL(stopped()), this, SLOT(keymonStopped()));
    connect(KeyMonManager::self(), SIGNAL(started()), this, SLOT(startGrab()));

    registerField("KeyCode*", keyCode);
    registerField("Text*", keyText);

    statusLabel->setText(i18n("Click on \"%1\"", startButton->text()));

}


KeyboardKeyPage::~KeyboardKeyPage()
{

    stop();

}


void KeyboardKeyPage::initializePage()
{



}


void KeyboardKeyPage::start()
{

    if (m_device.isEmpty()) {
        statusLabel->setText(i18n("Please select a device."));
        return;
    }

    statusLabel->setText(i18n("Please wait..."));

    startButton->setDisabled(true);
    m_key = -1;

    keyLabel->clear();
    keyCodeLabel->clear();

    if (!KeyMonManager::self()->start(QStringList() << m_device)) {
        statusLabel->setText(i18n("An error occurd: %1", KeyMonManager::self()->error()));
        stop();
    } else {
        statusLabel->setText(i18n("Wait for authentication!"));
    }

}


void KeyboardKeyPage::startGrab()
{

    statusLabel->setText(i18n("Press a key!"));
    grabKeyboard();
    m_grab = true;

}


void KeyboardKeyPage::stop()
{

    m_grab = false;
    releaseKeyboard();
    startButton->setDisabled(false);
    KeyMonManager::self()->stop();
    
}


void KeyboardKeyPage::keyEvent(const KeyMon::Event &event)
{

    if (m_grab) {
        m_key = event.keyCode;
    }

}


void KeyboardKeyPage::keymonStopped()
{

    stop();
    if (!KeyMonManager::self()->error().isEmpty()) {
        statusLabel->setText(i18n("An error occurd: %1", KeyMonManager::self()->error()));
    }

}


void KeyboardKeyPage::keyPressEvent(QKeyEvent *event)
{

    if (!m_grab) {
        QWizardPage::keyPressEvent(event);
    } else { // don`t close the dialog if the user presses Esc
        event->accept();
    }

}


void KeyboardKeyPage::keyReleaseEvent(QKeyEvent *event)
{

    if (!m_grab) {
        QWizardPage::keyReleaseEvent(event);
        return;
    }

    stop();

    kDebug() << "key:" << m_key;
    if (m_key != -1) {
        keyCode->setValue(m_key);

        KeyboardKey key = KeyboardKey::eventToKey(event);

        keyLabel->setText(key.text());
        keyText->setText(key.text());
        keyCodeLabel->setText(QString::number(m_key));

        statusLabel->setText(i18n("Success!"));
    } else {
        statusLabel->setText(i18n("Grab failed!\n"
                                  "Perhaps you have selected the wrong device."));
    }

}




#include "keyboardkeypage.moc"
