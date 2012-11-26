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
 * @file baro_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Barometer interface layer.
 */

#ifndef _BARO_INTERFACE_H_
#define _BARO_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <baro_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * Enumeration of barometer errors.
 */
typedef enum _baro_error_e_
{
    BARO_ERROR_NONE,
    BARO_ERROR_PARAMETER_INVALID,
    BARO_ERROR_ID_INVALID,
    BARO_ERROR_NOT_ENABLED,
    BARO_ERROR_COMMUNICATION
} baro_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Barometer type.
 */
typedef void baro_t;

/**
 * @brief Barometer configuration type.
 */
typedef void baro_config_t;

/**
 * @brief Barometer ID type.
 */
typedef struct _baro_id_t_
{
    baro_id_e id;
    const baro_t* baro;
} baro_id_t;

/**
 * @brief Barometer sample type.
 */
typedef uint16_t baro_sample_t;

/**
 * @brief Barometer reading type.
 */
typedef struct _baro_reading_t_
{
    baro_sample_t sample;
} baro_reading_t;

/**
 * @brief Function type that sets the barometer enabled or disabled.
 * @param baro Barometer to access.
 * @param enabled Flag that indicates if the barometer should be enabled or
 * disabled.
 * @return Barometer error.
 */
typedef baro_error_e baro_driver_set_enabled_f(const baro_t* baro,
                                               const bool enabled);

/**
 * @brief Function type that configures the barometer.
 * @param baro Barometer to access.
 * @param config Barometer configuration.
 * @return Barometer error.
 */
typedef baro_error_e baro_driver_configure_f(const baro_t* baro,
                                             const baro_config_t* config);

/**
 * @brief Function type that gets a reading from the barometer.
 * @param baro Barometer to access.
 * @param reading Barometer reading.
 * @return Barometer error.
 */
typedef baro_error_e baro_driver_read_f(const baro_t* baro,
                                        baro_reading_t* reading);

/**
 * @brief Barometer driver type.
 */
typedef struct _baro_driver_t_
{
    baro_driver_set_enabled_f* setEnabled;
    baro_driver_configure_f* configure;
    baro_driver_read_f* read;
} baro_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the barometer module.
 */
void baroInit(void);

/**
 * @brief Sets the driver for the barometer interface.
 * @param baroDriver Barometer driver.
 * @return Barometer error.
 */
baro_error_e baroSetDriver(const baro_driver_t* baroDriver);

/**
 * @brief Adds the given barometer.
 * @param id Barometer to add.
 * @return Barometer error.
 */
baro_error_e baroAddId(const baro_id_t* id);

/**
 * @brief Adds the given barometers.
 * @param ids Barometers to add.
 * @param size Number of barometers to add.
 * @return Barometer error.
 */
baro_error_e baroAddIds(const baro_id_t ids[], const unsigned int size);

/**
 * @brief Sets the given barometer enabled or disabled.
 * @param id Barometer to access.
 * @param enabled Flag that indicates if the barometer should be enabled or
 * disabled.
 * @return Barometer error.
 */
baro_error_e baroSetEnabled(const baro_id_e id, const bool enabled);

/**
 * @brief Checks to see if the given barometer is enabled or disabled.
 * @param id Barometer to access.
 * @param enabled Flag that indicates if the barometer is enabled or
 * disabled.
 * @return Barometer error.
 */
baro_error_e baroIsEnabled(const baro_id_e id, bool* enabled);

/**
 * @brief Configures the given barometer.
 * @param id Barometer to access.
 * @param baroConfig Barometer configuration.
 * @return Barometer error.
 */
baro_error_e baroConfigure(const baro_id_e id, const baro_config_t* baroConfig);

/**
 * @brief Gets a given barometer reading.
 * @param id Barometer to access.
 * @param reading Barometer reading.
 * @return Barometer error.
 */
baro_error_e baroRead(const baro_id_e id, baro_reading_t* reading);

#endif // _BARO_INTERFACE_H_
