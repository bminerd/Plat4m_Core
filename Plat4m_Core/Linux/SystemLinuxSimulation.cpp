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
/// @file SystemLinuxSimulation.cpp
/// @author Ben Minerd
/// @date 6/23/2021
/// @brief SystemLinuxSimulation class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <unistd.h>
#include <sys/time.h>

#include <Plat4m_Core/Linux/SystemLinuxSimulation.h>
#include <Plat4m_Core/Linux/ThreadLinuxSimulation.h>
#include <Plat4m_Core/Linux/ThreadLinuxSimulationTick.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>

using Plat4m::SystemLinuxSimulation;
using Plat4m::SystemLinux;
using Plat4m::Thread;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLinuxSimulation::SystemLinuxSimulation(
                                          const TimeUs timeStepUs,
                                          const std::uint32_t timeTickTopicId) :
    SystemLinux(),
    myTimeStepUs(timeStepUs),
    myTimeStepCounter(0),
    myTimeThread(
        System::createThread(
             createCallback(this, &SystemLinuxSimulation::timeThreadCallback))),
    myTimeTickTopicId(timeTickTopicId),
    myTimeTickTopic(timeTickTopicId),
    mySemaphore(),
    mySimulatedThreadCount(0)
{
    sem_init(&mySemaphore, 0, 0);
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLinuxSimulation::~SystemLinuxSimulation()
{
    sem_post(&mySemaphore);
    myTimeThread.~Thread();
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SystemLinuxSimulation::resetTime()
{
    myTimeStepCounter = 0;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for System
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::TimeMs SystemLinuxSimulation::driverGetTimeMs()
{
    return (driverGetTimeUs() / 1000);
}

//------------------------------------------------------------------------------
Plat4m::TimeUs SystemLinuxSimulation::driverGetTimeUs()
{
    return (myTimeStepCounter * myTimeStepUs);
}

//------------------------------------------------------------------------------
Plat4m::TimeStamp SystemLinuxSimulation::driverGetWallTimeStamp()
{
    struct timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);

    TimeStamp timeStamp;
    timeStamp.timeS  = timeSpec.tv_sec;
    timeStamp.timeNs = timeSpec.tv_nsec;

    return timeStamp;
}

//------------------------------------------------------------------------------
Thread& SystemLinuxSimulation::driverCreateThread(Thread::RunCallback& callback,
                                                  const TimeMs periodMs,
                                                  const uint32_t nStackBytes,
                                                  const bool isSimulated)
{
    if (isSimulated)
    {
        mySimulatedThreadCount++;

        if (periodMs != 0)
        {
            return *(MemoryAllocator::allocate<ThreadLinuxSimulationTick>(
                                                                callback,
                                                                myTimeTickTopicId,
                                                                mySemaphore,
                                                                periodMs));
        }

        return *(MemoryAllocator::allocate<ThreadLinuxSimulation>(callback,
                                                                mySemaphore,
                                                                periodMs));
    }

    return *(MemoryAllocator::allocate<ThreadLinux>(callback, periodMs));
}

//------------------------------------------------------------------------------
void SystemLinuxSimulation::driverRun()
{
    myTimeThread.enable();

    SystemLinux::driverRun();
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SystemLinuxSimulation::timeThreadCallback()
{
    TimeTickSample timeTick;
    myTimeTickTopic.publish(timeTick);

    if (mySimulatedThreadCount > 0)
    {
        int value = 1;

        while (value != 0)
        {
            sem_wait(&mySemaphore);

            sem_getvalue(&mySemaphore, &value);
        }
    }

    myTimeStepCounter++;
}