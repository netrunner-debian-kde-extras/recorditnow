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
#include "mousebutton.h"

// KDE
#include <kdebug.h>
#include <klocalizedstring.h>

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QFontMetrics>

// X11
#include <X11/Xlib.h>


MouseButton::MouseButton(QWidget *parent)
    : KPushButton(parent)
{

    setCheckable(true);
    connect(this, SIGNAL(clicked()), this, SLOT(getButton()));

    setIcon(KIcon("input-mouse"));
    setButton(NoButton);

}


MouseButton::~MouseButton()
{



}


int MouseButton::getXButton() const
{

    switch (m_button) {
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


MouseButton::Button MouseButton::getMouseButton() const
{

    return m_button;

}


QString MouseButton::getName(const MouseButton::Button &button)
{

    switch (button) {
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


MouseButton::Button MouseButton::getButtonFromName(const QString &name)
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


MouseButton::Button MouseButton::getButtonFromXButton(const int &button)
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


void MouseButton::setXButton(const int &button)
{

    switch (button) {
    case 1: setButton(LeftButton); break;
    case 2: setButton(MiddleButton); break;
    case 3: setButton(RightButton); break;
    case 4: setButton(WheelUp); break;
    case 5: setButton(WheelDown); break;
    case 8: setButton(SpecialButton1); break;
    case 9: setButton(SpecialButton2); break;
    default: setButton(NoButton); break;
    }

}


void MouseButton::setButton(const Button &button)
{

    m_button = button;
    setText(getName(button));

}


void MouseButton::setText(const QString &text)
{

    KPushButton::setText(text);
    QFontMetrics fm(font());
    int width = fm.width(text);
    width += fm.height()*3;
    setMinimumWidth(width);
    resize(width, height());

}


void MouseButton::getButton()
{

    grabMouse();

}


void MouseButton::mousePressEvent(QMouseEvent *event)
{

    if (!mouseGrabber()) {
        setText(i18n("Press a mouse button..."));
        KPushButton::mousePressEvent(event);
    }

}


void MouseButton::mouseReleaseEvent(QMouseEvent *event)
{

    if (!mouseGrabber()) {
        KPushButton::mouseReleaseEvent(event);
    } else {
        releaseMouse();
        const Button oldButton = m_button;
        switch (event->button()) {
        case Qt::NoButton: setButton(NoButton); break;
        case Qt::LeftButton: setButton(LeftButton); break;
        case Qt::RightButton: setButton(RightButton); break;
        case Qt::MidButton: setButton(MiddleButton); break;
        case Qt::XButton1: setButton(SpecialButton1); break;
        case Qt::XButton2: setButton(SpecialButton2); break;
        default: setButton(NoButton); break;
        }
        setChecked(false);
        emit buttonChanged(oldButton, m_button);
    }

}


void MouseButton::wheelEvent(QWheelEvent *event)
{

    if (!mouseGrabber()) {
        KPushButton::wheelEvent(event);
    }   else {
        const Button oldButton = m_button;
        releaseMouse();
        if (event->delta() > 0) {
            setButton(WheelUp);
        } else {
            setButton(WheelDown);
        }
        setChecked(false);
        emit buttonChanged(oldButton, m_button);
    }

}


void MouseButton::resizeEvent(QResizeEvent *event)
{

    KPushButton::resizeEvent(event);
    emit sizeChanged();

}


#include "mousebutton.moc"
