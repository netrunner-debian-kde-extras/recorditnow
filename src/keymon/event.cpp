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


// own
#include "event.h"

// KDE
#include <klocalizedstring.h>

// Qt
#include <QtCore/QMetaType>

// X
#include <X11/Xlib.h>


namespace KeyMon {


Event::Event()
{


}


Event::Event(const KeyMon::Event &other)
{

    key = other.key;
    pressed = other.pressed;

}


Event::~Event()
{



}



QString Event::name(const KeyMon::Event::Key &key)
{

    switch (key) {
    case LeftButton: return i18n("Left-Button");
    case RightButton: return i18n("Right-Button");
    case MiddleButton: return i18n("Middle-Button");
    case SpecialButton1: return i18n("Special-Button 1");
    case SpecialButton2: return i18n("Special-Button 2");
    case WheelUp: return i18n("Wheel up");
    case WheelDown: return i18n("Wheel down");
    default: return i18n("No-Button");
    }

}


KeyMon::Event::Key Event::keyFromName(const QString &name)
{


    if (name == i18n("Left-Button")) {
        return LeftButton;
    }

    if (name == i18n("Right-Button")) {
        return RightButton;
    }

    if (name == i18n("Middle-Button")) {
        return MiddleButton;
    }

    if (name == i18n("Special-Button 1")) {
        return SpecialButton1;
    }

    if (name == i18n("Special-Button 2")) {
        return SpecialButton2;
    }

    if (name == i18n("Wheel up")) {
        return WheelUp;
    }

    if (name == i18n("Wheel down")) {
        return WheelDown;
    }

    return NoButton;

}


KeyMon::Event::Key Event::xButtonToKey(const int &button)
{

    switch (button) {
    case Button1: return LeftButton;
    case Button3: return RightButton;
    case Button2: return MiddleButton;
    case 8: return SpecialButton1;
    case 9: return SpecialButton2;
    case Button4: return WheelUp;
    case Button5: return WheelDown;
    default: return NoButton;
    }

}


int Event::keyToXButton(const KeyMon::Event::Key &key)
{

    switch (key) {
    case LeftButton: return Button1;
    case RightButton: return Button3;
    case MiddleButton: return Button2;
    case SpecialButton1: return 8;
    case SpecialButton2: return 9;
    case WheelUp: return Button4;
    case WheelDown: return Button5;
    default: return -1;
    }

}



}; // Namespace KeyMon
