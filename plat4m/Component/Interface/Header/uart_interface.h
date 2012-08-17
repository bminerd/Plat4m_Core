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
 * @brief TODO Comment!
 */

#ifndef _UART_INTERFACE_H_
#define _UART_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <uart_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _uart_error_e_
{
    UART_ERROR_NONE = 0,
    UART_ERROR_INVALID_ID,
    UART_ERROR_TX_BUFFER_FULL,

    // Do not place values below!
    UART_ERROR_COUNT
} uart_error_e;

/**
 * TODO Comment!
 */
typedef enum _uart_interrupt_e_
{
    UART_INTERRUPT_TX = 0,
    UART_INTERRUPT_RX
} uart_interrupt_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef uart_error_e uart_driver_tx_f(uint8_t data);

/**
 * TODO Comment!
 */
typedef uart_error_e uart_driver_rx_f(uint8_t* data);

/**
 * TODO Comment!
 */
typedef struct _uart_driver_t_
{
    uart_driver_id_e id;
    set_enabled_f* setEnabled;
    uart_driver_tx_f* tx;
    uart_driver_rx_f* rx;
    set_enabled_f* txIntSetEnabled;
    set_enabled_f* rxIntSetEnabled;
} uart_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void uartInit(void);

/**
 * TODO Comment!
 */
extern bool uartAddDriver(uart_driver_t* uartDriver);

/**
 * TODO Comment!
 */
extern bool uartAddDrivers(uart_driver_t uartDriver[], uint8_t size);

/**
 * TODO Comment!
 */
extern bool uartAddRxCallback(uart_driver_id_e id,
                              data_callback_f* rxCallback);

/**
 * TODO Comment!
 */
extern uart_error_e uartSetEnabled(uart_driver_id_e id, bool enabled);

/**
 * TODO Comment!
 */
extern uart_error_e uartTx(uart_driver_id_e id, byte_array_t* data);

/**
 * TODO Comment!
 */
extern uart_error_e uartRead(uart_driver_id_e id, byte_array_t* data);

/**
 * TODO Comment!
 */
extern uart_error_e uartBytesAvailable(uart_driver_id_e id, uint8_t* byteCount);

/**
 * TODO Comment!
 */
extern void uartIntHandler(uart_driver_id_e id, uart_interrupt_e interrupt);

#endif // _UART_INTERFACE_H_
