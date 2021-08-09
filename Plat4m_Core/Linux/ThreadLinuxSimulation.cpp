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
/// @file ThreadLinuxSimulation.cpp
/// @author Ben Minerd
/// @date 8/2/2021
/// @brief ThreadLinuxSimulation class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <iostream>
#include <unistd.h>

#include <Plat4m_Core/Linux/ThreadLinuxSimulation.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/CallbackMethodParameter.h>

using Plat4m::ThreadLinuxSimulation;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadLinuxSimulation::ThreadLinuxSimulation(RunCallback& callback,
                                             sem_t& runCompleteSemaphore,
                                             const TimeMs periodMs) :
    ThreadLinux(createCallback(this, &ThreadLinuxSimulation::threadCallback),
                periodMs),
    myRunCallback(callback),
    myRunCompleteSemaphore(runCompleteSemaphore)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadLinuxSimulation::~ThreadLinuxSimulation()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadLinuxSimulation::driverSetPeriodMs(const TimeMs periodMs)
{
    // Do nothing
}

//------------------------------------------------------------------------------
uint32_t ThreadLinuxSimulation::driverSetPriority(const uint32_t priority)
{
	return 0;
}

//------------------------------------------------------------------------------
void ThreadLinuxSimulation::threadCallback()
{
    myRunCallback.call();

    sem_post(&myRunCompleteSemaphore);
}