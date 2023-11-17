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
/// @file PwmOutputSTM32F4xx.h
/// @author Ben Minerd
/// @date 11/23/2017
/// @brief PwmOutputSTM32F4xx class header file.
///

#ifndef PLAT4M_PWM_OUTPUT_STM32F30X_H
#define PLAT4M_PWM_OUTPUT_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/PwmOutput.h>
#include <Plat4m_Core/STM32F4xx/TimerSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/GpioPinSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class PwmOutputSTM32F4xx : public PwmOutput
{
public:

    //--------------------------------------------------------------------------
    // Private constructors
    //--------------------------------------------------------------------------

    PwmOutputSTM32F4xx(TimerSTM32F4xx& timer,
                       const TimerSTM32F4xx::ChannelId channelId,
                       GpioPinSTM32F4xx& gpioPin);

    PwmOutputSTM32F4xx(TimerSTM32F4xx& timer,
                       const TimerSTM32F4xx::ChannelId channelId,
                       GpioPinSTM32F4xx& gpioPin,
                       GpioPinSTM32F4xx& complementaryGpioPin);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

//    static const GpioSTM32F4xx::AlternateFunction myAlternateFunctionMap

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    TimerSTM32F4xx& myTimer;

    const TimerSTM32F4xx::ChannelId myChannelId;

    GpioPinSTM32F4xx& myGpioPin;

    GpioPinSTM32F4xx* myComplementaryGpioPin;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Private methods overridden for PwmOutput
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) override;

    virtual Error driverSetDutyCyclePercent(
                                         const float dutyCyclePercent) override;
};

}; // namespace Plat4m

#endif // PLAT4M_PWM_OUTPUT_STM32F30X_H
