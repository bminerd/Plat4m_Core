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
/// @file ThreadSimulationTick.h
/// @author Ben Minerd
/// @date 8/19/2021
/// @brief ThreadSimulationTick class header file.
///

#ifndef PLAT4M_THREAD_SIMULATION_TICK_H
#define PLAT4M_THREAD_SIMULATION_TICK_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/TopicSubscriberThread.h>
#include <Plat4m_Core/TimeTickSample.h>
#include <Plat4m_Core/Semaphore.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ThreadSimulationTick : public Thread
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

	ThreadSimulationTick(RunCallback& callback,
                         const std::uint32_t timeTickTopicId,
                         Semaphore& runCompleteSemaphore,
                         const TimeMs periodMs = 0,
                         const std::uint32_t nStackBytes = 0);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ThreadSimulationTick();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    TopicSubscriberThread<TimeTickSample> myTimeTickTopicSubscriberThread;

    Semaphore& myRunCompleteSemaphore;
    
    TimeMs myNextCallTimeMs;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from Thread
    //--------------------------------------------------------------------------

    void driverSetPeriodMs(const TimeMs periodMs);

    std::uint32_t driverSetPriority(const std::uint32_t priority);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void timeTickSampleCallback(const TimeTickSample& timeTick);
};

}; // namespace Plat4m

#endif // PLAT4M_THREAD_SIMULATION_TICK_H
