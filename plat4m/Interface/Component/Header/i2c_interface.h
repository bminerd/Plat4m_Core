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
 */

#ifndef _I2C_INTERFACE_H_
#define _I2C_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <i2c_application.h>
#include <i2c_driver.h>

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of I2C errors.
 */
typedef enum _i2c_error_e_
{
    I2C_ERROR_NONE,
    I2C_ERROR_INVALID_PARAMETER,
    I2C_ERROR_INVALID_ID,
    I2C_ERROR_NOT_ENABLED,
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
 * @brief Enumeration of I2C interrupts.
 */
typedef enum _i2c_interrupt_e_
{
    I2C_INTERRUPT_START = 0,
    I2C_INTERRUPT_STOP,
    I2C_INTERRUPT_ADDRESS,
    I2C_INTERRUPT_TX,
    I2C_INTERRUPT_RX,

    // Do not place values below!
    I2C_INTERRUPT_COUNT
} i2c_interrupt_e;

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
    i2c_driver_gpio_map_id_e mapId;
} i2c_config_t;

/**
 * @brief Function type that sets the given I2C enabled or disabled.
 * @param driverId I2C driver ID.
 * @param enabled Flag that indicates if the I2C should be enabled or disabled.
 */
typedef void i2c_driver_set_enabled_f(const i2c_driver_id_e driverId,
                                      const bool enabled);

/**
 * @brief Function type that configures the given I2C.
 * @param driverId I2C driver ID.
 * @param config I2C configuration.
 */
typedef void i2c_driver_config_f(const i2c_driver_id_e driverId,
                                 const i2c_config_t* config);

/**
 * @brief Function type that performs a start on the given I2C.
 * @param driverId I2C driver ID.
 */
typedef void i2c_driver_master_start_f(const i2c_driver_id_e driverId);

/**
 * @brief Function type that performs a stop on the given I2C.
 * @param driverId I2C driver ID.
 */
typedef void i2c_driver_master_stop_f(const i2c_driver_id_e driverId);

/**
 * @brief Function type that transmits a slave address over the given I2C.
 * @param driverId I2C driver ID.
 * @param address I2C slave address.
 * @param mode I2C master mode.
 */
typedef void i2c_driver_master_tx_address_f(const i2c_driver_id_e driverId,
                                            const i2c_address_t address,
                                            const i2c_master_mode_e mode);

/**
 * @brief Function type that transmits a byte over the given I2C.
 * @param driverId I2C driver ID.
 * @param data Byte to transmit.
 */
typedef void i2c_driver_tx_f(const i2c_driver_id_e driverId,
                             const uint8_t data);

/**
 * @brief Function type that receives a byte over the given I2C.
 * @param driverId I2C driver ID.
 * @param data Byte received.
 */
typedef void i2c_driver_rx_f(const i2c_driver_id_e driverId, uint8_t* data);

/**
 * @brief Function type that sets the given I2C interrupt enabled or disabled.
 * @param driverId I2C driver ID.
 * @param interrupt I2C interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
typedef void i2c_driver_int_set_enabled_f(const i2c_driver_id_e driverId,
                                          const i2c_interrupt_e interrupt,
                                          const bool enabled);

/**
 * @brief Structure that contains the necessary I2C driver functions.
 */
typedef struct _i2c_driver_t_
{
    const i2c_driver_set_enabled_f* setEnabled;
    const i2c_driver_config_f* configure;
    const i2c_driver_master_start_f* masterStart;
    const i2c_driver_master_stop_f* masterStop;
    const i2c_driver_master_tx_address_f* masterTxAddress;
    const i2c_driver_tx_f* tx;
    const i2c_driver_rx_f* rx;
    const i2c_driver_int_set_enabled_f* intSetEnabled;
} i2c_driver_t;

/**
 * @brief I2C ID map type.
 */
typedef struct _i2c_id_map_t_
{
    i2c_id_e id;
    i2c_driver_id_e driverId;
} i2c_id_map_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the I2C interface layer.
 */
extern void i2cInit(void);

/**
 * @brief Sets the driver for the I2C interface.
 * @param i2cDriver I2C driver.
 * @return I2C error.
 */
extern i2c_error_e i2cSetDriver(const i2c_driver_t* i2cDriver);

/**
 * @brief Adds the given I2Cs to the map.
 * @param i2cIdMaps Array of I2C ID maps.
 * @param size Size of array.
 * @return I2C error.
 */
extern i2c_error_e i2cAddIdMaps(const i2c_id_map_t i2cIdMaps[],
                                const unsigned int size);

/**
 * @brief Sets the given callback function for the given I2C.
 * @param id I2C ID.
 * @param callback Rx callback.
 * @return I2C error.
 */
extern i2c_error_e i2cSetRxCallBack(const i2c_id_e id,
                                    const data_callback_f* callback);

/**
 * @brief Adds the given I2C device to the given I2C.
 * @param id I2C ID.
 * @param deviceId I2C device ID.
 * @param address I2C device address.
 * @return I2C error.
 */
extern i2c_error_e i2cMasterAddSlave(const i2c_id_e id,
                                     const i2c_address_t address,
                                     id_t* deviceId);

/**
 * @brief Sets the given I2C enabled or disabled.
 * @param id I2C ID.
 * @param enabled Flag that indicates if the I2C should be enabled or disabled.
 * @return I2C error.
 */
extern i2c_error_e i2cSetEnabled(const i2c_id_e id, const bool enabled);

/**
 * @brief Checks to see if the given I2C is enabled or disabled.
 * @param id I2C ID.
 * @param isEnabled Flag that indicates if the I2C is enabled or disabled.
 * @return I2C error.
 */
extern i2c_error_e i2cIsEnabled(const i2c_id_e id, bool* isEnabled);

/**
 * @brief Transmits the given byte array to the given I2C device in master mode.
 * @param deviceId I2C device ID.
 * @param data Byte array to transmit.
 * @return I2C error.
 */
extern i2c_error_e i2cMasterTx(const id_t deviceId,
                               byte_array_t* data);

/**
 * @brief Receives data from the given I2C device.
 * @param deviceId I2C device ID.
 * @param data Byte array that was received.
 * @return I2C error.
 */
extern i2c_error_e i2cMasterRx(const id_t deviceId,
                               byte_array_t* data);

/**
 * @brief Transmits and receives data to/from the given I2C device.
 * @param deviceId I2C device ID.
 * @param txData Byte array to transmit.
 * @param rxData Byte array that was received.
 * @param I2C error.
 */
extern i2c_error_e i2cMasterTxRx(const id_t deviceId,
                                 byte_array_t* txData,
                                 byte_array_t* rxData);

#endif // _I2C_MASTER_H_
