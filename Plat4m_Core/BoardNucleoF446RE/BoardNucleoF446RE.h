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
/// @file BoardNucleoF446RE.h
/// @author Ben Minerd
/// @date 11/18/2015
/// @brief BoardNucleoF446RE class.
///

#ifndef BOARD_NUCLEO_F446RE_H
#define BOARD_NUCLEO_F446RE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/Board.h>
#include <Plat4m_Core/STM32F4xx/ProcessorSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/GpioPinSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/I2cSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/UartSTM32F4xx.h>
#include <Plat4m_Core/Button.h>
#include <Plat4m_Core/EnableLine.h>
#include <Plat4m_Core/GpioPin.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BoardNucleoF446RE : public Board
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum GpioPinId
    {
		GPIO_PIN_ID_ARDUINO_A0,
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
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    BoardNucleoF446RE();
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

	~BoardNucleoF446RE();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Button& getButton();

    EnableLine& getEnableLine();

    GpioPinSTM32F4xx& getGpioPin(const GpioPinId gpioPinId);
    
    ProcessorSTM32F4xx& getProcessor();
    
    I2cSTM32F4xx& getI2c();

    UartSTM32F4xx& getUart();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const float myProcessorCoreVoltage;

    static const uint32_t myProcessorExternalClockFrequencyHz;

    static const GpioPin::Id myGpioPinIdMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Button* myUserButton;

    EnableLine* myUserButtonEnableLine;

    GpioPinSTM32F4xx* myGpioPins[23];

    I2cSTM32F4xx* myI2c;

    ProcessorSTM32F4xx* myProcessor;

    UartSTM32F4xx* myUart;
};

}; // namespace Plat4m

#endif // BOARD_NUCLEO_F446RE_H
