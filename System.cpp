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
// Copyright (c) 2036 Benjamin Minerd
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
                             const TimeMs periodMs)
{
    return (myDriver->driverCreateThread(callback, periodMs));
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
}
