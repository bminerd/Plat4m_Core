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
/// @file ThreadSimulation.cpp
/// @author Ben Minerd
/// @date 8/19/2021
/// @brief ThreadSimulation class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Simulation/ThreadSimulation.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/CallbackMethodParameter.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadSimulation::ThreadSimulation(RunCallback& callback,
                                   Semaphore& runCompleteSemaphore,
                                   const TimeMs periodMs,
                                   const uint32_t nStackBytes) :
    Thread(callback, periodMs),
    myThread(
        System::createThread(
                        createCallback(this, &ThreadSimulation::threadCallback),
                        periodMs)),
    myRunCompleteSemaphore(runCompleteSemaphore)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ThreadSimulation::~ThreadSimulation()
{
}

//------------------------------------------------------------------------------
// Private methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ThreadSimulation::driverSetEnabled(const bool enabled)
{
    myThread.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods overridden for Thread
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ThreadSimulation::driverSetPeriodMs(const TimeMs periodMs)
{
    // Do nothing
}

//------------------------------------------------------------------------------
uint32_t ThreadSimulation::driverSetPriority(const uint32_t priority)
{
	return 0;
}

//------------------------------------------------------------------------------
void ThreadSimulation::threadCallback()
{
    Thread::run();

    myRunCompleteSemaphore.post();
}