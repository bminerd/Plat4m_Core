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
/// @file HardwareTimerSTM32F30x.cpp
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief HardwareTimerSTM32F30x class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F30x/HardwareTimerSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/ProcessorSTM32F30x.h>

using Plat4m::HardwareTimerSTM32F30x;
using Plat4m::HardwareTimer;
using Plat4m::Module;
using Plat4m::TimerSTM32F30x;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const TimerSTM32F30x::Interrupt HardwareTimerSTM32F30x::myInterruptMap[] =
{
    TimerSTM32F30x::INTERRUPT_TIM_1_UP_TIM_16, /// TimerSTM32F30x::ID_1
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerSTM32F30x::HardwareTimerSTM32F30x(const TimerSTM32F30x::Id id) :
    HardwareTimer(),
    myUpdateInterrupt(0),
    myTimer(id),
    myPeriod(0)
{
}

//------------------------------------------------------------------------------
HardwareTimerSTM32F30x::HardwareTimerSTM32F30x(
                           const TimerSTM32F30x::Id id,
                           InterruptHandlerCallback& interruptHandlerCallback) :
    HardwareTimer(),
    myUpdateInterrupt(0),
    myTimer(id, interruptHandlerCallback),
    myPeriod(0)
{
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F30x& HardwareTimerSTM32F30x::getTimer()
{
    return myTimer;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error HardwareTimerSTM32F30x::driverSetEnabled(const bool enabled)
{
    myTimer.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from HardwareTimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimer::Error HardwareTimerSTM32F30x::driverSetConfig(
                                                           const Config& config)
{
    TimerSTM32F30x::Config timerConfig;
    timerConfig.mode        = TimerSTM32F30x::MODE_COUNTING_UP;
    timerConfig.frequencyHz = config.frequencyHz;
    timerConfig.resolution  = TimerSTM32F30x::RESOLUTION_MAX;
    timerConfig.maxFrequencyErrorPercent = 0.5;
    timerConfig.direction   = TimerSTM32F30x::DIRECTION_UP;
    timerConfig.autoReloadPreloadEnabled = true;
    timerConfig.repetitionCount = 0;

    myTimer.setConfig(timerConfig);

    return Error(ERROR_CODE_NONE);
}
