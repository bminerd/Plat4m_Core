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
 * @file mag_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Magnetometer interface layer.
 */

#ifndef _MAG_INTERFACE_H_
#define _MAG_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <mag_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of magnetometer errors.
 */
typedef enum _mag_error_e_
{
    MAG_ERROR_NONE,
    MAG_ERROR_PARAMETER_INVALID,
    MAG_ERROR_ID_INVALID,
    MAG_ERROR_NOT_ENABLED,
    MAG_ERROR_COMMUNICATION
} mag_error_e;

/**
 * @brief Enumeration of magnetometer degrees of freedom.
 */
typedef enum _mag_dof_e_
{
    MAG_DOF_AZIMUTH = 0,
    MAG_DOF_ELEVATION,
    MAG_DOF_BANK
} mag_dof_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Magnetometer type.
 */
typedef void mag_t;

/**
 * @brief Magnetometer configuration type.
 */
typedef void mag_config_t;

/**
 * @brief Magnetometer ID type.
 */
typedef struct _mag_id_t_
{
    mag_id_e id;
    const mag_t* mag;
} mag_id_t;

/**
 * @brief Magnetometer sample type.
 */
typedef uint16_t mag_sample_t;

/**
 * @brief Magnetomter reading type.
 */
typedef struct _mag_reading_t_
{
    mag_sample_t samples[MAG_DOF_COUNT];
} mag_reading_t;

/**
 * @brief Function type that sets the magnetometer enabled or disabled.
 * @param mag Magnetometer to access.
 * @param enabled Flag that indicates if the magnetometer should be enabled or
 * disabled.
 * @return Magnetometer error.
 */
typedef mag_error_e mag_driver_set_enabled_f(const mag_t* mag,
                                             const bool enabled);

/**
 * @brief Function type that configures the magnetometer.
 * @param mag Magnetometer to access.
 * @param config Magnetometer configuration.
 * @return Magnetometer error.
 */
typedef mag_error_e mag_driver_configure_f(const mag_t* mag,
                                           const mag_config_t* config);

/**
 * @brief Function type that gets a reading from the magnetometer.
 * @param mag Magnetometer to access.
 * @param reading Magnetometer reading.
 * @return Magnetometer error.
 */
typedef mag_error_e mag_driver_read_f(const mag_t* mag, mag_reading_t* reading);

/**
 * @brief Magnetometer driver type.
 */
typedef struct _mag_driver_t_
{
    mag_driver_set_enabled_f* setEnabled;
    mag_driver_configure_f* configure;
    mag_driver_read_f* read;
} mag_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the magnetometer module.
 */
void magInit(void);

/**
 * @brief Sets the driver for the magnetometer interface.
 * @param magDriver Magnetometer driver.
 * @return Magnetometer error.
 */
mag_error_e magSetDriver(const mag_driver_t* magDriver);

/**
 * @brief Adds the given magnetometer ID.
 * @param id Magnetometer ID to add.
 * @return Magnetometer error.
 */
mag_error_e magAddId(const mag_id_t* id);

/**
 * @brief Adds the given magnetometer IDs.
 * @param ids Magnetometer IDs to add.
 * @param size Number of magnetometers to add.
 * @return Magnetometer error.
 */
mag_error_e magAddIds(const mag_id_t ids[], const unsigned int size);

/**
 * @brief Sets the magnetometer enabled or disabled.
 * @param id Magnetometer to access.
 * @param enabled Flag that indicates if the magnetometer should be enabled or
 * disabled.
 * @return Magnetometer error.
 */
mag_error_e magSetEnabled(const mag_id_e id, const bool enabled);

/**
 * @brief Checks to see if the magnetometer is enabled or disabled.
 * @param id Magnetometer to access.
 * @param enabled Flag that indicates if the magnetometer is enabled or
 * disabled.
 * @return Magnetometer error.
 */
mag_error_e magIsEnabled(const mag_id_e id, bool* enabled);

/**
 * @brief Configures the magnetometer.
 * @param id Magnetometer to access.
 * @param magConfig Magnetometer configuration.
 * @return Magnetometer error.
 */
mag_error_e magConfigure(const mag_id_e id, const mag_config_t* magConfig);

/**
 * @brief Gets a magnetometer reading.
 * @param id Magnetometer to access.
 * @param reading Magnetometer reading.
 * @return Magnetometer error.
 */
mag_error_e magRead(const mag_id_e id, mag_reading_t* reading);

#endif // _MAG_INTERFACE_H_
