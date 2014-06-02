/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file Stm32f40x.h
 * @author Ben Minerd
 * @date 4/9/14
 * @brief Stm32f40x namespace.
 */

#ifndef _STM32F40X_H_
#define _STM32F40X_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

namespace Stm32f40x
{
    /*--------------------------------------------------------------------------
     * Structures
     *------------------------------------------------------------------------*/
    
    namespace Gpio
    {
        /**
         * @brief Enumeration of GPIO ports.
         */
        enum PortId
        {
            PORT_ID_A = 0,
            PORT_ID_B,
            PORT_ID_C,
            PORT_ID_D,
            PORT_ID_E,
            PORT_ID_F,
            PORT_ID_G,
            PORT_ID_H,
            PORT_ID_I
        };
    
        /**
         * @brief Enumeration of GPIO pins.
         */
        enum PinId
        {
            PIN_ID_0 = 0,
            PIN_ID_1,
            PIN_ID_2,
            PIN_ID_3,
            PIN_ID_4,
            PIN_ID_5,
            PIN_ID_6,
            PIN_ID_7,
            PIN_ID_8,
            PIN_ID_9,
            PIN_ID_10,
            PIN_ID_11,
            PIN_ID_12,
            PIN_ID_13,
            PIN_ID_14,
            PIN_ID_15
        };

    /**
     * @brief Enumeration of GPIO alternate functions.
     */
    enum AlternateFunction
    {
        ALTERNATE_FUNCTION_0            = 0,
        ALTERNATE_FUNCTION_RTC_50Hz     = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_MCO          = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_TAMPER       = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_SWJ          = ALTERNATE_FUNCTION_0,
        ALTERNATE_FUNCTION_TRACE        = ALTERNATE_FUNCTION_0,

        ALTERNATE_FUNCTION_1            = 1,
        ALTERNATE_FUNCTION_TIM1         = ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_TIM2         = ALTERNATE_FUNCTION_1,

        ALTERNATE_FUNCTION_2            = 2,
        ALTERNATE_FUNCTION_TIM3         = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM4         = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM5         = ALTERNATE_FUNCTION_2,

        ALTERNATE_FUNCTION_3            = 3,
        ALTERNATE_FUNCTION_TIM8         = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_TIM9         = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_TIM10        = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_TIM11        = ALTERNATE_FUNCTION_3,

        ALTERNATE_FUNCTION_4            = 4,
        ALTERNATE_FUNCTION_I2C1         = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_I2C2         = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_I2C3         = ALTERNATE_FUNCTION_4,

        ALTERNATE_FUNCTION_5            = 5,
        ALTERNATE_FUNCTION_SPI1         = ALTERNATE_FUNCTION_5,
        ALTERNATE_FUNCTION_SPI2         = ALTERNATE_FUNCTION_5,

        ALTERNATE_FUNCTION_6            = 6,
        ALTERNATE_FUNCTION_SPI3         = ALTERNATE_FUNCTION_6,

        ALTERNATE_FUNCTION_7            = 7,
        ALTERNATE_FUNCTION_USART1       = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_USART2       = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_USART3       = ALTERNATE_FUNCTION_7,

        ALTERNATE_FUNCTION_8            = 8,
        ALTERNATE_FUNCTION_UART4        = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_UART5        = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_USART6       = ALTERNATE_FUNCTION_8,

        ALTERNATE_FUNCTION_9            = 9,
        ALTERNATE_FUNCTION_CAN1         = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_CAN2         = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM12        = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM13        = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM14        = ALTERNATE_FUNCTION_9,

        ALTERNATE_FUNCTION_10           = 10,
        ALTERNATE_FUNCTION_OTG_FS       = ALTERNATE_FUNCTION_10,
        ALTERNATE_FUNCTION_OTG_HS       = ALTERNATE_FUNCTION_10,

        ALTERNATE_FUNCTION_11           = 11,
        ALTERNATE_FUNCTION_ETH          = ALTERNATE_FUNCTION_11,

        ALTERNATE_FUNCTION_12           = 12,
        ALTERNATE_FUNCTION_FSMC         = ALTERNATE_FUNCTION_12,
        ALTERNATE_FUNCTION_OTG_HS_FS    = ALTERNATE_FUNCTION_12,
        ALTERNATE_FUNCTION_SDIO         = ALTERNATE_FUNCTION_12,

        ALTERNATE_FUNCTION_13           = 13,
        ALTERNATE_FUNCTION_DCMI         = ALTERNATE_FUNCTION_13,

        ALTERNATE_FUNCTION_15           = 15,
        ALTERNATE_FUNCTION_EVENTOUT     = ALTERNATE_FUNCTION_15
    };
    
    enum OutputType
    {
        OUTPUT_TYPE_PUSH_PULL   = GPIO_OType_PP,
        OUTPUT_TYPE_OPEN_DRAIN  = GPIO_OType_OD
    };
    
    enum OutputSpeed
    {
        OUTPUT_SPEED_2MHZ   = GPIO_Speed_2MHz,
        OUTPUT_SPEED_25MHZ  = GPIO_Speed_25MHz,
        OUTPUT_SPEED_50MHZ  = GPIO_Speed_50MHz,
        OUTPUT_SPEED_100MHZ = GPIO_Speed_100MHz
    };
    
    /*--------------------------------------------------------------------------
     * Structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        AlternateFunction alternateFunction;
        OutputSpeed outputSpeed;
    };
    
    /*--------------------------------------------------------------------------
     * Variables
     *------------------------------------------------------------------------*/
    
    static const char* portIdStringMap[GPIO_STM32F4XX_PORT_COUNT] =
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
    
    static const char* pinIdStringMap[GPIO_STM32F4XX_PIN_COUNT] =
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
    
    static GPIO_TypeDef* portMap[GPIO_STM32F4XX_PORT_COUNT] =
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
    
    static const uint32_t clockMap[GPIO_STM32F4XX_PORT_COUNT] =
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
    
    static const GPIOMode_TypeDef modeMap[] =
    {
        GPIO_Mode_OUT,  /// GPIO_MODE_DIGITAL_OUTPUT
        GPIO_Mode_IN,   /// GPIO_MODE_DIGITAL_INPUT
        GPIO_Mode_AN,   /// GPIO_MODE_ANALOG
        GPIO_Mode_AF    /// GPIO_MODE_ALTERNATE_FUNCTION
    };

    static const GPIOPuPd_TypeDef resistorMap[] =
    {
        GPIO_PuPd_NOPULL,   /// GPIO_RESISTOR_NONE
        GPIO_PuPd_UP,       /// GPIO_RESISTOR_PULL_UP
        GPIO_PuPd_DOWN      /// GPIO_RESISTOR_PULL_DOWN
    };
    
    static const uint16_t pinMap[] =
    {
        GPIO_Pin_0,     /// PIN_ID_0
        GPIO_Pin_1,     /// PIN_ID_1
        GPIO_Pin_2,     /// PIN_ID_2
        GPIO_Pin_3,     /// PIN_ID_3
        GPIO_Pin_4,     /// PIN_ID_4
        GPIO_Pin_5,     /// PIN_ID_5
        GPIO_Pin_6,     /// PIN_ID_6
        GPIO_Pin_7,     /// PIN_ID_7
        GPIO_Pin_8,     /// PIN_ID_8
        GPIO_Pin_9,     /// PIN_ID_9
        GPIO_Pin_10,    /// PIN_ID_10
        GPIO_Pin_11,    /// PIN_ID_11
        GPIO_Pin_12,    /// PIN_ID_12
        GPIO_Pin_13,    /// PIN_ID_13
        GPIO_Pin_14,    /// PIN_ID_14
        GPIO_Pin_15     /// PIN_ID_15
    };
};

#endif // _STM32F40X_H_