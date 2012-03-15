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
#include "movewidget.h"

// KDE
#include <kdebug.h>
#include <kicon.h>
#include <kiconloader.h>

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>


MoveWidget::MoveWidget(QWidget *parent)
    : QFrame(parent, Qt::FramelessWindowHint|Qt::Tool|Qt::X11BypassWindowManagerHint), m_move(false)
{

    Q_ASSERT(parent);
    parent->installEventFilter(this);
    setCursor(Qt::SizeAllCursor);

    setMaximumSize(50, 30);
    setMinimumSize(maximumSize());

    setMouseTracking(true);
    setFrameStyle(QFrame::Panel|QFrame::Plain);

    setVisible(parentWidget()->isVisible());

}


MoveWidget::~MoveWidget()
{



}


void MoveWidget::moveToParent()
{

    const QRect parentGeometry = parentWidget()->geometry();

    int size;
    parentWidget()->getContentsMargins(&size, &size, &size, &size);
    size = size/2;

    QPoint center;
    center.setX(parentGeometry.x()+(parentGeometry.width()/2));
    center.setY(parentGeometry.y()+size);
    QRect newGeometry = geometry();
    newGeometry.moveCenter(center);
    setGeometry(newGeometry);

}


bool MoveWidget::eventFilter(QObject *watched, QEvent *event)
{

    if (!m_move) {
        switch (event->type()) {
        case QEvent::Show: show();
        case QEvent::Move:
        case QEvent::Resize: moveToParent(); break;
        case QEvent::Hide: hide(); break;
        default: break;
        }
    }
    return QWidget::eventFilter(watched, event);

}


void MoveWidget::mouseMoveEvent(QMouseEvent *event)
{

    if (!m_move) {
        return;
    }

    QRect geometry = this->geometry();
    const QPoint way = m_pos-event->globalPos();

    geometry.moveTopLeft(geometry.topLeft()-way);
    m_pos = event->globalPos();

    setGeometry(geometry);

    geometry = parentWidget()->geometry();
    QPoint topLeft = this->geometry().center();

    int size;
    parentWidget()->getContentsMargins(&size, &size, &size, &size);
    size = size/2;

    topLeft.setX(topLeft.x()-(geometry.width()/2));
    topLeft.setY(topLeft.y()-size);

    geometry.moveTopLeft(topLeft);
    parentWidget()->setGeometry(geometry);

}


void MoveWidget::mousePressEvent(QMouseEvent *event)
{

    m_move = true;
    m_pos = event->globalPos();

}


void MoveWidget::mouseReleaseEvent(QMouseEvent *event)
{

    Q_UNUSED(event);
    m_move = false;

}


void MoveWidget::showEvent(QShowEvent *event)
{

    QFrame::showEvent(event);
    moveToParent();

}


#include "movewidget.moc"
