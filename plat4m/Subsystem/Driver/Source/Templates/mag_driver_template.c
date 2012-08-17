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
 * @file mag_driver.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief
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
//      #include <mag_driver_hmc5883l.h>

#include <mag_driver_template.h>

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
// Declare local magnetometer driver functions here.
//
// Example:
//
//      static i2c_error_e regWrite(mag_reg_e reg, uint8_t value);
//      static i2c_error_e regRead(mag_reg_e reg, uint8_t value[], int size);
//      ...

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
    //      i2cMasterAddSlave(I2C_DRIVER_ID_1, MAG_I2C_ADDRESS, &i2cDevice);
    //      ...
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

    return MAG_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local magnetometer driver functions declared above.
