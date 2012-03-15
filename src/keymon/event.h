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

#ifndef EVENT_H
#define EVENT_H


// KDE
#include <kdemacros.h>

// Qt
#include <QtCore/QString>


namespace KeyMon {


class KDE_EXPORT Event
{


public:
    enum Key {
        LeftButton = 0,
        RightButton = 1,
        MiddleButton = 3,
        SpecialButton1 = 4,
        SpecialButton2 = 5,
        WheelUp = 6,
        WheelDown = 7,
        NoButton = -1
    };
    Event();
    Event(const KeyMon::Event &other);
    ~Event();

    Key key;
    int keyCode;
    bool pressed;
    bool mouseEvent;

    static QString name(const KeyMon::Event::Key &key);
    static KeyMon::Event::Key keyFromName(const QString &name);
    static KeyMon::Event::Key xButtonToKey(const int &button);
    static int keyToXButton(const KeyMon::Event::Key &key);


};


}; // Namespace KeyMon


#endif // EVENT_H
