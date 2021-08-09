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
/// @file ThreadLinuxSimulation.h
/// @author Ben Minerd
/// @date 8/2/2021
/// @brief ThreadLinuxSimulation class header file.
///

#ifndef PLAT4M_THREAD_LINUX_SIMULATION_H
#define PLAT4M_THREAD_LINUX_SIMULATION_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <pthread.h>
#include <semaphore.h>

#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ThreadLinuxSimulation : public ThreadLinux
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

	ThreadLinuxSimulation(RunCallback& callback,
                          sem_t& runCompleteSemaphore,
                          const TimeMs periodMs = 0);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ThreadLinuxSimulation();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    RunCallback& myRunCallback;
    sem_t& myRunCompleteSemaphore;

    //--------------------------------------------------------------------------
    // Private methods implemented from Thread
    //--------------------------------------------------------------------------

    void driverSetPeriodMs(const TimeMs periodMs);

    uint32_t driverSetPriority(const uint32_t priority);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void threadCallback();
};

}; // namespace Plat4m

#endif // PLAT4M_THREAD_LINUX_SIMULATION_H