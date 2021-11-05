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
/// @file System.cpp
/// @author Ben Minerd
/// @date 6/4/2013
/// @brief System class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/System.h>
#include <Plat4m_Core/List.h>

using namespace std;
using namespace Plat4m;

using Plat4m::System;
using Plat4m::Thread;
using Plat4m::Mutex;
using Plat4m::WaitCondition;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

System* System::myDriver = 0;
bool System::myIsRunning = false;

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thread& System::createThread(Thread::RunCallback& callback,
                             const TimeMs periodMs,
                             const uint32_t nStackBytes,
                             const bool isSimulated)
{
    return (myDriver->driverCreateThread(callback,
                                         periodMs,
                                         nStackBytes,
                                         isSimulated));
}

//------------------------------------------------------------------------------
Mutex& System::createMutex(Thread& thread)
{
    return (myDriver->driverCreateMutex(thread));
}

//------------------------------------------------------------------------------
WaitCondition& System::createWaitCondition(Thread& thread)
{
    return (myDriver->driverCreateWaitCondition(thread));
}

//------------------------------------------------------------------------------
Semaphore& System::createSemaphore(const uint32_t maxValue,
                                   const uint32_t initialValue)
{
    return (myDriver->driverCreateSemaphore(maxValue, initialValue));
}

//------------------------------------------------------------------------------
void System::run()
{
	myIsRunning = true;
    myDriver->driverRun();
}

//------------------------------------------------------------------------------
bool System::isRunning()
{
    return myIsRunning;
}

//------------------------------------------------------------------------------
uint32_t System::getTimeMs()
{
    return (myDriver->driverGetTimeMs());
}

//------------------------------------------------------------------------------
TimeStamp System::getTimeStamp()
{
    return (myDriver->driverGetTimeStamp());
}

//------------------------------------------------------------------------------
TimeStamp System::getWallTimeStamp()
{
    return (myDriver->driverGetWallTimeStamp());
}

//------------------------------------------------------------------------------
uint32_t System::getTimeUs()
{
    return (myDriver->driverGetTimeUs());
}

//------------------------------------------------------------------------------
void System::delayTimeMs(const uint32_t timeMs)
{
    myDriver->driverDelayTimeMs(timeMs);
}

//------------------------------------------------------------------------------
bool System::checkTimeMs(const uint32_t timeMs)
{
    return (timeMs <= getTimeMs());
}

//------------------------------------------------------------------------------
void System::exit()
{
    myDriver->driverExit();
}

//------------------------------------------------------------------------------
void System::startTime()
{
    myDriver->driverStartTime();
}

//------------------------------------------------------------------------------
void System::stopTime()
{
    myDriver->driverStopTime();
}

//------------------------------------------------------------------------------
void System::resetTime()
{
    myDriver->driverResetTime();
}

//------------------------------------------------------------------------------
System::Error System::setTime(const TimeStamp& timeStamp)
{
    return (myDriver->driverSetTime(timeStamp));
}

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::TimeStamp System::driverGetTimeStamp()
{
    // Not implemented by subclass, default implementation
    //
    // Note: This will wrap after ~1.2hr!

    TimeUs timeUs = getTimeUs();

    TimeStamp timeStamp;
    timeStamp.timeS  = round(timeUs / 1000000);
    timeStamp.timeNs = (timeUs - timeStamp.timeS * 1000000) * 1000;

    return timeStamp;
}

//------------------------------------------------------------------------------
Plat4m::TimeStamp System::driverGetWallTimeStamp()
{
    // Not implemented by subclass, default implementation

    return driverGetTimeStamp();
}

//------------------------------------------------------------------------------
void System::driverStartTime()
{
    // Not implemented by subclass, default implementation

    // Do nothing
}

//------------------------------------------------------------------------------
void System::driverStopTime()
{
    // Not implemented by subclass, default implementation

    // Do nothing
}

//------------------------------------------------------------------------------
void System::driverResetTime()
{
    // Not implemented by subclass, default implementation

    // Do nothing
}

//------------------------------------------------------------------------------
System::Error System::driverSetTime(const TimeStamp& timeStamp)
{
    // Not implemented by subclass, default implementation

    // Do nothing

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
System::System()
{
    if (isNullPointer(myDriver))
    {
        myDriver = this;
    }
    else
    {
        // System lockup, trying to instantiate more than one System
        while (true)
        {
        }
    }
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
System::~System()
{
    myDriver = 0;
}
