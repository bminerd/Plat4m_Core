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
 * @file uart_driver.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief UART driver layer for the STM32F2xx.
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
typedef enum _uart_e_
{
    // Do not place values above!
    UART_NONE = -1,

    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    
    // Do not place values below!
    UART_COUNT
} uart_e;

/**
 * @brief Enumeration of UART interrupts.
 */
typedef enum _uart_interrupt_e_
{
    // Do not place values above!
    UART_INTERRUPT_NONE = -1,

    UART_INTERRUPT_TX ,     /// Transmit interrupt.
    UART_INTERRUPT_RX,      /// Receive interrupt.

    // Do not place values below!
    UART_INTERRUPT_COUNT
} uart_interrupt_e;

/**
 * @brief Enumeration of UART GPIO map entries.
 */
typedef enum _uart_gpio_map_e_
{
    // Do not place values above!
    UART_GPIO_MAP_NONE = -1,

    UART_GPIO_MAP_1,
    UART_GPIO_MAP_2,

    // Do not place values below!
    UART_GPIO_MAP_COUNT
} uart_gpio_map_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type for a UART interface interrupt handler.
 */
typedef void uart_interface_int_handler_f(const uart_e uart,
                                          const uart_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the UART driver layer.
 * @param handler UART interface interrupt handler.
 */
extern void uartDriverInit(const uart_interface_int_handler_f* handler);

#endif // _UART_DRIVER_H_
