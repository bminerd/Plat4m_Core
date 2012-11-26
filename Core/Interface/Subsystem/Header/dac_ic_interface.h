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
 * @file dac_ic_interface.h
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC IC interface layer.
 */

#ifndef _DAC_IC_INTERFACE_H_
#define _DAC_IC_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <dac_ic_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of DAC IC errors.
 */
typedef enum _dac_ic_error_e_
{
    DAC_IC_ERROR_NONE,
    DAC_IC_ERROR_PARAMETER_INVALID,
    DAC_IC_ERROR_ID_INVALID,
    DAC_IC_ERROR_CHANNEL_INVALID,
    DAC_IC_ERROR_NOT_ENABLED,
    DAC_IC_ERROR_COMMUNICATION
} dac_ic_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief DAC IC type.
 */
typedef void dac_ic_t;

/**
 * @brief DAC IC channel type.
 */
typedef int dac_ic_channel_t;

/**
 * @brief DAC IC ID type.
 */
typedef struct _dac_ic_id_t_
{
    dac_ic_id_e id;
    const dac_ic_t* dac;
} dac_ic_id_t;

/**
 * @brief DAC IC configuration type.
 */
typedef void dac_ic_config_t;

/**
 * @brief Function type that sets the given DAC IC enabled or disabled.
 * @param dac DAC IC to access.
 * @param enabled Flag that indicates if the DAC IC should be enabled or
 * disabled.
 * @return DAC IC error.
 */
typedef dac_ic_error_e dac_ic_driver_set_enabled_f(const dac_ic_t* dac,
                                                   const bool enabled);

/**
 * @brief Function type that configures the given DAC IC.
 * @param dac DAC IC to access.
 * @param config DAC IC configuration.
 * @return DAC IC error.
 */
typedef dac_ic_error_e dac_ic_driver_configure_f(const dac_ic_t* dac,
                                                 const dac_ic_config_t* config);

/**
 * @brief Function type that sets the given voltage on the given DAC IC.
 * @param dac DAC IC to access.
 * @param channel DAC IC channel to access.
 * @param voltage Voltage to set on the DAC.
 * @return DAC IC error.
 */
typedef dac_ic_error_e dac_ic_driver_channel_set_f(
                                                const dac_ic_t* dac,
                                                const dac_ic_channel_t channel,
                                                const float voltage);

/**
 * @brief DAC IC driver type.
 */
typedef struct _dac_ic_driver_t_
{
    dac_ic_driver_set_enabled_f* setEnabled;
    dac_ic_driver_configure_f* configure;
    dac_ic_driver_channel_set_f* channelSet;
} dac_ic_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the DAC IC module.
 */
void dacIcInit(void);

/**
 * @brief Sets the driver for the DAC IC interface.
 * @param dacDriver DAC driver.
 * @return DAC IC error.
 */
dac_ic_error_e dacIcSetDriver(const dac_ic_driver_t* dacDriver);

/**
 * @brief Adds the given DAC IC ID.
 * @param dac DAC IC ID to add.
 * @return DAC IC error.
 */
dac_ic_error_e dacIcAddId(const dac_ic_id_t* id);

/**
 * @brief Adds the given DAC IC IDs.
 * @param dacs DAC IC IDs to add.
 * @param size Number of DAC IC IDs to add.
 * @return DAC IC error.
 */
dac_ic_error_e dacIcAddIds(const dac_ic_id_t ids[], const unsigned int size);

/**
 * @brief Sets the given DAC IC enabled or disabled.
 * @param id DAC IC to access.
 * @param enabled Flag that indicates if the DAC IC should be enabled or
 * disabled.
 * @return DAC IC error.
 */
dac_ic_error_e dacIcSetEnabled(const dac_ic_id_e id, const bool enabled);

/**
 * @brief Checks to see if the given DAC IC is enabled or disabled.
 * @param id DAC IC to access.
 * @param isEnabled Flag that indicates if the DAC IC is enabled or disabled.
 * @return DAC IC error.
 */
dac_ic_error_e dacIcIsEnabled(const dac_ic_id_e id, bool* isEnabled);

/**
 * @brief Configures the given DAC IC.
 * @param id DAC IC to access.
 * @param config DAC IC configuration.
 * @return DAC IC error.
 */
dac_ic_error_e dacIcConfigure(const dac_ic_id_e id,
                              const dac_ic_config_t* config);

/**
 * @brief Sets a voltage on the given DAC IC.
 * @param id DAC IC to access.
 * @param voltage Voltage to set on the DAC IC.
 * @return DAC IC error.
 */
dac_ic_error_e dacIcChannelSet(const dac_ic_id_e id,
                               const dac_ic_channel_t channel,
                               const float voltage);

#endif // _DAC_IC_INTERFACE_H_
