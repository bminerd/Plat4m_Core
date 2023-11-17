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
// Copyright (c) 2022-2023 Benjamin Minerd
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
/// @file GpioPortSTM32H7xx.cpp
/// @author Ben Minerd
/// @date 6/22/2022
/// @brief GpioPortSTM32H7xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32h7xx.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/STM32H7xx/GpioPortSTM32H7xx.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32H7xx::Peripheral GpioPortSTM32H7xx::myPeripheralMap[] =
{
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_A, /// ID_A
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_B, /// ID_B
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_C, /// ID_C
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_D, /// ID_D
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_E, /// ID_E
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_F, /// ID_F
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_G, /// ID_G
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_H, /// ID_H
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_I, /// ID_I
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_J, /// ID_J
    ProcessorSTM32H7xx::PERIPHERAL_GPIO_K  /// ID_K
};

const std::uint32_t GpioPortSTM32H7xx::myModeMap[] =
{
    0x55555555, /// GPIO_MODE_DIGITAL_OUTPUT
    0x00000000, /// GPIO_MODE_DIGITAL_INPUT
    0xFFFFFFFF, /// GPIO_MODE_ANALOG
    0xAAAAAAAA  /// GPIO_MODE_ALTERNATE_FUNCTION
};

const std::uint32_t GpioPortSTM32H7xx::myResistorMap[] =
{
    0x00000000, /// GPIO_RESISTOR_NONE
    0x55555555, /// GPIO_RESISTOR_PULL_UP
    0xAAAAAAAA  /// GPIO_RESISTOR_PULL_DOWN
};

// Variables

GPIO_TypeDef* GpioPortSTM32H7xx::myPortMap[] =
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
GpioPortSTM32H7xx::GpioPortSTM32H7xx(const Id id) :
    GpioPort(),
    myId(id),
    myPort(myPortMap[id])
{
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPortSTM32H7xx::setValueFast(const std::uint16_t value)
{
    myPort->ODR = value;
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32H7xx::getValueFast()
{
    return (myPort->ODR);
}

//------------------------------------------------------------------------------
uint16_t GpioPortSTM32H7xx::readValueFast()
{
    return (myPort->IDR);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32H7xx::Id GpioPortSTM32H7xx::getId()
{
    return myId;
}

//------------------------------------------------------------------------------
GPIO_TypeDef*& GpioPortSTM32H7xx::getPort()
{
    return myPort;
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPortSTM32H7xx::driverSetEnabled(const bool enabled)
{
    ProcessorSTM32H7xx::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                 enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32H7xx::driverConfigure(
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
GpioPort<uint16_t>::Error GpioPortSTM32H7xx::driverSetValue(
                                                           const std::uint16_t value)
{
    myPort->ODR = (uint16_t) value;
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32H7xx::driverGetValue(uint16_t& value)
{
    value = (myPort->ODR & 0xFFFF);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint16_t>::Error GpioPortSTM32H7xx::driverReadValue(uint16_t& value)
{
    value = (myPort->IDR & 0xFFFF);
    
    return Error(ERROR_CODE_NONE);
}
