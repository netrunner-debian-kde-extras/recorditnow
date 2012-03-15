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


#ifndef TIMELINE_H
#define TIMELINE_H


// own
#include "ui_timeline.h"

// Qt
#include <QtGui/QWidget>


class Topic;
class KConfigGroup;
class QTimer;
class Timeline : public QWidget, public Ui::Timeline
{
    Q_OBJECT


public:
    Timeline(QWidget *parent = 0);
    ~Timeline();

    unsigned long duration() const;

    void loadTopics(KConfigGroup *cfg);
    void saveTopics(KConfigGroup *cfg);
    void enableNotifications(const bool &enable);


public slots:
    void start();
    void stop();
    void pauseOrContinue();


private:
    enum State {
        Idle = 0,
        Running = 1,
        Paused = 2
    };

    QTimer *m_timer;
    State m_state;
    bool m_notifications;

    inline State state() const;
    void setState(const State &state);


private slots:
    void updateTime();
    void configure();
    void topicChanged(Topic *topic);
    void resetSlider();
    void durationChanged(const unsigned long newDuration);


signals:
    void finished();
    void currentTopicChanged(Topic *current);


};


#endif // TIMELINE_H
