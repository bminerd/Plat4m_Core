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
/// @file AnalogInputSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 8/9/16
/// @brief AnalogInputSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AnalogInputSTM32F4xx.h>
#include <Processor.h>

using Plat4m::AnalogInputSTM32F4xx;
using Plat4m::AnalogInput;
using Plat4m::AdcSTM32F4xx;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInputSTM32F4xx::AnalogInputSTM32F4xx(
                                        AdcSTM32F4xx& adc,
                                        const AdcSTM32F4xx::ChannelId channelId,
                                        GpioPinSTM32F4xx& gpioPin) :
    AnalogInput(),
	myAdc(adc),
    myChannelId(channelId),
    myGpioPin(gpioPin)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AnalogInputSTM32F4xx::~AnalogInputSTM32F4xx()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from AnalogInput
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::VoltageV AnalogInputSTM32F4xx::readVoltageVFast()
{
    return (myAdc.readChannelVoltageVFast(myChannelId));
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F4xx& AnalogInputSTM32F4xx::getAdc()
{
    return myAdc;
}

//------------------------------------------------------------------------------
void AnalogInputSTM32F4xx::setSTM32F4xxChannelConfig(
                                      const AdcSTM32F4xx::ChannelConfig& config)
{
    myAdc.setSTM32F4xxChannelConfig(myChannelId, config);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error AnalogInputSTM32F4xx::driverSetEnabled(const bool enabled)
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
AnalogInput::Error AnalogInputSTM32F4xx::driverReadVoltageV(VoltageV& voltageV)
{
    myAdc.readChannelVoltageV(myChannelId, voltageV);

    return Error(ERROR_CODE_NONE);
}
