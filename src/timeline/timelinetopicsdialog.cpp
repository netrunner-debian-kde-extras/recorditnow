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
#include "timelinetopicsdialog.h"
#include "topicwidget.h"
#include "topic.h"
#include "topictreeitem.h"

// KDE
#include <kicon.h>
#include <klocalizedstring.h>
#include <kicondialog.h>
#include <kdebug.h>

// Qt
#include <QtGui/QTimeEdit>


TimelineTopicsDialog::TimelineTopicsDialog(QWidget *parent, TopicWidget *topicWidget)
    : KDialog(parent), m_topicWidget(topicWidget)
{

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(i18n("Topics"));

    QWidget *widget = new QWidget(this);
    setupUi(widget);    
    setMainWidget(widget);

    addButton->setIcon(KIcon("list-add"));
    removeButton->setIcon(KIcon("list-remove"));
    upButton->setIcon(KIcon("go-up"));
    downButton->setIcon(KIcon("go-down"));

    connect(addButton, SIGNAL(clicked()), this, SLOT(addTopic()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeTopic()));
    connect(this, SIGNAL(finished(int)), this, SLOT(updateTopicWidget(int)));
    connect(upButton, SIGNAL(clicked()), this, SLOT(upClicked()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(downClicked()));
    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

    treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

    // load
    QList<Topic*> topics = m_topicWidget->topics();
    for (int i = 0; i < topics.size(); i++) {
        addTopic(); // create widgets
    }

    for (int i = 0; i < topics.size(); i++) {
        TopicTreeItem *item = static_cast<TopicTreeItem*>(treeWidget->invisibleRootItem()->child(i));
        item->setTopic(topics[i]);
    }

}


TimelineTopicsDialog::~TimelineTopicsDialog()
{


}


void TimelineTopicsDialog::addTopic()
{

    TopicTreeItem *item = new TopicTreeItem(treeWidget);
    connect(item, SIGNAL(durationChanged()), this, SLOT(updateTotalDuration()));

}


void TimelineTopicsDialog::removeTopic()
{

    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    foreach (QTreeWidgetItem *child, items) {
        treeWidget->invisibleRootItem()->removeChild(child);
    }
    updateTotalDuration();

}


void TimelineTopicsDialog::updateTopicWidget(const int &ret)
{

    if (ret != KDialog::Accepted) {
        return;
    }

    m_topicWidget->clear();

    for (int i = 0; i < treeWidget->invisibleRootItem()->childCount(); i++) {
        QTreeWidgetItem *item = treeWidget->invisibleRootItem()->child(i);

        QString title = item->text(0);
        QString icon = static_cast<KIconButton*>(treeWidget->itemWidget(item, 1))->icon();
        QTime dTime = static_cast<QTimeEdit*>(treeWidget->itemWidget(item, 2))->time();

        m_topicWidget->addTopic(dTime, title, icon);
    }

}


void TimelineTopicsDialog::upClicked()
{

    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    if (items.isEmpty() || items.size() > 1) {
        return;
    }
    TopicTreeItem *item = static_cast<TopicTreeItem*>(items[0]);

    const int index = treeWidget->indexOfTopLevelItem(item)-1;

    if (index < 0) {
        return;
    }

    TopicTreeItem *copy = new TopicTreeItem(treeWidget, item, index);
    connect(copy, SIGNAL(durationChanged()), this, SLOT(updateTotalDuration()));

    treeWidget->invisibleRootItem()->removeChild(item);
    treeWidget->setCurrentItem(copy);

}


void TimelineTopicsDialog::downClicked()
{

    QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
    if (items.isEmpty() || items.size() > 1) {
        return;
    }
    TopicTreeItem *item = static_cast<TopicTreeItem*>(items[0]);

    const int index = treeWidget->indexOfTopLevelItem(item)+2;

    if (index > treeWidget->topLevelItemCount()) {
        return;
    }

    TopicTreeItem *copy = new TopicTreeItem(treeWidget, item, index);
    connect(copy, SIGNAL(durationChanged()), this, SLOT(updateTotalDuration()));

    treeWidget->invisibleRootItem()->removeChild(item);
    treeWidget->setCurrentItem(copy);

}


void TimelineTopicsDialog::itemSelectionChanged()
{

    if(treeWidget->selectedItems().isEmpty() || treeWidget->selectedItems().size() > 1) {
        upButton->setEnabled(false);
        downButton->setEnabled(false);
        removeButton->setEnabled(false);
    } else {
        const int index = treeWidget->indexOfTopLevelItem(treeWidget->selectedItems().first());
        upButton->setEnabled(index != 0);
        downButton->setEnabled(index != treeWidget->topLevelItemCount()-1);
        removeButton->setEnabled(true);
    }

}


void TimelineTopicsDialog::updateTotalDuration()
{

    unsigned long duration = 0;
    for (int i = 0; i < treeWidget->invisibleRootItem()->childCount(); i++) {
        TopicTreeItem *item = static_cast<TopicTreeItem*>(treeWidget->invisibleRootItem()->child(i));
        duration += item->duration();
    }
    totalDurationEdit->setTime(Topic::secondsToTime(duration));

}


#include "timelinetopicsdialog.moc"

