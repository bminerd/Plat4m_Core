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
 * @file gpio_driver.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief TODO Comment!
 */

#ifndef _GPIO_DRIVER_H_
#define _GPIO_DRIVER_H_

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
 * @brief Enumeration of GPIO port IDs.
 */
typedef enum _gpio_driver_port_id_e_
{
    // GPIO driver IDs go here
    // ex.
    //      GPIO_DRIVER_PORT_ID_LED_1
    //
    // Note: first GPIO driver ID should be set to 0 (zero).
    // ex.
    //      GPIO_DRIVER_PORT_ID_LED_1 = 0,
    //      GPIO_DRIVER_PORT_ID_LED_2,
    //      ...
    
    GPIO_DRIVER_PORT_ID_A = 0,
    GPIO_DRIVER_PORT_ID_B,
    GPIO_DRIVER_PORT_ID_C,
    GPIO_DRIVER_PORT_ID_D,
    GPIO_DRIVER_PORT_ID_E,
    GPIO_DRIVER_PORT_ID_F,
    GPIO_DRIVER_PORT_ID_G,
    GPIO_DRIVER_PORT_ID_H,
    GPIO_DRIVER_PORT_ID_I,

    // Do not place values below!
    GPIO_DRIVER_PORT_ID_COUNT
} gpio_driver_port_id_e;

/**
 * @brief Enumeration of GPIO pin IDs.
 */
typedef enum _gpio_driver_pin_id_e_
{
    GPIO_DRIVER_PIN_ID_0 = 0,
    GPIO_DRIVER_PIN_ID_1,
    GPIO_DRIVER_PIN_ID_2,
    GPIO_DRIVER_PIN_ID_3,
    GPIO_DRIVER_PIN_ID_4,
    GPIO_DRIVER_PIN_ID_5,
    GPIO_DRIVER_PIN_ID_6,
    GPIO_DRIVER_PIN_ID_7,
    GPIO_DRIVER_PIN_ID_8,
    GPIO_DRIVER_PIN_ID_9,
    GPIO_DRIVER_PIN_ID_10,
    GPIO_DRIVER_PIN_ID_11,
    GPIO_DRIVER_PIN_ID_12,
    GPIO_DRIVER_PIN_ID_13,
    GPIO_DRIVER_PIN_ID_14,
    GPIO_DRIVER_PIN_ID_15,
    
    // Do not place values below!
    GPIO_DRIVER_PIN_ID_COUNT
} gpio_driver_pin_id_e;

/**
 * @brief Enumeration of GPIO alternate functions.
 */
typedef enum _gpio_driver_alternate_function_e_
{
    GPIO_DRIVER_ALTERNATE_FUNCTION_NONE,

    GPIO_DRIVER_ALTERNATE_FUNCTION_0        = 0,
    GPIO_DRIVER_ALTERNATE_FUNCTION_RTC_50Hz = GPIO_DRIVER_ALTERNATE_FUNCTION_0,
    GPIO_DRIVER_ALTERNATE_FUNCTION_MCO      = GPIO_DRIVER_ALTERNATE_FUNCTION_0,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TAMPER   = GPIO_DRIVER_ALTERNATE_FUNCTION_0,
    GPIO_DRIVER_ALTERNATE_FUNCTION_SWJ      = GPIO_DRIVER_ALTERNATE_FUNCTION_0,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TRACE    = GPIO_DRIVER_ALTERNATE_FUNCTION_0,

    GPIO_DRIVER_ALTERNATE_FUNCTION_1        = 1,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM1     = GPIO_DRIVER_ALTERNATE_FUNCTION_1,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM2     = GPIO_DRIVER_ALTERNATE_FUNCTION_1,

    GPIO_DRIVER_ALTERNATE_FUNCTION_2        = 2,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM3     = GPIO_DRIVER_ALTERNATE_FUNCTION_2,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM4     = GPIO_DRIVER_ALTERNATE_FUNCTION_2,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM5     = GPIO_DRIVER_ALTERNATE_FUNCTION_2,

    GPIO_DRIVER_ALTERNATE_FUNCTION_3        = 3,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM8     = GPIO_DRIVER_ALTERNATE_FUNCTION_3,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM9     = GPIO_DRIVER_ALTERNATE_FUNCTION_3,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM10    = GPIO_DRIVER_ALTERNATE_FUNCTION_3,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM11    = GPIO_DRIVER_ALTERNATE_FUNCTION_3,

    GPIO_DRIVER_ALTERNATE_FUNCTION_4        = 4,
    GPIO_DRIVER_ALTERNATE_FUNCTION_I2C1     = GPIO_DRIVER_ALTERNATE_FUNCTION_4,
    GPIO_DRIVER_ALTERNATE_FUNCTION_I2C2     = GPIO_DRIVER_ALTERNATE_FUNCTION_4,
    GPIO_DRIVER_ALTERNATE_FUNCTION_I2C3     = GPIO_DRIVER_ALTERNATE_FUNCTION_4,

    GPIO_DRIVER_ALTERNATE_FUNCTION_5        = 5,
    GPIO_DRIVER_ALTERNATE_FUNCTION_SPI1     = GPIO_DRIVER_ALTERNATE_FUNCTION_5,
    GPIO_DRIVER_ALTERNATE_FUNCTION_SPI2     = GPIO_DRIVER_ALTERNATE_FUNCTION_5,

    GPIO_DRIVER_ALTERNATE_FUNCTION_6        = 6,
    GPIO_DRIVER_ALTERNATE_FUNCTION_SPI3     = GPIO_DRIVER_ALTERNATE_FUNCTION_6,

    GPIO_DRIVER_ALTERNATE_FUNCTION_7        = 7,
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART1   = GPIO_DRIVER_ALTERNATE_FUNCTION_7,
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART2   = GPIO_DRIVER_ALTERNATE_FUNCTION_7,
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART3   = GPIO_DRIVER_ALTERNATE_FUNCTION_7,

    GPIO_DRIVER_ALTERNATE_FUNCTION_8        = 8,
    GPIO_DRIVER_ALTERNATE_FUNCTION_UART4    = GPIO_DRIVER_ALTERNATE_FUNCTION_8,
    GPIO_DRIVER_ALTERNATE_FUNCTION_UART5    = GPIO_DRIVER_ALTERNATE_FUNCTION_8,
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART6   = GPIO_DRIVER_ALTERNATE_FUNCTION_8,

    GPIO_DRIVER_ALTERNATE_FUNCTION_9        = 9,
    GPIO_DRIVER_ALTERNATE_FUNCTION_CAN1     = GPIO_DRIVER_ALTERNATE_FUNCTION_9,
    GPIO_DRIVER_ALTERNATE_FUNCTION_CAN2     = GPIO_DRIVER_ALTERNATE_FUNCTION_9,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM12    = GPIO_DRIVER_ALTERNATE_FUNCTION_9,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM13    = GPIO_DRIVER_ALTERNATE_FUNCTION_9,
    GPIO_DRIVER_ALTERNATE_FUNCTION_TIM14    = GPIO_DRIVER_ALTERNATE_FUNCTION_9,

    GPIO_DRIVER_ALTERNATE_FUNCTION_10       = 10,
    GPIO_DRIVER_ALTERNATE_FUNCTION_OTG_FS   = GPIO_DRIVER_ALTERNATE_FUNCTION_10,
    GPIO_DRIVER_ALTERNATE_FUNCTION_OTG_HS   = GPIO_DRIVER_ALTERNATE_FUNCTION_10,

    GPIO_DRIVER_ALTERNATE_FUNCTION_11       = 11,
    GPIO_DRIVER_ALTERNATE_FUNCTION_ETH      = GPIO_DRIVER_ALTERNATE_FUNCTION_11,

    GPIO_DRIVER_ALTERNATE_FUNCTION_12       = 12,
    GPIO_DRIVER_ALTERNATE_FUNCTION_FSMC     = GPIO_DRIVER_ALTERNATE_FUNCTION_12,
    GPIO_DRIVER_ALTERNATE_FUNCTION_OTG_HS_FS= GPIO_DRIVER_ALTERNATE_FUNCTION_12,
    GPIO_DRIVER_ALTERNATE_FUNCTION_SDIO     = GPIO_DRIVER_ALTERNATE_FUNCTION_12,

    GPIO_DRIVER_ALTERNATE_FUNCTION_13       = 13,
    GPIO_DRIVER_ALTERNATE_FUNCTION_DCMI     = GPIO_DRIVER_ALTERNATE_FUNCTION_13,

    GPIO_DRIVER_ALTERNATE_FUNCTION_15       = 15,
    GPIO_DRIVER_ALTERNATE_FUNCTION_EVENTOUT = GPIO_DRIVER_ALTERNATE_FUNCTION_15
} gpio_driver_alternate_function_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the GPIO driver level.
 */
extern void gpioDriverInit(void);

#endif // _GPIO_DRIVER_H_
