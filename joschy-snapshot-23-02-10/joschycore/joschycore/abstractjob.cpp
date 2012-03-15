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
#include "abstractjob.h"


namespace Joschy {


class AbstractJobPrivate
{
public:
    AbstractJobPrivate(AbstractJob *job)
        : q(job)
    {}


    Joschy::AbstractJob::Priority priority;
    bool runInThread;
    bool isRunning;
    bool isCanceled;


private:
    AbstractJob *q;


};



AbstractJob::AbstractJob(QObject *parent)
    : QObject(parent), d(new AbstractJobPrivate(this))
{

    d->priority = Joschy::AbstractJob::NormalPriority;
    d->runInThread = false;
    d->isRunning = false;
    d->isCanceled = false;

}


AbstractJob::~AbstractJob()
{

    delete d;

}


Joschy::AbstractJob::Priority AbstractJob::priority() const
{

    return d->priority;

}


bool AbstractJob::runInThread() const
{

    return d->runInThread;

}


bool AbstractJob::operator<(const Joschy::AbstractJob *job) const
{

    return (int) priority() < (int) job->priority();

}


bool AbstractJob::isRunning() const
{

    return d->isRunning;

}


bool AbstractJob::isCanceled() const
{

    return d->isCanceled;

}


void AbstractJob::setPriority(const Joschy::AbstractJob::Priority &priority)
{

    d->priority = priority;

}


void AbstractJob::setRunInThread(const bool &thread)
{

    d->runInThread = thread;

}


void AbstractJob::cancel()
{

    Q_ASSERT(!isCanceled());

    if ((isRunning() && cancelJob()) || (!isRunning())) {
        d->isCanceled = true;
        jobDone();
    }

}


void AbstractJob::start()
{

    Q_ASSERT(!isRunning());
    Q_ASSERT(!isCanceled());

    d->isRunning = true;
    startJob();

}


void AbstractJob::emitFinished()
{

    emit finished(this);

}


void AbstractJob::jobDone()
{

    emit finishedInternal();

}


} // namespace Joschy


#include "abstractjob.moc"
