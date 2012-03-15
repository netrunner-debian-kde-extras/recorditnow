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
#include "proxylayout.h"

// Qt
#include <QtCore/QPropertyAnimation>
#include <QtGui/QWidget>


ProxyLayout::ProxyLayout(QWidget *parent)
    : QLayout(parent), m_item(0), m_animation( new QPropertyAnimation)
{


}


ProxyLayout::~ProxyLayout()
{

    delete m_animation;
    if (m_item) {
        takeAt(0);
        delete m_item;
    }

}


void ProxyLayout::addItem(QLayoutItem *item)
{

    Q_ASSERT(!m_item);
    Q_ASSERT(item->widget());
    m_item = item;

    m_animation->setTargetObject(m_item->widget());
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::Linear);
    m_animation->setPropertyName("geometry");

    setGeometry(geometry());

}


void ProxyLayout::setGeometry(const QRect &newGeometry)
{

    QLayout::setGeometry(newGeometry);
    if (m_item) {
        if (m_animation->state() == QPropertyAnimation::Running) {
            m_animation->stop();
        }
        m_animation->setStartValue(m_item->geometry());
        m_animation->setEndValue(newGeometry);
        m_animation->start();
    }

}


int ProxyLayout::count() const
{

    if (m_item) {
        return 1;
    } else {
        return 0;
    }

}


QLayoutItem *ProxyLayout::itemAt(int index) const
{

    if (index == 0) {
        return m_item;
    } else {
        return 0;
    }

}


QLayoutItem *ProxyLayout::takeAt(int index)
{

    if (index != 0) {
        return 0;
    }

    QLayoutItem *item = m_item;
    m_item = 0;

    return item;

}


Qt::Orientations ProxyLayout::expandingDirections() const
{

    return 0;

}


QRect ProxyLayout::geometry() const
{

    return QLayout::geometry();

}


bool ProxyLayout::isEmpty() const
{

    return !m_item;

}


QSize ProxyLayout::maximumSize() const
{

    if (m_item) {
        return m_item->maximumSize();
    } else {
        return QSize();
    }

}


QSize ProxyLayout::minimumSize() const
{

    if (m_item) {
        return m_item->minimumSize();
    } else {
        return QSize();
    }

}


QSize ProxyLayout::sizeHint() const
{

    if (m_item) {
        return m_item->sizeHint();
    } else {
        return QSize();
    }

}


