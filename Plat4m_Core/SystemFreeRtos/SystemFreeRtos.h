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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file SystemFreeRtos.h
/// @author Ben Minerd
/// @date 12/24/2017
/// @brief SystemFreeRtos class header file.
///

#ifndef PLAT4M_SYSTEM_FREE_RTOS_H
#define PLAT4M_SYSTEM_FREE_RTOS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Mutex.h>
#include <Plat4m_Core/WaitCondition.h>
#include <Plat4m_Core/QueueDriver.h>
#include <Plat4m_Core/Semaphore.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SystemFreeRtos : public System
{
public:

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for System
    //--------------------------------------------------------------------------

    virtual Thread& driverCreateThread(Thread::RunCallback& callback,
                                       const TimeMs periodMs,
                                       const std::uint32_t nStackBytes,
                                       const bool isSimulated) override;

    virtual Mutex& driverCreateMutex(Thread& thread) override;

    virtual WaitCondition& driverCreateWaitCondition(Thread& thread) override;

    virtual QueueDriver& driverCreateQueueDriver(
                                             const std::uint32_t nValues,    
                                             const std::uint32_t valueSizeBytes,
                                             Thread& thread) override;

    virtual Semaphore& driverCreateSemaphore(
                                     const std::uint32_t maxValue,
                                     const std::uint32_t initialValue) override;

    virtual void driverRun() override;

    virtual TimeMs driverGetTimeMs() override;

    virtual TimeUs driverGetTimeUs() override;

    virtual void driverDelayTimeMs(const TimeMs timeMs) override;

    virtual void driverExit() override;

    virtual void driverEnterCriticalSection() override;

    virtual void driverExitCriticalSection() override;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    std::uint32_t getTimeMsRollOverCounter();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    SystemFreeRtos();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SystemFreeRtos();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    TimeMs myLastTimeMs;

    std::uint32_t myTimeMsRollOverCounter;
};

}; // namespace Plat4m

#endif // PLAT4M_SYSTEM_FREE_RTOS_H
