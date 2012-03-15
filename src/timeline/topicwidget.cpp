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
#include "topicwidget.h"
#include "topic.h"

// KDE
#include <kdebug.h>
#include <klocalizedstring.h>

// Qt
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QLabel>


TopicWidget::TopicWidget(QWidget *parent)
    : QWidget(parent)
{

    m_layout = new QStackedLayout;
    setLayout(m_layout);

    clear();

}


TopicWidget::~TopicWidget()
{


}


void TopicWidget::setTime(const unsigned long &seconds)
{

    m_seconds = seconds;

}


Topic *TopicWidget::addTopic(const QTime &duration, const QString title, const QString &icon)
{

    Topic *newTopic = new Topic(this, duration, title, icon);
    m_layout->addWidget(newTopic);

    unsigned long total = 0;
    for (int i = 0; i < m_layout->count(); i++) {
        Topic *topic = static_cast<Topic*>(m_layout->widget(i));
        if (topic != m_noTopic) {
            total += topic->durationToSeconds();
        }
    }
    emit durationChanged(total);

    return newTopic;

}


QList<Topic*> TopicWidget::topics() const
{

    QList<Topic*> list;
    for (int i = 0; i < m_layout->count(); i++) {
        Topic *topic = static_cast<Topic*>(m_layout->widget(i));
        if (topic != m_noTopic) {
            list.append(topic);
        }
    }
    return list;

}


void TopicWidget::setCurrentSecond(const unsigned long &second)
{

    if (second == (unsigned long)-1) {
        m_layout->setCurrentWidget(m_noTopic);
        emit topicChanged(0);
        return;
    }


    Topic *oldTopic = static_cast<Topic*>(m_layout->currentWidget());

    bool found = false;
    unsigned long duration = 0;
    for (int i = 1; i < m_layout->count(); i++) {
        Topic *topic = static_cast<Topic*>(m_layout->widget(i));
        duration += topic->durationToSeconds();

        if (duration > second) {
            m_layout->setCurrentWidget(topic);
            found = true;
            break;
        }

    }

    if (!found) {
        m_layout->setCurrentWidget(m_noTopic);
    }

    Topic *topic = static_cast<Topic*>(m_layout->currentWidget());
    if (topic) {
        topic->setCurrentSecond((duration-second));
        if (!oldTopic || topic != oldTopic) {
            emit topicChanged(topic);
        }
    }

}


void TopicWidget::clear()
{

    while (!m_layout->isEmpty()) {
        delete m_layout->takeAt(0);
    }
    m_noTopic = new Topic(this, QTime(0, 0, 10, 0), i18n("No Topic"), "dialog-information");
    m_layout->addWidget(m_noTopic);

    emit durationChanged(0);

}


#include "topicwidget.moc"


