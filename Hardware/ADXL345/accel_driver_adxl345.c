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
 * @brief TODO Comment!
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

#include <accel_driver.h>

#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define ACCEL_I2C_ADDRESS (0x53)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
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
 * TODO Comment!
 */
static i2c_error_e regWrite(accel_reg_e reg, uint8_t value);

/**
 * TODO Comment!
 */
static i2c_error_e regRead(accel_reg_e reg, uint8_t value[], int size);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void accelDriverInit(void)
{
    i2c_device_t i2cDevice =
    {
        .device     = I2C_DEVICE_ACCEL,
        .i2c        = I2C_1,
        .address    = ACCEL_I2C_ADDRESS
    };
    
    i2cMasterAddDevice(&i2cDevice);

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
        return ACCEL_ERROR_COMMUNICATION;
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
    uint8_t bytes[] = {(uint8_t) reg, value};
    byte_array_t data = BYTE_ARRAY(bytes);
    
    return i2cMasterTx(I2C_DEVICE_ACCEL, &data);
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(accel_reg_e reg, uint8_t value[], int size)
{
    uint8_t txBytes[] = {reg};
    byte_array_t txData = BYTE_ARRAY(txBytes);
    uint8_t rxBytes[2];
    byte_array_t rxData = BYTE_ARRAY(rxBytes);
    
    return i2cMasterTxRx(I2C_DEVICE_ACCEL, &txData, &rxData);
}
