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
 * Copyright (c) 2013 Benjamin Minerd
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
 * @file GpioStm32f4xx.h
 * @author Ben Minerd
 * @date 3/22/13
 * @brief GpioStm32f4xx class.
 */

#ifndef _GPIO_STM32F4XX_H_
#define _GPIO_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define GPIO_STM32F4XX_PORT_COUNT   9
#define GPIO_STM32F4XX_PIN_COUNT    16

/*------------------------------------------------------------------------------
 * Forward class declarations
 *----------------------------------------------------------------------------*/

class GpioPinStm32f4xx;
class GpioPortStm32f4xx;

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class GpioStm32f4xx
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    friend class GpioPortStm32f4xx;
    friend class GpioPinStm32f4xx;
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
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
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        AlternateFunction alternateFunction;
        OutputSpeed outputSpeed;
    };
    
private:
    
    /*--------------------------------------------------------------------------
     * Private static data members
     *------------------------------------------------------------------------*/
    
    static const char* portIdStringMap[];
    
    static const char* pinIdStringMap[];
    
    static GPIO_TypeDef* portMap[];
    
    static const uint32_t clockMap[];
    
    static const GPIOMode_TypeDef modeMap[];

    static const GPIOPuPd_TypeDef resistorMap[];
    
    static GpioPortStm32f4xx* portObjectMap[];
    
    static GpioPinStm32f4xx* pinObjectMap[][16];
};

#endif // _GPIO_STM32F4XX_H_