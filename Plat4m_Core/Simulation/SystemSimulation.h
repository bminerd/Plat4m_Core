//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2021-2023 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file SystemSimulation.h
/// @author Ben Minerd
/// @date 8/19/2021
/// @brief SystemSimulation class.
///

#ifndef PLAT4M_SYSTEM_SIMULATION_H
#define PLAT4M_SYSTEM_SIMULATION_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TimeTickSample.h>
#include <Plat4m_Core/Semaphore.h>
#include <Plat4m_Core/Simulation/ThreadSimulation.h>
#include <Plat4m_Core/Simulation/ThreadSimulationTick.h>
#include <Plat4m_Core/Simulation/QueueDriverSimulation.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <class SystemDriver>
class SystemSimulation : public SystemDriver
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    SystemSimulation(const TimeStamp timeStepTimeStamp,
                     const TopicBase::Id timeTickTopicId,
                     const std::uint32_t timeThreadStackBytes = 0) :
        SystemDriver(),
        myTimeStepTimeStamp(timeStepTimeStamp),
        myCurrentTimeStamp(),
        myTimeThread(
            System::createThread(
                    createCallback(this, &SystemSimulation::timeThreadCallback),
                    0,
                    timeThreadStackBytes,
                    false,
                    "SystemSimulation Time Thread")),
        myTimeTickTopic(Topic<TimeTickSample>::create(timeTickTopicId)),
        myThreadsNotifiedSemaphore(System::createSemaphore()),
        myThreadsCompleteSemaphore(System::createSemaphore()),
        mySimulatedThreadCount(0)
    {
    }

    //--------------------------------------------------------------------------
    // Public deprecated constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    SystemSimulation(const TimeUs timeStepUs,
                     const std::uint32_t timeTickTopicId,
                     const TimeMs timeThreadPeriodMs,
                     const std::uint32_t timeThreadStackBytes = 0) :
        SystemDriver(),
        myTimeStepTimeStamp(),
        myCurrentTimeStamp(),
        myTimeThread(
            System::createThread(
                    createCallback(this, &SystemSimulation::timeThreadCallback),
                    timeThreadPeriodMs,
                    timeThreadStackBytes,
                    false,
                    "SystemSimulation Time Thread")),
        myTimeTickTopic(Topic<TimeTickSample>::create(timeTickTopicId)),
        myThreadsNotifiedSemaphore(System::createSemaphore()),
        myThreadsCompleteSemaphore(System::createSemaphore()),
        mySimulatedThreadCount(0)
    {
        myTimeStepTimeStamp.fromTimeUs(timeStepUs);
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~SystemSimulation()
    {
        if (mySimulatedThreadCount > 0)
        {
            myThreadsNotifiedSemaphore.post();
            myThreadsCompleteSemaphore.post();
        }

        myTimeThread.~Thread();

        myThreadsNotifiedSemaphore.~Semaphore();
        myThreadsCompleteSemaphore.~Semaphore();
    }

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for System
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual TimeMs driverGetTimeMs() override
    {
        return (myCurrentTimeStamp.toTimeMs());
    }

    //--------------------------------------------------------------------------
    virtual TimeUs driverGetTimeUs() override
    {
        return (myCurrentTimeStamp.toTimeUs());
    }

    //--------------------------------------------------------------------------
    virtual Plat4m::TimeStamp driverGetTimeStamp() override
    {
        return myCurrentTimeStamp;
    }

    //--------------------------------------------------------------------------
    virtual TimeStamp driverGetWallTimeStamp() override
    {
        return (SystemDriver::driverGetTimeStamp());
    }

    //--------------------------------------------------------------------------
    virtual Thread& driverCreateThread(Thread::RunCallback& callback,
                                       const TimeMs periodMs,
                                       const std::uint32_t nStackBytes,
                                       const bool isSimulated,
                                       const char* name) override
    {
        if (isSimulated)
        {
            mySimulatedThreadCount++;

            if (periodMs != 0)
            {
                const std::uint32_t timeTopicId = myTimeTickTopic.getId();

                return *(MemoryAllocator::allocate<ThreadSimulationTick>(
                                                     callback,
                                                     timeTopicId,
                                                     myThreadsNotifiedSemaphore,
                                                     myThreadsCompleteSemaphore,
                                                     periodMs,
                                                     nStackBytes,
                                                     name));
            }

            return *(MemoryAllocator::allocate<ThreadSimulation>(
                                                     callback,
                                                     myThreadsCompleteSemaphore,
                                                     periodMs,
                                                     nStackBytes,
                                                     name));
        }

        return (SystemDriver::driverCreateThread(callback,
                                                 periodMs,
                                                 nStackBytes,
                                                 false,
                                                 name));
    }

    //--------------------------------------------------------------------------
    virtual QueueDriver& driverCreateQueueDriver(
                                             const std::uint32_t nValues,
                                             const std::uint32_t valueSizeBytes,
                                             Thread& thread,
                                             const bool isSimulated) override
    {
        if (isSimulated)
        {
            return *(MemoryAllocator::allocate<QueueDriverSimulation>(
                                                   nValues,
                                                   valueSizeBytes,
                                                   thread,
                                                   myThreadsNotifiedSemaphore));
        }

        return (SystemDriver::driverCreateQueueDriver(nValues,
                                                      valueSizeBytes,
                                                      thread,
                                                      false));
    }

    //--------------------------------------------------------------------------
    virtual void driverStartTime() override
    {
        myTimeThread.enable();
    }

    //--------------------------------------------------------------------------
    virtual void driverStopTime() override
    {
        myTimeThread.disable();
    }

    //--------------------------------------------------------------------------
    virtual void driverResetTime() override
    {
        myCurrentTimeStamp.set(0);
    }

    //--------------------------------------------------------------------------
    virtual System::Error driverSetTime(const TimeStamp& timeStamp) override
    {
        if (timeStamp < myCurrentTimeStamp)
        {
            return PLAT4M_REPORT_ERROR(System::Error,
                                       System::ERROR_CODE_PARAMETER_INVALID,
                                       ErrorBase::SEVERITY_LOW,
                                       this);
        }

        myCurrentTimeStamp = timeStamp;

        TimeTickSample timeTick;
        timeTick.timeStamp = myCurrentTimeStamp;

        myTimeTickTopic.publish(timeTick);

        if (mySimulatedThreadCount > 0)
        {
            std::uint32_t startedValue = myThreadsNotifiedSemaphore.getValue();

            if (startedValue > 0)
            {
                std::uint32_t completeValue = 1;
                std::uint32_t nFinishedThreads = 0;

                while ((completeValue != 0) ||
                                              (nFinishedThreads < startedValue))
                {
                    myThreadsCompleteSemaphore.wait();

                    completeValue = myThreadsCompleteSemaphore.getValue();
                    startedValue = myThreadsNotifiedSemaphore.getValue();

                    nFinishedThreads++;
                }

                while (myThreadsNotifiedSemaphore.getValue())
                {
                    myThreadsNotifiedSemaphore.wait();
                }
            }
        }

        return System::Error(System::ERROR_CODE_NONE);
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Thread& getTimeThread()
    {
        return myTimeThread;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    TimeStamp myTimeStepTimeStamp;

    TimeStamp myCurrentTimeStamp;

    Thread& myTimeThread;

    Topic<TimeTickSample>& myTimeTickTopic;

    Semaphore& myThreadsNotifiedSemaphore;

    Semaphore& myThreadsCompleteSemaphore;

    std::uint32_t mySimulatedThreadCount;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void timeThreadCallback()
    {
        driverSetTime(System::getTimeStamp());

        myCurrentTimeStamp += myTimeStepTimeStamp;
    }
};

}; // namespace Plat4m

#endif // PLAT4M_SYSTEM_SIMULATION_H
