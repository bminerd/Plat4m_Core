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
 * @file accel_driver_adxl345.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Accelerometer driver layer for the ADXL345.
 */

#ifndef _ACCEL_DRIVER_ADXL345_H_
#define _ACCEL_DRIVER_ADXL345_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <accel_application.h>

#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define ACCEL_ADXL345_DOF_COUNT 3

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Accelerometer ADXL345 info type.
 */
typedef struct _accel_adxl345_info_t_
{
    int a; // TODO Add info
} accel_adxl345_info_t;

/**
 * @brief Accelerometer ADXL345 type.
 */
typedef struct _accel_adxl345_t_
{
    i2c_device_id_t* i2cDeviceId;
    generic_data_t private[sizeof(accel_adxl345_info_t)];
} accel_adxl345_t;

/**
 * @brief Accelerometer ADXL345 configuration type.
 */
typedef struct _accel_adxl435_config_t_
{
    int a; // TODO Add configuration
} accel_adxl345_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the accelerometer driver layer.
 */
void accelDriverAdxl345Init(void);

#endif // _ACCEL_DRIVER_ADXL345_H_
