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
/// @file AnalogInputSTM32F10x.h
/// @author Ben Minerd
/// @date 12/20/2016
/// @brief AnalogInputSTM32F10x class header file.
///

#ifndef PLAT4M_ANALOG_INPUT_STM32F10X_H
#define PLAT4M_ANALOG_INPUT_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <AnalogInput.h>
#include <AdcSTM32F10x.h>
#include <GpioPinSTM32F10x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class AnalogInputSTM32F10x : public AnalogInput
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    AnalogInputSTM32F10x(AdcSTM32F10x& adc,
                         const AdcSTM32F10x::ChannelId adcChannelId,
                         GpioPinSTM32F10x& gpioPin);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    virtual ~AnalogInputSTM32F10x();

    //--------------------------------------------------------------------------
    // Public methods implemented from AnalogInput
    //--------------------------------------------------------------------------

    VoltageV readVoltageVFast();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    AdcSTM32F10x& getAdc();

    AdcSTM32F10x::ChannelId getAdcChannelId() const;

    void setAdcChannelConfig(const AdcSTM32F10x::ChannelConfig& config);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    AdcSTM32F10x& myAdc;

    const AdcSTM32F10x::ChannelId myAdcChannelId;
    
    GpioPinSTM32F10x& myGpioPin;

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

#endif // PLAT4M_ANALOG_INPUT_STM32F10X_H
