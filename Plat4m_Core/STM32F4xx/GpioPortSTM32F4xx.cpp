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
/// @file GpioPortSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 3/25/13
/// @brief GpioPortSTM32F4xx class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F4xx/GpioPortSTM32F4xx.h>
#include <Plat4m_Core/Module.h>

using Plat4m::GpioPortSTM32F4xx;
using Plat4m::GpioPort;
using Plat4m::Module;
using Plat4m::ProcessorSTM32F4xx;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

GPIO_TypeDef* GpioPortSTM32F4xx::myPortMap[] =
{
	GPIOA, /// ID_A
	GPIOB, /// ID_B
	GPIOC, /// ID_C
	GPIOD, /// ID_D
	GPIOE, /// ID_E
	GPIOF, /// ID_F
	GPIOG, /// ID_G
	GPIOH, /// ID_H
	GPIOI  /// ID_I
};

const ProcessorSTM32F4xx::Peripheral GpioPortSTM32F4xx::myPeripheralMap[] =
{
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_A, /// ID_A
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_B, /// ID_B
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_C, /// ID_C
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_D, /// ID_D
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_E, /// ID_E
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_F, /// ID_F
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_G, /// ID_G
	ProcessorSTM32F4xx::PERIPHERAL_GPIO_H  /// ID_H
};

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

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F4xx::GpioPortSTM32F4xx(const Id id) :
    GpioPort(),
    myId(id),
    myPort(myPortMap[id])
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F4xx::~GpioPortSTM32F4xx()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F4xx::Id GpioPortSTM32F4xx::getId() const
{
	return myId;
}

//------------------------------------------------------------------------------
GPIO_TypeDef* GpioPortSTM32F4xx::getPort()
{
	return myPort;
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPortSTM32F4xx::setValueFast(const uint16_t value)
{
	myPort->ODR = value;
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32F4xx::getValueFast()
{
	return (myPort->ODR & 0xFFFF);
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32F4xx::readValueFast()
{
	return (myPort->IDR & 0xFFFF);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPortSTM32F4xx::driverSetEnabled(const bool enabled)
{
	ProcessorSTM32F4xx::setPeripheralClockEnabled(myPeripheralMap[myId],
												  enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F4xx::driverConfigure(
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
GpioPort<uint16_t>::Error GpioPortSTM32F4xx::driverSetValue(
														   const uint16_t value)
{
    myPort->ODR = value;
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F4xx::driverGetValue(uint16_t& value)
{
    value = (myPort->ODR & 0xFFFF);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F4xx::driverReadValue(uint16_t& value)
{
    value = (myPort->IDR & 0xFFFF);
    
    return Error(ERROR_CODE_NONE);
}
