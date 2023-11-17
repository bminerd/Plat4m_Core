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
/// @file BoardNucleoH753ZI.h
/// @author Ben Minerd
/// @date 7/24/2023
/// @brief BoardNucleoH753ZI class header file.
///

#ifndef PLAT4M_BOARD_NUCLEO_H753ZI_H
#define PLAT4M_BOARD_NUCLEO_H753ZI_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Board.h>
#include <Plat4m_Core/STM32H7xx/ProcessorSTM32H7xx.h>
#include <Plat4m_Core/STM32H7xx/GpioPortSTM32H7xx.h>
#include <Plat4m_Core/STM32H7xx/GpioPinSTM32H7xx.h>
#include <Plat4m_Core/STM32H7xx/UartSTM32H7xx.h>
#include <Plat4m_Core/Button.h>
#include <Plat4m_Core/EnableLine.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BoardNucleoH753ZI : public Board
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum GpioPinId
    {
        GPIO_PIN_ID_ARDUINO_A0 = 0,
        GPIO_PIN_ID_ARDUINO_A1,
        GPIO_PIN_ID_ARDUINO_A2,
        GPIO_PIN_ID_ARDUINO_A3,
        GPIO_PIN_ID_ARDUINO_A4,
        GPIO_PIN_ID_ARDUINO_A5,
        GPIO_PIN_ID_ARDUINO_D0,
        GPIO_PIN_ID_UART_RX = GPIO_PIN_ID_ARDUINO_D0,
        GPIO_PIN_ID_ARDUINO_D1,
        GPIO_PIN_ID_UART_TX = GPIO_PIN_ID_ARDUINO_D1,
        GPIO_PIN_ID_ARDUINO_D2,
        GPIO_PIN_ID_ARDUINO_D3,
        GPIO_PIN_ID_ARDUINO_D4,
        GPIO_PIN_ID_ARDUINO_D5,
        GPIO_PIN_ID_ARDUINO_D6,
        GPIO_PIN_ID_ARDUINO_D7,
        GPIO_PIN_ID_ARDUINO_D8,
        GPIO_PIN_ID_ARDUINO_D9,
        GPIO_PIN_ID_ARDUINO_D10,
        GPIO_PIN_ID_ARDUINO_D11,
        GPIO_PIN_ID_ARDUINO_D12,
        GPIO_PIN_ID_ARDUINO_D13,
        GPIO_PIN_ID_USER_LED = GPIO_PIN_ID_ARDUINO_D13,
        GPIO_PIN_ID_ARDUINO_D14,
        GPIO_PIN_ID_I2C_SDA = GPIO_PIN_ID_ARDUINO_D14,
        GPIO_PIN_ID_ARDUINO_D15,
        GPIO_PIN_ID_I2C_SCL = GPIO_PIN_ID_ARDUINO_D15,
        GPIO_PIN_ID_USER_BUTTON,
        GPIO_PIN_ID_VCOM_UART_TX,
        GPIO_PIN_ID_VCOM_UART_RX
    };

    enum UartId
    {
        UART_ID_VCOM = 0
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    BoardNucleoH753ZI();

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    ~BoardNucleoH753ZI();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Button& getButton();

    EnableLine& getEnableLine();

    GpioPortSTM32H7xx& getGpioPort(const GpioPortSTM32H7xx::Id portId);

    GpioPinSTM32H7xx& getGpioPin(const GpioPinId gpioPinId);

    ProcessorSTM32H7xx& getProcessor();

    UartSTM32H7xx& getUart();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const VoltageV myProcessorCoreVoltageV;

    static const std::uint32_t myProcessorExternalClockFrequencyHz;

    static const GpioPin::Id myGpioPinIdMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Button* myUserButton;

    EnableLine* myUserButtonEnableLine;

    GpioPortSTM32H7xx* myGpioPorts[11];

    GpioPinSTM32H7xx* myGpioPins[25];

    ProcessorSTM32H7xx* myProcessor;

    UartSTM32H7xx* myUart;
};

}; // namespace Plat4m

#endif // PLAT4M_BOARD_NUCLEO_H753ZI_H
