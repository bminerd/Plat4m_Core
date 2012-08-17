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
 * @file accel_driver_adxl345.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <accel_interface.h>

// plat4m
//
// Replace below with proper accelerometer driver header file.
//
// Example:
//
//      #include <accel_driver_adxl345.h>

#include <accel_driver_adxl345.h>

#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 *
 */
#define ACCEL_I2C_ADDRESS (0x53)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef enum _accel_reg_e_
{
    ACCEL_REG_DEVID         = (uint8_t) 0x00,
    ACCEL_REG_OFSX          = (uint8_t) 0x1E,
    ACCEL_REG_OFSY          = (uint8_t) 0x1F,
    ACCEL_REG_OFSZ          = (uint8_t) 0x20,
    ACCEL_REG_BW_RATE       = (uint8_t) 0x2C,
    ACCEL_REG_POWER_CTL     = (uint8_t) 0x2D,
    ACCEL_REG_DATA_FORMAT   = (uint8_t) 0x31,
    ACCEL_REG_DATAX0        = (uint8_t) 0x32,
    ACCEL_REG_DATAX1        = (uint8_t) 0x33,
    ACCEL_REG_DATAY0        = (uint8_t) 0x34,
    ACCEL_REG_DATAY1        = (uint8_t) 0x35,
    ACCEL_REG_DATAZ0        = (uint8_t) 0x36,
    ACCEL_REG_DATAZ1        = (uint8_t) 0x37
} accel_reg_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 *
 */
static i2c_device_t i2cDevice;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local accelerometer driver functions here.
//
// Example:
//
//      static i2c_error_e regWrite(accel_reg_e reg, uint8_t value);
//      static i2c_error_e regRead(accel_reg_e reg,
//                                 uint8_t value[],
//                                 uint8_t size);
//      ...

/**
 *
 */
static i2c_error_e regWrite(accel_reg_e reg, uint8_t value);

/**
 *
 */
static i2c_error_e regRead(accel_reg_e reg, uint8_t value[], int size);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void accelDriverInit(void)
{
    // plat4m
    //
    // Initialize accelerometer driver here.
    //
    // Example:
    //
    //      i2cMasterAddSlave(I2C_DRIVER_ID_1, ACCEL_I2C_ADDRESS, &i2cDevice);
    //      ...
    
    i2cMasterAddSlave(I2C_DRIVER_ID_1, ACCEL_I2C_ADDRESS, &i2cDevice);

    regWrite(ACCEL_REG_POWER_CTL, 0);
    regWrite(ACCEL_REG_DATA_FORMAT, 0);
}

//------------------------------------------------------------------------------
extern accel_error_e accelDriverRead(accel_reading_t* reading)
{
    // plat4m
    //
    // Fill reading with raw samples.
    //
    // Example:
    //
    //      reading->samples[ACCEL_DOF_X] = (data[1] << 8) | data[0];
    //      reading->samples[ACCEL_DOF_Y] = (data[3] << 8) | data[2];
    //      reading->samples[ACCEL_DOF_Z] = (data[5] << 8) | data[4];
    
    uint8_t data[6];

    if (regRead(ACCEL_REG_DATAX0, data, 6) != I2C_ERROR_NONE)
    {
        return ACCEL_ERROR_COMM;
    }

    reading->samples[ACCEL_DOF_X] = (data[1] << 8) | data[0];
    reading->samples[ACCEL_DOF_Y] = (data[3] << 8) | data[2];
    reading->samples[ACCEL_DOF_Z] = (data[5] << 8) | data[4];

    return ACCEL_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local accelerometer driver functions declared above.

//------------------------------------------------------------------------------
static i2c_error_e regWrite(accel_reg_e reg, uint8_t value)
{
    uint8_t data[] = {(uint8_t) reg, value};
    
    return i2cMasterTx(&i2cDevice, data, sizeof(data));
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(accel_reg_e reg, uint8_t value[], int size)
{
    uint8_t txData[] = {reg};
    uint8_t rxData[2];
    
    return i2cMasterTxRx(&i2cDevice,
                         txData,
                         sizeof(txData),
                         rxData,
                         size);
}
