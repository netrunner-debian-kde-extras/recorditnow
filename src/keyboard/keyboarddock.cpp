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
#include "keyboarddock.h"
#include "keywidget.h"
#include "../keymonmanager.h"
#include "flowlayout.h"
#include <recorditnow.h>

// KDE
#include <kauth.h>
#include <kdebug.h>

// Qt
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtGui/QMainWindow>
#include <QtGui/QSlider>


KeyboardDock::KeyboardDock(QWidget *parent)
    : QDockWidget(parent)
{

    setObjectName("Keyboard");
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);

    setupUi(this);

    connect(KeyMonManager::self(), SIGNAL(keyEvent(KeyMon::Event)), this,
            SLOT(keyPressed(KeyMon::Event)));

    m_layout = new FlowLayout;
    mainLayout->addLayout(m_layout);

    QWidget *title = new QWidget(this);
    ui_title.setupUi(title);

    ui_title.sizeSlider->setValue(Settings::keySize());
    connect(ui_title.sizeSlider, SIGNAL(valueChanged(int)), this, SLOT(sizeChanged(int)));

    setTitleBarWidget(title);

    sizeChanged(Settings::keySize());


}


KeyboardDock::~KeyboardDock()
{

}


void KeyboardDock::init(const QList<KeyboardKey> &map)
{

    m_layout->clear();
    m_keyList.clear();

    foreach (const KeyboardKey &key, map) {
        KeyWidget *widget = new KeyWidget(key, this);
        m_keyList.append(widget);
        m_layout->addWidget(widget);
    }

}


void KeyboardDock::keyPressed(const KeyMon::Event &event)
{

    if (event.mouseEvent) {
        return;
    }

    foreach (KeyWidget * widget, m_keyList) {
        if (widget->keyCode() == event.keyCode) {
            kDebug() << "Event:" << "Code:" << event.keyCode << "Pressed?" << event.pressed;
            widget->setPressed(event.pressed);
            break;
        }
    }

}


void KeyboardDock::sizeChanged(const int &value)
{

    m_layout->setItemHeight(value);
    Settings::self()->setKeySize(value);
    scrollAreaWidgetContents->adjustSize();

}


#include "keyboarddock.moc"
