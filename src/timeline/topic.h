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

#ifndef TOPIC_H
#define TOPIC_H


//KDE
#include <kicon.h>

// Qt
#include <QtGui/QProgressBar>
#include <QtCore/QTime>


class QToolButton;
class QLabel;
class Topic: public QProgressBar
{
    Q_OBJECT


public:
    Topic(QWidget *parent, const QTime duration, const QString &title, const QString &icon);
    ~Topic();

    unsigned long currentSecond() const;
    QString icon() const;
    QString title() const;
    QTime duration() const;
    unsigned long durationToSeconds() const;
    static QTime secondsToTime(const unsigned long seconds);

    void setCurrentSecond(const unsigned long &second);
    void setIcon(const QString &icon);
    void setTitle(const QString &title);


private:
    unsigned long m_currentSecond;
    QTime m_duration;
    QString m_title;
    QString m_icon;


};


#endif // TOPIC_H
