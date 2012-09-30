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
 * @file uart_driver_template.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief TODO Comment!
 */

#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <uart_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define UART_BUFFER_SIZE (128u)

/**
 * TODO Comment!
 */
#define UART_RX_CALLBACK_COUNT (1u)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of UART driver IDs.
 */
typedef enum _uart_driver_id_e_
{
    // plat4m
    //
    // Add UART driver IDs here.
    //
    // Example:
    //
    //      UART_DRIVER_ID_2,
    //      ...
    //
    // Note: first UART driver ID should be set to 0 (zero).
    //
    // Example:
    //
    //      UART_DRIVER_ID_1 = 0,
    //      UART_DRIVER_ID_2,
    //      ...

    UART_DRIVER_ID_1 = 0,
    UART_DRIVER_ID_2,
    UART_DRIVER_ID_3,
    UART_DRIVER_ID_4,
    UART_DRIVER_ID_5,
    UART_DRIVER_ID_6,
    
    // Do not place values below!
    UART_DRIVER_ID_COUNT
} uart_driver_id_e;

/**
 * @brief Enumeration of UART driver GPIO map entries.
 */
typedef enum _uart_driver_gpio_map_id_e_
{
    UART_DRIVER_GPIO_MAP_ID_1 = 0,
    UART_DRIVER_GPIO_MAP_ID_2,

    // Do not place values below!
    UART_DRIVER_GPIO_MAP_ID_COUNT
} uart_driver_gpio_map_id_e;

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
extern void uartDriverInit(const interface_int_handler_f* handler);

#endif // _UART_DRIVER_H_
