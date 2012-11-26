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
 * @brief UART interface layer.
 */

#ifndef _UART_INTERFACE_H_
#define _UART_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <uart_application.h>
#include <buffer_interface.h>
#include <mutex_interface.h>

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
    UART_ERROR_PARAMETER_INVALID,   /// Parameter is invalid.
    UART_ERROR_ID_DUPLICATE,        /// UART ID already added.
    UART_ERROR_UART_INVALID,        /// UART is invalid.
    UART_ERROR_NOT_INITIALIZED,     /// UART is not initialized.
    UART_ERROR_NOT_ENABLED,         /// UART is not enabled.
    UART_ERROR_TX_BUFFER_FULL       /// UART transmit buffer is full.
} uart_error_e;

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
    uart_gpio_map_e map;
} uart_config_t;

/**
 * @brief Function type that handles received data from a UART.
 * @param byte Byte received.
 */
typedef void uart_rx_data_callback_f(const uint8_t byte);

/**
 * @brief UART type.
 */
typedef struct _uart_info_t_
{
    bool isInitialized;
    bool isEnabled;
    mutex_t mutex;
    uart_config_t config;
    uint8_t txBufferMemory[UART_TX_BUFFER_SIZE];
    buffer_t txBuffer;
    uint8_t rxBufferMemory[UART_RX_BUFFER_SIZE];
    buffer_t rxBuffer;
    uart_rx_data_callback_f* rxCallback;
} uart_info_t;

/**
 * @brief UART ID type.
 */
typedef struct _uart_id_t_
{
    const uart_e uart;
    const generic_data_t private[sizeof(uart_info_t) / sizeof(generic_data_t)];
} uart_id_t;

/**
 * @brief Function type that sets the given UART enabled or disabled.
 * @param uart UART to access.
 * @param enabled Flag that indicates if the UART should be enabled or disabled.
 */
typedef void uart_driver_set_enabled_f(const uart_e uart, const bool enabled);

/**
 * @brief Function type that configures the given UART.
 * @param uart UART to access.
 * @param config UART configuration.
 */
typedef void uart_driver_configure_f(const uart_e uart,
                                     const uart_config_t* config);

/**
 * @brief Function type that transmits the given byte over the given UART.
 * @param uart UART to access.
 * @param byte Byte to transmit.
 */
typedef void uart_driver_tx_f(const uart_e uart, const uint8_t byte);

/**
 * @brief Function type that receives a byte over the given UART.
 * @param uart UART to access.
 * @param byte Byte received.
 */
typedef void uart_driver_rx_f(const uart_e uart, uint8_t* byte);

/**
 * @brief Function type that sets the given UART interrupt enabled or disabled.
 * @param uart UART to access.
 * @param interrupt UART interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
typedef void uart_driver_int_set_enabled_f(const uart_e uart,
                                           const uart_interrupt_e interrupt,
                                           const bool enabled);

/**
 * @brief Structure that contains the necessary UART driver functions.
 */
typedef struct _uart_driver_t_
{
    uart_driver_set_enabled_f* setEnabled;
    uart_driver_configure_f* configure;
    uart_driver_tx_f* tx;
    uart_driver_rx_f* rx;
    uart_driver_int_set_enabled_f* intSetEnabled;
} uart_driver_t;

/**
 * @brief Function type for a UART interface interrupt handler.
 * @param uart UART that caused an interrupt.
 * @param interrupt Interrupt that occurred.
 */
typedef void uart_interface_int_handler_f(const uart_e uart,
                                          const uart_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the UART module.
 */
void uartInit(void);

/**
 * @brief Sets the driver for the UART interface.
 * @param uartDriver UART driver.
 * @return UART error.
 */
uart_error_e uartSetDriver(const uart_driver_t* uartDriver,
                           uart_interface_int_handler_f** handler);

/**
 * @brief Adds the given UART ID.
 * @param id UART ID to add.
 * @return UART error.
 */
uart_error_e uartInitId(uart_id_t* id);

/**
 * @brief Sets the given callback function for the given UART.
 * @param id UART to access.
 * @param callback Rx callback.
 * @return UART error.
 */
uart_error_e uartSetRxCallback(uart_id_t* id,
                               uart_rx_data_callback_f* callback);

/**
 * @brief Sets the given UART enabled or disabled.
 * @param id UART to access.
 * @param enabled Flag that indicates if the UART should be enabled or disabled.
 * @return UART error.
 */
uart_error_e uartSetEnabled(uart_id_t* id, const bool enabled);

/**
 * @brief Checks to see if the given UART is enabled or disabled.
 * @param id UART to access.
 * @param isEnabled Flag that indicates if the UART is enabled or disabled.
 * @return UART error.
 */
uart_error_e uartIsEnabled(uart_id_t* id, bool* isEnabled);

/**
 * @brief Configures the given UART.
 * @brief id UART to access.
 * @brief config UART configuration.
 * @return UART error.
 */
uart_error_e uartConfigure(uart_id_t* id, const uart_config_t* config);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param id UART to access.
 * @param data Byte array to transmit.
 * @return true if the function was successful, false if not.
 */
uart_error_e uartTx(uart_id_t* id, const byte_array_t* data);

/**
 * @brief Reads data from the given UART into the given byte array.
 * @param id UART to access.
 * @param data Byte array for received data.
 * @return true if the function was successful, false if not.
 */
uart_error_e uartRead(uart_id_t* id, byte_array_t* data);

/**
 * @brief Gets the number of bytes that have been received.
 * @param id UART to access.
 * @param byteCount Number of bytes that have been received.
 * @return true if the function was successful, false if not.
 */
uart_error_e uartBytesAvailable(uart_id_t* id, int* byteCount);

#endif // _UART_INTERFACE_H_
