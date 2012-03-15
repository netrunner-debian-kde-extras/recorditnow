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
#include "timelinedock.h"
#include "timeline.h"
#include <recorditnow.h>
#include "topic.h"

// KDE
#include <klocalizedstring.h>
#include <kconfiggroup.h>
#include <kicon.h>


TimelineDock::TimelineDock(QWidget *parent)
    : QDockWidget(i18n("Timeline"), parent)
{

    setObjectName("Timeline");
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    Timeline *line = new Timeline(this);
    setWidget(line);

    QWidget *title = new QWidget(this);
    ui_titleWidget.setupUi(title);

    resetTitle();

    setTitleBarWidget(title);

    connect(line, SIGNAL(currentTopicChanged(Topic*)), this, SLOT(topicChanged(Topic*)));

    KConfigGroup cfg(Settings::self()->config(), "Timeline");
    line->loadTopics(&cfg);

}


TimelineDock::~TimelineDock()
{

    KConfigGroup cfg(Settings::self()->config(), "Timeline");
    timeline()->saveTopics(&cfg);

}


Timeline *TimelineDock::timeline() const
{

    return static_cast<Timeline*>(widget());

}


void TimelineDock::resetTitle()
{

    ui_titleWidget.currentTopicLabel->setText(i18n("No Topic"));
    ui_titleWidget.iconButton->setIcon(KIcon("dialog-information"));

}


void TimelineDock::topicChanged(Topic *topic)
{

    if (!topic) {
        resetTitle();
        return;
    }
    ui_titleWidget.currentTopicLabel->setText(topic->title());
    ui_titleWidget.iconButton->setIcon(KIcon(topic->icon()));

}


#include "timelinedock.moc"
