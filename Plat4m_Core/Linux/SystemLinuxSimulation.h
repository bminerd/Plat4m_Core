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
/// @file SystemLinuxSimulation.h
/// @author Ben Minerd
/// @date 6/23/2021
/// @brief SystemLinuxSimulation class.
///

#ifndef PLAT4M_SYSTEM_LINUX_SIMULATION_H
#define PLAT4M_SYSTEM_LINUX_SIMULATION_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <pthread.h>
#include <semaphore.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Linux/SystemLinux.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TimeTickSample.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SystemLinuxSimulation : public SystemLinux
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    SystemLinuxSimulation(const TimeUs timeStepUs,
                          const std::uint32_t timeTickTopicId);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SystemLinuxSimulation();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void resetTime();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------    

    const TimeUs myTimeStepUs;

    std::uint32_t myTimeStepCounter;

    Thread& myTimeThread;

    const std::uint32_t myTimeTickTopicId;

    Topic<TimeTickSample> myTimeTickTopic;

    sem_t mySemaphore;

    std::uint32_t mySimulatedThreadCount;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for System
    //--------------------------------------------------------------------------

    virtual TimeMs driverGetTimeMs() override;

    virtual TimeUs driverGetTimeUs() override;

    virtual TimeStamp driverGetWallTimeStamp() override;

    virtual Thread& driverCreateThread(Thread::RunCallback& callback,
                                       const TimeMs periodMs,
                                       const uint32_t nStackBytes,
                                       const bool isSimulated) override;

    virtual void driverRun() override;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void timeThreadCallback();
};

}; // namespace Plat4m

#endif // PLAT4M_SYSTEM_LINUX_SIMULATION_H
