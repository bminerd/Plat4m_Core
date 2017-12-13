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
/// @file AnalogInputSTM32F4xx.h
/// @author Ben Minerd
/// @date 8/9/2016
/// @brief AnalogInputSTM32F4xx class header file.
///

#ifndef PLAT4M_ANALOG_INPUT_STM32F4XX_H
#define PLAT4M_ANALOG_INPUT_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <AnalogInput.h>
#include <AdcSTM32F4xx.h>
#include <GpioPinSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class AnalogInputSTM32F4xx : public AnalogInput
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    AnalogInputSTM32F4xx(AdcSTM32F4xx& adc,
                         const AdcSTM32F4xx::ChannelId channelId,
                         GpioPinSTM32F4xx& gpioPin);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    virtual ~AnalogInputSTM32F4xx();

    //--------------------------------------------------------------------------
    // Public methods implemented from AnalogInput
    //--------------------------------------------------------------------------

    VoltageV readVoltageVFast();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    AdcSTM32F4xx& getAdc();

    void setSTM32F4xxChannelConfig(const AdcSTM32F4xx::ChannelConfig& config);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    AdcSTM32F4xx& myAdc;

    const AdcSTM32F4xx::ChannelId myChannelId;
    
    GpioPinSTM32F4xx& myGpioPin;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from AnalogInput
    //--------------------------------------------------------------------------
    
    AnalogInput::Error driverReadVoltageV(VoltageV& voltageV);
};

}; // namespace Plat4m

#endif // PLAT4M_ANALOG_INPUT_STM32F4XX_H
