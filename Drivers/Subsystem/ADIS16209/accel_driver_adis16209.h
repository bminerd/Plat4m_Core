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
 * @file accel_driver_adis16209.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Accel driver layer for the ADIS16209.
 */

#ifndef _ACCEL_DRIVER_ADIS16209_H_
#define _ACCEL_DRIVER_ADIS16209_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <accel_application.h>
#include <spi_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define ACCEL_ADIS16209_DOF_COUNT 2

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Accelerometer ADIS16209 info type.
 */
typedef struct _accel_adis16209_info_t_
{
    int a; // TODO Add info
} accel_adis16209_info_t;

/**
 * @brief Accelerometer ADIS16209 ID type.
 */
typedef struct _accel_adis16209_t_
{
    spi_device_id_t* spiDeviceId;
    generic_data_t private[sizeof(accel_adis16209_info_t)];
} accel_adis16209_t;

/**
 * @brief Accelerometer ADIS16209 configuration type.
 */
typedef struct _accel_adis16209_config_t_
{
    int a;
} accel_adis16209_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the accelerometer driver layer.
 */
void accelDriverAdis16209Init(void);

#endif // _ACCEL_DRIVER_ADIS16209_H_
