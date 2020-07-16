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
    // Private inline methods
    //--------------------------------------------------------------------------

    static inline TimeMs getCurrentLinuxTimeMs();

    static inline TimeUs getCurrentLinuxTimeUs();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    timespec myFirstTimeSpec;

    TimeMs myFirstTimeMs;

    TimeUs myFirstTimeUs;

    //--------------------------------------------------------------------------
    // Private methods implemented from System
    //--------------------------------------------------------------------------

    TimeUs driverGetTimeUs();

    Thread& driverCreateThread(Thread::RunCallback& callback,
                               const TimeMs periodMs,
                               const uint32_t nStackBytes);

    Mutex& driverCreateMutex(Thread& thread);

    WaitCondition& driverCreateWaitCondition(Thread& thread);

    QueueDriver& driverCreateQueueDriver(const uint32_t nValues,
                                         const uint32_t valueSizeBytes,
                                         Thread& thread);

    void driverRun();

    TimeMs driverGetTimeMs();

    void driverDelayTimeMs(const TimeMs timeMs);
};

}; // namespace Plat4m

#endif // PLAT4M_SYSTEM_LINUX_H
