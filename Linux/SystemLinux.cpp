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
// Copyright (c) 2019 Benjamin Minerd
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
/// @file SystemLinux.cpp
/// @author Ben Minerd
/// @date 5/3/2019
/// @brief SystemLinux class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <unistd.h>
#include <sys/time.h>

#include <Plat4m_Core/Linux/SystemLinux.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>
#include <Plat4m_Core/Linux/MutexLinux.h>
#include <Plat4m_Core/Linux/WaitConditionLinux.h>
#include <Plat4m_Core/Linux/QueueDriverLinux.h>

using Plat4m::SystemLinux;
using Plat4m::Thread;
using Plat4m::Mutex;
using Plat4m::WaitCondition;
using Plat4m::QueueDriver;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLinux::SystemLinux() :
    System(),
    myFirstTimeVal()
{
    gettimeofday(&myFirstTimeVal, NULL);
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLinux::~SystemLinux()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from System
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::TimeUs SystemLinux::driverGetTimeUs()
{
    struct timeval timeVal;
    gettimeofday(&timeVal, NULL);

    Plat4m::TimeUs timeUs = (timeVal.tv_sec * 1000000 + timeVal.tv_usec) -
                            (myFirstTimeVal.tv_sec * 1000000 + timeVal.tv_usec);

    return timeUs;
}

//------------------------------------------------------------------------------
Thread& SystemLinux::driverCreateThread(Thread::RunCallback& callback,
                                        const TimeMs periodMs,
                                        const uint32_t nStackBytes)
{
    return *(new ThreadLinux(callback, periodMs));
}

//------------------------------------------------------------------------------
Mutex& SystemLinux::driverCreateMutex(Thread& thread)
{
    return *(new MutexLinux);
}

//------------------------------------------------------------------------------
WaitCondition& SystemLinux::driverCreateWaitCondition(Thread& thread)
{
    return *(new WaitConditionLinux);
}

//------------------------------------------------------------------------------
QueueDriver& SystemLinux::driverCreateQueueDriver(
                                                  const uint32_t nValues,
                                                  const uint32_t valueSizeBytes,
                                                  Thread& thread)
{
    return *(new QueueDriverLinux(valueSizeBytes));
}

//------------------------------------------------------------------------------
void SystemLinux::driverRun()
{
    while (true)
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
Plat4m::TimeMs SystemLinux::driverGetTimeMs()
{
    struct timeval timeVal;
    gettimeofday(&timeVal, NULL);

    Plat4m::TimeMs timeMs = 
                        (timeVal.tv_sec * 1000 + timeVal.tv_usec / 1000) -
                        (myFirstTimeVal.tv_sec * 1000 + timeVal.tv_usec / 1000);

    return timeMs;
}

//------------------------------------------------------------------------------
void SystemLinux::driverDelayTimeMs(const TimeMs timeMs)
{
    struct timespec timeSpec;
    timeSpec.tv_sec = timeMs / 1000;
    timeSpec.tv_nsec = (timeMs % 1000) * 1000000;
    nanosleep(&timeSpec, NULL);
}
