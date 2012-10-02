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
 * @brief Interface layer for the UART module.
 */

#ifndef _UART_INTERFACE_H_
#define _UART_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <uart_application.h>
#include <uart_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of UART errors.
 */
typedef enum _uart_error_e_
{
    UART_ERROR_NONE,                /// No error.
    UART_ERROR_INVALID_PARAMETER,   /// Parameter is invalid.
    UART_ERROR_INVALID_ID,          /// UART ID is invalid.
    UART_ERROR_NOT_ENABLED,         /// UART is not enabled.
    UART_ERROR_TX_BUFFER_FULL       /// UART transmit buffer is full.
} uart_error_e;

/**
 * @brief Enumeration of UART interrupts.
 */
typedef enum _uart_interrupt_e_
{
    UART_INTERRUPT_TX = 0,  /// Transmit interrupt.
    UART_INTERRUPT_RX,      /// Receive interrupt.

    // Do not place values below!
    UART_INTERRUPT_COUNT
} uart_interrupt_e;

/**
 * @brief Enumeration of UART word lengths.
 */
typedef enum _uart_word_bits_e_
{
    UART_WORD_BITS_8 = 0,
    UART_WORD_BITS_9,

    // Do not place values below!
    UART_WORD_BITS_COUNT
} uart_word_bits_e;

/**
 * @brief Enumeration of UART stop bits.
 */
typedef enum _uart_stop_bits_e_
{
    UART_STOP_BITS_1 = 0,
    UART_STOP_BITS_2,

    // Do not place values below!
    UART_STOP_BITS_COUNT
} uart_stop_bits_e;

/**
 * @brief Enumeration of UART parities.
 */
typedef enum _uart_parity_e_
{
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN,
    UART_PARITY_ODD,

    // Do not place values below!
    UART_PARITY_COUNT
} uart_parity_e;

/**
 * @brief Enumeration of UART hardware flow controls.
 */
typedef enum _uart_hardware_flow_control_e_
{
    UART_HARDWARE_FLOW_CONTROL_NONE = 0,

    // Do not place values below!
    UART_HARDWARE_FLOW_CONTROL_COUNT
} uart_hardware_flow_control_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief UART configuration type.
 */
typedef struct _uart_config_t_
{
    uint32_t baudRate;
    uart_word_bits_e wordBits;
    uart_stop_bits_e stopBits;
    uart_parity_e parity;
    uart_hardware_flow_control_e flowControl;
    uart_driver_gpio_map_id_e mapId;
} uart_config_t;

/**
 * @brief Function type that sets the given UART enabled or disabled.
 * @param driverId UART driver ID.
 * @param enabled Flag that indicates if the UART should be enabled or disabled.
 */
typedef void uart_driver_set_enabled_f(const uart_driver_id_e driverId,
                                       const bool enabled);

/**
 * @brief Function type that configures the given UART.
 * @param driverId UART driver ID.
 * @param config UART configuration.
 */
typedef void uart_driver_config_f(const uart_driver_id_e driverId,
                                  const uart_config_t* config);

/**
 * @brief Function type that transmits the given byte over the given UART.
 * @param driverId UART driver ID.
 * @param data Byte to transmit.
 * @return true if the function was successful, false if not.
 */
typedef void uart_driver_tx_f(const uart_driver_id_e driverId, uint8_t data);

/**
 * @brief Function type that receives a byte over the given UART.
 * @param driverId UART driver ID.
 * @param data Byte received.
 */
typedef void uart_driver_rx_f(const uart_driver_id_e driverId, uint8_t* data);

/**
 * @brief Function type that sets the given UART interrupt enabled or disabled.
 * @param driverId UART driver ID.
 * @param intId UART interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
typedef void uart_driver_int_set_enabled_f(const uart_driver_id_e driverId,
                                           const uart_interrupt_e interrupt,
                                           const bool enabled);

/**
 * @brief Structure that contains the necessary UART driver functions.
 */
typedef struct _uart_driver_t_
{
    const uart_driver_set_enabled_f* setEnabled;
    const uart_driver_config_f* configure;
    const uart_driver_tx_f* tx;
    const uart_driver_rx_f* rx;
    const uart_driver_int_set_enabled_f* intSetEnabled;
} uart_driver_t;

/**
 * @brief UART ID map type.
 */
typedef struct _uart_id_map_t_
{
    uart_id_e id;
    uart_driver_id_e driverId;
} uart_id_map_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the UART interface layer.
 */
extern void uartInit(void);

/**
 * @brief Sets the driver for the UART interface.
 * @param uartDriver UART driver.
 * @return UART error.
 */
extern uart_error_e uartSetDriver(const uart_driver_t* uartDriver);

/**
 * @brief Adds the given UARTs to the map.
 * @param uartIdMaps Array of UART ID maps.
 * @param size Size of array.
 * @return UART error.
 */
extern uart_error_e uartAddIdMaps(const uart_id_map_t uartIdMaps[],
                                  const unsigned int size);

/**
 * @brief Sets the given callback function for the given UART.
 * @param id UART ID.
 * @param callback Rx callback.
 * @return UART error.
 */
extern uart_error_e uartSetRxCallback(const uart_id_e id,
                                      const data_callback_f* callback);

/**
 * @brief Sets the given UART enabled or disabled.
 * @param id UART ID.
 * @param enabled Flag that indicates if the UART should be enabled or disabled.
 * @return UART error.
 */
extern uart_error_e uartSetEnabled(const uart_id_e id, const bool enabled);

/**
 * @brief Checks to see if the given UART is enabled or disabled.
 * @param id UART ID.
 * @param isEnabled Flag that indicates if the UART is enabled or disabled.
 * @return UART error.
 */
extern uart_error_e uartIsEnabled(const uart_id_e id, bool* isEnabled);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param id UART ID.
 * @param data Byte array to transmit.
 * @return true if the function was successful, false if not.
 */
extern uart_error_e uartTx(const uart_id_e id, const byte_array_t* data);

/**
 * @brief Reads data from the given UART into the given byte array.
 * @param id UART ID.
 * @param data Byte array for received data.
 * @return true if the function was successful, false if not.
 */
extern uart_error_e uartRead(const uart_id_e id, byte_array_t* data);

/**
 * @brief Gets the number of bytes that have been received.
 * @param id UART ID.
 * @param byteCount Number of bytes that have been received.
 * @return true if the function was successful, false if not.
 */
extern uart_error_e uartBytesAvailable(const uart_id_e id, uint8_t* byteCount);

#endif // _UART_INTERFACE_H_
