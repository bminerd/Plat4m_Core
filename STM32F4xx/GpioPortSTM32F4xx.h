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
/// @file GpioPortSTM32F4xx.h
/// @author Ben Minerd
/// @date 3/22/13
/// @brief GpioPortSTM32F4xx class header file.
///

#ifndef GPIO_PORT_STM32F4XX_H
#define GPIO_PORT_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <GpioPort.h>
#include <GpioSTM32F4xx.h>

#include <stm32f4xx_gpio.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPortSTM32F4xx : public GpioPort
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    GpioPortSTM32F4xx(const GpioSTM32F4xx::PortId portId);
    
private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const uint32_t myModeMap[];

    static const uint32_t myResistorMap[];
    
    static const GpioSTM32F4xx::OutputSpeed myDefaultOutputSpeed;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const GpioSTM32F4xx::PortId myPortId;
    
    GPIO_TypeDef* myPort;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverEnable(const bool enable);
    
    //--------------------------------------------------------------------------
    // Private methods implemented from GpioPort
    //--------------------------------------------------------------------------
    
    Error driverConfigure(const Config& config);
    
    Error driverSetValue(const unsigned int value);
    
    Error driverGetValue(unsigned int& value);
    
    Error driverReadValue(unsigned int& value);
};

}; // namespace Plat4m

#endif // GPIO_PORT_STM32F4XX_H
