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
 * @file uart_driver_stm32f2xx.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief UART driver layer for the STM32F2xx.
 */

#ifndef _UART_DRIVER_STM32F2XX_H_
#define _UART_DRIVER_STM32F2XX_H_

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
 * @brief Enumeration of UART driver IDs.
 */
typedef enum _uart_e_
{
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    
    // Do not place values below!
    UART_COUNT
} uart_e;

/**
 * @brief Enumeration of UART GPIO map entries.
 */
typedef enum _uart_gpio_map_e_
{
    UART_GPIO_MAP_1 = 0,
    UART_GPIO_MAP_2,

    // Do not place values below!
    UART_GPIO_MAP_COUNT
} uart_gpio_map_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the UART driver layer.
 * @param handler UART interface interrupt handler.
 */
extern void uartDriverStm32f2xxInit(void);

#endif // _UART_DRIVER_STM32F2XX_H_
