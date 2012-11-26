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
 * @file dac_interface.h
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC interface layer.
 */

#ifndef _DAC_INTERFACE_H_
#define _DAC_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <dac_application.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of DAC errors.
 */
typedef enum _dac_error_e_
{
    DAC_ERROR_NONE,
    DAC_ERROR_PARAMETER_INVALID,
    DAC_ERROR_DAC_INVALID,
    DAC_ERROR_CHANNEL_INVALID,
    DAC_ERROR_NOT_INITIALIZED,
    DAC_ERROR_NOT_ENABLED
} dac_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief DAC type.
 */
typedef struct _dac_t_
{
    dac_e dac;
    dac_channel_e channel;
} dac_t;

/**
 * @brief DAC configuration type.
 */
typedef struct _dac_config_t_
{
    int a;
} dac_config_t;

/**
 * @brief DAC type.
 */
typedef struct _dac_info_t_
{
    bool isInitialized;
    bool isEnabled;
    mutex_t mutex;
    dac_config_t config;
} dac_info_t;

/**
 * @brief DAC ID type.
 */
typedef struct _dac_id_t_
{
    const dac_t dac;
    const generic_data_t private[sizeof(dac_info_t) / sizeof(generic_data_t)];
} dac_id_t;

/**
 * @brief Function type that sets the given DAC enabled or disabled.
 * @param dac DAC to access.
 * @param enabled Flag that indicates if the DAC should be enabled or disabled.
 */
typedef void dac_driver_set_enabled_f(const dac_t* dac, const bool enabled);

/**
 * @brief Function type that configures the given DAC.
 * @param dac DAC to access.
 * @param config DAC configuration.
 */
typedef void dac_driver_configure_f(const dac_t* dac,
                                    const dac_config_t* config);

/**
 * @brief Function type that sets the given voltage on the given DAC.
 * @param dac DAC to access.
 * @param voltage Voltage to set on the DAC.
 */
typedef void dac_driver_set_f(const dac_t* dac, const float voltage);

/**
 * @brief DAC driver type.
 */
typedef struct _dac_driver_t_
{
    dac_driver_set_enabled_f* setEnabled;
    dac_driver_configure_f* configure;
    dac_driver_set_f* set;
} dac_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the DAC module.
 */
void dacInit(void);

/**
 * @brief Sets the driver for the DAC interface.
 * @param dacDriver DAC driver.
 * @return DAC error.
 */
dac_error_e dacSetDriver(const dac_driver_t* dacDriver);

/**
 * @brief Adds the given DAC ID.
 * @param dac DAC ID to add.
 * @return DAC error.
 */
dac_error_e dacInitId(dac_id_t* id);

/**
 * @brief Sets the given DAC enabled or disabled.
 * @param id DAC to access.
 * @param enabled Flag that indicates if the DAC should be enabled or disabled.
 * @return DAC error.
 */
dac_error_e dacSetEnabled(dac_id_t* id, const bool enabled);

/**
 * @brief Checks to see if the given DAC is enabled or disabled.
 * @param id DAC to access.
 * @param isEnabled Flag that indicates if the DAC is enabled or disabled.
 * @return DAC error.
 */
dac_error_e dacIsEnabled(dac_id_t* id, bool* isEnabled);

/**
 * @brief Configures the given DAC.
 * @param id DAC to access.
 * @param config DAC configuration.
 * @return DAC error.
 */
dac_error_e dacConfigure(dac_id_t* id, const dac_config_t* config);

/**
 * @brief Sets a voltage on the given DAC.
 * @param id DAC to access.
 * @param voltage Voltage to set on the DAC.
 * @return DAC error.
 */
dac_error_e dacSet(dac_id_t* id, const float voltage);

#endif // _DAC_INTERFACE_H_
