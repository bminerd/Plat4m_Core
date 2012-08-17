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
 * @file uart_interface.h
 * @author Ben Minerd
 * @date 12/30/11
 * @brief
 */

#ifndef _UART_INTERFACE_H_
#define _UART_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <uart_driver.h>
#include <buffer_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef enum _uart_error_e_
{
    UART_ERROR_NONE = 0,
    UART_ERROR_INVALID_ID,

    // Do not place values below!
    UART_ERROR_COUNT
} uart_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef set_enabled_f uart_driver_set_enabled_f;

/**
 *
 */
typedef bool uart_driver_tx_f(uint8_t data);

/**
 *
 */
typedef bool uart_driver_rx_f(uint8_t* data);

/**
 *
 */
typedef set_enabled_f uart_driver_tx_int_set_enabled_f;

/**
 *
 */
typedef set_enabled_f uart_driver_rx_int_set_enabled_f;

/**
 *
 */
typedef bool uart_driver_callback_f(uart_driver_id_e id);

/**
 *
 */
typedef struct _uart_driver_t_
{
    uart_driver_id_e id;
    buffer_t txBuffer;
    buffer_t rxBuffer;
    uart_driver_set_enabled_f* setEnabled;
    uart_driver_tx_f* tx;
    uart_driver_rx_f* rx;
    uart_driver_tx_int_set_enabled_f* txIntSetEnabled;
    uart_driver_rx_int_set_enabled_f* rxIntSetEnabled;
} uart_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
extern void uartInit(void);

/**
 *
 */
extern bool uartAddDriver(uart_driver_t uartDriver);

/**
 *
 */
extern bool uartAddDrivers(uart_driver_t uartDriver[], uint8_t size);

/**
 *
 */
extern bool uartTx(uart_driver_id_e id, uint8_t data[], uint8_t size);

#endif // _UART_INTERFACE_H_
