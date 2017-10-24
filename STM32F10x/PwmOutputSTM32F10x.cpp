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
/// @file PwmOutputSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/21/2016
/// @brief PwmOutputSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <PwmOutputSTM32F10x.h>
#include <ProcessorSTM32F10x.h>

using Plat4m::PwmOutputSTM32F10x;
using Plat4m::PwmOutput;
using Plat4m::Module;
using Plat4m::GpioPinSTM32F10x;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutputSTM32F10x::PwmOutputSTM32F10x(
                                      TimerSTM32F10x& timer,
                                      const TimerSTM32F10x::ChannelId channelId,
                                      GpioPinSTM32F10x& gpioPin) :
    PwmOutput(),
    myTimer(timer),
    myChannelId(channelId),
    myGpioPin(gpioPin),
    myComplementaryGpioPin(0)
{
}

//------------------------------------------------------------------------------
PwmOutputSTM32F10x::PwmOutputSTM32F10x(
                                      TimerSTM32F10x& timer,
                                      const TimerSTM32F10x::ChannelId channelId,
                                      GpioPinSTM32F10x& gpioPin,
                                      GpioPinSTM32F10x& complementaryGpioPin) :
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
Module::Error PwmOutputSTM32F10x::driverSetEnabled(const bool enabled)
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
PwmOutput::Error PwmOutputSTM32F10x::driverSetConfig(const Config& config)
{
    GpioPin::Config gpioPinConfig;
    gpioPinConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
    gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;

    GpioPinSTM32F10x::STM32F10xConfig gpioPinSTM32F10xConfig;
    gpioPinSTM32F10xConfig.outputType = GpioPinSTM32F10x::OUTPUT_TYPE_PUSH_PULL;
    gpioPinSTM32F10xConfig.outputSpeed = GpioPinSTM32F10x::OUTPUT_SPEED_50MHZ;

    myGpioPin.configure(gpioPinConfig);
    myGpioPin.setSTM32F10xConfig(gpioPinSTM32F10xConfig);

    if (isValidPointer(myComplementaryGpioPin))
    {
        myComplementaryGpioPin->configure(gpioPinConfig);
        myComplementaryGpioPin->setSTM32F10xConfig(gpioPinSTM32F10xConfig);
    }

//    myTimer.setChannelCaptureCompareMode(
//                                   myChannelId,
//                                   TimerSTM32F10x::CAPTURE_COMPARE_MODE_OUTPUT);
//    myTimer.setChannelOutputCompareMode(
//                                myChannelId,
//                                TimerSTM32F10x::OUTPUT_COMPARE_MODE_PWM_MODE_2);
//    myTimer.setChannelOutputComparePreloadEnabled(myChannelId, true);
//    myTimer.setChannelPolarity(myChannelId,
//                               (TimerSTM32F10x::Polarity) config.polarity);

    TimerSTM32F10x::OutputCompareConfig outputCompareConfig;
    outputCompareConfig.mode = TimerSTM32F10x::OUTPUT_COMPARE_MODE_PWM_MODE_2;
    outputCompareConfig.polarity = (TimerSTM32F10x::Polarity) (config.polarity);
    outputCompareConfig.preloadEnabled = true;

    myTimer.setChannelOutputCompareConfig(myChannelId, outputCompareConfig);

    if (config.complementaryOutputEnabled)
    {
        myTimer.setComplementaryChannelPolarity(
                       (TimerSTM32F10x::ComplementaryChannelId) myChannelId,
                       (TimerSTM32F10x::Polarity) config.complementaryPolarity);
    }

    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    pulse = (uint32_t) round((config.dutyCyclePercent / 100.0) * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);

//    setBits(myTimer.getTimer()->CR2, ((uint32_t) 1) << 8 + myChannelId*2);
//    setBits(myTimer.getTimer()->CR2, ((uint32_t) 1) << 9 + myChannelId*2);

    // TODO Should this be in driverSetEnabled()?
//    myTimer.setChannelCaptureCompareEnabled(myChannelId, true);
//
//    if (config.complementaryOutputEnabled)
//    {
//        myTimer.setComplementaryChannelCaptureCompareEnabled(
//                           (TimerSTM32F10x::ComplementaryChannelId) myChannelId,
//                           true);
//    }

    myTimer.generateCaptureCompareControlUpdate();

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F10x::driverSetDutyCyclePercent(
                                                   const float dutyCyclePercent)
{
    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    pulse = (uint32_t) round((dutyCyclePercent / 100.0) * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F10x::driverSetActive(const bool active)
{
    myTimer.setChannelCaptureCompareEnabled(myChannelId, active);
    myTimer.setComplementaryChannelCaptureCompareEnabled(
                  (TimerSTM32F10x::ComplementaryChannelId) myChannelId, active);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F10x::driverSetComplementaryActive(
                                                              const bool active)
{
    myTimer.setComplementaryChannelCaptureCompareEnabled(
                           (TimerSTM32F10x::ComplementaryChannelId) myChannelId,
                           active);

    return Error(ERROR_CODE_NONE);
}
