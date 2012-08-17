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
 * @brief
 */

#ifndef _I2C_INTERFACE_H_
#define _I2C_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <i2c_driver.h>

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief
 */
typedef enum _i2c_error_e_
{
    I2C_ERROR_NONE = 0,
    I2C_ERROR_INVALID_ID,
    I2C_ERROR_TIMEOUT,

    // Do not place values below!
    I2C_ERROR_MAX
} i2c_error_e;

/**
 *
 */
typedef enum _i2c_master_mode_e_
{
    I2C_MASTER_MODE_TX = 0,
    I2C_MASTER_MODE_RX
} i2c_master_mode_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef set_enabled_f i2c_driver_set_enabled_f;

/**
 *
 */
typedef i2c_error_e i2c_driver_master_start_f(void);

/**
 *
 */
typedef i2c_error_e i2c_driver_master_stop_f(void);

/**
 *
 */
typedef i2c_error_e i2c_driver_master_tx_address_f(uint8_t address,
                                                   i2c_master_mode_e mode);

/**
 *
 */
typedef i2c_error_e i2c_driver_tx_f(uint8_t data);

/**
 *
 */
typedef i2c_error_e i2c_driver_rx_f(uint8_t* data);

/**
 * @brief
 */
typedef struct _i2c_driver_t_
{
    i2c_driver_id_e id;
    i2c_driver_set_enabled_f* setEnabled;
    i2c_driver_master_start_f* masterStart;
    i2c_driver_master_stop_f* masterStop;
    i2c_driver_master_tx_address_f* masterTxAddress;
    i2c_driver_tx_f* tx;
    i2c_driver_rx_f* rx;
} i2c_driver_t;

/**
 *
 */
typedef uint8_t i2c_address_t;

/**
 *
 */
typedef struct _i2c_device_t_
{
    i2c_address_t address;
    i2c_driver_id_e driverId;
} i2c_device_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
extern void i2cInit(void);

/**
 *
 */
extern bool i2cAddDriver(i2c_driver_t i2cDriver);

/**
 *
 */
extern bool i2cAddDrivers(i2c_driver_t i2cDrivers[], uint8_t size);

/**
 *
 */
extern bool i2cMasterAddSlave(i2c_driver_id_e id,
                              i2c_address_t address,
                              i2c_device_t* device);

/**
 *
 */
extern i2c_error_e i2cMasterTx(const i2c_device_t* i2cDevice,
                               uint8_t data[],
                               uint8_t size);

/**
 *
 */
extern i2c_error_e i2cMasterRx(const i2c_device_t* i2cDevice,
                               uint8_t data[],
                               uint8_t size);

/**
 *
 */
extern i2c_error_e i2cMasterTxRx(const i2c_device_t* i2cDevice,
                                 uint8_t txData[],
                                 uint8_t txDataSize,
                                 uint8_t rxData[],
                                 uint8_t rxDataSize);

#endif // _I2C_MASTER_H_
