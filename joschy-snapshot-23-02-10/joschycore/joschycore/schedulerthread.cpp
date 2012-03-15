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
#include "schedulerthread.h"
#include "joschy_global.h"
#include "abstractjob.h"


// Qt
#include <QtCore/QEventLoop>


namespace Joschy {


SchedulerThread::SchedulerThread(QObject *parent, Joschy::AbstractJob *job)
    : QThread(parent), m_job(job)
{

    if (m_job) {
        connect(m_job, SIGNAL(finishedInternal()), this, SLOT(quit()));
    }

}


SchedulerThread::~SchedulerThread()
{

    wait(); // wait until run() has exited

}


Joschy::AbstractJob *SchedulerThread::job() const
{

    return m_job;

}


void SchedulerThread::run()
{

    if (m_job) {
        m_job->start();

        QEventLoop loop;
        loop.exec();
    }

}



} // namespace Joschy


#include "schedulerthread.moc"
