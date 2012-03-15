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


#ifndef ZOOMVIEW_H
#define ZOOMVIEW_H


// Qt
#include <QtGui/QWidget>


class QTimer;
class ZoomView : public QWidget
{
    Q_OBJECT


public:
    enum Quality {
        Low = 0,
        High = 1
    };

    ZoomView(QWidget *parent = 0);
    ~ZoomView();

    qreal factor() const;

    void start();
    void stop();
    void setFactor(const qreal &factor);
    void setQuality(const ZoomView::Quality &quality);
    void setFPS(const int &fps);


private:
    QPixmap m_pixmap;
    qreal m_factor;
    QTimer *m_timer;
    Quality m_quality;


private slots:
    void updateView();
    

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);


signals:
    void factorChanged(const int &factor);


};


#endif // ZOOMVIEW_H
