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
/// @file GpioPortSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 3/25/13
/// @brief GpioPortSTM32F4xx class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <GpioPortSTM32F4xx.h>
#include <Module.h>

#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>

using Plat4m::GpioPortSTM32F4xx;
using Plat4m::GpioPort;
using Plat4m::GpioSTM32F4xx;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

/**
 * @brief Define for GPIO clock speed.
 */
#define GPIO_CLOCK_SPEED (GPIO_Speed_50MHz)

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint32_t GpioPortSTM32F4xx::myModeMap[] =
{
    0x55555555, /// GPIO_MODE_DIGITAL_OUTPUT
    0x00000000, /// GPIO_MODE_DIGITAL_INPUT
    0xFFFFFFFF, /// GPIO_MODE_ANALOG
    0xAAAAAAAA  /// GPIO_MODE_ALTERNATE_FUNCTION
};

const uint32_t GpioPortSTM32F4xx::myResistorMap[] =
{
    0x00000000, /// GPIO_RESISTOR_NONE
    0x55555555, /// GPIO_RESISTOR_PULL_UP
    0xAAAAAAAA  /// GPIO_RESISTOR_PULL_DOWN
};

const GpioSTM32F4xx::OutputSpeed myDefaultOutputSpeed =
                                              GpioSTM32F4xx::OUTPUT_SPEED_50MHZ;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F4xx::GpioPortSTM32F4xx(const GpioSTM32F4xx::PortId portId) :
    GpioPort(),
    myPortId(portId),
    myPort(GpioSTM32F4xx::portMap[portId])
{
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPortSTM32F4xx::driverEnable(const bool enable)
{
    RCC_AHB1PeriphClockCmd(GpioSTM32F4xx::clockMap[myPortId],
                           (FunctionalState) enable);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPort::Error GpioPortSTM32F4xx::driverConfigure(const Config& config)
{
    // Set mode
    
    // Set mode bits for this port
    myPort->MODER = myModeMap[config.mode];
    
    // Set resistor
    
    // Set resistor bits for this pin
    myPort->PUPDR = myResistorMap[config.resistor];
    
    if ((config.mode) == MODE_DIGITAL_OUTPUT)
    {
        // Set speed bits for this pin
        myPort->OSPEEDR = 0xAAAAAAAA;
        
        // Set output bits for this pin
        myPort->OTYPER = 0x00000000;
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPort::Error GpioPortSTM32F4xx::driverSetValue(const unsigned int value)
{
    myPort->ODR = (uint16_t) value;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPort::Error GpioPortSTM32F4xx::driverGetValue(unsigned int& value)
{
    value = (myPort->ODR & 0xFFFF);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
GpioPort::Error GpioPortSTM32F4xx::driverReadValue(unsigned int& value)
{
    value = (myPort->IDR & 0xFFFF);
    
    return ERROR_NONE;
}
