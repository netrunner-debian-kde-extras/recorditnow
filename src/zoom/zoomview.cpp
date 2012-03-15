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
#include "zoomview.h"

// KDE
#include <kdebug.h>

// Qt
#include <QtGui/QX11Info>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <QtGui/QWheelEvent>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

// X
#ifdef XFIXES_FOUND
    #include <X11/extensions/Xfixes.h>
    #include <X11/Xlib.h>
#endif


ZoomView::ZoomView(QWidget *parent)
    : QWidget(parent)
{

    m_factor = 2;
    m_quality = Low;

    m_timer = new QTimer(this);
    m_timer->setInterval(1000/18);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateView()));

}



ZoomView::~ZoomView()
{

    delete m_timer;

}


qreal ZoomView::factor() const
{

    return m_factor;

}


void ZoomView::start()
{

    m_timer->start();

}


void ZoomView::stop()
{

    m_timer->stop();
    update();

}


void ZoomView::setFactor(const qreal &factor)
{

    m_factor = factor;
    m_factor = qMin((int)m_factor, 10);
    m_factor = qMax((int)m_factor, 1);
    updateView();

    emit factorChanged((int)factor);

}


void ZoomView::setQuality(const ZoomView::Quality &quality)
{

    m_quality = quality;
    updateView();

}


void ZoomView::setFPS(const int &fps)
{

    m_timer->setInterval(1000/fps);

}


void ZoomView::updateView()
{

    QRect target(0, 0, size().width()/m_factor, size().height()/m_factor);
    target.moveCenter(QCursor::pos());


    const int screen = x11Info().appScreen();
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(screen);

    // adjust target
    if (target.width() > screenGeometry.width()) {
        target.setWidth(screenGeometry.width());
    }

    if (target.height() > screenGeometry.height()) {
        target.setHeight(screenGeometry.height());
    }

    if (target.left() < screenGeometry.left()) {
        target.moveLeft(screenGeometry.left());
    }

    if (target.right() > screenGeometry.right()) {
        target.moveRight(screenGeometry.right());
    }

    if (target.top() < screenGeometry.top()) {
        target.moveTop(screenGeometry.top());
    }

    if (target.bottom() > screenGeometry.bottom()) {
        target.moveBottom(screenGeometry.bottom());
    }

    // grab
    m_pixmap = QPixmap::grabWindow(x11Info().appRootWindow(screen),
                                   target.x(),
                                   target.y(),
                                   target.width(),
                                   target.height());

#ifdef XFIXES_FOUND
    // cursor
    XFixesCursorImage *xcursor = XFixesGetCursorImage(QX11Info::display());
    unsigned char *pixels = (unsigned char*) malloc(xcursor->width*xcursor->height*4);
    for (int i = 0; i < xcursor->width*xcursor->height; i++) {
        unsigned long pix = xcursor->pixels[i];
        pixels[i * 4] = pix & 0xff;
        pixels[(i * 4) + 1] = (pix >> 8) & 0xff;
        pixels[(i * 4) + 2] = (pix >> 16) & 0xff;
        pixels[(i * 4) + 3] = (pix >> 24) & 0xff;
    }
    QImage qcursor(pixels, xcursor->width, xcursor->height, QImage::Format_ARGB32);
    QPainter painter(&m_pixmap);

    switch (m_quality) {
    case Low: painter.setRenderHints(QPainter::Antialiasing); break;
    case High: {
            painter.setRenderHints(QPainter::Antialiasing |
                                   QPainter::HighQualityAntialiasing |
                                   QPainter::SmoothPixmapTransform);
            break;
        }
    }

    float x = xcursor->x;
    x -= target.x()+xcursor->xhot;

    float y = xcursor->y;
    y -= target.y()+xcursor->yhot;

    painter.drawImage(x, y, qcursor);

    painter.end();

    free(pixels);
    XFree(xcursor);
#endif
    

    Qt::TransformationMode mode;
    switch (m_quality) {
    case Low: mode = Qt::FastTransformation; break;
    case High: mode = Qt::SmoothTransformation; break;
    default: mode = Qt::FastTransformation; break;
    }

    m_pixmap = m_pixmap.scaled(contentsRect().size(), Qt::KeepAspectRatio, mode);

    update();

}


void ZoomView::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setClipRegion(event->region());

    if (!m_timer->isActive()) {
        painter.fillRect(contentsRect(), Qt::black);
    } else {
        painter.drawPixmap(contentsRect(), m_pixmap);
    }

}


void ZoomView::wheelEvent(QWheelEvent *event)
{

    if (event->delta() > 0) {
        m_factor++;
        m_factor = qMin((int)m_factor, 10);
    } else {
        m_factor--;
        m_factor = qMax((int)m_factor, 1);
    }
    updateView();

}


#include "zoomview.moc"
