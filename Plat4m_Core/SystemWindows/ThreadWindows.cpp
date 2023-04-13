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
// Copyright (c) 2018-2023 Benjamin Minerd
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
/// @file ThreadWindows.cpp
/// @author Ben Minerd
/// @date 2/19/2018
/// @brief ThreadWindows class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/SystemWindows/ThreadWindows.h>
#include <Plat4m_Core/System.h>

using Plat4m::ThreadWindows;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadWindows::ThreadWindows(RunCallback& callback,
                             const TimeMs periodMs,
                             const char* name) :
    Thread(callback, periodMs, name),
    myThreadHandle(0),
    myThreadId(0)
{
    myThreadHandle = CreateThread(0,
                                  0,
                                  &ThreadWindows::threadCallback,
                                  (LPVOID) this,
                                  0,
                                  &myThreadId);

    if (isNullPointer(myThreadHandle))
    {
        while (true)
        {
            // Lock up, unable to create thread
        }
    }

    SuspendThread(myThreadHandle);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadWindows::~ThreadWindows()
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DWORD ThreadWindows::getThreadId() const
{
    return myThreadId;
}

//------------------------------------------------------------------------------
// Private static methods implemented from Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DWORD WINAPI ThreadWindows::threadCallback(LPVOID lpParameter)
{
    ThreadWindows* thread = static_cast<ThreadWindows*>(lpParameter);

    while (true) // Loop forever
    {
        if (thread->getPeriodMs() != 0)
        {
            Sleep(thread->getPeriodMs());
        }

        thread->run();
    }

    return 0;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ThreadWindows::driverSetEnabled(const bool enabled)
{
    if (enabled)
    {
        ResumeThread(myThreadHandle);
    }
    else
    {
        SuspendThread(myThreadHandle);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadWindows::driverSetPeriodMs(const TimeMs periodMs)
{
    // Do nothing
}

//------------------------------------------------------------------------------
uint32_t ThreadWindows::driverSetPriority(const uint32_t priority)
{
    return 0;
}
