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
 * TODO Comment!
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
 * TODO Comment!
 */
typedef enum _i2c_error_e_
{
    I2C_ERROR_NONE = 0,
    I2C_ERROR_INVALID_ID,
    I2C_ERROR_INVALID_PARAMETER,
    I2C_ERROR_TX_BUFFER_FULL,
    I2C_ERROR_TIMEOUT,

    // Do not place values below!
    I2C_ERROR_MAX
} i2c_error_e;

/**
 * TODO Comment!
 */
typedef enum _i2c_master_mode_e_
{
    I2C_MASTER_MODE_TX = 0,
    I2C_MASTER_MODE_RX
} i2c_master_mode_e;

/**
 * TODO Comment!
 */
typedef enum _i2c_interrupt_e_
{
    I2C_INTERRUPT_START = 0,
    I2C_INTERRUPT_ADDRESS,
    I2C_INTERRUPT_STOP,
    I2C_INTERRUPT_TX,
    I2C_INTERRUPT_RX,

    // Do not place values below!
    I2C_INTERRUPT_COUNT
} i2c_interrupt_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef uint8_t i2c_address_t;

/**
 * TODO Comment!
 */
typedef void i2c_driver_master_start_f(void);

/**
 * TODO Comment!
 */
typedef void i2c_driver_master_stop_f(void);

/**
 * TODO Comment!
 */
typedef void i2c_driver_master_tx_address_f(uint8_t address,
                                            i2c_master_mode_e mode);

/**
 * TODO Comment!
 */
typedef void i2c_driver_tx_f(uint8_t data);

/**
 * TODO Comment!
 */
typedef void i2c_driver_rx_f(uint8_t* data);

/**
 * TODO Comment!
 */
typedef struct _i2c_device_t_
{
    i2c_address_t address;
    i2c_driver_id_e driverId;
} i2c_device_t;

/**
 * TODO Comment!
 */
typedef struct _i2c_driver_t_
{
    i2c_driver_id_e id;
    i2c_address_t address;
    set_enabled_f* setEnabled;
    i2c_driver_master_start_f* masterStart;
    i2c_driver_master_stop_f* masterStop;
    i2c_driver_master_tx_address_f* masterTxAddress;
    i2c_driver_tx_f* tx;
    i2c_driver_rx_f* rx;
    set_enabled_f* txIntSetEnabled;
    set_enabled_f* rxIntSetEnabled;
} i2c_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void i2cInit(void);

/**
 * TODO Comment!
 */
extern bool i2cAddDriver(i2c_driver_t* i2cDriver);

/**
 * TODO Comment!
 */
extern bool i2cAddDrivers(i2c_driver_t i2cDrivers[], uint8_t size);

/**
 * TODO Comment!
 */
extern bool i2cAddRxCallBack(i2c_driver_id_e id, data_callback_f* callback);

/**
 * TODO Comment!
 */
extern bool i2cMasterAddSlave(i2c_driver_id_e id,
                              i2c_address_t address,
                              i2c_device_t* device);

/**
 * TODO Comment!
 */
extern i2c_error_e i2cSetEnabled(i2c_driver_id_e id, bool enabled);

/**
 * TODO Comment!
 */
extern i2c_error_e i2cMasterTx(i2c_device_t* i2cDevice,
                               byte_array_t* data);

/**
 * TODO Comment!
 */
extern i2c_error_e i2cMasterRx(i2c_device_t* i2cDevice,
                               byte_array_t* data);

/**
 * TODO Comment!
 */
extern i2c_error_e i2cMasterTxRx(i2c_device_t* i2cDevice,
                                 byte_array_t* txData,
                                 byte_array_t* rxData);

/**
 * TODO Comment!
 */
extern void i2cIntHandler(i2c_driver_id_e id, i2c_interrupt_e interrupt);

#endif // _I2C_MASTER_H_
