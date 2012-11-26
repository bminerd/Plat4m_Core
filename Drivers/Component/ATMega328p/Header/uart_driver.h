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
 * @date 9/19/12
 * @brief UART driver layer for the ATMega328p.
 */

#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

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
 * @brief Enumeration of UARTs.
 */
typedef enum _uart_e_
{
    // Do not place values above!
	UART_NONE = -1,

    UART_0,
    
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

	UART_INTERRUPT_TX,
	UART_INTERRUPT_RX,

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
 * @param handler Interface interrupt handler.
 */
extern void uartDriverInit(void);

#endif // _UART_DRIVER_H_
