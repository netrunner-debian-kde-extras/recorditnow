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

#ifndef CHECKICON_H
#define CHECKICON_H


// Qt
#include <QtGui/QWidget>

class CheckIcon : public QWidget
{
    Q_OBJECT


public:
    explicit CheckIcon(QWidget *parent = 0);
    ~CheckIcon();

    bool isChecked() const;
    QString icon() const;

    void setIcon(const QString &icon);


public slots:
    void setChecked(const bool &c);


private:
    QString m_icon;
    bool m_checked;
    QPixmap m_pixmap;
    bool m_hover;


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);


signals:
    void checked(const bool &checked);


};


#endif // CHECKICON_H
