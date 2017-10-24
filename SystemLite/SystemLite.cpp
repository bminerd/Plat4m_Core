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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file SystemLite.cpp
/// @author Ben Minerd
/// @date 6/4/2013
/// @brief SystemLite class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <SystemLite.h>
#include <MutexLite.h>
#include <WaitConditionLite.h>
#include <Processor.h>

using Plat4m::SystemLite;
using Plat4m::Processor;
using Plat4m::Thread;
using Plat4m::Mutex;
using Plat4m::WaitCondition;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

volatile Plat4m::TimeMs SystemLite::myTimeMs = 0;

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLite::SystemLite() :
    System(),
    myThreadList(),
    myThreadDepth(0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLite::~SystemLite()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SystemLite::enableSystemClock(const uint32_t coreClockFrequencyHz)
{
    driverEnableSystemClock(coreClockFrequencyHz);
}

//------------------------------------------------------------------------------
// Private methods implemented from System
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thread& SystemLite::driverCreateThread(Thread::RunCallback& callback,
                                       const TimeMs periodMs)
{
    ThreadLite* thread = new ThreadLite(callback, periodMs);

    myThreadList.append(thread);

    return (*thread);
}

//------------------------------------------------------------------------------
Mutex& SystemLite::driverCreateMutex()
{
    return *(new MutexLite());
}

//------------------------------------------------------------------------------
WaitCondition& SystemLite::driverCreateWaitCondition()
{
    return *(new WaitConditionLite());
}

//------------------------------------------------------------------------------
void SystemLite::driverRun()
{
    while (true) // Loop forever
    {
        checkThreads();
    }
}

//------------------------------------------------------------------------------
Plat4m::TimeMs SystemLite::driverTimeGetMs()
{
    return myTimeMs;
}

//------------------------------------------------------------------------------
void SystemLite::driverTimeDelayMs(const TimeMs timeMs)
{
    const TimeMs delayTimeMs = myTimeMs + timeMs;

    while (myTimeMs < delayTimeMs)
    {
        if (isRunning())
        {
            checkThreads(timeMs);
        }
    }
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SystemLite::checkThreads(const TimeMs timeWindowMs)
{
    List<ThreadLite*>::Iterator iterator = myThreadList.iterator();

    while (iterator.hasCurrent())
    {
        ThreadLite* thread = iterator.current();

        if (thread->isEnabled()   &&
            !(thread->isActive()) &&
            thread->isCallTime()  &&
            ((timeWindowMs == 0) || thread->canRunInTimeWindow(timeWindowMs)))
        {
            myThreadDepth++;

            thread->run();

            myThreadDepth--;
        }

        iterator.next();
    }

    Processor::setPowerMode(Processor::POWER_MODE_SLEEP);
}
