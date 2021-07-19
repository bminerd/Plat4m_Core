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
/// @file HardwareTimerSimulated.cpp
/// @author Ben Minerd
/// @date 6/16/2021
/// @brief HardwareTimerSimulated class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/HardwareTimerSimulated/HardwareTimerSimulated.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethod.h>

using Plat4m::HardwareTimerSimulated;
using Plat4m::HardwareTimer;
using Plat4m::Module;
using Plat4m::Thread;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerSimulated::HardwareTimerSimulated() :
    HardwareTimer(),
    myInterruptHandlerCallback(0),
    myThread(
        System::createThread(
                 createCallback(this, &HardwareTimerSimulated::threadCallback)))
{
}

//------------------------------------------------------------------------------
HardwareTimerSimulated::HardwareTimerSimulated(
                           InterruptHandlerCallback& interruptHandlerCallback) :
    HardwareTimer(),
    myInterruptHandlerCallback(0),
    myThread(
        System::createThread(
                 createCallback(this, &HardwareTimerSimulated::threadCallback)))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerSimulated::~HardwareTimerSimulated()
{
    myThread.~Thread();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void HardwareTimerSimulated::setInterruptHandlerCallback(
                             InterruptHandlerCallback& interruptHandlerCallback)
{
    myInterruptHandlerCallback = &interruptHandlerCallback;
}

//------------------------------------------------------------------------------
Thread& HardwareTimerSimulated::getThread()
{
    return myThread;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for HardwareTimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimer::Error HardwareTimerSimulated::driverSetConfig(
                                                           const Config& config)
{
    return HardwareTimer::Error(HardwareTimer::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error HardwareTimerSimulated::driverSetEnabled(const bool enabled)
{
    if (isNullPointer(myInterruptHandlerCallback))
    {
        return Module::Error(Module::ERROR_CODE_ENABLE_FAILED);
    }

    myThread.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void HardwareTimerSimulated::threadCallback()
{
    myInterruptHandlerCallback->call();
}
