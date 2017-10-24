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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file HardwareTimerSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/16/2016
/// @brief HardwareTimerSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <HardwareTimerSTM32F10x.h>
#include <ProcessorSTM32F10x.h>

#include <stm32f10x_tim.h>

using Plat4m::HardwareTimerSTM32F10x;
using Plat4m::HardwareTimer;
using Plat4m::Module;
using Plat4m::TimerSTM32F10x;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerSTM32F10x::HardwareTimerSTM32F10x(const TimerSTM32F10x::Id id) :
    HardwareTimer(),
    myUpdateInterrupt(0),
    myTimer(id, TimerSTM32F10x::REMAP_0),
    myPeriod(0)
{
}

//------------------------------------------------------------------------------
HardwareTimerSTM32F10x::HardwareTimerSTM32F10x(
                           const TimerSTM32F10x::Id id,
                           InterruptHandlerCallback& interruptHandlerCallback) :
    HardwareTimer(),
    myUpdateInterrupt(0),
    myTimer(id, TimerSTM32F10x::REMAP_0),
    myPeriod(0)
{
    myTimer.setUpdateInterruptCallback(interruptHandlerCallback);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerSTM32F10x::~HardwareTimerSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F10x& HardwareTimerSTM32F10x::getTimer()
{
    return myTimer;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error HardwareTimerSTM32F10x::driverSetEnabled(const bool enabled)
{
    myTimer.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from HardwareTimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimer::Error HardwareTimerSTM32F10x::driverSetConfig(
                                                           const Config& config)
{
    TimerSTM32F10x::Config timerConfig;
    timerConfig.mode        = TimerSTM32F10x::MODE_COUNTING_UP;
    timerConfig.frequencyHz = config.frequencyHz;
    timerConfig.resolution  = TimerSTM32F10x::RESOLUTION_MAX;
    timerConfig.maxFrequencyErrorPercent = 0.5;
    timerConfig.autoReloadPreloadEnabled = true;

    myTimer.setConfig(timerConfig);

    if (isValidPointer(myUpdateInterrupt))
    {
        myTimer.setInterruptEventEnabled(TimerSTM32F10x::INTERRUPT_EVENT_UPDATE,
                                         true);
    }

    return Error(ERROR_CODE_NONE);
}
