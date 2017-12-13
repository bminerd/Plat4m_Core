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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file SystemLite.h
/// @author Ben Minerd
/// @date 6/4/2013
/// @brief SystemLite class header file.
///

#ifndef PLAT4M_SYSTEM_LITE_H
#define PLAT4M_SYSTEM_LITE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <System.h>
#include <List.h>
#include <ThreadLite.h>
#include <Mutex.h>
#include <WaitCondition.h>
#include <QueueDriver.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SystemLite : public System
{
public:

    //--------------------------------------------------------------------------
    // Public static inline methods
    //--------------------------------------------------------------------------

    static inline void timeMsHandler()
    {
        ++myTimeMs;
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void enableSystemClock(const uint32_t coreClockFrequencyHz);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    SystemLite();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SystemLite();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static volatile TimeMs myTimeMs;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    List<ThreadLite*> myThreadList;

    uint32_t myThreadDepth;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual void driverEnableSystemClock(
                                       const uint32_t coreClockFrequencyHz) = 0;

    //--------------------------------------------------------------------------
    // Private methods implemented from System
    //--------------------------------------------------------------------------

    Thread& driverCreateThread(Thread::RunCallback& callback,
                               const TimeMs periodMs);

    Mutex& driverCreateMutex();

    WaitCondition& driverCreateWaitCondition();

    QueueDriver& driverCreateQueueDriver(const uint32_t nValues,
    									 const uint32_t valueSizeBytes);

    void driverRun();

    TimeMs driverGetTimeMs();

    TimeUs driverGetTimeUs();

    void driverDelayTimeMs(const TimeMs timeMs);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void checkThreads(const uint32_t timePeriodMs = 0);
};

}; // namespace Plat4m

#endif // SYSTEM_LITE_H
