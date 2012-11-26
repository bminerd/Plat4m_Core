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
 * @file compass_interface.h
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief Compass interface layer.
 */

#ifndef _COMPASS_INTERFACE_H_
#define _COMPASS_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <compass_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of compass errors.
 */
typedef enum _compass_error_e_
{
    COMPASS_ERROR_NONE,
    COMPASS_ERROR_PARAMETER_INVALID,
    COMPASS_ERROR_ID_INVALID,
    COMPASS_ERROR_NOT_ENABLED,
    COMPASS_ERROR_COMMUNICATION
} compass_error_e;

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Compass type.
 */
typedef void compass_t;

/**
 * @brief Compass configuration type.
 */
typedef void compass_config_t;

/**
 * @brief Compass ID type.
 */
typedef struct _compass_id_t_
{
    compass_id_e id;
    const compass_t* compass;
} compass_id_t;

/**
 * @brief Compass reading type.
 */
typedef float compass_reading_t;

/**
 * @brief Function type that sets the given compass enabled or disabled.
 * @param compass Compass to access.
 * @param enabled Flag that indicates if the compass should be enabled or
 * disabled.
 * @return Compass error.
 */
typedef compass_error_e compass_driver_set_enabled_f(const compass_t* compass,
                                                     const bool enabled);

/**
 * @brief Function type that configures the compass.
 * @param compass Compass to access.
 * @param config Compass config.
 * @return Compass error.
 */
typedef compass_error_e compass_driver_configure_f(
                                                const compass_t* compass,
                                                const compass_config_t* config);

/**
 * @brief Function type that gets a reading from the compass.
 * @param compass Compass to access.
 * @param reading Compass reading.
 * @return Compass error.
 */
typedef compass_error_e compass_driver_read_f(const compass_t* compass,
                                              compass_reading_t* reading);

/**
 * @brief Compass driver type.
 */
typedef struct _compass_driver_t_
{
    compass_driver_set_enabled_f* setEnabled;
    compass_driver_configure_f* configure;
    compass_driver_read_f* read;
} compass_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the compass module.
 */
void compassInit(void);

/**
 * @brief Sets the driver for the compass interface.
 * @param compassDriver Compass driver.
 * @return Compass error.
 */
compass_error_e compassSetDriver(const compass_driver_t* compassDriver);

/**
 * @brief Adds the given compass.
 * @param id Compass ID to add.
 * @return Compass error.
 */
compass_error_e compassAddId(const compass_id_t* id);

/**
 * @brief Adds the given compasses.
 * @param ids Compass IDs to add.
 * @param size Number of compasses to add.
 * @return Compass error.
 */
compass_error_e compassAddIds(const compass_id_t ids[],
                              const unsigned int size);

/**
 * @brief Sets the given compass enabled or disabled.
 * @param id Compass to access.
 * @param enabled Flag that indicates if the compass should be enabled or
 * disabled.
 * @return Compass error.
 */
compass_error_e compassSetEnabled(const compass_id_e id,
                                  const bool enabled);

/**
 * @brief Checks to see if the given compass is enabled or disabled.
 * @param id Compass to access.
 * @param enabled Flag that indicates if the compass is enabled or
 * disabled.
 * @return Compass error.
 */
compass_error_e compassIsEnabled(const compass_id_e id,
                                 bool* enabled);

/**
 * @brief Configures the given compass.
 * @param id Compass to access.
 * @param config Compass configuration.
 * @return Compass error.
 */
compass_error_e compassConfigure(const compass_id_e id,
                                 const compass_config_t* config);

/**
 * @brief Gets a given compass reading.
 * @param id Compass to access.
 * @param reading Compass reading.
 * @return Compass error.
 */
compass_error_e compassRead(const compass_id_e id,
                            compass_reading_t* reading);

#endif // _COMPASS_INTERFACE_H_
