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


#ifndef FRAMEINFOWIDGET_H
#define FRAMEINFOWIDGET_H


// Qt
#include <QtGui/QLabel>


class Frame;
class QTimer;
class FrameInfoWidget : public QLabel
{
    Q_OBJECT


public:
    explicit FrameInfoWidget(Frame *parent);
    ~FrameInfoWidget();


private:
    QTimer *m_timer;
    Frame *m_frame;

    void moveToFrame();


private slots:
    void timeout();


protected:
    bool eventFilter(QObject *watched, QEvent *event);


};


#endif // FRAMEINFOWIDGET_H
