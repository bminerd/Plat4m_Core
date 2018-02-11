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
/// @file GpioPortSTM32F30x.cpp
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief GpioPortSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/STM32F30x/GpioPortSTM32F30x.h>

using Plat4m::GpioPortSTM32F30x;
using Plat4m::GpioPort;
using Plat4m::Module;
     
//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const uint32_t GpioPortSTM32F30x::myClockMap[] =
{
    RCC_AHBPeriph_GPIOA, /// PORT_ID_A
    RCC_AHBPeriph_GPIOB, /// PORT_ID_B
    RCC_AHBPeriph_GPIOC, /// PORT_ID_C
    RCC_AHBPeriph_GPIOD, /// PORT_ID_D
    RCC_AHBPeriph_GPIOE, /// PORT_ID_E
    RCC_AHBPeriph_GPIOF, /// PORT_ID_F
    RCC_AHBPeriph_GPIOG, /// PORT_ID_G
    RCC_AHBPeriph_GPIOH  /// PORT_ID_H
};

const uint32_t GpioPortSTM32F30x::myModeMap[] =
{
    0x55555555, /// GPIO_MODE_DIGITAL_OUTPUT
    0x00000000, /// GPIO_MODE_DIGITAL_INPUT
    0xFFFFFFFF, /// GPIO_MODE_ANALOG
    0xAAAAAAAA  /// GPIO_MODE_ALTERNATE_FUNCTION
};

const uint32_t GpioPortSTM32F30x::myResistorMap[] =
{
    0x00000000, /// GPIO_RESISTOR_NONE
    0x55555555, /// GPIO_RESISTOR_PULL_UP
    0xAAAAAAAA  /// GPIO_RESISTOR_PULL_DOWN
};

// Variables

GPIO_TypeDef* GpioPortSTM32F30x::myPortMap[] =
{
    GPIOA, /// PORT_ID_A
    GPIOB, /// PORT_ID_B
    GPIOC, /// PORT_ID_C
    GPIOD, /// PORT_ID_D
    GPIOE, /// PORT_ID_E
    GPIOF, /// PORT_ID_F
    GPIOG, /// PORT_ID_G
    GPIOH  /// PORT_ID_H
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F30x::GpioPortSTM32F30x(const Id id) :
    GpioPort(),
    myId(id),
    myPort(myPortMap[id])
{
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPortSTM32F30x::setValueFast(const uint16_t value)
{
    myPort->ODR = value;
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32F30x::getValueFast()
{
    return (myPort->ODR);
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32F30x::readValueFast()
{
    return (myPort->IDR);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F30x::Id GpioPortSTM32F30x::getId()
{
    return myId;
}

//------------------------------------------------------------------------------
GPIO_TypeDef*& GpioPortSTM32F30x::getPort()
{
    return myPort;
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPortSTM32F30x::driverSetEnabled(const bool enabled)
{
    RCC_AHBPeriphClockCmd(myClockMap[myId], (FunctionalState) enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F30x::driverConfigure(
                                                           const Config& config)
{
    // Set mode
    
    // Set mode bits for this port
    myPort->MODER = myModeMap[config.mode];
    
    // Set resistor
    
    // Set resistor bits for this pin
    myPort->PUPDR = myResistorMap[config.resistor];
    
    if ((config.mode) == MODE_DIGITAL_OUTPUT_PUSH_PULL)
    {
        // Set speed bits for this pin
        myPort->OSPEEDR = 0xAAAAAAAA;
        
        // Set output bits for this pin
        myPort->OTYPER = 0x00000000;
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F30x::driverSetValue(
                                                           const uint16_t value)
{
    myPort->ODR = (uint16_t) value;
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F30x::driverGetValue(uint16_t& value)
{
    value = (myPort->ODR & 0xFFFF);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F30x::driverReadValue(uint16_t& value)
{
    value = (myPort->IDR & 0xFFFF);
    
    return Error(ERROR_CODE_NONE);
}
