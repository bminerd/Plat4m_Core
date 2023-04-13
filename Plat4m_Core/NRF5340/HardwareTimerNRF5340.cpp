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
// Copyright (c) 2021-2023 Benjamin Minerd
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
/// @file HardwareTimerNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief HardwareTimerNRF5340 class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/NRF5340/HardwareTimerNRF5340.h>
#include <Plat4m_Core/NRF5340/ProcessorNRF5340.h>

using Plat4m::HardwareTimerNRF5340;
using Plat4m::HardwareTimer;
using Plat4m::Module;
using Plat4m::TimerNRF5340;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerNRF5340::HardwareTimerNRF5340(const TimerNRF5340::Id id) :
    HardwareTimer(),
    myTimer(id)
{
}

//------------------------------------------------------------------------------
HardwareTimerNRF5340::HardwareTimerNRF5340(
                           const TimerNRF5340::Id id,
                           InterruptHandlerCallback& interruptHandlerCallback) :
    HardwareTimer(),
    myTimer(id, interruptHandlerCallback)
{
}

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerNRF5340& HardwareTimerNRF5340::getTimer()
{
    return myTimer;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error HardwareTimerNRF5340::driverSetEnabled(const bool enabled)
{
    myTimer.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for HardwareTimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimer::Error HardwareTimerNRF5340::driverSetConfig(
                                                           const Config& config)
{
    TimerNRF5340::Config timerConfig;
    timerConfig.mode        = TimerNRF5340::MODE_TIMER;
    timerConfig.frequencyHz = config.frequencyHz;
    timerConfig.resolution  = TimerNRF5340::RESOLUTION_MAX;
    // timerConfig.resolution  = TimerNRF5340::RESOLUTION_MIN;
    timerConfig.maxFrequencyErrorPercent = 0.5;

    myTimer.setConfig(timerConfig);

    return Error(ERROR_CODE_NONE);
}
