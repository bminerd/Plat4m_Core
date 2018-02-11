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
/// @file BoardNucleoF303RE.cpp
/// @author Ben Minerd
/// @date 2/5/2016
/// @brief BoardNucleoF303RE class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/BoardNucleoF303RE/BoardNucleoF303RE.h>

using Plat4m::BoardNucleoF303RE;
using Plat4m::GpioPinSTM32F30x;
using Plat4m::GpioPortSTM32F30x;
using Plat4m::GpioPin;
using Plat4m::Button;
using Plat4m::EnableLine;
using Plat4m::ProcessorSTM32F30x;
using Plat4m::UartSTM32F30x;
using Plat4m::I2cSTM32F30x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

const Plat4m::VoltageV BoardNucleoF303RE::myProcessorCoreVoltageV = 3.3;
const uint32_t BoardNucleoF303RE::myProcessorExternalClockFrequencyHz =
                                                               8000000; // 8 MHz

const GpioPin::Id BoardNucleoF303RE::myGpioPinIdMap[] =
{
    /// GPIO_PIN_ID_ARDUINO_A0
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_0},
    /// GPIO_PIN_ID_ARDUINO_A1
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_1},
    /// GPIO_PIN_ID_ARDUINO_A2
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_4},
    /// GPIO_PIN_ID_ARDUINO_A3
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_0},
    /// GPIO_PIN_ID_ARDUINO_A4
    {GpioPortSTM32F30x::ID_C, GpioPinSTM32F30x::ID_1},
    /// GPIO_PIN_ID_ARDUINO_A5
    {GpioPortSTM32F30x::ID_C, GpioPinSTM32F30x::ID_0},
    /// GPIO_PIN_ID_ARDUINO_D0, GPIO_PIN_ID_UART_RX
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_3},
    /// GPIO_PIN_ID_ARDUINO_D1, GPIO_PIN_ID_UART_TX
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_2},
    /// GPIO_PIN_ID_ARDUINO_D2
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_10},
    /// GPIO_PIN_ID_ARDUINO_D3
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_3},
    /// GPIO_PIN_ID_ARDUINO_D4
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_5},
    /// GPIO_PIN_ID_ARDUINO_D5
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_4},
    /// GPIO_PIN_ID_ARDUINO_D6
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_10},
    /// GPIO_PIN_ID_ARDUINO_D7
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_8},
    /// GPIO_PIN_ID_ARDUINO_D8
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_9},
    /// GPIO_PIN_ID_ARDUINO_D9
    {GpioPortSTM32F30x::ID_C, GpioPinSTM32F30x::ID_7},
    /// GPIO_PIN_ID_ARDUINO_D10
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_6},
    /// GPIO_PIN_ID_ARDUINO_D11
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_7},
    /// GPIO_PIN_ID_ARDUINO_D12
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_6},
    /// GPIO_PIN_ID_ARDUINO_D13, GPIO_PIN_ID_USER_LED
    {GpioPortSTM32F30x::ID_A, GpioPinSTM32F30x::ID_5},
    /// GPIO_PIN_ID_ARDUINO_D14, GPIO_PIN_ID_I2C_SDA
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_9},
    /// GPIO_PIN_ID_ARDUINO_D15
    {GpioPortSTM32F30x::ID_B, GpioPinSTM32F30x::ID_8},
    /// GPIO_PIN_ID_USER_BUTTON
    {GpioPortSTM32F30x::ID_C, GpioPinSTM32F30x::ID_13},
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardNucleoF303RE::BoardNucleoF303RE() :
    Board(),
    myUserButton(0),
	myUserButtonEnableLine(0),
	myGpioPorts(),
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
BoardNucleoF303RE::~BoardNucleoF303RE()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Button& BoardNucleoF303RE::getButton()
{
    if (Plat4m::isNullPointer(myUserButton))
    {
        myUserButton = new Button(getEnableLine());
    }

    return (*myUserButton);
}

//------------------------------------------------------------------------------
EnableLine& BoardNucleoF303RE::getEnableLine()
{
    if (Plat4m::isNullPointer(myUserButtonEnableLine))
    {
        myUserButtonEnableLine = new EnableLine(
                                            EnableLine::MODE_INPUT,
                                            EnableLine::ACTIVE_LEVEL_HIGH,
                                            getGpioPin(GPIO_PIN_ID_USER_BUTTON),
                                            false);
    }

    return (*myUserButtonEnableLine);
}

//------------------------------------------------------------------------------
GpioPortSTM32F30x& BoardNucleoF303RE::getGpioPort(
                                             const GpioPortSTM32F30x::Id portId)
{
    if (isNullPointer(myGpioPorts[portId]))
    {
        myGpioPorts[portId] = new GpioPortSTM32F30x(portId);
    }

    return (*(myGpioPorts[portId]));
}

//------------------------------------------------------------------------------
GpioPinSTM32F30x& BoardNucleoF303RE::getGpioPin(const GpioPinId gpioPinId)
{
    GpioPortSTM32F30x& gpioPort =
        getGpioPort((GpioPortSTM32F30x::Id) (myGpioPinIdMap[gpioPinId].portId));

    if (isNullPointer(myGpioPins[gpioPinId]))
    {
        myGpioPins[gpioPinId] = new GpioPinSTM32F30x(
                      gpioPort,
                      (GpioPinSTM32F30x::Id) (myGpioPinIdMap[gpioPinId].pinId));
    }

    return (*(myGpioPins[gpioPinId]));
}

//------------------------------------------------------------------------------
ProcessorSTM32F30x& BoardNucleoF303RE::getProcessor()
{
    if (isNullPointer(myProcessor))
    {
        myProcessor = new ProcessorSTM32F30x(
                                           myProcessorCoreVoltageV,
                                           myProcessorExternalClockFrequencyHz);
    }

    return (*myProcessor);
}

//------------------------------------------------------------------------------
I2cSTM32F30x& BoardNucleoF303RE::getI2c()
{
    if (isNullPointer(myI2c))
    {
        GpioPinSTM32F30x& i2c1Scl = getGpioPin(GPIO_PIN_ID_I2C_SCL);
        GpioPinSTM32F30x& i2c1Sda = getGpioPin(GPIO_PIN_ID_I2C_SDA);

        myI2c = new I2cSTM32F30x(I2cSTM32F30x::ID_1, i2c1Scl, i2c1Sda);
    }

    return (*myI2c);
}

//------------------------------------------------------------------------------
UartSTM32F30x& BoardNucleoF303RE::getUart()
{
    if (isNullPointer(myUart))
    {
        GpioPinSTM32F30x& uart2Tx = getGpioPin(GPIO_PIN_ID_UART_TX);
        GpioPinSTM32F30x& uart2Rx = getGpioPin(GPIO_PIN_ID_UART_RX);

        myUart = new UartSTM32F30x(UartSTM32F30x::ID_2, uart2Tx, uart2Rx);
    }

    return (*myUart);
}
