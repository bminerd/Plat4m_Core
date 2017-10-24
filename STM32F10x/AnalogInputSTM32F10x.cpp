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
/// @file AnalogInputSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/20/16
/// @brief AnalogInputSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AnalogInputSTM32F10x.h>
#include <Processor.h>

using Plat4m::AnalogInputSTM32F10x;
using Plat4m::AnalogInput;
using Plat4m::AdcSTM32F10x;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInputSTM32F10x::AnalogInputSTM32F10x(
                                     AdcSTM32F10x& adc,
                                     const AdcSTM32F10x::ChannelId adcChannelId,
                                     GpioPinSTM32F10x& gpioPin) :
    AnalogInput(),
    myAdcChannelId(adcChannelId),
    myAdc(adc),
    myGpioPin(gpioPin)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInputSTM32F10x::~AnalogInputSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from AnalogInput
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::VoltageV AnalogInputSTM32F10x::readVoltageVFast()
{
    return (myAdc.readChannelVoltageVFast(myAdcChannelId));
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F10x& AnalogInputSTM32F10x::getAdc()
{
    return myAdc;
}

//------------------------------------------------------------------------------
AdcSTM32F10x::ChannelId AnalogInputSTM32F10x::getAdcChannelId() const
{
    return myAdcChannelId;
}

//------------------------------------------------------------------------------
void AnalogInputSTM32F10x::setAdcChannelConfig(
                                      const AdcSTM32F10x::ChannelConfig& config)
{
    myAdc.setChannelConfig(myAdcChannelId, config);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error AnalogInputSTM32F10x::driverSetEnabled(const bool enabled)
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
AnalogInput::Error AnalogInputSTM32F10x::driverReadVoltageV(VoltageV& voltage)
{
    myAdc.readChannelVoltageV(myAdcChannelId, voltage);

    return Error(ERROR_CODE_NONE);
}
