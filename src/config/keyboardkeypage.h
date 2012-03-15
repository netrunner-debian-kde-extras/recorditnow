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


#ifndef KEYBOARDKEYPAGE_H
#define KEYBOARDKEYPAGE_H


// own
#include "ui_keyboardkeypage.h"
#include "keymon/event.h"

// KDE
#include <kauth.h>

// Qt
#include <QtGui/QWizardPage>
#include <QtCore/QVariantMap>


using namespace KAuth;
class KeyboardKeyPage : public QWizardPage, Ui::KeyboardKeyPage
{
    Q_OBJECT


public:
    explicit KeyboardKeyPage(const QString &device, QWidget *parent = 0);
    ~KeyboardKeyPage();

    void initializePage();


private:
    int m_key;
    bool m_grab;
    QString m_device;


private slots:
    void start();
    void startGrab();
    void stop();
    void keyEvent(const KeyMon::Event &event);
    void keymonStopped();


protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


};


#endif // KEYBOARDKEYPAGE_H
