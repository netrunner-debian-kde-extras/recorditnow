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
#include "flowlayout.h"
#include "proxylayout.h"

// KDE
#include <kdebug.h>

// Qt
#include <QtGui/QWidget>


FlowLayout::FlowLayout()
    : m_itemHeight(50)
{

    setSpacing(10);
    setMargin(0);

}


FlowLayout::~FlowLayout()
{

    clear();

}


void FlowLayout::addItem(QLayoutItem *item)
{

    ProxyLayout *proxy = new ProxyLayout;
    proxy->addItem(item);

    m_proxyList.append(proxy);

}


int FlowLayout::count() const
{

    return m_proxyList.size();

}


QLayoutItem *FlowLayout::itemAt(int index) const
{

    if (index >= 0 && index < m_proxyList.size()) {
        return m_proxyList.at(index)->itemAt(0);;
    } else {
        return 0;
    }

}


QLayoutItem *FlowLayout::takeAt(int index)
{

    if (index >= 0 && index < m_proxyList.size()) {
        ProxyLayout *proxy = m_proxyList.takeAt(0);
        QLayoutItem *item = proxy->takeAt(0);

        delete proxy;
        return item;
    } else {
        return 0;
    }

}


Qt::Orientations FlowLayout::expandingDirections() const
{

    return 0;

}


bool FlowLayout::hasHeightForWidth() const
{

    return true;

}


int FlowLayout::heightForWidth(int width) const
{

    return doLayout(QRect(0, 0, width, 0), true);

}


void FlowLayout::setGeometry(const QRect &rect)
{

    QLayout::setGeometry(rect);
    doLayout(rect, false);

}


void FlowLayout::setItemHeight(const int &height)
{

    if (m_itemHeight == height) {
        return;
    }

    m_itemHeight = height;
    doLayout(geometry(), false);

}


void FlowLayout::clear()
{

    while (count() != 0) {
        QLayoutItem *item = takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

}


QSize FlowLayout::sizeHint() const
{

    return minimumSize();

}


QSize FlowLayout::minimumSize() const
{

    QSize size;
    for (int i = 0; i < count(); i++) {
        size = size.expandedTo(QSize(m_itemHeight, m_itemHeight));
    }

    size += QSize(2*margin(), 2*margin());
    return size;

}


int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{

    int x = rect.x();
    int y = rect.y();
    int lineHeight = 0;
    foreach (QLayoutItem *item, m_proxyList) {
        int nextX = x + m_itemHeight + spacing();
        if (nextX - spacing() > rect.right() && lineHeight > 0) {
            x = rect.x();
            y = y + lineHeight + spacing();
            nextX = x + m_itemHeight + spacing();
            lineHeight = 0;
        }

        if (!testOnly) {
            item->setGeometry(QRect(QPoint(x, y), QSize(m_itemHeight, m_itemHeight)));
        }

        x = nextX;
        lineHeight = qMax(lineHeight, m_itemHeight);
    }
    return y + lineHeight - rect.y();

}
