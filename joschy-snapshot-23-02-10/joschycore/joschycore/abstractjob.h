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

#ifndef ABSTRACTJOB_H
#define ABSTRACTJOB_H


// own
#include "joschy_global.h"

// Qt
#include <QtCore/QObject>


namespace Joschy {


class AbstractJobPrivate;


/**
 * @class AbstractJob joschycore/abstractjob.h
 *
 * @short The base class for all jobs
 **/
class JOSCHY_EXPORT AbstractJob : public QObject
{
    Q_OBJECT
    friend class Scheduler;


public:
    /**
     * Job Priority
     * @see priority, setPriority
     **/
    enum Priority {
        LowPriority= 0,        /**< Run the job after High and Normal */
        NormalPriority = 1,    /**< Run the job before low but after High */
        HighPriority = 2       /**< Run the job before Normal and Low */
    };

    /**
     * Creates a new AbstractJob object.
     * @param parent the parent QObject
     **/
    explicit AbstractJob(QObject *parent = 0);
    virtual ~AbstractJob();

    /**
     * @return Returns the priority of the job
     **/
    Joschy::AbstractJob::Priority priority() const;
    /**
     * @return Returns true if the job should be executed in another thread; otherwise returns false.
     **/
    bool runInThread() const;
    /**
     * @internal
     **/
    bool operator<(const Joschy::AbstractJob *job) const;
    /**
     * @return Returns true if the job is running; otherwise returns false.
     **/
    bool isRunning() const;
    /**
     * @return Returns true if the job is canceled; otherwise returns false.
     **/
    bool isCanceled() const;

    /**
     * This function sets the priority for the job.
     * @see priority, setPriority
     **/
    void setPriority(const Joschy::AbstractJob::Priority &priority);
    /**
     * Set to true if the job could block the GUI
     * @see runInThread
     **/
    void setRunInThread(const bool &thread);
    /**
     * This function cancels the job.
     * The finished signals will be emitted and isCanceled returns true.
     * @see isCanceled, finished
     **/
    void cancel();
    /**
     * Starts the job.
     * @note Do not call this function yourself, use Joschy::Scheduler::schedule
     * @see cancel, finished
     **/
    void start();


private:
    AbstractJobPrivate *d;
    /**
     * @internal
     **/
    Q_SIGNAL void finishedInternal();
    /**
     * Emitted when the job is finished, in any case.
     * @note This is a private signal, it can't be emitted directly by subclasses of AbstractJob,
     * use jobDone() instead.
     **/
    Q_SIGNAL void finished(Joschy::AbstractJob *job);

    /**
     * @internal
     **/
    void emitFinished();


protected:
    /**
     * This pure virtual function is called by start(), the job should now start its work.
     * Call jobDone() if the job is finished.
     * @see jobDone, cancelJob
     **/
    virtual void startJob() = 0;
    /**
     * This pure virtual function is called by cancel() and returns true if the job is canceled;
     * otherwise returns false.
     * @see startJob
     **/
    virtual bool cancelJob() = 0;
    /**
     * Call this function if the job is done.
    **/
    void jobDone();


};


} // namespace Joschy


#endif // ABSTRACTJOB_H
