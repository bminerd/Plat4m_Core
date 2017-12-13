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
/// @file PwmOutputSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 11/23/2017
/// @brief PwmOutputSTM32F4xx class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <PwmOutputSTM32F4xx.h>
#include <ProcessorSTM32F4xx.h>

using Plat4m::PwmOutputSTM32F4xx;
using Plat4m::PwmOutput;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

//static const GpioSTM32F4xx::OutputSpeed gpioSpeed =
//                                             GpioSTM32F4xx::OUTPUT_SPEED_100MHZ;

static const float maxFrequencyErrorPercent = 0.5;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutputSTM32F4xx::PwmOutputSTM32F4xx(
                                      TimerSTM32F4xx& timer,
                                      const TimerSTM32F4xx::ChannelId channelId,
                                      GpioPinSTM32F4xx& gpioPin) :
    PwmOutput(),
    myTimer(timer),
    myChannelId(channelId),
    myGpioPin(gpioPin),
    myComplementaryGpioPin(0)
{
}

//------------------------------------------------------------------------------
PwmOutputSTM32F4xx::PwmOutputSTM32F4xx(
                                      TimerSTM32F4xx& timer,
                                      const TimerSTM32F4xx::ChannelId channelId,
                                      GpioPinSTM32F4xx& gpioPin,
                                      GpioPinSTM32F4xx& complementaryGpioPin) :
    PwmOutput(),
    myTimer(timer),
    myChannelId(channelId),
    myGpioPin(gpioPin),
    myComplementaryGpioPin(&complementaryGpioPin)
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error PwmOutputSTM32F4xx::driverSetEnabled(const bool enabled)
{
    myGpioPin.setEnabled(enabled);

    if (isValidPointer(myComplementaryGpioPin))
    {
        myComplementaryGpioPin->setEnabled(enabled);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Timer
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F4xx::driverSetConfig(const Config& config)
{
    GpioPin::Config gpioPinConfig;
    gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
    gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;

    myGpioPin.configure(gpioPinConfig);

    if (isValidPointer(myComplementaryGpioPin))
    {
        myComplementaryGpioPin->configure(gpioPinConfig);
    }

    myTimer.setChannelCaptureCompareMode(
                                   myChannelId,
                                   TimerSTM32F4xx::CAPTURE_COMPARE_MODE_OUTPUT);
    myTimer.setChannelOutputCompareMode(
                                myChannelId,
                                TimerSTM32F4xx::OUTPUT_COMPARE_MODE_PWM_MODE_2);
    myTimer.setChannelOutputComparePreloadEnabled(myChannelId, true);
    myTimer.setChannelPolarity(myChannelId,
                               (TimerSTM32F4xx::Polarity) config.polarity);

    if (config.complementaryOutputEnabled)
    {
        myTimer.setComplementaryChannelPolarity(
                           (TimerSTM32F4xx::ComplementaryChannelId) myChannelId,
                           (TimerSTM32F4xx::Polarity) config.polarity);
    }

    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    // TODO: Is this the same as edge? Think so
    pulse = (uint32_t) round((config.dutyCyclePercent / 100.0) * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);

    // TODO Should this be in driverSetEnabled()?
    myTimer.setChannelCaptureCompareEnabled(myChannelId, true);

    if (config.complementaryOutputEnabled)
    {
        myTimer.setComplementaryChannelCaptureCompareEnabled(
                           (TimerSTM32F4xx::ComplementaryChannelId) myChannelId,
                           true);
    }

    myTimer.generateCaptureCompareControlUpdate();

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F4xx::driverSetDutyCyclePercent(
                                                   const float dutyCyclePercent)
{
    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    // TODO: Is this the same as edge? Think so
    pulse = (uint32_t) round((dutyCyclePercent / 100.0) * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);

    return Error(ERROR_CODE_NONE);
}
