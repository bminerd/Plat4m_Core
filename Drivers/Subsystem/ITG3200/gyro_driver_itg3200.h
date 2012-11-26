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
 * @file gyro_driver_itg3200.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Gyroscope driver layer for the ITG3200.
 */

#ifndef _GYRO_DRIVER_ITG3200_H_
#define _GYRO_DRIVER_ITG3200_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define GYRO_ITG3200_DOF_COUNT 3

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Gyroscope ITG3200 info type.
 */
typedef struct _gyro_itg3200_info_t_
{
    int a; // TODO Add info
} gyro_itg3200_info_t;

/**
 * @brief Gyroscope ITG3200 type.
 */
typedef struct _gyro_itg3200_t_
{
    i2c_device_id_t* i2cDeviceId;
    generic_data_t private[sizeof(gyro_itg3200_info_t)];
} gyro_itg3200_t;

/**
 * @brief Gyroscope ITG3200 configuration type.
 */
typedef struct _gyro_itg3200_config_t_
{
    int a; // TODO Add configuration
} gyro_itg3200_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the gyroscope driver layer.
 */
void gyroDriverItg3200Init(void);

#endif // _GYRO_DRIVER_ITG3200_H_
