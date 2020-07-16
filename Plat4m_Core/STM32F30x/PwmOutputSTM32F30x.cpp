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
/// @file PwmOutputSTM32F30x.cpp
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief PwmOutputSTM32F30x class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <PwmOutputSTM32F30x.h>
#include <ProcessorSTM32F30x.h>

using Plat4m::PwmOutputSTM32F30x;
using Plat4m::PwmOutput;
using Plat4m::Module;
using Plat4m::GpioPinSTM32F30x;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutputSTM32F30x::PwmOutputSTM32F30x(
                                      TimerSTM32F30x& timer,
                                      const TimerSTM32F30x::ChannelId channelId,
                                      GpioPinSTM32F30x& gpioPin) :
    PwmOutput(),
    myTimer(timer),
    myChannelId(channelId),
    myGpioPin(gpioPin),
    myComplementaryGpioPin(0)
{
}

//------------------------------------------------------------------------------
PwmOutputSTM32F30x::PwmOutputSTM32F30x(
                                      TimerSTM32F30x& timer,
                                      const TimerSTM32F30x::ChannelId channelId,
                                      GpioPinSTM32F30x& gpioPin,
                                      GpioPinSTM32F30x& complementaryGpioPin) :
    PwmOutput(),
    myTimer(timer),
    myChannelId(channelId),
    myGpioPin(gpioPin),
    myComplementaryGpioPin(&complementaryGpioPin)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
PwmOutputSTM32F30x::~PwmOutputSTM32F30x()
{

}

//------------------------------------------------------------------------------
// Public methods implemented from PwmOutput
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void PwmOutputSTM32F30x::setDutyCyclePercentFast(const float dutyCyclePercent)
{
    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    pulse = (uint32_t) round((dutyCyclePercent / 100.0) * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);
}

//------------------------------------------------------------------------------
void PwmOutputSTM32F30x::setDutyCycleNormalizedFast(const float dutyCycleNormalized)
{
    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    pulse = (uint32_t) (dutyCycleNormalized * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error PwmOutputSTM32F30x::driverSetEnabled(const bool enabled)
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
PwmOutput::Error PwmOutputSTM32F30x::driverSetConfig(const Config& config)
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
                                   TimerSTM32F30x::CAPTURE_COMPARE_MODE_OUTPUT);
    myTimer.setChannelOutputCompareMode(
                                myChannelId,
                                TimerSTM32F30x::OUTPUT_COMPARE_MODE_PWM_MODE_2);
    myTimer.setChannelOutputComparePreloadEnabled(myChannelId, true);
    myTimer.setChannelPolarity(myChannelId,
                               (TimerSTM32F30x::Polarity) config.polarity);

    if (config.complementaryOutputEnabled)
    {
        myTimer.setComplementaryChannelPolarity(
                           (TimerSTM32F30x::ComplementaryChannelId) myChannelId,
                           (TimerSTM32F30x::Polarity) config.polarity);
    }

    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    pulse = (uint32_t) round((config.dutyCyclePercent / 100.0) * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);

    // TODO Should this be in driverSetEnabled()?
    myTimer.setChannelCaptureCompareEnabled(myChannelId, true);

    if (config.complementaryOutputEnabled)
    {
        myTimer.setComplementaryChannelCaptureCompareEnabled(
                           (TimerSTM32F30x::ComplementaryChannelId) myChannelId,
                           true);
    }

    myTimer.generateCaptureCompareControlUpdate();

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F30x::driverSetDutyCyclePercent(
                                                   const float dutyCyclePercent)
{
    uint32_t period = myTimer.getPeriod();
    uint32_t pulse;

    pulse = (uint32_t) round((dutyCyclePercent / 100.0) * period);

    myTimer.setChannelCaptureCompareValue(myChannelId, pulse);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F30x::driverSetActive(const bool active)
{
    myTimer.setChannelCaptureCompareEnabled(myChannelId, active);
    myTimer.setComplementaryChannelCaptureCompareEnabled(
                  (TimerSTM32F30x::ComplementaryChannelId) myChannelId, active);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
PwmOutput::Error PwmOutputSTM32F30x::driverSetComplementaryActive(
                                                              const bool active)
{
    myTimer.setComplementaryChannelCaptureCompareEnabled(
                           (TimerSTM32F30x::ComplementaryChannelId) myChannelId,
                           active);

    return Error(ERROR_CODE_NONE);
}
