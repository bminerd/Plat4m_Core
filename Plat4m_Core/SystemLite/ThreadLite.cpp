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
// Copyright (c) 2016-2023 Benjamin Minerd
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
/// @file ThreadLite.cpp
/// @author Ben Minerd
/// @date 7/1/2016
/// @brief ThreadLite class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemLite/ThreadLite.h>
#include <Plat4m_Core/System.h>

using Plat4m::ThreadLite;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadLite::ThreadLite(RunCallback& callback,
                       const TimeMs periodMs,
                       const char* name) :
    Thread(callback, periodMs, name),
    myIsActive(false),
    myCallTimeMs(0),
    myWorstCaseRunTimeMs(0xFFFFFFFF)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadLite::~ThreadLite()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ThreadLite::isActive() const
{
    return myIsActive;
}

//------------------------------------------------------------------------------
void ThreadLite::setActive(const bool active)
{
    myIsActive = active;
}

//------------------------------------------------------------------------------
Plat4m::TimeMs ThreadLite::getCallTimeMs() const
{
    return myCallTimeMs;
}

//------------------------------------------------------------------------------
void ThreadLite::setCallTimeMs(const TimeMs callTimeMs)
{
    myCallTimeMs = callTimeMs;
}

//------------------------------------------------------------------------------
Plat4m::TimeMs ThreadLite::getWorstCaseRunTimeMs() const
{
    return myWorstCaseRunTimeMs;
}

//------------------------------------------------------------------------------
void ThreadLite::setWorstCaseRunTimeMs(const TimeMs worstCaseRunTimeMs)
{
    myWorstCaseRunTimeMs = worstCaseRunTimeMs;
}

//------------------------------------------------------------------------------
bool ThreadLite::isCallTime() const
{
    return System::checkTimeMs(myCallTimeMs);
}

//------------------------------------------------------------------------------
bool ThreadLite::canRunInTimeWindow(const TimeMs timeWindowMs)
{
    uint32_t timeWindowEndTimeMs = System::getTimeMs() + timeWindowMs;

    return ((System::getTimeMs() + myWorstCaseRunTimeMs) < timeWindowEndTimeMs);
}

//------------------------------------------------------------------------------
// Public virtual methods overridden for Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// void ThreadLite::run()
// {
//     myIsActive = true;

//     TimeMs startTimeMs = System::getTimeMs();

//     Thread::run();

//     TimeMs dTimeMs = System::getTimeMs() - startTimeMs;

//     if ((myWorstCaseRunTimeMs == 0xFFFFFFFF) ||
//         (dTimeMs > myWorstCaseRunTimeMs))
//     {
//         myWorstCaseRunTimeMs = dTimeMs;
//     }

//     myCallTimeMs = startTimeMs + getPeriodMs();

//     myIsActive = false;
// }

//------------------------------------------------------------------------------
// Private virtual methods overridden for Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadLite::driverSetPeriodMs(const TimeMs periodMs)
{
    myCallTimeMs = System::getTimeMs() + periodMs;
}

//------------------------------------------------------------------------------
uint32_t ThreadLite::driverSetPriority(const uint32_t priority)
{
	return 0;
}
