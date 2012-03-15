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
#include "frame.h"
#include "frameinfowidget.h"
#include "movewidget.h"

// KDE
#include <kdebug.h>
#include <kwindowsystem.h>

// Qt
#include <QtGui/QResizeEvent>
#include <QtCore/QTimer>
#include <QtGui/QPainter>


#define FRAME_MIN_SIZE 100
#define PARENT_SPACING QPoint(0, 10)

Frame::Frame(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint|Qt::Tool|Qt::X11BypassWindowManagerHint)
{

    Q_ASSERT(parentWidget());
    m_moveWidget = 0;

    resize(FRAME_MIN_SIZE, FRAME_MIN_SIZE);
    setContentsMargins(8, 8, 8, 8);
    setMinimumSize(FRAME_MIN_SIZE-(8*2), FRAME_MIN_SIZE-(8*2));

    parent->installEventFilter(this);
    setMouseTracking(true);
    m_side = NoSide;
    m_active = false;

    m_infoWidget = new FrameInfoWidget(this);
    m_infoWidget->hide();

}


Frame::~Frame()
{

    delete m_infoWidget;
    if (m_moveWidget) {
        delete m_moveWidget;
    }

}


QRect Frame::getFrameGeometry() const
{

    QRect geometry = this->geometry();
    geometry.setHeight(geometry.height()-(getLineSize()*2));
    geometry.setWidth(geometry.width()-(getLineSize()*2));
    geometry.moveCenter(this->geometry().center());
    return geometry;

}


void Frame::setFrameSize(const QSize &size)
{

    resize(size.width()+(getLineSize()*2), size.height()+(getLineSize()*2));
    moveToParent();

}


void Frame::setVisible(bool visible)
{

    m_active = visible;
    QWidget::setVisible(visible);

}


void Frame::setMoveEnabled(const bool &enabled)
{

    if (enabled) {
        if (!m_moveWidget) {
            m_moveWidget = new MoveWidget(this);
        }
    } else {
        if (m_moveWidget) {
            delete m_moveWidget;
            m_moveWidget = 0;
        }
    }
    moveToParent(true);

}


QRect Frame::getRect(const Side &side) const
{

    const int size = getLineSize();
    switch (side) {
    case TopLeft: return QRect(0, 0, size, size);
    case TopRight: return QRect(rect().topRight()-QPoint(size, 0), QSize(size, size));
    case BottomLeft: return QRect(rect().bottomLeft()-QPoint(0, size), QSize(size, size));
    case BottomRight: return QRect(rect().bottomRight()-QPoint(size, size), QSize(size, size));
    case Left: return QRect(0, 0, size, height());
    case Right: return QRect(rect().topRight()-QPoint(size, 0), QSize(size, height()));
    case Top: return QRect(0, 0, width(), size);
    case Bottom: return QRect(rect().bottomLeft()-QPoint(0, size), QSize(width(), size));
    default: return QRect();
    }

}


int Frame::getLineSize() const
{

    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    return top;

}


void Frame::moveToParent(const bool &force)
{

    if (m_moveWidget && !force) {
        return;
    }

    QRect parentGeometry = parentWidget()->frameGeometry();
    // geometry() returns sometimes/always x(0) y(0) if the window has not yet moved
    if (parentGeometry.x() < 1 && parentGeometry.y() < 1) {
        const KWindowInfo info = KWindowSystem::self()->windowInfo(parentWidget()->winId(),
                                                                   NET::WMFrameExtents);
        parentGeometry = info.frameGeometry();
    }

    QRect newGeometry = geometry();
    newGeometry.moveTopLeft(parentGeometry.bottomLeft()+PARENT_SPACING);
    setGeometry(newGeometry);

}


void Frame::moveParentToFrame()
{

    if (m_moveWidget) {
        return;
    }

    QRect geometry = parentWidget()->frameGeometry();
    const QPoint pos = geometry.topLeft()-parentWidget()->geometry().topLeft();
    const QSize size = geometry.size()-parentWidget()->geometry().size();

    geometry.moveBottomLeft(this->geometry().topLeft()-PARENT_SPACING);
    geometry.moveTopLeft(geometry.topLeft()-pos);
    geometry.setSize(geometry.size()-size);

    parentWidget()->setGeometry(geometry);

}


void Frame::show()
{

    QWidget::setVisible(true);

}


void Frame::hide()
{

    QWidget::setVisible(false);

}


bool Frame::eventFilter(QObject *watched, QEvent *event)
{

    if (m_side == NoSide) {
        if (watched == parent()) {
            switch (event->type()) {
            case QEvent::Move:
            case QEvent::Resize: moveToParent(); break;
            case QEvent::WindowDeactivate: {
                    if (m_active && !m_moveWidget) {
                        hide();
                    }
                    break;
                }
            case QEvent::WindowActivate: {
                    if (m_active && !m_moveWidget) {
                        show();
                    }
                    break;
                }
            default: break;
            }
        }
    }
    return QWidget::eventFilter(watched, event);

}


void Frame::resizeEvent(QResizeEvent *event)
{

    QWidget::resizeEvent(event);
    setMask(QRegion(contentsRect()).xored(QRegion(rect())));

}


void Frame::mouseMoveEvent(QMouseEvent *event)
{

    if (m_side == NoSide) { // set cursor
        if (getRect(TopLeft).contains(event->pos())) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (getRect(TopRight).contains(event->pos())) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (getRect(BottomLeft).contains(event->pos())) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (getRect(BottomRight).contains(event->pos())) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (getRect(Left).contains(event->pos())) {
            setCursor(Qt::SizeHorCursor);
        } else if (getRect(Right).contains(event->pos())) {
            setCursor(Qt::SizeHorCursor);
        } else if (getRect(Top).contains(event->pos())) {
            setCursor(Qt::SizeVerCursor);
        } else if (getRect(Bottom).contains(event->pos())) {
            setCursor(Qt::SizeVerCursor);
        }
    } else { // resize
        QRect geometry = this->geometry();
        QRect tmp = geometry;
        switch (m_side) {
        case Left: {
                tmp.setLeft(event->globalX());
                if (tmp.width() < FRAME_MIN_SIZE) {
                    tmp.setRight(geometry.right());
                    tmp.setLeft(tmp.right()-FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        case Right: {
                tmp.setRight(event->globalX());
                if (tmp.width() < FRAME_MIN_SIZE) {
                    tmp.setLeft(geometry.left());
                    tmp.setRight(tmp.left()+FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        case Top:  {
                tmp.setY(event->globalY());
                if (tmp.height() < FRAME_MIN_SIZE) {
                    tmp.setBottom(geometry.bottom());
                    tmp.setTop(tmp.bottom()-FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        case Bottom: {
                tmp.setBottom(event->globalY());
                if (tmp.height() < FRAME_MIN_SIZE) {
                    tmp.setTop(geometry.top());
                    tmp.setBottom(tmp.top()+FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        case TopLeft: {
                tmp.setTopLeft(event->globalPos());
                if (tmp.height() < FRAME_MIN_SIZE) {
                    tmp.setBottom(geometry.bottom());
                    tmp.setTop(tmp.bottom()-FRAME_MIN_SIZE);
                }

                if (tmp.width() < FRAME_MIN_SIZE) {
                    tmp.setRight(geometry.right());
                    tmp.setLeft(tmp.right()-FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        case TopRight:  {
                tmp.setTopRight(event->globalPos());
                if (tmp.height() < FRAME_MIN_SIZE) {
                    tmp.setBottom(geometry.bottom());
                    tmp.setTop(tmp.bottom()-FRAME_MIN_SIZE);
                }

                if (tmp.width() < FRAME_MIN_SIZE) {
                    tmp.setLeft(geometry.left());
                    tmp.setRight(tmp.left()+FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        case BottomLeft:  {
                tmp.setBottomLeft(event->globalPos());
                if (tmp.height() < FRAME_MIN_SIZE) {
                    tmp.setTop(geometry.top());
                    tmp.setBottom(tmp.top()+FRAME_MIN_SIZE);
                }

                if (tmp.width() < FRAME_MIN_SIZE) {
                    tmp.setRight(geometry.right());
                    tmp.setLeft(tmp.right()-FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        case BottomRight:  {
                tmp.setBottomRight(event->globalPos());
                if (tmp.height() < FRAME_MIN_SIZE) {
                    tmp.setTop(geometry.top());
                    tmp.setBottom(tmp.top()+FRAME_MIN_SIZE);
                }

                if (tmp.width() < FRAME_MIN_SIZE) {
                    tmp.setLeft(geometry.left());
                    tmp.setRight(tmp.left()+FRAME_MIN_SIZE);
                }
                geometry = tmp;
                break;
            }
        default: break;
        }

        setGeometry(geometry);
        moveParentToFrame();
    }

}


void Frame::mousePressEvent(QMouseEvent *event)
{

    if (event->button() & Qt::LeftButton) {
        if (getRect(TopLeft).contains(event->pos())) {
            m_side = TopLeft;
        } else if (getRect(TopRight).contains(event->pos())) {
            m_side = TopRight;
        } else if (getRect(BottomLeft).contains(event->pos())) {
            m_side = BottomLeft;
        } else if (getRect(BottomRight).contains(event->pos())) {
            m_side = BottomRight;
        } else if (getRect(Left).contains(event->pos())) {
            m_side = Left;
        } else if (getRect(Right).contains(event->pos())) {
            m_side = Right;
        } else if (getRect(Top).contains(event->pos())) {
            m_side = Top;
        } else if (getRect(Bottom).contains(event->pos())) {
            m_side = Bottom;
        } else {
            m_side = NoSide;
        }
    } else {
        m_side = NoSide;
    }

}


void Frame::mouseReleaseEvent(QMouseEvent *event)
{

    Q_UNUSED(event);
    m_side = NoSide;

}


void Frame::showEvent(QShowEvent *event)
{

    moveToParent();
    QWidget::showEvent(event);

}


void Frame::paintEvent(QPaintEvent *event)
{

    QWidget::paintEvent(event);

    QPainter painter(this);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(palette().color(QPalette::Shadow));
    painter.setPen(pen);

    QRect in = contentsRect();
    in.setHeight(in.height()+painter.pen().width());
    in.setWidth(in.width()+painter.pen().width());
    in.moveTopLeft(in.topLeft()-QPoint(painter.pen().width(), painter.pen().width()));
    painter.drawRect(in);

    QRect out = rect();
    out.setHeight(out.height()-painter.pen().width());
    out.setWidth(out.width()-painter.pen().width());
    painter.drawRect(out);

}


#include "frame.moc"

