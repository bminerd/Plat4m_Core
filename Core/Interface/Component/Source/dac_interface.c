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
 * @file dac_interface.c
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <dac_interface.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief DAC driver.
 */
static dac_driver_t driver;

/**
 * @brief DAC map.
 */
static dac_info_t* dacToInfoMap[DAC_COUNT][DAC_CHANNEL_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void dacInit(void)
{
    int i, j;

    for (i = 0; i < DAC_COUNT; i++)
    {
        for (j = 0; j < DAC_CHANNEL_COUNT; j++)
        {
            dacToInfoMap[i][j] = NULL_POINTER;
        }
    }

    dacApplicationInit();
}

//------------------------------------------------------------------------------
dac_error_e dacSetDriver(const dac_driver_t* dacDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(dacDriver)              ||
        IS_NULL_POINTER(dacDriver->setEnabled)  ||
        IS_NULL_POINTER(dacDriver->configure)   ||
        IS_NULL_POINTER(dacDriver->set))
    {
        return DAC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *dacDriver;

    return DAC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_error_e dacInitId(dac_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return DAC_ERROR_PARAMETER_INVALID;
    }

    if (id->dac.dac == DAC_COUNT)
    {
        return DAC_ERROR_DAC_INVALID;
    }

    if (id->dac.channel == DAC_CHANNEL_COUNT)
    {
        return DAC_ERROR_CHANNEL_INVALID;
    }

#endif // PLAT4M_DEBUG

    dac_info_t* info = (dac_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    mutexInitMutex(&(info->mutex));

    dacToInfoMap[id->dac.dac][id->dac.channel] = (dac_info_t*) id->private;

    return DAC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_error_e dacSetEnabled(dac_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return DAC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    dac_info_t* info = (dac_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DAC_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.setEnabled(&(id->dac), enabled);
    info->isEnabled = enabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return DAC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_error_e dacIsEnabled(dac_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return DAC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    dac_info_t* info = (dac_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DAC_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *isEnabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return DAC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_error_e dacConfigure(dac_id_t* id, const dac_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return DAC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    dac_info_t* info = (dac_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DAC_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return DAC_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.configure(&(id->dac), config);
    memcpy((uint8_t*) &(info->config), (uint8_t*) config, sizeof(*config));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return DAC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_error_e dacSet(dac_id_t* id, const float voltage)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return DAC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    dac_info_t* info = (dac_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DAC_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.set(&(id->dac), voltage);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return DAC_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

