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


#ifndef KEYBOARDKEY_H
#define KEYBOARDKEY_H


// Qt
#include <QtCore/QString>


class QKeyEvent;
class KeyboardKey
{


public:
    explicit KeyboardKey(const int &code, const QString &icon, const QString &text);
    ~KeyboardKey();

    int code() const;
    QString icon() const;
    QString text() const;

    static KeyboardKey eventToKey(const QKeyEvent *event);

    void setCode(const int &code);
    void setIcon(const QString &icon);
    void setText(const QString &text);


private:
    int m_code;
    QString m_icon;
    QString m_text;


};

#endif // KEYBOARDKEY_H
