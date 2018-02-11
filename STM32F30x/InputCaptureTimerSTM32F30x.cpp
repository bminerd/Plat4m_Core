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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file InputCaptureTimerSTM32F30x.cpp
/// @author Ben Minerd
/// @date 5/15/2017
/// @brief InputCaptureTimerSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F30x/InputCaptureTimerSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/ProcessorSTM32F30x.h>
#include <Plat4m_Core/CallbackMethod.h>

using Plat4m::InputCaptureTimerSTM32F30x;
using Plat4m::InputCaptureTimer;
using Plat4m::Module;
using Plat4m::GpioPinSTM32F30x;
using Plat4m::TimerSTM32F30x;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InputCaptureTimerSTM32F30x::InputCaptureTimerSTM32F30x(
                                      TimerSTM32F30x& timer,
                                      const TimerSTM32F30x::ChannelId channelId,
                                      GpioPinSTM32F30x& gpioPin) :
    InputCaptureTimer(gpioPin),
    myTimer(timer),
    myChannelId(channelId),
    myGpioPin(gpioPin)
{
    myTimer.setChannelCaptureCompareInterruptCallback(
                   myChannelId,
                   createCallback((InputCaptureTimer*) this,
                                  &InputCaptureTimer::captureInterruptHandler));
    myTimer.setUpdateInterruptCallback(
                                 createCallback((InputCaptureTimer*) this,
                                 &InputCaptureTimer::overflowInterruptHandler));
}

//------------------------------------------------------------------------------
InputCaptureTimerSTM32F30x::InputCaptureTimerSTM32F30x(
                                   TimerSTM32F30x& timer,
                                   const TimerSTM32F30x::ChannelId channelId,
                                   GpioPinSTM32F30x& gpioPin,
                                   CaptureReadyCallback& captureReadyCallback) :
    InputCaptureTimer(gpioPin),
    myTimer(timer),
    myChannelId(channelId),
    myGpioPin(gpioPin)
{
    myTimer.setChannelCaptureCompareInterruptCallback(
                   myChannelId,
                   createCallback((InputCaptureTimer*) this,
                                  &InputCaptureTimer::captureInterruptHandler));
    myTimer.setUpdateInterruptCallback(
                                 createCallback((InputCaptureTimer*) this,
                                 &InputCaptureTimer::overflowInterruptHandler));
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InputCaptureTimerSTM32F30x::~InputCaptureTimerSTM32F30x()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from InputCapturetimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t InputCaptureTimerSTM32F30x::getValueFast()
{
    return (myTimer.getChannelCaptureCompareValue(myChannelId));
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F30x& InputCaptureTimerSTM32F30x::getTimer()
{
	return myTimer;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InputCaptureTimerSTM32F30x::driverSetEnabled(const bool enabled)
{
    myGpioPin.setEnabled(enabled);

    if (enabled)
    {
        GpioPin::Config gpioPinConfig;
        gpioPinConfig.mode = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;

        myGpioPin.configure(gpioPinConfig);

        TimerSTM32F30x::InputCaptureConfig inputCaptureConfig;
        inputCaptureConfig.selection =
                     TimerSTM32F30x::INPUT_CAPTURE_SELECTION_IC_1_MAPPED_ON_TRC;

        myTimer.setChannelInputCaptureConfig(myChannelId, inputCaptureConfig);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from InputCaptureTimer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InputCaptureTimer::Error InputCaptureTimerSTM32F30x::driverSetConfig(
                                                           const Config& config)
{
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t InputCaptureTimerSTM32F30x::driverGetTimerValue()
{
    return (myTimer.getCounterValue());
}

//------------------------------------------------------------------------------
uint32_t InputCaptureTimerSTM32F30x::driverGetCaptureValue()
{
    return (myTimer.getChannelCaptureCompareValue(myChannelId));
}

//------------------------------------------------------------------------------
uint32_t InputCaptureTimerSTM32F30x::driverGetPeriodValue()
{
    return (myTimer.getPeriod());
}
