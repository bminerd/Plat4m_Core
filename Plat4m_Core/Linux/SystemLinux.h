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
/// @file SystemLinux.h
/// @author Ben Minerd
/// @date 5/3/2019
/// @brief SystemLinux class.
///

#ifndef PLAT4M_SYSTEM_LINUX_H
#define PLAT4M_SYSTEM_LINUX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ctime>
#include <time.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Mutex.h>
#include <Plat4m_Core/WaitCondition.h>
#include <Plat4m_Core/QueueDriver.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SystemLinux : public System
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    SystemLinux();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SystemLinux();

    //--------------------------------------------------------------------------
    // Public inline methods
    //--------------------------------------------------------------------------

    static inline TimeMs getCurrentLinuxTimeMs();

    static inline TimeUs getCurrentLinuxTimeUs();

protected:

    void runProtected();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    timespec myFirstTimeSpec;

    TimeMs myFirstTimeMs;

    TimeUs myFirstTimeUs;

    bool myIsRunning;

    //--------------------------------------------------------------------------
    // Private methods implemented from System
    //--------------------------------------------------------------------------

    virtual Thread& driverCreateThread(Thread::RunCallback& callback,
                                       const TimeMs periodMs,
                                       const uint32_t nStackBytes,
                                       const bool isSimulated) override;

    virtual Mutex& driverCreateMutex(Thread& thread) override;

    virtual WaitCondition& driverCreateWaitCondition(Thread& thread) override;

    virtual QueueDriver& driverCreateQueueDriver(const uint32_t nValues,
                                                 const uint32_t valueSizeBytes,
                                                 Thread& thread) override;

    virtual void driverRun() override;

    virtual TimeUs driverGetTimeUs() override;

    virtual TimeMs driverGetTimeMs() override;

    virtual void driverDelayTimeMs(const TimeMs timeMs) override;

    virtual void driverExit() override;

    virtual TimeStamp driverGetWallTimeStamp() override;
};

}; // namespace Plat4m

#endif // PLAT4M_SYSTEM_LINUX_H
