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
 * @file i2c_driver_template.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief TODO Comment!
 */

#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define I2C_DRIVER_BUFFER_SIZE (128u)

/**
 * TODO Comment!
 */
#define I2C_DRIVER_TRANSFER_BUFFER_SIZE (4u)

/**
 * TODO Comment!
 */
#define I2C_DRIVER_RX_CALLBACK_COUNT (1u)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _i2c_driver_id_e_
{
    // plat4m
    //
    // Add I2C driver IDs here.
    //
    // Example:
    //
    //      I2C_DRIVER_ID_2,
    //      ...
    //
    // Note: first I2C driver ID should be set to 0 (zero).
    //
    // Example:
    //
    //      I2C_DRIVER_ID_1 = 0,
    //      I2C_DRIVER_ID_2,
    //      ...
    
    I2C_DRIVER_ID_1 = 0,
    
    // Do not place values below!
    I2C_DRIVER_ID_COUNT
} i2c_driver_id_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void i2cDriverInit(void);

#endif // _I2C_DRIVER_H_
