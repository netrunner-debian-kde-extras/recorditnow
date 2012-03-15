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

#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H


// Qt
#include <QtGui/QLayout>


class ProxyLayout;
class FlowLayout : public QLayout
{


public:
    FlowLayout();
    ~FlowLayout();

    Qt::Orientations expandingDirections() const;
    bool hasHeightForWidth() const;
    int heightForWidth(int) const;
    int count() const;
    QLayoutItem *itemAt(int index) const;
    QSize minimumSize() const;
    QSize sizeHint() const;
    QLayoutItem *takeAt(int index);

    void addItem(QLayoutItem *item);
    void setGeometry(const QRect &rect);
    void setItemHeight(const int &height);
    void clear();


 private:
    int doLayout(const QRect &rect, bool testOnly) const;
    int m_itemHeight;
    QList<ProxyLayout*> m_proxyList;


};


#endif // FLOWLAYOUT_H
