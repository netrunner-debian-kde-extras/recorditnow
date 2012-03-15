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
#include "keyboardkey.h"

// KDE
#include <klocalizedstring.h>

// Qt
#include <QtGui/QKeyEvent>


KeyboardKey::KeyboardKey(const int &code, const QString &icon, const QString &text)
    : m_code(code), m_icon(icon), m_text(text)
{



}


KeyboardKey::~KeyboardKey()
{



}


int KeyboardKey::code() const
{

    return m_code;

}


QString KeyboardKey::icon() const
{

    return m_icon;

}


QString KeyboardKey::text() const
{

    return m_text;

}


KeyboardKey KeyboardKey::eventToKey(const QKeyEvent *event)
{

    QString text = event->text().trimmed();

    switch (event->key()) {
    case Qt::Key_Escape: text = i18nc("Keyboard key", "Escape"); break;
    case Qt::Key_Tab: text = i18nc("Keyboard key", "Tab"); break;
    case Qt::Key_Shift: text = i18nc("Keyboard key", "Shift"); break;
    case Qt::Key_Alt: text = i18nc("Keyboard key", "Alt"); break;
    case Qt::Key_AltGr: text = i18nc("Keyboard key", "AltGr"); break;
    case Qt::Key_Backtab: text = i18nc("Keyboard key", "Backtab"); break;
    case Qt::Key_Backspace: text = i18nc("Keyboard key", "Backspace"); break;
    case Qt::Key_Return: text = i18nc("Keyboard key", "Return"); break;
    case Qt::Key_Enter: text = i18nc("Keyboard key", "Enter"); break;
    case Qt::Key_Left: text = i18nc("Keyboard key", "Left"); break;
    case Qt::Key_Up: text = i18nc("Keyboard key", "Up"); break;
    case Qt::Key_Right: text = i18nc("Keyboard key", "Right"); break;
    case Qt::Key_Down: text = i18nc("Keyboard key", "Down"); break;
    case Qt::Key_PageUp: text = i18nc("Keyboard key", "Page up"); break;
    case Qt::Key_PageDown: text = i18nc("Keyboard key", "Page Down"); break;
    case Qt::Key_Control: text = i18nc("Keyboard key", "Control"); break;
    case Qt::Key_Meta: text = i18nc("Keyboard key", "Meta"); break;
    case Qt::Key_CapsLock: text = i18nc("Keyboard key", "Caps lock"); break;
    case Qt::Key_NumLock: text = i18nc("Keyboard key", "Num lock"); break;
    case Qt::Key_Space: text = i18nc("Keyboard key", "Space"); break;
    case Qt::Key_F1: text = i18nc("Keyboard key", "F1"); break;
    case Qt::Key_F2: text = i18nc("Keyboard key", "F2"); break;
    case Qt::Key_F3: text = i18nc("Keyboard key", "F3"); break;
    case Qt::Key_F4: text = i18nc("Keyboard key", "F4"); break;
    case Qt::Key_F5: text = i18nc("Keyboard key", "F5"); break;
    case Qt::Key_F6: text = i18nc("Keyboard key", "F6"); break;
    case Qt::Key_F7: text = i18nc("Keyboard key", "F7"); break;
    case Qt::Key_F8: text = i18nc("Keyboard key", "F8"); break;
    case Qt::Key_F9: text = i18nc("Keyboard key", "F9"); break;
    case Qt::Key_F10: text = i18nc("Keyboard key", "F10"); break;
    case Qt::Key_F11: text = i18nc("Keyboard key", "F11"); break;
    case Qt::Key_F12: text = i18nc("Keyboard key", "F12"); break;
    case Qt::Key_Print: text = i18nc("Keyboard key", "Print"); break;
    case Qt::Key_Insert: text = i18nc("Keyboard key", "Insert"); break;
    case Qt::Key_Delete: text = i18nc("Keyboard key", "Delete"); break;
    case Qt::Key_End: text = i18nc("Keyboard key", "End"); break;
    case Qt::Key_Pause: text = i18nc("Keyboard key", "Pause"); break;
    default: break;
    }

    if (text.isEmpty()) {
        text = i18n("Special key");
    }

    return KeyboardKey(-1, QString(), text);

}


void KeyboardKey::setCode(const int &code)
{

    m_code = code;

}


void KeyboardKey::setIcon(const QString &icon)
{

    m_icon = icon;

}


void KeyboardKey::setText(const QString &text)
{

    m_text = text;

}


