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
 * @file gyro_driver_template.c
 * @author Ben Minerd
 * @date 7/31/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gyro_interface.h>

// plat4m
//
// Replace below with proper gyroscope driver header file.
//
// Example:
//
//      #include <gyro_driver_itg3200.h>

#include <gyro_driver_template.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

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
// Declare local gyroscope driver functions here.
//
// Example:
//
//      static i2c_error_e regWrite(gyro_reg_e reg, uint8_t value);
//      static i2c_error_e regRead(gyro_reg_e reg,
//                                 uint8_t value[],
//                                 uint8_t size);
//      ...

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

	return GYRO_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local gyroscope driver functions declared above.
