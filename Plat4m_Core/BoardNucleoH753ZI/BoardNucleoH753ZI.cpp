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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file BoardNucleoH753ZI.cpp
/// @author Ben Minerd
/// @date 7/24/2023
/// @brief BoardNucleoH753ZI class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/BoardNucleoH753ZI/BoardNucleoH753ZI.h>
#include <Plat4m_Core/MemoryAllocator.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

const Plat4m::VoltageV BoardNucleoH753ZI::myProcessorCoreVoltageV = 3.3;
const uint32_t BoardNucleoH753ZI::myProcessorExternalClockFrequencyHz =
                                                               8000000; // 8 MHz

const GpioPin::Id BoardNucleoH753ZI::myGpioPinIdMap[] =
{
    /// GPIO_PIN_ID_ARDUINO_A0
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_0},
    /// GPIO_PIN_ID_ARDUINO_A1
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_1},
    /// GPIO_PIN_ID_ARDUINO_A2
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_4},
    /// GPIO_PIN_ID_ARDUINO_A3
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_0},
    /// GPIO_PIN_ID_ARDUINO_A4
    {GpioPortSTM32H7xx::ID_C, GpioPinSTM32H7xx::ID_1},
    /// GPIO_PIN_ID_ARDUINO_A5
    {GpioPortSTM32H7xx::ID_C, GpioPinSTM32H7xx::ID_0},
    /// GPIO_PIN_ID_ARDUINO_D0, GPIO_PIN_ID_UART_RX
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_7},
    /// GPIO_PIN_ID_ARDUINO_D1, GPIO_PIN_ID_UART_TX
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_6},
    /// GPIO_PIN_ID_ARDUINO_D2
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_10},
    /// GPIO_PIN_ID_ARDUINO_D3
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_3},
    /// GPIO_PIN_ID_ARDUINO_D4
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_5},
    /// GPIO_PIN_ID_ARDUINO_D5
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_4},
    /// GPIO_PIN_ID_ARDUINO_D6
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_10},
    /// GPIO_PIN_ID_ARDUINO_D7
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_8},
    /// GPIO_PIN_ID_ARDUINO_D8
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_9},
    /// GPIO_PIN_ID_ARDUINO_D9
    {GpioPortSTM32H7xx::ID_C, GpioPinSTM32H7xx::ID_7},
    /// GPIO_PIN_ID_ARDUINO_D10
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_6},
    /// GPIO_PIN_ID_ARDUINO_D11
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_7},
    /// GPIO_PIN_ID_ARDUINO_D12
    {GpioPortSTM32H7xx::ID_A, GpioPinSTM32H7xx::ID_6},
    /// GPIO_PIN_ID_ARDUINO_D13, GPIO_PIN_ID_USER_LED
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_0},
    /// GPIO_PIN_ID_ARDUINO_D14, GPIO_PIN_ID_I2C_SDA
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_9},
    /// GPIO_PIN_ID_ARDUINO_D15, GPIO_PIN_ID_I2C_SCL
    {GpioPortSTM32H7xx::ID_B, GpioPinSTM32H7xx::ID_8},
    /// GPIO_PIN_ID_USER_BUTTON
    {GpioPortSTM32H7xx::ID_C, GpioPinSTM32H7xx::ID_13},
    /// GPIO_PIN_ID_VCOM_UART_TX
    {GpioPortSTM32H7xx::ID_D, GpioPinSTM32H7xx::ID_8},
    /// GPIO_PIN_ID_VCOM_UART_RX
    {GpioPortSTM32H7xx::ID_D, GpioPinSTM32H7xx::ID_9},
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardNucleoH753ZI::BoardNucleoH753ZI() :
    Board(),
    myUserButton(0),
    myUserButtonEnableLine(0),
    myGpioPorts(),
    myGpioPins(),
    myProcessor(0),
    myUart(0)
{
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardNucleoH753ZI::~BoardNucleoH753ZI()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Button& BoardNucleoH753ZI::getButton()
{
    if (Plat4m::isNullPointer(myUserButton))
    {
        myUserButton = MemoryAllocator::allocate<Button>(getEnableLine());
    }

    return (*myUserButton);
}

//------------------------------------------------------------------------------
EnableLine& BoardNucleoH753ZI::getEnableLine()
{
    if (Plat4m::isNullPointer(myUserButtonEnableLine))
    {
        myUserButtonEnableLine =
            MemoryAllocator::allocate<EnableLine>(
                                            EnableLine::MODE_INPUT,
                                            EnableLine::ACTIVE_LEVEL_HIGH,
                                            getGpioPin(GPIO_PIN_ID_USER_BUTTON),
                                            false);
    }

    return (*myUserButtonEnableLine);
}

//------------------------------------------------------------------------------
GpioPortSTM32H7xx& BoardNucleoH753ZI::getGpioPort(
                                             const GpioPortSTM32H7xx::Id portId)
{
    if (isNullPointer(myGpioPorts[portId]))
    {
        myGpioPorts[portId] =
                           MemoryAllocator::allocate<GpioPortSTM32H7xx>(portId);
    }

    return (*(myGpioPorts[portId]));
}

//------------------------------------------------------------------------------
GpioPinSTM32H7xx& BoardNucleoH753ZI::getGpioPin(const GpioPinId gpioPinId)
{
    GpioPortSTM32H7xx& gpioPort =
        getGpioPort((GpioPortSTM32H7xx::Id) (myGpioPinIdMap[gpioPinId].portId));

    if (isNullPointer(myGpioPins[gpioPinId]))
    {
        myGpioPins[gpioPinId] =
            MemoryAllocator::allocate<GpioPinSTM32H7xx>(
                      gpioPort,
                      (GpioPinSTM32H7xx::Id) (myGpioPinIdMap[gpioPinId].pinId));
    }

    return (*(myGpioPins[gpioPinId]));
}

//------------------------------------------------------------------------------
ProcessorSTM32H7xx& BoardNucleoH753ZI::getProcessor()
{
    if (isNullPointer(myProcessor))
    {
        myProcessor =
            MemoryAllocator::allocate<ProcessorSTM32H7xx>(
                                           myProcessorCoreVoltageV,
                                           myProcessorExternalClockFrequencyHz);
    }

    return (*myProcessor);
}

//------------------------------------------------------------------------------
UartSTM32H7xx& BoardNucleoH753ZI::getUart()
{
    if (isNullPointer(myUart))
    {
        GpioPinSTM32H7xx& uart3Tx = getGpioPin(GPIO_PIN_ID_VCOM_UART_TX);
        GpioPinSTM32H7xx& uart3Rx = getGpioPin(GPIO_PIN_ID_VCOM_UART_RX);

        myUart = MemoryAllocator::allocate<UartSTM32H7xx>(UartSTM32H7xx::ID_3,
                                                          uart3Tx,
                                                          uart3Rx);
    }

    return (*myUart);
}
