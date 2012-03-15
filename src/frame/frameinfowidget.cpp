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
#include "frameinfowidget.h"
#include "frame.h"

// Qt
#include <QtCore/QTimer>
#include <QtCore/QEvent>


FrameInfoWidget::FrameInfoWidget(Frame *parent)
    : QLabel(parent, Qt::FramelessWindowHint|Qt::Tool), m_frame(parent)
{

    Q_ASSERT(m_frame);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    parent->installEventFilter(this);
    setAlignment(Qt::AlignCenter);
    setFrameStyle(QFrame::Panel|QFrame::Raised);

}


FrameInfoWidget::~FrameInfoWidget()
{

    delete m_timer;

}


void FrameInfoWidget::moveToFrame()
{

    QRect geometry = this->geometry();
    geometry.moveCenter(m_frame->geometry().center());
    move(geometry.topLeft());
    resize(sizeHint().width()+10, sizeHint().height()+10);
    raise();

}


void FrameInfoWidget::timeout()
{

    hide();

}


bool FrameInfoWidget::eventFilter(QObject *watched, QEvent *event)
{

        if (watched == parent()) {
            switch (event->type()) {
            case QEvent::Move: moveToFrame(); break;
            case QEvent::Show:
            case QEvent::Resize: {
                    if (m_timer->isActive()) {
                        m_timer->stop();
                    }
                    QRect geometry = m_frame->getFrameGeometry();
                    QString text;
                    text = QString::number(geometry.width());
                    text += " x ";
                    text += QString::number(geometry.height());
                    setText(text);
                    if (!isVisible() && parentWidget()->isVisible()) {
                        show();
                    }
                    moveToFrame();
                    m_timer->start(2000);
                    break;
                }
            case QEvent::Hide: hide(); break;
            default: break;
            }
        }
    return QWidget::eventFilter(watched, event);

}


#include "frameinfowidget.moc"

