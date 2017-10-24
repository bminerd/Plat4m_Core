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
/// @file AnalogInputSTM32F30x.cpp
/// @author Ben Minerd
/// @date 8/9/16
/// @brief AnalogInputSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AnalogInputSTM32F30x.h>
#include <Processor.h>

using Plat4m::AnalogInputSTM32F30x;
using Plat4m::AnalogInput;
using Plat4m::AdcSTM32F30x;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInputSTM32F30x::AnalogInputSTM32F30x(
                                        AdcSTM32F30x& adc,
                                        const AdcSTM32F30x::ChannelId channelId,
                                        GpioPinSTM32F30x& gpioPin) :
    AnalogInput(),
    myChannelId(channelId),
    myAdc(adc),
    myGpioPin(gpioPin)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInputSTM32F30x::~AnalogInputSTM32F30x()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from AnalogInput
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
float AnalogInputSTM32F30x::readVoltageFast()
{
    return (myAdc.readChannelVoltageFast(myChannelId));
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F30x& AnalogInputSTM32F30x::getAdc()
{
    return myAdc;
}

//------------------------------------------------------------------------------
void AnalogInputSTM32F30x::setSTM32F30xChannelConfig(
                                      const AdcSTM32F30x::ChannelConfig& config)
{
    myAdc.setSTM32F30xChannelConfig(myChannelId, config);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error AnalogInputSTM32F30x::driverSetEnabled(const bool enabled)
{
    myGpioPin.setEnabled(enabled);

    if (enabled)
    {
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_ANALOG;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;

        myGpioPin.configure(gpioConfig);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from AnalogInput
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInput::Error AnalogInputSTM32F30x::driverReadVoltage(float& voltage)
{
    myAdc.readChannelVoltage(myChannelId, voltage);

    return Error(ERROR_CODE_NONE);
}
