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
 * @file i2c_interface.h
 * @author Ben Minerd
 * @date 1/10/12
 * @brief Interface layer for the I2C module.
 * @todo Add slave implementation.
 */

#ifndef _I2C_INTERFACE_H_
#define _I2C_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <i2c_application.h>
#include <mutex_interface.h>
#include <buffer_interface.h>

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of I2C errors.
 */
typedef enum _i2c_error_e_
{
    I2C_ERROR_NONE,
    I2C_ERROR_PARAMETER_INVALID,
    I2C_ERROR_I2C_INVALID,
    I2C_ERROR_ID_INVALID,
    I2C_ERROR_DEVICE_ID_INVALID,
    I2C_ERROR_ADDRESS_INVALID,
    I2C_ERROR_NOT_ENABLED,
    I2C_ERROR_NOT_INITIALIZED,
    I2C_ERROR_TX_BUFFER_FULL,
    I2C_ERROR_TIMEOUT
} i2c_error_e;

/**
 * @brief Enumeration of I2C master modes.
 */
typedef enum _i2c_master_mode_e_
{
    I2C_MASTER_MODE_TX = 0,
    I2C_MASTER_MODE_RX
} i2c_master_mode_e;

/**
 * @brief Enumeration of I2C states.
 */
typedef enum _i2c_state_e_
{
    I2C_STATE_IDLE,
    I2C_STATE_BUSY
} i2c_state_e;

/**
 * @brief Enumeration of I2C address bits.
 */
typedef enum _i2c_address_bits_e_
{
    I2C_ADDRESS_BITS_7 = 0,
    I2C_ADDRESS_BITS_10,

    // Do not place values below!
    I2C_ADDRESS_BITS_COUNT
} i2c_address_bits_e;

/**
 * @brief Enumeration of I2C interrupts.
 */
typedef enum _i2c_interrupt_e_
{
    // Do not place values above!
    I2C_INTERRUPT_NONE = -1,

    // I2C master interrupts
    I2C_INTERRUPT_MASTER_START_TRANSMITTED,
    I2C_INTERRUPT_MASTER_TRANSMITTER_SELECTED,
    I2C_INTERRUPT_MASTER_RECEIVER_SELECTED,
    I2C_INTERRUPT_BYTE_TRANSMITTING,
    I2C_INTERRUPT_BYTE_TRANSMITTED,
    I2C_INTERRUPT_BYTE_RECEIVED,

    // I2C slave interrupts
    I2C_INTERRUPT_SLAVE_STOP_RECEIVED,

    // I2C error interrupts
    I2C_INTERRUPT_NACK_RECEIVED,

    // Do not place values below!
    I2C_INTERRUPT_COUNT
} i2c_interrupt_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C address type.
 */
typedef uint8_t i2c_address_t;

/**
 * @brief I2C configuration type.
 */
typedef struct _i2c_config_t_
{
    uint32_t clockSpeed;
    i2c_address_bits_e addressBits;
    i2c_gpio_map_e map;
} i2c_config_t;

struct _i2c_device_id_t_;

/**
 * @brief I2C transfer type.
 */
typedef struct _i2c_transfer_t_
{
    const struct _i2c_device_id_t_* device;
    const unsigned int txCount;
    const unsigned int rxCount;
    buffer_t txBuffer;
    buffer_t rxBuffer;
} i2c_transfer_t;

/**
 * @brief I2C info type.
 */
typedef struct _i2c_info_t_
{
    bool isInitialized;
    bool isEnabled;
    i2c_state_e state;
    mutex_t mutex;
    i2c_config_t config;
    i2c_transfer_t bufferMemory[I2C_TRANSFER_BUFFER_SIZE];
    buffer_t buffer;
    uint8_t txBufferMemory[I2C_TX_BUFFER_SIZE];
    uint8_t rxBufferMemory[I2C_RX_BUFFER_SIZE];
} i2c_info_t;

/**
 * @brief I2C ID type.
 */
typedef struct _i2c_id_t_
{
    const i2c_e i2c;
    const generic_data_t private[sizeof(i2c_info_t) / sizeof(generic_data_t)];
} i2c_id_t;

///**
// * @brief I2C device wrapper type.
// */
//typedef struct _i2c_device_info_t_
//{
//    i2c_id_t* i2c;
//} i2c_device_info_t;

/**
 * @brief I2C device type.
 */
typedef struct _i2c_device_id_t_
{
    const i2c_address_t address;
    i2c_id_t* i2cId;
} i2c_device_id_t;

/**
 * @brief Function type that sets the given I2C enabled or disabled.
 * @param i2c I2C to access.
 * @param enabled Flag that indicates if the I2C should be enabled or disabled.
 */
typedef void i2c_driver_set_enabled_f(const i2c_e i2c, const bool enabled);

/**
 * @brief Function type that configures the given I2C.
 * @param i2c I2C to access.
 * @param config I2C configuration.
 */
typedef void i2c_driver_configure_f(const i2c_e i2c,
                                    const i2c_config_t* config);

/**
 * @brief Function type that performs a start on the given I2C.
 * @param i2c I2C to access.
 */
typedef void i2c_driver_master_start_f(const i2c_e i2c);

/**
 * @brief Function type that performs a stop on the given I2C.
 * @param i2c I2C to access.
 */
typedef void i2c_driver_master_stop_f(const i2c_e i2c);

/**
 * @brief Function type that transmits a slave address over the given I2C.
 * @param i2c I2C to access.
 * @param address I2C slave address.
 * @param mode I2C master mode.
 */
typedef void i2c_driver_master_tx_address_f(const i2c_e i2c,
                                            const i2c_address_t address,
                                            const i2c_master_mode_e mode);

/**
 * @brief Function type that transmits a byte over the given I2C.
 * @param i2c I2C to access.
 * @param data Byte to transmit.
 */
typedef void i2c_driver_tx_f(const i2c_e i2c, const uint8_t data);

/**
 * @brief Function type that receives a byte over the given I2C.
 * @param i2c I2C to access.
 * @param isLastByte Flag that indicates if this is the last byte to be read or
 * not.
 */
typedef void i2c_driver_rx_f(const i2c_e i2c, bool isLastByte);

/**
 * @brief Function type that reads a received byte from the given I2C.
 * @param i2c I2C to access.
 * @param data Byte received on the given I2C.
 */
typedef void i2c_driver_read_f(const i2c_e i2c, uint8_t* data);

/**
 * @brief Function type that sets the given I2C interrupt enabled or disabled.
 * @param i2c I2C to access.
 * @param interrupt I2C interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
typedef void i2c_driver_int_set_enabled_f(const i2c_e i2c,
                                          const i2c_interrupt_e interrupt,
                                          const bool enabled);

/**
 * @brief Structure that contains the necessary I2C driver functions.
 */
typedef struct _i2c_driver_t_
{
    i2c_driver_set_enabled_f* setEnabled;
    i2c_driver_configure_f* configure;
    i2c_driver_master_start_f* masterStart;
    i2c_driver_master_stop_f* masterStop;
    i2c_driver_master_tx_address_f* masterTxAddress;
    i2c_driver_tx_f* tx;
    i2c_driver_rx_f* rx;
    i2c_driver_read_f* read;
    i2c_driver_int_set_enabled_f* intSetEnabled;
} i2c_driver_t;

/**
 * @brief Function type for a I2C interface interrupt handler.
 */
typedef void i2c_interface_int_handler_f(const i2c_e uart,
                                         const i2c_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the I2C interface layer.
 */
void i2cInit(void);

/**
 * @brief Sets the driver for the I2C interface.
 * @param i2cDriver I2C driver.
 * @return I2C error.
 */
i2c_error_e i2cSetDriver(const i2c_driver_t* i2cDriver,
                         i2c_interface_int_handler_f** handler);

/**
 * @brief Initializes the given I2C ID.
 * @param id I2C to access.
 * @return I2C error.
 */
i2c_error_e i2cInitId(i2c_id_t* id);

/**
 * @brief Sets the given I2C enabled or disabled.
 * @param id I2C to access.
 * @param enabled Flag that indicates if the I2C should be enabled or disabled.
 * @return I2C error.
 */
i2c_error_e i2cSetEnabled(i2c_id_t* id, const bool enabled);

/**
 * @brief Checks to see if the given I2C is enabled or disabled.
 * @param id I2C to access.
 * @param isEnabled Flag that indicates if the I2C is enabled or disabled.
 * @return I2C error.
 */
i2c_error_e i2cIsEnabled(i2c_id_t* id, bool* isEnabled);

/**
 * @brief Configures the given I2C.
 * @param id I2C to access.
 * @param config I2C configuration.
 * @return I2C error.
 */
i2c_error_e i2cConfigure(i2c_id_t* id, const i2c_config_t* config);

/**
 * @brief Transmits the given byte array to the given I2C device in master mode.
 * @param id I2C device to access.
 * @param data Byte array to transmit.
 * @return I2C error.
 */
i2c_error_e i2cMasterTx(const i2c_device_id_t* id, const byte_array_t* data);

/**
 * @brief Receives data from the given I2C device.
 * @param id I2C device to access.
 * @param data Byte array that was received.
 * @return I2C error.
 */
i2c_error_e i2cMasterRx(const i2c_device_id_t* id, byte_array_t* data);

/**
 * @brief Transmits and receives data to/from the given I2C device.
 * @param id I2C device to access.
 * @param txData Byte array to transmit.
 * @param rxData Byte array that was received.
 * @param I2C error.
 */
i2c_error_e i2cMasterTxRx(const i2c_device_id_t* id,
                          const byte_array_t* txData,
                          byte_array_t* rxData);

#endif // _I2C_MASTER_H_
