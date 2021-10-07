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
// Copyright (c) 2021 Benjamin Minerd
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
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TimeTickSample.h>
#include <Plat4m_Core/Semaphore.h>
#include <Plat4m_Core/Simulation/ThreadSimulation.h>
#include <Plat4m_Core/Simulation/ThreadSimulationTick.h>

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
    SystemSimulation(const TimeUs timeStepUs,
                     const std::uint32_t timeTickTopicId,
                     const std::uint32_t timeThreadStackBytes = 0) :
    SystemDriver(),
    myTimeStepUs(timeStepUs),
    myTimeStepCounter(0),
    myTimeThread(
        System::createThread(
                    createCallback(this, &SystemSimulation::timeThreadCallback),
                    0,
                    timeThreadStackBytes)),
    myTimeTickTopic(timeTickTopicId),
    mySemaphore(System::createSemaphore()),
    mySimulatedThreadCount(0)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~SystemSimulation()
    {
        if (mySimulatedThreadCount > 0)
        {
            mySemaphore.post();
        }

        myTimeThread.~Thread();
    }

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for System
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual TimeMs driverGetTimeMs() override
    {
        return (driverGetTimeUs() / 1000);
    }

    //--------------------------------------------------------------------------
    virtual TimeUs driverGetTimeUs() override
    {
        return (myTimeStepCounter * myTimeStepUs);
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
                                       const bool isSimulated) override
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
                                                                    mySemaphore,
                                                                    periodMs));
            }

            return *(MemoryAllocator::allocate<ThreadSimulation>(callback,
                                                                 mySemaphore,
                                                                 periodMs));
        }

        return (SystemDriver::driverCreateThread(callback,
                                                 periodMs,
                                                 nStackBytes,
                                                 isSimulated));
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
        myTimeStepCounter = 0;
    }

    //--------------------------------------------------------------------------
    virtual void driverSetTime(const TimeStamp& timeStamp) override
    {
        myTimeStepCounter =
            (timeStamp.timeS * 1000000 + timeStamp.timeNs / 1000) /
                                                                   myTimeStepUs;

        TimeTickSample timeTick;
        timeTick.header.timeStamp = timeStamp;

        myTimeTickTopic.publish(timeTick);

        if (mySimulatedThreadCount > 0)
        {
            std::uint32_t value = 1;

            while (value != 0)
            {
                mySemaphore.wait();

                value = mySemaphore.getValue();
            }
        }
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

    const TimeUs myTimeStepUs;

    std::uint32_t myTimeStepCounter;

    Thread& myTimeThread;

    Topic<TimeTickSample> myTimeTickTopic;

    Semaphore& mySemaphore;

    std::uint32_t mySimulatedThreadCount;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void timeThreadCallback()
    {
        driverSetTime(System::getTimeStamp());

        myTimeStepCounter++;
    }
};

}; // namespace Plat4m

#endif // PLAT4M_SYSTEM_SIMULATION_H
