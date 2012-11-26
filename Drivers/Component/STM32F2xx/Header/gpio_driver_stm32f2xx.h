/*------------------------------------------------------------------------------
 *                  __                           ___
 *                 || |             __          //  |
 *       _______   || |   _______  || |__      //   |    _____  ___
 *      ||  ___ \  || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      || |  || | || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |__|| | || | || |__|| | || |     // /_|| |_  || | || | || |
 *      ||  ____/  || |  \\____  | || |__  //_____   _| || | || | || |
 *      || |       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *      || |
 *      ||_|
 *
 * Copyright (c) 2012 Ben Minerd. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *----------------------------------------------------------------------------*/

/**
 * @file gpio_driver_stm32f2xx.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief GPIO driver layer for the STM32F2xx.
 */

#ifndef _GPIO_DRIVER_STM32F2XX_H_
#define _GPIO_DRIVER_STM32F2XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of GPIO ports.
 */
typedef enum _gpio_port_e_
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H,
    GPIO_PORT_I,

    // Do not place values below!
    GPIO_PORT_COUNT
} gpio_port_e;

/**
 * @brief Enumeration of GPIO pins.
 */
typedef enum _gpio_pin_e_
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    
    // Do not place values below!
    GPIO_PIN_COUNT
} gpio_pin_e;

/**
 * @brief Enumeration of GPIO alternate functions.
 */
typedef enum _gpio_alternate_function_e_
{
    GPIO_ALTERNATE_FUNCTION_NONE        = -1,

    GPIO_ALTERNATE_FUNCTION_0           = 0,
    GPIO_ALTERNATE_FUNCTION_RTC_50Hz    = GPIO_ALTERNATE_FUNCTION_0,
    GPIO_ALTERNATE_FUNCTION_MCO         = GPIO_ALTERNATE_FUNCTION_0,
    GPIO_ALTERNATE_FUNCTION_TAMPER      = GPIO_ALTERNATE_FUNCTION_0,
    GPIO_ALTERNATE_FUNCTION_SWJ         = GPIO_ALTERNATE_FUNCTION_0,
    GPIO_ALTERNATE_FUNCTION_TRACE       = GPIO_ALTERNATE_FUNCTION_0,

    GPIO_ALTERNATE_FUNCTION_1           = 1,
    GPIO_ALTERNATE_FUNCTION_TIM1        = GPIO_ALTERNATE_FUNCTION_1,
    GPIO_ALTERNATE_FUNCTION_TIM2        = GPIO_ALTERNATE_FUNCTION_1,

    GPIO_ALTERNATE_FUNCTION_2           = 2,
    GPIO_ALTERNATE_FUNCTION_TIM3        = GPIO_ALTERNATE_FUNCTION_2,
    GPIO_ALTERNATE_FUNCTION_TIM4        = GPIO_ALTERNATE_FUNCTION_2,
    GPIO_ALTERNATE_FUNCTION_TIM5        = GPIO_ALTERNATE_FUNCTION_2,

    GPIO_ALTERNATE_FUNCTION_3           = 3,
    GPIO_ALTERNATE_FUNCTION_TIM8        = GPIO_ALTERNATE_FUNCTION_3,
    GPIO_ALTERNATE_FUNCTION_TIM9        = GPIO_ALTERNATE_FUNCTION_3,
    GPIO_ALTERNATE_FUNCTION_TIM10       = GPIO_ALTERNATE_FUNCTION_3,
    GPIO_ALTERNATE_FUNCTION_TIM11       = GPIO_ALTERNATE_FUNCTION_3,

    GPIO_ALTERNATE_FUNCTION_4           = 4,
    GPIO_ALTERNATE_FUNCTION_I2C1        = GPIO_ALTERNATE_FUNCTION_4,
    GPIO_ALTERNATE_FUNCTION_I2C2        = GPIO_ALTERNATE_FUNCTION_4,
    GPIO_ALTERNATE_FUNCTION_I2C3        = GPIO_ALTERNATE_FUNCTION_4,

    GPIO_ALTERNATE_FUNCTION_5           = 5,
    GPIO_ALTERNATE_FUNCTION_SPI1        = GPIO_ALTERNATE_FUNCTION_5,
    GPIO_ALTERNATE_FUNCTION_SPI2        = GPIO_ALTERNATE_FUNCTION_5,

    GPIO_ALTERNATE_FUNCTION_6           = 6,
    GPIO_ALTERNATE_FUNCTION_SPI3        = GPIO_ALTERNATE_FUNCTION_6,

    GPIO_ALTERNATE_FUNCTION_7           = 7,
    GPIO_ALTERNATE_FUNCTION_USART1      = GPIO_ALTERNATE_FUNCTION_7,
    GPIO_ALTERNATE_FUNCTION_USART2      = GPIO_ALTERNATE_FUNCTION_7,
    GPIO_ALTERNATE_FUNCTION_USART3      = GPIO_ALTERNATE_FUNCTION_7,

    GPIO_ALTERNATE_FUNCTION_8           = 8,
    GPIO_ALTERNATE_FUNCTION_UART4       = GPIO_ALTERNATE_FUNCTION_8,
    GPIO_ALTERNATE_FUNCTION_UART5       = GPIO_ALTERNATE_FUNCTION_8,
    GPIO_ALTERNATE_FUNCTION_USART6      = GPIO_ALTERNATE_FUNCTION_8,

    GPIO_ALTERNATE_FUNCTION_9           = 9,
    GPIO_ALTERNATE_FUNCTION_CAN1        = GPIO_ALTERNATE_FUNCTION_9,
    GPIO_ALTERNATE_FUNCTION_CAN2        = GPIO_ALTERNATE_FUNCTION_9,
    GPIO_ALTERNATE_FUNCTION_TIM12       = GPIO_ALTERNATE_FUNCTION_9,
    GPIO_ALTERNATE_FUNCTION_TIM13       = GPIO_ALTERNATE_FUNCTION_9,
    GPIO_ALTERNATE_FUNCTION_TIM14       = GPIO_ALTERNATE_FUNCTION_9,

    GPIO_ALTERNATE_FUNCTION_10          = 10,
    GPIO_ALTERNATE_FUNCTION_OTG_FS      = GPIO_ALTERNATE_FUNCTION_10,
    GPIO_ALTERNATE_FUNCTION_OTG_HS      = GPIO_ALTERNATE_FUNCTION_10,

    GPIO_ALTERNATE_FUNCTION_11          = 11,
    GPIO_ALTERNATE_FUNCTION_ETH         = GPIO_ALTERNATE_FUNCTION_11,

    GPIO_ALTERNATE_FUNCTION_12          = 12,
    GPIO_ALTERNATE_FUNCTION_FSMC        = GPIO_ALTERNATE_FUNCTION_12,
    GPIO_ALTERNATE_FUNCTION_OTG_HS_FS   = GPIO_ALTERNATE_FUNCTION_12,
    GPIO_ALTERNATE_FUNCTION_SDIO        = GPIO_ALTERNATE_FUNCTION_12,

    GPIO_ALTERNATE_FUNCTION_13          = 13,
    GPIO_ALTERNATE_FUNCTION_DCMI        = GPIO_ALTERNATE_FUNCTION_13,

    GPIO_ALTERNATE_FUNCTION_15          = 15,
    GPIO_ALTERNATE_FUNCTION_EVENTOUT    = GPIO_ALTERNATE_FUNCTION_15
} gpio_alternate_function_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the GPIO driver level.
 */
void gpioDriverStm32f2xxInit(void);

#endif // _GPIO_DRIVER_STM32F2XX_H_
