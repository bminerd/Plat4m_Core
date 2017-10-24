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
/// @file GpioPortSTM32F10x.cpp
/// @author Ben Minerd
/// @date 1/5/2016
/// @brief GpioPortSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <GpioPortSTM32F10x.h>

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

using Plat4m::GpioPortSTM32F10x;
using Plat4m::GpioPort;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const uint32_t GpioPortSTM32F10x::myClockMap[] =
{
    RCC_APB2Periph_GPIOA, /// ID_A
    RCC_APB2Periph_GPIOB, /// ID_B
    RCC_APB2Periph_GPIOC, /// ID_C
    RCC_APB2Periph_GPIOD, /// ID_D
    RCC_APB2Periph_GPIOE, /// ID_E
    RCC_APB2Periph_GPIOF, /// ID_F
    RCC_APB2Periph_GPIOG  /// ID_G
};

const uint32_t GpioPortSTM32F10x::myModeMap[] =
{
    0x55555555, /// GPIO_MODE_DIGITAL_OUTPUT
    0x00000000, /// GPIO_MODE_DIGITAL_INPUT
    0xFFFFFFFF, /// GPIO_MODE_ANALOG
    0xAAAAAAAA  /// GPIO_MODE_ALTERNATE_FUNCTION
};

const uint32_t GpioPortSTM32F10x::myResistorMap[] =
{
    0x00000000, /// GPIO_RESISTOR_NONE
    0x55555555, /// GPIO_RESISTOR_PULL_UP
    0xAAAAAAAA  /// GPIO_RESISTOR_PULL_DOWN
};

// Variables

GPIO_TypeDef* GpioPortSTM32F10x::myPortMap[] =
{
    GPIOA, /// ID_A
    GPIOB, /// ID_B
    GPIOC, /// ID_C
    GPIOD, /// ID_D
    GPIOE, /// ID_E
    GPIOF, /// ID_F
    GPIOG  /// ID_G
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F10x::GpioPortSTM32F10x(const Id id) :
    GpioPort(),
    myId(id),
    myPort(myPortMap[id])
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F10x::~GpioPortSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPortSTM32F10x::setValueFast(const uint16_t value)
{
    myPort->ODR = (uint32_t) value;
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32F10x::getValueFast()
{
    return (myPort->ODR & 0xFFFF);
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32F10x::readValueFast()
{
    return (myPort->IDR & 0xFFFF);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F10x::Id GpioPortSTM32F10x::getId()
{
    return myId;
}

//------------------------------------------------------------------------------
GPIO_TypeDef*& GpioPortSTM32F10x::getPort()
{
    return myPort;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPortSTM32F10x::driverSetEnabled(const bool enabled)
{
	RCC_APB2PeriphClockCmd(myClockMap[myId], (FunctionalState) enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F10x::driverConfigure(
                                                           const Config& config)
{
	// Set mode

	// TODO: Verify these values!

	// CRL/CRH register
	// Clear configuration and mode bits
	clearBits(myPort->CRL, 0xFFFFFFFF);
	clearBits(myPort->CRH, 0xFFFFFFFF);

	if (config.mode == GpioPort::MODE_DIGITAL_OUTPUT_PUSH_PULL)
	{
		setBits(myPort->CRL, 0x33333333);
		setBits(myPort->CRH, 0x33333333);
	}
	else if (config.mode == GpioPort::MODE_DIGITAL_OUTPUT_OPEN_DRAIN)
	{
		setBits(myPort->CRL, 0x77777777);
		setBits(myPort->CRH, 0x77777777);
	}
	else if (config.mode == GpioPort::MODE_DIGITAL_INPUT)
	{
		setBits(myPort->CRL, 0x44444444);
		setBits(myPort->CRH, 0x44444444);
	}
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F10x::driverSetValue(
                                                           const uint16_t value)
{
    myPort->ODR = (uint32_t) value;
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F10x::driverGetValue(uint16_t& value)
{
    value = (myPort->ODR & 0xFFFF);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32F10x::driverReadValue(uint16_t& value)
{
    value = (myPort->IDR & 0xFFFF);
    
    return Error(ERROR_CODE_NONE);
}
