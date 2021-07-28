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
/// @file SystemWindows.cpp
/// @author Ben Minerd
/// @date 6/4/2015
/// @brief SystemWindows class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Windows.h>

#include <Plat4m_Core/SystemWindows/SystemWindows.h>
#include <Plat4m_Core/SystemWindows/ThreadWindows.h>
#include <Plat4m_Core/SystemWindows/MutexWindows.h>
#include <Plat4m_Core/SystemWindows/WaitConditionWindows.h>
#include <Plat4m_Core/SystemWindows/QueueDriverWindows.h>
#include <Plat4m_Core/MemoryAllocator.h>

using Plat4m::SystemWindows;
using Plat4m::Thread;
using Plat4m::Mutex;
using Plat4m::WaitCondition;
using Plat4m::QueueDriver;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemWindows::SystemWindows() :
    System()
{
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemWindows::~SystemWindows()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from System
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::TimeUs SystemWindows::driverGetTimeUs()
{
    // Default if not implemented by subclass

    return (driverGetTimeMs() * 1000);
}

//------------------------------------------------------------------------------
Thread& SystemWindows::driverCreateThread(Thread::RunCallback& callback,
                                          const TimeMs periodMs,
                                          const uint32_t nStackBytes)
{
    return *(MemoryAllocator::allocate<ThreadWindows>(callback, periodMs));
}

//------------------------------------------------------------------------------
Mutex& SystemWindows::driverCreateMutex(Thread& thread)
{
    return *(MemoryAllocator::allocate<MutexWindows>());
}

//------------------------------------------------------------------------------
WaitCondition& SystemWindows::driverCreateWaitCondition(Thread& thread)
{
    return *(MemoryAllocator::allocate<WaitConditionWindows>());
}

//------------------------------------------------------------------------------
QueueDriver& SystemWindows::driverCreateQueueDriver(
                                                  const uint32_t nValues,
                                                  const uint32_t valueSizeBytes,
                                                  Thread& thread)
{
    return *(MemoryAllocator::allocate<QueueDriverWindows>(thread));
}

//------------------------------------------------------------------------------
void SystemWindows::driverRun()
{
    while (true)
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
Plat4m::TimeMs SystemWindows::driverGetTimeMs()
{
    return ((TimeMs) (GetTickCount()));
}

//------------------------------------------------------------------------------
void SystemWindows::driverDelayTimeMs(const TimeMs timeMs)
{
    Sleep((DWORD) timeMs);
}
