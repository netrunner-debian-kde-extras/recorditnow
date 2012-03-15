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

#ifndef MOUSEBUTTON_H
#define MOUSEBUTTON_H


// KDE
#include <kpushbutton.h>


class MouseButton : public KPushButton
{
    Q_OBJECT


public:
    enum Button {
        NoButton = -1,
        LeftButton = 0,
        RightButton = 1,
        MiddleButton = 2,
        SpecialButton1 = 3,
        SpecialButton2 = 4,
        WheelUp = 5,
        WheelDown = 6
    };

    MouseButton(QWidget *parent = 0);
    ~MouseButton();

    int getXButton() const;
    MouseButton::Button getMouseButton() const;
    static QString getName(const MouseButton::Button &button);
    static MouseButton::Button getButtonFromName(const QString &name);
    static MouseButton::Button getButtonFromXButton(const int &button);

    void setXButton(const int &button);
    void setButton(const Button &button);


private:
    Button m_button;

    void setText(const QString &text);


private slots:
    void getButton();


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);


signals:
    void buttonChanged(const MouseButton::Button &oldButton, const MouseButton::Button &newButton);
    void sizeChanged();


};


#endif // MOUSEBUTTON_H
