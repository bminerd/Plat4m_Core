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
// Copyright (c) 2021 Benjamin Minerd
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
/// @file BoardNucleoF446RE.cpp
/// @author Ben Minerd
/// @date 11/18/2015
/// @brief BoardNucleoF446RE class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/BoardNucleoF446RE/BoardNucleoF446RE.h>
#include <Plat4m_Core/MemoryAllocator.h>

using Plat4m::BoardNucleoF446RE;
using Plat4m::GpioPin;
using Plat4m::GpioSTM32F4xx;
using Plat4m::GpioPinSTM32F4xx;
using Plat4m::Button;
using Plat4m::EnableLine;
using Plat4m::ProcessorSTM32F4xx;
using Plat4m::I2cSTM32F4xx;
using Plat4m::UartSTM32F4xx;

//------------------------------------------------------------------------------
// Private static data member initialization
//------------------------------------------------------------------------------

const float BoardNucleoF446RE::myProcessorCoreVoltage                 = 3.3;
const uint32_t BoardNucleoF446RE::myProcessorExternalClockFrequencyHz
                                                             = 8000000; // 8 MHz

const GpioPin::Id BoardNucleoF446RE::myGpioPinIdMap[] =
{
    /// GPIO_PIN_ID_ARDUINO_A0
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_0},
    /// GPIO_PIN_ID_ARDUINO_A1
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_1},
    /// GPIO_PIN_ID_ARDUINO_A2
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_4},
    /// GPIO_PIN_ID_ARDUINO_A3
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_0},
    /// GPIO_PIN_ID_ARDUINO_A4
    {GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_1},
    /// GPIO_PIN_ID_ARDUINO_A5
    {GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_0},
    /// GPIO_PIN_ID_ARDUINO_D0, GPIO_PIN_ID_UART_RX
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_3},
    /// GPIO_PIN_ID_ARDUINO_D1, GPIO_PIN_ID_UART_TX
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_2},
    /// GPIO_PIN_ID_ARDUINO_D2
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_10},
    /// GPIO_PIN_ID_ARDUINO_D3
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_3},
    /// GPIO_PIN_ID_ARDUINO_D4
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_5},
    /// GPIO_PIN_ID_ARDUINO_D5
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_4},
    /// GPIO_PIN_ID_ARDUINO_D6
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_10},
    /// GPIO_PIN_ID_ARDUINO_D7
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_8},
    /// GPIO_PIN_ID_ARDUINO_D8
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_9},
    /// GPIO_PIN_ID_ARDUINO_D9
    {GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_7},
    /// GPIO_PIN_ID_ARDUINO_D10
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_6},
    /// GPIO_PIN_ID_ARDUINO_D11
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_7},
    /// GPIO_PIN_ID_ARDUINO_D12
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_6},
    /// GPIO_PIN_ID_ARDUINO_D13, GPIO_PIN_ID_USER_LED
    {GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_5},
    /// GPIO_PIN_ID_ARDUINO_D14, GPIO_PIN_ID_I2C_SDA
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_9},
    /// GPIO_PIN_ID_ARDUINO_D15
    {GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_8},
    /// GPIO_PIN_ID_USER_BUTTON
    {GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_13},
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardNucleoF446RE::BoardNucleoF446RE() :
    Board(),
    myUserButton(0),
	myUserButtonEnableLine(0),
	myGpioPins(),
	myI2c(0),
	myProcessor(0),
	myUart(0)
{
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardNucleoF446RE::~BoardNucleoF446RE()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Button& BoardNucleoF446RE::getButton()
{
    if (isNullPointer(myUserButton))
    {
        myUserButton = MemoryAllocator::allocate<Button>(getEnableLine());
    }

    return (*myUserButton);
}

//------------------------------------------------------------------------------
EnableLine& BoardNucleoF446RE::getEnableLine()
{
    if (isNullPointer(myUserButtonEnableLine))
    {
        myUserButtonEnableLine = MemoryAllocator::allocate<EnableLine>(
                                            EnableLine::MODE_INPUT,
                                            EnableLine::ACTIVE_LEVEL_HIGH,
                                            getGpioPin(GPIO_PIN_ID_USER_BUTTON),
                                            false);
    }

    return (*myUserButtonEnableLine);
}

//------------------------------------------------------------------------------
GpioPinSTM32F4xx& BoardNucleoF446RE::getGpioPin(const GpioPinId gpioPinId)
{
    if (isNullPointer(myGpioPins[gpioPinId]))
    {
        myGpioPins[gpioPinId] = MemoryAllocator::allocate<GpioPinSTM32F4xx>(
                                                     myGpioPinIdMap[gpioPinId]);
    }

    return (*(myGpioPins[gpioPinId]));
}

//------------------------------------------------------------------------------
ProcessorSTM32F4xx& BoardNucleoF446RE::getProcessor()
{
    if (isNullPointer(myProcessor))
    {
        myProcessor = MemoryAllocator::allocate<ProcessorSTM32F4xx>(
                                           myProcessorCoreVoltage,
                                           myProcessorExternalClockFrequencyHz);
    }

    return (*myProcessor);
}

//------------------------------------------------------------------------------
I2cSTM32F4xx& BoardNucleoF446RE::getI2c()
{
    if (isNullPointer(myI2c))
    {
        GpioPinSTM32F4xx& i2c1Scl = getGpioPin(GPIO_PIN_ID_I2C_SCL);
        GpioPinSTM32F4xx& i2c1Sda = getGpioPin(GPIO_PIN_ID_I2C_SDA);

        myI2c = MemoryAllocator::allocate<I2cSTM32F4xx>(I2cSTM32F4xx::ID_1,
                                                        i2c1Scl,
                                                        i2c1Sda);
    }

    return (*myI2c);
}

//------------------------------------------------------------------------------
UartSTM32F4xx& BoardNucleoF446RE::getUart()
{
    if (isNullPointer(myUart))
    {
        GpioPinSTM32F4xx& uart2Tx = getGpioPin(GPIO_PIN_ID_UART_TX);
        GpioPinSTM32F4xx& uart2Rx = getGpioPin(GPIO_PIN_ID_UART_RX);

        myUart = MemoryAllocator::allocate<UartSTM32F4xx(UartSTM32F4xx::ID_2, uart2Tx, uart2Rx);
    }

	return (*myUart);
}
