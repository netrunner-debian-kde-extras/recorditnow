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

#ifndef KEYBOARDDOCK_H
#define KEYBOARDDOCK_H


// own
#include "ui_keyboarddock.h"
#include "ui_keyboarddocktitlewidget.h"
#include "../keymon/event.h"
#include "../config/keyboardconfig.h"

// Qt
#include <QtGui/QDockWidget>
#include <QtCore/QVariantMap>


class FlowLayout;
class KeyWidget;
class KConfig;
class KConfigGroup;
class KeyboardDock: public QDockWidget, Ui::KeyboardDock
{
    Q_OBJECT


public:
    explicit KeyboardDock(QWidget *parent = 0);
    ~KeyboardDock();

    void init(const QList<KeyboardKey> &map);


private:
    Ui::KeyboardDockTitleWidget ui_title;
    QList<KeyWidget*> m_keyList;
    FlowLayout *m_layout;


private slots:
    void keyPressed(const KeyMon::Event &event);
    void sizeChanged(const int &value);


};


#endif // KEYBOARDDOCK_H
