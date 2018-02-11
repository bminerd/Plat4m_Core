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
/// @file BoardNucleoF303RE.h
/// @author Ben Minerd
/// @date 2/5/2016
/// @brief BoardNucleoF303RE class header file.
///

#ifndef PLAT4M_BOARD_NUCLEO_F303RE_H
#define PLAT4M_BOARD_NUCLEO_F303RE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Board.h>
#include <Plat4m_Core/STM32F30x/ProcessorSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/GpioPortSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/GpioPinSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/I2cSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/UartSTM32F30x.h>
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

class BoardNucleoF303RE : public Board
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
        GPIO_PIN_ID_USER_BUTTON
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    BoardNucleoF303RE();
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

	~BoardNucleoF303RE();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Button& getButton();

    EnableLine& getEnableLine();

    GpioPortSTM32F30x& getGpioPort(const GpioPortSTM32F30x::Id portId);

    GpioPinSTM32F30x& getGpioPin(const GpioPinId gpioPinId);
    
    ProcessorSTM32F30x& getProcessor();
    
    I2cSTM32F30x& getI2c();

    UartSTM32F30x& getUart();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const VoltageV myProcessorCoreVoltageV;

    static const uint32_t myProcessorExternalClockFrequencyHz;

    static const GpioPin::Id myGpioPinIdMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Button* myUserButton;

    EnableLine* myUserButtonEnableLine;

    GpioPortSTM32F30x* myGpioPorts[3];

    GpioPinSTM32F30x* myGpioPins[23];

    ProcessorSTM32F30x* myProcessor;

    I2cSTM32F30x* myI2c;

    UartSTM32F30x* myUart;
};

}; // namespace Plat4m

#endif // PLAT4M_BOARD_NUCLEO_F303RE_H
