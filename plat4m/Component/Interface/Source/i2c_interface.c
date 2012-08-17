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
 * @file i2c_interface.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  I2C_DRIVER_ID_LIMIT
#define I2C_DRIVER_COUNT (I2C_DRIVER_ID_LIMIT)
#else
#define I2C_DRIVER_COUNT (I2C_DRIVER_ID_COUNT)
#endif

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 *
 */
static i2c_driver_t drivers[I2C_DRIVER_COUNT];

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cInit(void)
{
    int i;

    for (i = 0; i < I2C_DRIVER_COUNT; i++)
    {
        drivers[i].id = (i2c_driver_id_e) i;
        drivers[i].masterStart = 0;
        drivers[i].masterStop = 0;
        drivers[i].masterTxAddress = 0;
        drivers[i].tx = 0;
        drivers[i].rx = 0;
    }

    i2cDriverInit();
}

//------------------------------------------------------------------------------
extern bool i2cAddDriver(i2c_driver_t i2cDriver)
{
    if (i2cDriver.id >= I2C_DRIVER_ID_COUNT ||
        !i2cDriver.masterStart ||
        !i2cDriver.masterStop ||
        !i2cDriver.masterTxAddress ||
        !i2cDriver.tx ||
        !i2cDriver.rx)
    {
        return false;
    }

    DRIVER_ADD(drivers, i2cDriver, i2c_driver_t);

    return true;
}

//------------------------------------------------------------------------------
extern bool i2cAddDrivers(i2c_driver_t i2cDrivers[], uint8_t size)
{
    int i;

    if (size > I2C_DRIVER_COUNT)
    {
        return false;
    }

    for (i = 0; i < size; i++)
    {
        if (!i2cAddDriver(i2cDrivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern bool i2cMasterAddSlave(i2c_driver_id_e id,
                              i2c_address_t address,
                              i2c_device_t* device)
{
    if (id >= I2C_DRIVER_ID_COUNT || !device)
    {
        return false;
    }

    device->address = address;
    device->driverId = id;

    return true;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTx(const i2c_device_t* i2cDevice,
                               uint8_t data[],
                               uint8_t size)
{
    int i;
    i2c_error_e error;
    i2c_driver_id_e id = i2cDevice->driverId;

    // Start
    error = drivers[id].masterStart();
    
    if (error != I2C_ERROR_NONE)
    {
        // Check?
        return error;
    }

    // Tx data
    for (i = 0; i < size; i++)
    {
        error = drivers[id].tx(data[i]);
        
        if (error != I2C_ERROR_NONE)
        {
            // Check?
            return error;
        }
    }

    // Stop
    return drivers[id].masterStop();
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterRx(const i2c_device_t* i2cDevice,
                               uint8_t data[],
                               uint8_t size)
{
    int i;
    i2c_error_e error;
    i2c_driver_id_e id = i2cDevice->driverId;

    // Start
    error = drivers[id].masterStart();
    
    if (error != I2C_ERROR_NONE)
    {
        // Check?
        return error;
    }

    // Tx data
    for (i = 0; i < size; i++)
    {
        error = drivers[id].rx(&data[i]);
        
        if (error != I2C_ERROR_NONE)
        {
            // Check?
            return error;
        }
    }

    // Stop
    return drivers[id].masterStop();
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTxRx(const i2c_device_t* i2cDevice,
                                 uint8_t txData[],
                                 uint8_t txDataSize,
                                 uint8_t rxData[],
                                 uint8_t rxDataSize)
{
    i2c_error_e error;

    error = i2cMasterTx(i2cDevice, txData, txDataSize);

    if (error != I2C_ERROR_NONE)
    {
        return error;
    }

    return i2cMasterRx(i2cDevice, rxData, rxDataSize);
}
