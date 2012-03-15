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
#include "checkicon.h"

// KDE
#include <kicon.h>
#include <kiconloader.h>
#include <kdebug.h>
#include <kiconeffect.h>

// Qt
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>


CheckIcon::CheckIcon(QWidget *parent)
    : QWidget(parent), m_checked(false), m_hover(false)
{


}


CheckIcon::~CheckIcon()
{



}


bool CheckIcon::isChecked() const
{

    return m_checked;

}


QString CheckIcon::icon() const
{

    return m_icon;

}


void CheckIcon::setIcon(const QString &icon)
{

    m_icon = icon;
    m_pixmap = KIcon(icon).pixmap(KIconLoader::SizeMedium, KIconLoader::SizeMedium);

    update();

}


void CheckIcon::setChecked(const bool &c)
{

    if (c== m_checked) {
        return;
    }

    m_checked = c;
    update();

    emit checked(c);

}


void CheckIcon::paintEvent(QPaintEvent *event)
{

    if (m_pixmap.isNull()) {
        kWarning() << "Null pixmap!";
        return;
    }

    QPixmap result = m_pixmap;
    if (!m_checked) {
        QPainter pixPainter(&result);
        pixPainter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

        if (isEnabled()) {
            pixPainter.setPen(Qt::red);
        } else {
            pixPainter.setPen(Qt::gray);
        }

        QLine line1(result.rect().topLeft(), result.rect().bottomRight());
        QLine line2(result.rect().bottomLeft(), result.rect().topRight());

        pixPainter.drawLine(line1);
        pixPainter.drawLine(line2);

        pixPainter.end();
    }


    QIcon::Mode mode;

    if (!isEnabled()) {
        mode = QIcon::Disabled;
    } else {
        mode = QIcon::Normal;
    }

    QStyleOption options;
    options.initFrom(this);

    result = style()->generatedIconPixmap(mode, result, &options);

    if (isEnabled() && m_hover) {
        KIconEffect *effect = KIconLoader::global()->iconEffect();
        if (effect->hasEffect(KIconLoader::NoGroup, KIconLoader::ActiveState)) {
            result = effect->apply(result, KIconLoader::Desktop, KIconLoader::ActiveState);
        }
    }

    QPainter painter(this);
    painter.setClipRegion(event->region());
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

    QRect rect = contentsRect();
    rect.setHeight(rect.width());
    rect.moveCenter(contentsRect().center());

    painter.drawPixmap(rect, result);

}


void CheckIcon::mousePressEvent(QMouseEvent *event)
{

    event->accept();

}


void CheckIcon::mouseReleaseEvent(QMouseEvent *event)
{

    event->accept();
    setChecked(!isChecked());

}


void CheckIcon::enterEvent(QEvent *event)
{

    event->accept();
    m_hover = true;
    update();

}


void CheckIcon::leaveEvent(QEvent *event)
{

    event->accept();
    m_hover = false;
    update();

}


#include "checkicon.moc"
