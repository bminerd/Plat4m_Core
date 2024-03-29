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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file Thread.cpp
/// @author Ben Minerd
/// @date 6/14/2013
/// @brief Thread class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/ThreadPolicyManager.h>

using Plat4m::Thread;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Thread::run()
{
    if (isValidPointer(myThreadPolicy))
    {
        myThreadPolicy->apply(*this, myRunCallback);
    }
    else
    {
        myRunCallback.call();
    }
}

//------------------------------------------------------------------------------
uint32_t Thread::getPeriodMs() const
{
    return myPeriodMs;
}

//------------------------------------------------------------------------------
void Thread::setPeriodMs(const TimeMs periodMs)
{
    myPeriodMs = periodMs;

    driverSetPeriodMs(periodMs);
}

//------------------------------------------------------------------------------
void Thread::setPriority(const uint32_t priority)
{
    myPriority = driverSetPriority(priority);
}

//------------------------------------------------------------------------------
const char* Thread::getName() const
{
    return myName;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thread::Thread(RunCallback& runCallback,
               const TimeMs periodMs,
               const char* name) :
    myRunCallback(runCallback),
    myPeriodMs(periodMs),
    myName(name),
    myPriority(0),
    myThreadPolicy(ThreadPolicyManager::getThreadPolicy(*this))
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thread::~Thread()
{
}
