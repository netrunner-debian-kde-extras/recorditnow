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
#include "topictreeitem.h"
#include "topic.h"

// KDE
#include <klocalizedstring.h>
#include <kicondialog.h>

// Qt
#include <QtGui/QTimeEdit>


TopicTreeItem::TopicTreeItem(QTreeWidget *parent)
    : QTreeWidgetItem(), m_parent(parent)
{

    setText(0, i18n("Untitled"));
    setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    KIconButton *button = new KIconButton;
    QTimeEdit *duration = new QTimeEdit;
    connect(duration, SIGNAL(timeChanged(QTime)), this, SIGNAL(durationChanged()));

    duration->setDisplayFormat("hh:mm:ss");

    button->setMaximumSize(KIconLoader::SizeMedium, KIconLoader::SizeMedium);
    button->setMinimumSize(KIconLoader::SizeMedium, KIconLoader::SizeMedium);
    button->setIconSize(KIconLoader::SizeSmall);
    button->setIcon("dialog-information");

    parent->invisibleRootItem()->addChild(this);
    parent->setItemWidget(this, 1, button);
    parent->setItemWidget(this, 2, duration);

}


TopicTreeItem::TopicTreeItem(QTreeWidget *parent, TopicTreeItem *other, const int &index)
    : QTreeWidgetItem(), m_parent(parent)
{

    setText(0, other->text(0));
    setFlags(other->flags());

    KIconButton *button = new KIconButton;
    QTimeEdit *duration = new QTimeEdit;
    connect(duration, SIGNAL(timeChanged(QTime)), this, SIGNAL(durationChanged()));

    duration->setDisplayFormat("hh:mm:ss");

    button->setMaximumSize(KIconLoader::SizeMedium, KIconLoader::SizeMedium);
    button->setMinimumSize(KIconLoader::SizeMedium, KIconLoader::SizeMedium);
    button->setIconSize(KIconLoader::SizeSmall);

    button->setIcon(static_cast<KIconButton*>(parent->itemWidget(other, 1))->icon());
    duration->setTime(static_cast<QTimeEdit*>(parent->itemWidget(other, 2))->time());

    parent->invisibleRootItem()->insertChild(index, this);
    parent->setItemWidget(this, 1, button);
    parent->setItemWidget(this, 2, duration);

}


TopicTreeItem::~TopicTreeItem()
{




}


Topic *TopicTreeItem::topic() const
{

    return m_topic;

}


unsigned long TopicTreeItem::duration()
{

    QTime time = static_cast<QTimeEdit*>(m_parent->itemWidget(this, 2))->time();
    unsigned long duration = time.second();
    duration += time.minute()*60;
    duration += (time.hour()*60)*60;
    return duration;

}


void TopicTreeItem::setTopic(Topic *topic)
{

    m_topic = topic;

    QTimeEdit *duration = static_cast<QTimeEdit*>(m_parent->itemWidget(this, 2));
    KIconButton *button = static_cast<KIconButton*>(m_parent->itemWidget(this, 1));

    setText(0, topic->title());
    button->setIcon(topic->icon());
    duration->setTime(topic->duration());

}


#include "topictreeitem.moc"
