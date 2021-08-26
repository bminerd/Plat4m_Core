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

#include <pthread.h>

#include <Plat4m_Core/Linux/SystemLinux.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>
#include <Plat4m_Core/Linux/MutexLinux.h>
#include <Plat4m_Core/Linux/WaitConditionLinux.h>
#include <Plat4m_Core/Linux/QueueDriverLinux.h>
#include <Plat4m_Core/Linux/SemaphoreLinux.h>
#include <Plat4m_Core/MemoryAllocator.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLinux::SystemLinux() :
    System(),
    myFirstTimeSpec(),
    myIsRunning(false)
{
    clock_gettime(CLOCK_REALTIME, &myFirstTimeSpec);

    myFirstTimeMs =
            (myFirstTimeSpec.tv_sec * 1000 + myFirstTimeSpec.tv_nsec / 1000000);

    myFirstTimeUs =
            (myFirstTimeSpec.tv_sec * 1000000 + myFirstTimeSpec.tv_nsec / 1000);
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SystemLinux::~SystemLinux()
{
    myIsRunning = false;
}

//------------------------------------------------------------------------------
// Public static inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
inline Plat4m::TimeMs SystemLinux::getCurrentLinuxTimeMs()
{
    struct timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);

    return (timeSpec.tv_sec * 1000 + timeSpec.tv_nsec / 1000000);
}

//------------------------------------------------------------------------------
inline Plat4m::TimeUs SystemLinux::getCurrentLinuxTimeUs()
{
    struct timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);

    return (timeSpec.tv_sec * 1000000 + timeSpec.tv_nsec / 1000);
}

//------------------------------------------------------------------------------
// Public virtual methods overridden for System
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::TimeUs SystemLinux::driverGetTimeUs()
{
    Plat4m::TimeUs timeUs = getCurrentLinuxTimeUs() - myFirstTimeUs;

    return timeUs;
}

//------------------------------------------------------------------------------
Thread& SystemLinux::driverCreateThread(Thread::RunCallback& callback,
                                        const TimeMs periodMs,
                                        const uint32_t nStackBytes,
                                        const bool isSimulated)
{
    return *(MemoryAllocator::allocate<ThreadLinux>(callback, periodMs));
}

//------------------------------------------------------------------------------
Mutex& SystemLinux::driverCreateMutex(Thread& thread)
{
    return *(MemoryAllocator::allocate<MutexLinux>());
}

//------------------------------------------------------------------------------
WaitCondition& SystemLinux::driverCreateWaitCondition(Thread& thread)
{
    return *(MemoryAllocator::allocate<WaitConditionLinux>());
}

//------------------------------------------------------------------------------
QueueDriver& SystemLinux::driverCreateQueueDriver(
                                                  const uint32_t nValues,
                                                  const uint32_t valueSizeBytes,
                                                  Thread& thread)
{
    return *(MemoryAllocator::allocate<QueueDriverLinux>(valueSizeBytes));
}

//------------------------------------------------------------------------------
Semaphore& SystemLinux::driverCreateSemaphore(const uint32_t maxValue,
                                              const uint32_t initialValue)
{
    return *(MemoryAllocator::allocate<SemaphoreLinux>(maxValue, initialValue));
}

//------------------------------------------------------------------------------
void SystemLinux::driverRun()
{
    myIsRunning = true;

    while (myIsRunning)
    {
        driverDelayTimeMs(1);
    }
}

//------------------------------------------------------------------------------
Plat4m::TimeMs SystemLinux::driverGetTimeMs()
{
    TimeMs timeMs = getCurrentLinuxTimeMs() - myFirstTimeMs;

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

//------------------------------------------------------------------------------
void SystemLinux::driverExit()
{
    myIsRunning = false;
}

//------------------------------------------------------------------------------
Plat4m::TimeStamp SystemLinux::driverGetWallTimeStamp()
{
    struct timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);

    TimeStamp timeStamp;
    timeStamp.timeS  = timeSpec.tv_sec - myFirstTimeSpec.tv_sec;
    timeStamp.timeNs = timeSpec.tv_nsec - myFirstTimeSpec.tv_nsec;

    return timeStamp;
}
