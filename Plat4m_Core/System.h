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
/// @file System.h
/// @author Ben Minerd
/// @date 6/4/2013
/// @brief System class header file.
///

#ifndef PLAT4M_SYSTEM_H
#define PLAT4M_SYSTEM_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Mutex.h>
#include <Plat4m_Core/WaitCondition.h>
#include <Plat4m_Core/Queue.h>
#include <Plat4m_Core/Semaphore.h>
#include <Plat4m_Core/MemoryAllocator.h>
#include <Plat4m_Core/TimeStamp.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class System
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_MODE_INVALID,
        ERROR_CODE_NOT_ENABLED
    };

    typedef ErrorTemplate<ErrorCode> Error;
    
    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static Thread& createThread(Thread::RunCallback& callback,
                                const TimeMs periodMs = 0,
                                const std::uint32_t nStackBytes = 0,
                                const bool isSimulated = false,
                                const char* name = 0);

    static Mutex& createMutex(Thread& thread);

    static WaitCondition& createWaitCondition(Thread& thread);

    //--------------------------------------------------------------------------
    template <typename T>
    static Queue<T>& createQueue(const std::uint32_t nValues,
                                 Thread& thread)
	{
    	return *(MemoryAllocator::allocate<Queue<T>>(
                                    myDriver->driverCreateQueueDriver(nValues,
                                                                      sizeof(T),
                                                                      thread)));
	}

    static Semaphore& createSemaphore(const std::uint32_t maxValue = 0,
                                      const std::uint32_t initialValue = 0);

    static void run();
    
    static bool isRunning();

    static TimeMs getTimeMs();

    static TimeUs getTimeUs();

    static TimeStamp getTimeStamp();

    static TimeStamp getWallTimeStamp();

    static void delayTimeMs(const TimeMs timeMs);

    static bool checkTimeMs(const TimeMs timeMs);

    static void exit();

    static void startTime();

    static void stopTime();

    static void resetTime();

    static Error setTime(const TimeStamp& timeStamp);

    static void enterCriticalSection();

    static void exitCriticalSection();

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual Thread& driverCreateThread(Thread::RunCallback& callback,
                                       const TimeMs periodMs,
                                       const std::uint32_t nStackBytes,
                                       const bool isSimulated,
                                       const char* name) = 0;

    virtual Mutex& driverCreateMutex(Thread& thread) = 0;

    virtual WaitCondition& driverCreateWaitCondition(Thread& thread) = 0;

    virtual QueueDriver& driverCreateQueueDriver(
                                             const std::uint32_t nValues,
    									     const std::uint32_t valueSizeBytes,
    									     Thread& thread) = 0;

    virtual Semaphore& driverCreateSemaphore(
                                          const std::uint32_t maxValue,
                                          const std::uint32_t initialValue) = 0;

    virtual void driverRun() = 0;

    virtual TimeMs driverGetTimeMs() = 0;

    virtual TimeUs driverGetTimeUs() = 0;

    virtual void driverDelayTimeMs(const TimeMs timeMs) = 0;

    virtual void driverExit() = 0;

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual TimeStamp driverGetTimeStamp();

    virtual TimeStamp driverGetWallTimeStamp();

    virtual void driverStartTime();

    virtual void driverStopTime();

    virtual void driverResetTime();

    virtual Error driverSetTime(const TimeStamp& timeStamp);

    virtual void driverEnterCriticalSection();

    virtual void driverExitCriticalSection();

protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    System();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~System();

private:
    
    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    static System* myDriver;
    
    static bool myIsRunning;
};

}; // namespace Plat4m

#endif // PLAT4M_SYSTEM_H
