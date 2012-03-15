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

// own
#include "scheduler.h"
#include "schedulerthread.h"
#include "abstractjob.h"

// Qt
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QPointer>
#include <QtCore/QMutex>



QMutex mutex;
namespace Joschy {


typedef QPointer<Joschy::AbstractJob> JobPtr;
class SchedulerPrivate
{
public:
    SchedulerPrivate(Scheduler *sheduler)
        : q(sheduler)
    {

        runningJobs = 0;
        slowDelay = 300;
        mode = Joschy::Plugin::NormalSchedulerMode;
        m_timer = new QTimer(q);
        m_timer->setSingleShot(true);
        q->connect(m_timer, SIGNAL(timeout()), q, SLOT(sheduleJob()));

    }


    ~SchedulerPrivate()
    {

        delete m_timer;

    }


    JobPtr nextJob()
    {

        if (!m_pendingJobs.isEmpty()) {
            return m_pendingJobs.takeFirst();
        } else {
            return 0;
        }

    }


    int pendingJobs() const
    {

        return m_pendingJobs.size();

    }


    bool isTimer(QObject *sender) const
    {

        return sender == m_timer;

    }


    bool timerActive()
    {

        return m_timer->isActive();

    }


    void addPendingJob(Joschy::AbstractJob *job)
    {

        m_pendingJobs.append(job);
        qSort(m_pendingJobs);

    }


    void removePendingJob(Joschy::AbstractJob *job)
    {

        m_pendingJobs.removeAll(job);

    }


    void startTimer()
    {

        m_timer->start(slowDelay);

    }


    void stopTimer()
    {

        m_timer->stop();

    }


    QList<Joschy::SchedulerThread*> threads;
    int runningJobs;
    Joschy::Plugin::SchedulerMode mode;
    int slowDelay;


private:
    Scheduler *q;

    QList<JobPtr> m_pendingJobs;
    QTimer *m_timer;


};



class SchedulerSingleton
{

public:
    Scheduler self;

};


JOSCHY_SINGLETON(SchedulerSingleton, shedulerSelf)


Scheduler::Scheduler(QObject *parent)
    : QObject(parent), d(new SchedulerPrivate(this))
{

    JOSCHY_DEBUG();

}


Scheduler::~Scheduler()
{

    JOSCHY_DEBUG();
    foreach (QThread *thread, d->threads) {
        thread->disconnect(this); // dont send the finished signal
        thread->quit();
    }
    qDeleteAll(d->threads);
    delete d;

}


Joschy::Plugin::SchedulerMode Scheduler::mode()
{

    return (&Joschy::shedulerSelf->self)->d->mode;

}


int Scheduler::delay() const
{

    return (&Joschy::shedulerSelf->self)->d->slowDelay;

}


void Scheduler::schedule(Joschy::AbstractJob *job)
{

    (&Joschy::shedulerSelf->self)->scheduleInternal(job);

}

void Scheduler::setMode(const Joschy::Plugin::SchedulerMode &mode, const int &delay)
{

    (&Joschy::shedulerSelf->self)->d->mode = mode;
    (&Joschy::shedulerSelf->self)->d->slowDelay = delay;

}


void Scheduler::scheduleInternal(Joschy::AbstractJob *job)
{

    d->addPendingJob(job);
    sheduleJob();

}


void Scheduler::commonJobFinished(QPointer<Joschy::AbstractJob> job)
{

    mutex.lock();
    d->runningJobs--;
    mutex.unlock();

    if (job) {
        if (job->isCanceled()) {
            d->removePendingJob(job);
        }
        job->emitFinished();
    }
    sheduleJob();

}



void Scheduler::sheduleJob()
{

    mutex.lock();
    int maxJobs = 1;
    switch (mode()) {
    case Joschy::Plugin::SlowSchedulerMode: break;
    case Joschy::Plugin::NormalSchedulerMode: break;
    case Joschy::Plugin::FastSchedulerMode: maxJobs = qMax(1, QThread::idealThreadCount()); break;
    }

    JOSCHY_DEBUG() << "Pending jobs:" << d->pendingJobs();
    JOSCHY_DEBUG() << "Running jobs:" << d->runningJobs;
    JOSCHY_DEBUG() << "Max job count:" << maxJobs;

    if (d->runningJobs+1 > maxJobs) {
        JOSCHY_DEBUG() << "too many jobs...";
        mutex.unlock();
        return;
    }

    if (d->pendingJobs() == 0) {
        JOSCHY_DEBUG() << "no jobs left...";
        mutex.unlock();
        return;
    }

    bool schedule = false;
    switch (mode()) {
    case Joschy::Plugin::SlowSchedulerMode: {
            if (d->runningJobs == 0 && !d->timerActive()) { // first job
                d->startTimer();
                schedule = true;
            } else if (d->isTimer(sender())) {
                schedule = true;
            } else {
               d->stopTimer();
               d->startTimer();
            }
            break;
        }
    case Joschy::Plugin::NormalSchedulerMode:
    case Joschy::Plugin::FastSchedulerMode: schedule = true; break;
    }

    if (!schedule) {
        mutex.unlock();
        return;
    }

    JOSCHY_DEBUG() << "next job...";
    JobPtr job = d->nextJob();
    if (!job) {
        JOSCHY_DEBUG() << "no jobs left...";
        mutex.unlock();
        return;
    }

    d->runningJobs++;
    mutex.unlock();

    JOSCHY_DEBUG() << "run in thread:" << job->runInThread() << "priority:" << job->priority();
    if (job->runInThread()) {
        Joschy::SchedulerThread *thread = new Joschy::SchedulerThread(this, job);
        d->threads.append(thread);

        connect(thread, SIGNAL(finished()), this, SLOT(threadFinsihed()));

#ifdef Q_WS_WIN32
        QThread::Priority priority;
        switch (job->priority()) {
        case Joschy::AbstractJob::LowPriority: priority = QThread::LowPriority; break;
        case Joschy::AbstractJob::NormalPriority: priority = QThread::NormalPriority; break;
        case Joschy::AbstractJob::HighPriority: priority = QThread::HighPriority; break;
        }

        thread->start(priority);
#else
        thread->start();
#endif
    } else {
        connect(job, SIGNAL(finishedInternal()), this, SLOT(jobFinished()));
        job->start();
    }

}


void Scheduler::threadFinsihed()
{

    JOSCHY_DEBUG() << "thread finsihed....";


    Joschy::SchedulerThread *thread = static_cast<Joschy::SchedulerThread*>(sender());

    Joschy::JobPtr job = thread->job();
    d->threads.removeAll(thread);
    thread->deleteLater();

    commonJobFinished(job);

}


void Scheduler::jobFinished()
{

    AbstractJob *job = static_cast<AbstractJob*>(sender());
    commonJobFinished(job);

}


} // namespace Joschy


#include "scheduler.moc"

