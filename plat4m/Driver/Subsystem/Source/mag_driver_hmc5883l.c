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
 * @file mag_hmc58831.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <mag_interface.h>

// plat4m
//
// Replace below with proper magnetometer driver header file.
//
// Example:
//
//      #include <mag_hmc5883l.h>

#include <mag_driver_hmc5883l.h>

#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define MAG_I2C_ADDRESS (0x53)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _mag_reg_e_
{
    MAG_REG_CNFG_A      = (uint8_t) 0x00,
    MAG_REG_CNFG_B      = (uint8_t) 0x01,
    MAG_REG_MODE        = (uint8_t) 0x02,
    MAG_REG_X_MSB       = (uint8_t) 0x03,
    MAG_REG_X_LSB       = (uint8_t) 0x04,
    MAG_REG_Z_MSB       = (uint8_t) 0x05,
    MAG_REG_Z_LSB       = (uint8_t) 0x06,
    MAG_REG_Y_MSB       = (uint8_t) 0x07,
    MAG_REG_Y_LSB       = (uint8_t) 0x08
} mag_reg_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static i2c_device_t i2cDevice;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local magnetometer driver functions here.
//
// Example:
//
//      static i2c_error_e regWrite(mag_reg_e reg, uint8_t value);
//      static i2c_error_e regRead(mag_reg_e reg,
//                                 uint8_t value[],
//                                 uint8_t size);
//      ...

/**
 * TODO Comment!
 */
static i2c_error_e regWrite(mag_reg_e reg, uint8_t value);

/**
 * TODO Comment!
 */
static i2c_error_e regRead(mag_reg_e reg, uint8_t value[], int size);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void magDriverInit(void)
{
    // plat4m
    //
    // Initialize magnetometer driver here.
    //
    // Example:
    //
    //      i2cMasterAddSlave(I2C_ID_1, MAG_I2C_ADDRESS, &i2cDevice);
    //      ...
    
    i2cMasterAddSlave(I2C_ID_1, MAG_I2C_ADDRESS, &i2cDevice);

    // Check?
    regWrite(MAG_REG_CNFG_A, 0x70);
    // Check?
    regWrite(MAG_REG_CNFG_B, 0xA0);
    // Check?
    regWrite(MAG_REG_MODE,   0x00);
}

//------------------------------------------------------------------------------
extern mag_error_e magDriverRead(mag_reading_t* reading)
{
    // plat4m
    //
    // Fill reading with raw samples.
    //
    // Example:
    //
    //      reading->samples[MAG_DOF_AZIMUTH]   = (data[0] << 8) | data[1];
    //      reading->samples[MAG_DOF_ELEVATION] = (data[2] << 8) | data[3];
    //      reading->samples[MAG_DOF_BANK]      = (data[4] << 8) | data[5];
    
    uint8_t data[6];

    if (regRead(MAG_REG_X_MSB, data, 6) != I2C_ERROR_NONE)
    {
        return MAG_ERROR_COMM;
    }

    reading->samples[MAG_DOF_AZIMUTH]   = (data[0] << 8) | data[1];
    reading->samples[MAG_DOF_ELEVATION] = (data[2] << 8) | data[3];
    reading->samples[MAG_DOF_BANK]      = (data[4] << 8) | data[5];

    return MAG_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local magnetometer driver functions declared above.

//------------------------------------------------------------------------------
static i2c_error_e regWrite(mag_reg_e reg, uint8_t value)
{
	// i2c write

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(mag_reg_e reg, uint8_t value[], int size)
{
	// i2c read

    return I2C_ERROR_NONE;
}
