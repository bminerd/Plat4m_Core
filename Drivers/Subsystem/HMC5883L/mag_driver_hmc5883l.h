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
 * @file mag_driver.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Magnetometer driver layer for HMC5883L.
 */

#ifndef _MAG_DRIVER_HMC5883L_H_
#define _MAG_DRIVER_HMC5883L_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <mag_application.h>
#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define MAG_HMC5883L_DOF_COUNT 3

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Magnetometer type.
 */
typedef struct _mag_hmc5883l_t_
{
    i2c_device_id_t* i2cDeviceId;
} mag_hmc5883l_t;

/**
 * @brief Magnetometer configuration type.
 */
typedef struct _mag_hmc5883l_config_t_
{
    int a;
} mag_hmc5883l_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the magnetometer driver layer.
 */
void magDriverHmc5883lInit(void);

#endif // _MAG_DRIVER_HMC5883L_H_
