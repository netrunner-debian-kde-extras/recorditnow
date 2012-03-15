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

// own
#include "keywidget.h"

// KDE
#include <kdebug.h>
#include <kicon.h>
#include <kiconloader.h>

// Qt
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtCore/QTimer>
#include <QtGui/QSizePolicy>
#include <QtCore/QPropertyAnimation>


KeyWidget::KeyWidget(const KeyboardKey &key, QWidget *parent)
    : QWidget(parent), m_key(key), m_image(key.icon()), m_pressed(false)
{

    setContentsMargins(2, 2, 2, 2);
    setMinimumSize(36, 36);
    setMaximumSize(200, 200);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(150);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    m_animation = new QPropertyAnimation(this);
    m_animation->setTargetObject(this);
    m_animation->setPropertyName("opacity");
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::Linear);

    connect(m_animation, SIGNAL(finished()), this, SLOT(animationFinished()));

    m_opacity = 0.0;
    setOpacity(0.3);

    show();

}


KeyWidget::~KeyWidget()
{

    delete m_timer;
    delete m_animation;

}


int KeyWidget::keyCode() const
{

    return m_key.code();

}


double KeyWidget::opacity() const
{

    return m_opacity;

}


void KeyWidget::setPressed(const bool &pressed)
{

    if (m_pressed == pressed) {
        return;
    }

    if (m_timer->isActive()) {
        m_timer->stop();
    }

    m_pressed = pressed;
    if (m_animation->state() == QPropertyAnimation::Running) {
        if (!pressed) {
            return;
        }
        m_animation->stop();
    }

    m_animation->setStartValue(opacity());
    if (pressed) {
        m_animation->setEndValue(1.0);
        m_animation->start();
    } else {
        if (m_animation->state() != QPropertyAnimation::Running) {
            m_timer->start();
        }
    }

}


void KeyWidget::setOpacity(const double &opacity)
{

    if (m_opacity == opacity) {
        return;
    }
    m_opacity = opacity;
    update();

}


void KeyWidget::timeout()
{

    m_animation->setStartValue(opacity());
    m_animation->setEndValue(0.3);
    m_animation->start();

}


void KeyWidget::animationFinished()
{

    if (!m_pressed) {
        m_timer->start();
    }

}


void KeyWidget::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.setClipRegion(event->region());
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

    painter.setOpacity(opacity());

    QRect rect = contentsRect();
    rect.setWidth(rect.height());
    rect.moveCenter(contentsRect().center());

    painter.drawPixmap(rect, m_image);

}


#include "keywidget.moc"

