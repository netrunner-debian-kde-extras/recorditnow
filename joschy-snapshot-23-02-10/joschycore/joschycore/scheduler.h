/*
    Copyright (C) 2010  Kai Dombrowe <just89@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H


// own
#include "joschy_global.h"
#include "plugin.h"

// Qt
#include <QtCore/QObject>
#include <QtCore/QPointer>


namespace Joschy {


class AbstractJob;
class SchedulerPrivate;
class AbstractResponseParser;

/**
 * @class Scheduler joschycore/scheduler.h
 *
 * @short Joschys job scheduler
 **/
class JOSCHY_EXPORT Scheduler : public QObject
{
    Q_OBJECT
    friend class SchedulerSingleton;
    friend class SchedulerPrivate;
    friend class Manager;


public:
    ~Scheduler();
    /**
     * Calling ths function makes that job gets scheduled for later execution,
     * if multiple jobs are registered it might wait for other jobs to finish.
     *
     * @param job the job to schedule
     **/
    static void schedule(Joschy::AbstractJob *job);


private:
    SchedulerPrivate *d;

    /**
     * @internal
     **/
    explicit Scheduler(QObject *parent = 0);
    /**
     * @internal
     **/
    static Joschy::Plugin::SchedulerMode mode();
    /**
     * @internal
     **/
    int delay() const;
    /**
     * @internal
     **/
    void scheduleInternal(Joschy::AbstractJob *job);
    /**
     * @internal
     **/
    inline void commonJobFinished(QPointer<Joschy::AbstractJob> job);
    /**
     * @internal
     **/
    static void setMode(const Joschy::Plugin::SchedulerMode &mode, const int &delay = 300);


private slots:
    /**
     * @internal
     **/
    void sheduleJob();
    /**
     * @internal
     **/
    void threadFinsihed();
    /**
     * @internal
     **/
    void jobFinished();


};


} // namespace Joschy


#endif // SCHEDULER_H
