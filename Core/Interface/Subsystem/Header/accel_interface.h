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
 * @file accel_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Accelerometer interface layer.
 */

#ifndef _ACCEL_INTERFACE_H_
#define _ACCEL_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <accel_application.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of accelerometer errors.
 */
typedef enum _accel_error_e_
{
    ACCEL_ERROR_NONE,
    ACCEL_ERROR_PARAMETER_INVALID,
    ACCEL_ERROR_ID_INVALID,
    ACCEL_ERROR_NOT_INITIALIZED,
    ACCEL_ERROR_NOT_ENABLED,
    ACCEL_ERROR_COMMUNICATION
} accel_error_e;

/**
 * @brief Enumeration of accelerometer degrees of freedom.
 */
typedef enum _accel_dof_e_
{
    ACCEL_DOF_X = 0,
    ACCEL_DOF_Y,
    ACCEL_DOF_Z
} accel_dof_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Accelerometer type.
 */
typedef void accel_t;

/**
 * @brief Accelerometer configuration type.
 */
typedef void accel_config_t;

/**
 * @brief Accelerometer type.
 */
typedef struct _accel_info_t_
{
    bool isInitialized;
    bool isEnabled;
    mutex_t mutex;
} accel_info_t;

/**
 * @brief Accelerometer ID type.
 */
typedef struct _accel_id_t_
{
    const accel_t* accel;
    const generic_data_t private[sizeof(accel_info_t) / sizeof(generic_data_t)];
} accel_id_t;

/**
 * @brief Accelerometer sample type.
 */
typedef uint16_t accel_sample_t;

/**
 * @brief Accelerometer reading type.
 */
typedef struct _accel_reading_t_
{
    accel_sample_t samples[ACCEL_DOF_COUNT];
} accel_reading_t;

/**
 * @brief Function type that sets the accelerometer enabled or disabled.
 * @param accel Accelerometer to access.
 * @param enabled Flag that indicates if the accelerometer should be enabled or
 * disabled.
 * @return Accelerometer error.
 */
typedef accel_error_e accel_driver_set_enabled_f(const accel_t* accel,
                                                 const bool enabled);

/**
 * @brief Function type that configures the accelerometer.
 * @param accel Accelerometer to access.
 * @param config Accelerometer config.
 * @return Accelerometer error.
 */
typedef accel_error_e accel_driver_configure_f(const accel_t* accel,
                                               const accel_config_t* config);

/**
 * @brief Function type that gets a reading from the accelerometer.
 * @param accel Accelerometer to access.
 * @param reading Accelerometer reading.
 * @return Accelerometer error.
 */
typedef accel_error_e accel_driver_read_f(const accel_t* accel,
                                          accel_reading_t* reading);

/**
 * @brief Accelerometer driver type.
 */
typedef struct _accel_driver_t_
{
    accel_driver_set_enabled_f* setEnabled;
    accel_driver_configure_f* configure;
    accel_driver_read_f* read;
} accel_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the accelerometer module.
 */
void accelInit(void);

/**
 * @brief Sets the driver for the accelerometer interface.
 * @param accelDriver Accelerometer driver.
 * @return Accelerometer error.
 */
accel_error_e accelSetDriver(const accel_driver_t* accelDriver);

/**
 * @brief Adds the given accelerometer.
 * @param accel Accelerometer to add.
 * @return Accelerometer error.
 */
accel_error_e accelInitId(accel_id_t* id);

/**
 * @brief Sets the given accelerometer enabled or disabled.
 * @param id Accelerometer to access.
 * @param enabled Flag that indicates if the accelerometer should be enabled or
 * disabled.
 * @return Accelerometer error.
 */
accel_error_e accelSetEnabled(accel_id_t* id, const bool enabled);

/**
 * @brief Checks to see if the given accelerometer is enabled or disabled.
 * @param id Accelerometer to access.
 * @param enabled Flag that indicates if the accelerometer is enabled or
 * disabled.
 * @return Accelerometer error.
 */
accel_error_e accelIsEnabled(accel_id_t* id, bool* enabled);

/**
 * @brief Configures the given accelerometer.
 * @param id Accelerometer to access.
 * @param config Accelerometer configuration.
 * @return Accelerometer error.
 */
accel_error_e accelConfigure(accel_id_t* id, const accel_config_t* config);

/**
 * @brief Gets a given accelerometer reading.
 * @param id Accelerometer to access.
 * @param reading Accelerometer reading.
 * @return Accelerometer error.
 */
accel_error_e accelRead(accel_id_t* id, accel_reading_t* reading);

#endif // _ACCEL_INTERFACE_H_
