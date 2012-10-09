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
 * @file baro_driver.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Baro driver layer for BMP085.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <baro_interface.h>

// plat4m
//
// Replace below with proper barometric pressure sensor driver header file.
//
// Example:
//
//      #include <baro_driver_bmp085.h>

#include <baro_driver.h>

#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define BARO_I2C_ADDRESS (0x00)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local barometric pressure sensor driver functions here.
//
// Example:
//
//      static i2c_error_e regWrite(baro_reg_e reg, uint8_t value);
//      static i2c_error_e regRead(baro_reg_e reg, uint8_t value[], int size);
//      ...

///**
// *
// */
//static i2c_error_e regWrite(baro_reg_e reg, uint8_t value);
//
///**
// *
// */
//static i2c_error_e regRead(baro_reg_e reg, uint8_t value[], int size);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void baroDriverInit(void)
{
    // plat4m
    //
    // Initialize barometric pressure sensor driver here.
    //
    // Example:
    //
    //      i2cMasterAddSlave(I2C_DRIVER_ID_1, BARO_I2C_ADDRESS, &i2cDevice);
    //      ...
}

//------------------------------------------------------------------------------
extern baro_error_e baroDriverRead(baro_reading_t* reading)
{
    // plat4m
    //
    // Fill barometric pressure sensor reading with raw sample.
    //
    // Example:
    //
    //      reading->sample = data;

    return BARO_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local barometric pressure sensor driver functions declared above.
