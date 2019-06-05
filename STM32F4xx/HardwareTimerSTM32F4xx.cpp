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
/// @file HardwareTimerSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 5/15/2013
/// @brief HardwareTimerSTM32F4xx class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F4xx/HardwareTimerSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/ProcessorSTM32F4xx.h>

using Plat4m::HardwareTimerSTM32F4xx;
using Plat4m::HardwareTimer;
using Plat4m::Module;
using Plat4m::TimerSTM32F4xx;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerSTM32F4xx::HardwareTimerSTM32F4xx(const TimerSTM32F4xx::Id id) :
    HardwareTimer(),
	myUpdateInterrupt(0),
	myTimer(id),
	myPeriod(0)
{
}

//------------------------------------------------------------------------------
HardwareTimerSTM32F4xx::HardwareTimerSTM32F4xx(
						   const TimerSTM32F4xx::Id id,
		   	   	   	   	   InterruptHandlerCallback& interruptHandlerCallback) :
    HardwareTimer(),
	myUpdateInterrupt(),
	myTimer(id, interruptHandlerCallback),
	myPeriod(0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimerSTM32F4xx::~HardwareTimerSTM32F4xx()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F4xx& HardwareTimerSTM32F4xx::getTimer()
{
	return myTimer;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error HardwareTimerSTM32F4xx::driverSetEnabled(const bool enabled)
{
	myTimer.setEnabled(enabled);

	return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from HardwareTimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
HardwareTimer::Error HardwareTimerSTM32F4xx::driverSetConfig(
														   const Config& config)
{
    TimerSTM32F4xx::Config timerConfig;
    timerConfig.mode        = TimerSTM32F4xx::MODE_COUNTING_UP;
    timerConfig.frequencyHz = config.frequencyHz;
    timerConfig.resolution  = TimerSTM32F4xx::RESOLUTION_MAX;
    timerConfig.maxFrequencyErrorPercent = 0.5;
    timerConfig.direction   = TimerSTM32F4xx::DIRECTION_UP;
    timerConfig.autoReloadPreloadEnabled = true;

    myTimer.setConfig(timerConfig);
    
    return Error(ERROR_CODE_NONE);
}
