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
 * @file gpio_driver_template.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <i2c_interface.h>

// plat4m
//
// Replace below with proper I2C driver header file.
//
// Example:
//
//      #include <i2c_driver_stm32f10x.h>

#include <i2c_driver_stm32f2xx.h>

// plat4m
//
// Include necessary manufacturer I2C driver header files here.
//
// Example:
//
//      #include <stm32f10x_i2c.h>

#include <stm32f2xx_i2c.h>

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
// Declare local I2C driver functions here.
//
// Example:
//
//      static void i2c1SetEnabled(bool setEnabled);
//      static void i2c1MasterStart(void);
//      ...

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cDriverInit(void)
{
    // plat4m
    //
    // Initialize I2C driver.
    // Add drivers to I2C interface.
    //
    // Example:
    //
    //      i2c_driver_t i2cs[] =
    //      {
    //          // I2C1
    //          {
    //              .id         = I2C_DRIVER_ID_1,
    //              .setEnabled = i2c1SetEnabled,
    //              ...
    //          },
    //          // I2C2
    //          {
    //              .id         = I2C_DRIVER_ID_2,
    //              .setEnabled = i2c2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      i2cAddDrivers(i2cs, ARRAY_SIZE(i2cs, i2c_driver_t));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local I2C driver functions declared above.
