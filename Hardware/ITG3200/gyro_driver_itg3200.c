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
 * @file gyro_driver_itg3200.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gyro_interface.h>
#include <gyro_driver.h>

#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define GYRO_I2C_ADDRESS (0x68)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _gyro_reg_e_
{
    GYRO_REG_WHO_AM_I       = (uint8_t) 0x00,
    GYRO_REG_SMPLRT_DIV     = (uint8_t) 0x15,
    GYRO_REG_DLPF_FS        = (uint8_t) 0x16,
    GYRO_REG_TEMP_OUT_H     = (uint8_t) 0x1B,
    GYRO_REG_TEMP_OUT_L     = (uint8_t) 0x1C,
    GYRO_REG_GYRO_XOUT_H    = (uint8_t) 0x1D,
    GYRO_REG_GYRO_XOUT_L    = (uint8_t) 0x1E,
    GYRO_REG_GYRO_YOUT_H    = (uint8_t) 0x1F,
    GYRO_REG_GYRO_YOUT_L    = (uint8_t) 0x20,
    GYRO_REG_GYRO_ZOUT_H    = (uint8_t) 0x21,
    GYRO_REG_GYRO_ZOUT_L    = (uint8_t) 0x22,
    GYRO_REG_PWR_MGM        = (uint8_t) 0x3E
} gyro_reg_e;

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
// Declare local gyro driver functions here.
//
// Example:
//
//      static i2c_error_e regWrite(gyro_reg_e reg, uint8_t value);
//      static i2c_error_e regRead(gyro_reg_e reg,
//                                 uint8_t value[],
//                                 uint8_t size);
//      ...

/**
 * TODO Comment!
 */
static i2c_error_e regWrite(gyro_reg_e reg, uint8_t value);

/**
 * TODO Comment!
 */
static i2c_error_e regRead(gyro_reg_e reg, uint8_t value[], uint8_t size);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gyroDriverInit(void)
{
    // plat4m
    //
    // Initialize gyroscope driver here.
    //
    // Example:
    //
    //      i2cMasterAddSlave(I2C_DRIVER_ID_1, GYRO_I2C_ADDRESS, &i2cDevice);
    //      ...
    
    i2c_device_t i2cDevice =
    {
        .device     = I2C_DEVICE_GYRO,
        .i2c        = I2C_1,
        .address    = GYRO_I2C_ADDRESS
    };

    i2cMasterAddDevice(&i2cDevice);

    regWrite(GYRO_REG_SMPLRT_DIV, 0x15);
    regWrite(GYRO_REG_DLPF_FS, (0x03 << 3));
    regWrite(GYRO_REG_PWR_MGM, 0x01);
}

//------------------------------------------------------------------------------
extern gyro_error_e gyroDriverRead(gyro_reading_t* reading)
{
    // plat4m
    //
    // Fill reading with raw samples.
    //
    // Example:
    //
    //      reading->temp                       = (data[0] << 8) | data[1];
    //      reading->samples[GYRO_DOF_YAW]      = (data[2] << 8) | data[3];
    //      reading->samples[GYRO_DOF_PITCH]    = (data[4] << 8) | data[5];
    //      reading->samples[GYRO_DOF_ROLL]     = (data[6] << 8) | data[7];
    
    uint8_t data[8];

    if (regRead(GYRO_REG_TEMP_OUT_H, data, 8) != I2C_ERROR_NONE)
    {
        return GYRO_ERROR_COMMUNICATION;
    }

    reading->temp                       = (data[0] << 8) | data[1];
    reading->samples[GYRO_DOF_YAW]      = (data[2] << 8) | data[3];
    reading->samples[GYRO_DOF_PITCH]    = (data[4] << 8) | data[5];
    reading->samples[GYRO_DOF_ROLL]     = (data[6] << 8) | data[7];

    return GYRO_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local gyroscope driver functions declared above.

//------------------------------------------------------------------------------
static i2c_error_e regWrite(gyro_reg_e reg, uint8_t value)
{
    uint8_t bytes[] = {reg};
    byte_array_t data = BYTE_ARRAY(bytes);
    
    return i2cMasterTx(I2C_DEVICE_GYRO, &data);
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(gyro_reg_e reg, uint8_t value[], uint8_t size)
{
    uint8_t txBytes[] = {reg};
    byte_array_t txData = BYTE_ARRAY(txBytes);
    uint8_t rxBytes[2];
    byte_array_t rxData = BYTE_ARRAY(rxBytes);
    
    return i2cMasterTxRx(I2C_DEVICE_GYRO, &txData, &rxData);
}
