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

#ifndef TIMELINEDOCK_H
#define TIMELINEDOCK_H


// own
#include <ui_timelinedocktitlewidget.h>

// Qt
#include <QtGui/QDockWidget>


class Topic;
class Timeline;
class TimelineDock : public QDockWidget
{
    Q_OBJECT


public:
    explicit TimelineDock(QWidget *parent = 0);
    ~TimelineDock();


    Timeline *timeline() const;


private:
    Ui::TimelineDockTitleWidget ui_titleWidget;

    void resetTitle();


private slots:
    void topicChanged(Topic *topic);


};


#endif // TIMELINEDOCK_H
