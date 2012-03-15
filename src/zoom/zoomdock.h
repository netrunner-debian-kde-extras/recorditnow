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

#ifndef ZOOMDOCK_H
#define ZOOMDOCK_H


// own
#include "ui_zoomdock.h"
#include "ui_zoomdocktitlewidget.h"

// Qt
#include <QtGui/QDockWidget>


class QTimer;
class ZoomDock : public QDockWidget, Ui::ZoomDock
{
    Q_OBJECT


public:
    explicit ZoomDock(QWidget *parent = 0);
    ~ZoomDock();

    void setFactor(const qreal &factor);
    void setQuality(const ZoomView::Quality &quality);
    void setFPS(const int &fps);


public slots:
    void zoomIn();
    void zoomOut();


private:
    bool m_started;
    Ui::ZoomDockTitleWidget ui_title;
    QTimer *m_zoomTimer;


private slots:
    void startStopZoom();
    void factorChanged(const int &factor);
    void hideLabel();


};


#endif // ZOOMDOCK_H
