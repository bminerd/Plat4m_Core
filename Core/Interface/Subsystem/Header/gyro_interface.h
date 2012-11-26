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
 * @file gyro_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Gyroscope interface layer.
 */

#ifndef _GYRO_INTERFACE_H_
#define _GYRO_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <gyro_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of gyroscope errors.
 */
typedef enum _gyro_error_e_
{
    GYRO_ERROR_NONE,
    GYRO_ERROR_PARAMETER_INVALID,
    GYRO_ERROR_ID_INVALID,
    GYRO_ERROR_NOT_ENABLED,
    GYRO_ERROR_COMMUNICATION
} gyro_error_e;

/**
 * @brief Enumeration of gyroscope degrees of freedom.
 */
typedef enum _gyro_dof_e_
{
    GYRO_DOF_YAW = 0,
    GYRO_DOF_PITCH,
    GYRO_DOF_ROLL
} gyro_dof_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Gyroscope type.
 */
typedef void gyro_t;

/**
 * @brief Gyroscope configuration type.
 */
typedef void gyro_config_t;

/**
 * @brief Gyroscope ID type.
 */
typedef struct _gyro_id_t_
{
    gyro_id_e id;
    const gyro_t* gyro;
} gyro_id_t;

/**
 * @brief Gyroscope sample type.
 */
typedef uint16_t gyro_sample_t;

/**
 * @brief Gyroscope reading type.
 */
typedef struct _gyro_reading_t_
{
	gyro_sample_t temp;
	gyro_sample_t samples[GYRO_DOF_COUNT];
} gyro_reading_t;

/**
 * @brief Function type that sets the gyroscope enabled or disabled.
 * @param gyro Gyroscope to access.
 * @param enabled Flag that indicates if the gyroscope should be enabled or
 * disabled.
 * @return Gyroscope error.
 */
typedef gyro_error_e gyro_driver_set_enabled_f(const gyro_t* gyro,
                                               const bool enabled);

/**
 * @brief Function type that configures the gyroscope.
 * @param gyro Gyroscope to access.
 * @param config Gyroscope config.
 * @return Gyroscope error.
 */
typedef gyro_error_e gyro_driver_configure_f(const gyro_t* gyro,
                                             const gyro_config_t* config);

/**
 * @brief Function type that gets a reading from the gyroscope.
 * @param gyro Gyroscope to access.
 * @param reading Gyroscope reading.
 * @return Gyroscope error.
 */
typedef gyro_error_e gyro_driver_read_f(const gyro_t* gyro,
                                        gyro_reading_t* reading);

/**
 * @brief Gyroscope driver type.
 */
typedef struct _gyro_driver_t_
{
    gyro_driver_set_enabled_f* setEnabled;
    gyro_driver_configure_f* configure;
    gyro_driver_read_f* read;
} gyro_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the gyro module.
 */
void gyroInit(void);

/**
 * @brief Sets the driver for the gyroscope interface.
 * @param gyroDriver Gyroscope driver.
 * @return Gyroscope error.
 */
gyro_error_e gyroSetDriver(const gyro_driver_t* gyroDriver);

/**
 * @brief Adds the given gyroscope ID.
 * @param id Gyroscope ID to add.
 * @return Gyroscope error.
 */
gyro_error_e gyroAddId(const gyro_id_t* id);

/**
 * @brief Adds the given gyroscope IDs.
 * @param ids Gyroscope IDs to add.
 * @param size Number of gyroscopes to add.
 * @return Gyroscope error.
 */
gyro_error_e gyroAddIds(const gyro_id_t ids[], const unsigned int size);

/**
 * @brief Sets the given gyroscope enabled or disabled.
 * @param id Gyroscope to access.
 * @param enabled Flag that indicates if the gyroscope should be enabled or
 * disabled.
 * @return Gyroscope error.
 */
gyro_error_e gyroSetEnabled(const gyro_id_e id, const bool enabled);

/**
 * @brief Checks to see if the given gyroscope is enabled or disabled.
 * @param id Gyroscope to access.
 * @param enabled Flag that indicates if the gyroscope is enabled or
 * disabled.
 * @return Gyroscope error.
 */
gyro_error_e gyroIsEnabled(const gyro_id_e id, bool* enabled);

/**
 * @brief Configures the given gyroscope.
 * @param id Gyroscope to access.
 * @param gyroConfig Gyroscope configuration.
 * @return Gyroscope error.
 */
gyro_error_e gyroConfigure(const gyro_id_e id, const gyro_config_t* gyroConfig);

/**
 * @brief Gets a given gyroscope reading.
 * @param id Gyroscope to access.
 * @param reading Gyroscope reading.
 * @return Gyroscope error.
 */
gyro_error_e gyroRead(const gyro_id_e id, gyro_reading_t* reading);

#endif // _GYRO_INTERFACE_H_
