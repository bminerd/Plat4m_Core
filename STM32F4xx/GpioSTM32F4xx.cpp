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
/// @file GpioSTM32F4xx.h
/// @author Ben Minerd
/// @date 4/9/14
/// @brief GpioSTM32F4xx class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <GpioPortSTM32F4xx.h>
#include <GpioPinSTM32F4xx.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

using Plat4m::GpioSTM32F4xx;
using Plat4m::GpioPortSTM32F4xx;
using Plat4m::GpioPinSTM32F4xx;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const char* GpioSTM32F4xx::portIdStringMap[] =
{
    "A",    /// PORT_ID_A
    "B",    /// PORT_ID_B
    "C",    /// PORT_ID_C
    "D",    /// PORT_ID_D
    "E",    /// PORT_ID_E
    "F",    /// PORT_ID_F
    "G",    /// PORT_ID_G
    "H",    /// PORT_ID_H
    "I"     /// PORT_ID_I
};
    
const char* GpioSTM32F4xx::pinIdStringMap[] =
{
    "0",    /// PIN_ID_0
    "1",    /// PIN_ID_1
    "2",    /// PIN_ID_2
    "3",    /// PIN_ID_3
    "4",    /// PIN_ID_4
    "5",    /// PIN_ID_5
    "6",    /// PIN_ID_6
    "7",    /// PIN_ID_7
    "8",    /// PIN_ID_8
    "9",    /// PIN_ID_9
    "10",   /// PIN_ID_10
    "11",   /// PIN_ID_11
    "12",   /// PIN_ID_12
    "13",   /// PIN_ID_13
    "14",   /// PIN_ID_14
    "15"    /// PIN_ID_15
};

GPIO_TypeDef* GpioSTM32F4xx::portMap[] =
{
    GPIOA,  /// PORT_ID_A
    GPIOB,  /// PORT_ID_B
    GPIOC,  /// PORT_ID_C
    GPIOD,  /// PORT_ID_D
    GPIOE,  /// PORT_ID_E
    GPIOF,  /// PORT_ID_F
    GPIOG,  /// PORT_ID_G
    GPIOH,  /// PORT_ID_H
    GPIOI   /// PORT_ID_I
};

const uint32_t GpioSTM32F4xx::clockMap[] =
{
    RCC_AHB1Periph_GPIOA,   /// PORT_ID_A
    RCC_AHB1Periph_GPIOB,   /// PORT_ID_B
    RCC_AHB1Periph_GPIOC,   /// PORT_ID_C
    RCC_AHB1Periph_GPIOD,   /// PORT_ID_D
    RCC_AHB1Periph_GPIOE,   /// PORT_ID_E
    RCC_AHB1Periph_GPIOF,   /// PORT_ID_F
    RCC_AHB1Periph_GPIOG,   /// PORT_ID_G
    RCC_AHB1Periph_GPIOH,   /// PORT_ID_H
    RCC_AHB1Periph_GPIOI    /// PORT_ID_I
};

const GPIOMode_TypeDef GpioSTM32F4xx::modeMap[] =
{
    GPIO_Mode_OUT,  /// MODE_DIGITAL_OUTPUT_PUSH_PULL
    GPIO_Mode_OUT,  /// MODE_DIGITAL_OUTPUT_OPEN_DRAIN
    GPIO_Mode_IN,   /// MODE_DIGITAL_INPUT
    GPIO_Mode_AN,   /// MODE_ANALOG
    GPIO_Mode_AF    /// MODE_ALTERNATE_FUNCTION
};

const GPIOPuPd_TypeDef GpioSTM32F4xx::resistorMap[] =
{
    GPIO_PuPd_NOPULL,   /// GPIO_RESISTOR_NONE
    GPIO_PuPd_UP,       /// GPIO_RESISTOR_PULL_UP
    GPIO_PuPd_DOWN      /// GPIO_RESISTOR_PULL_DOWN
};

GpioPortSTM32F4xx* GpioSTM32F4xx::portObjectMap[9];

GpioPinSTM32F4xx* GpioSTM32F4xx::pinObjectMap[9][16];
