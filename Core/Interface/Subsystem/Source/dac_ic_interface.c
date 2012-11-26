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
 * @file dac_ic_interface.c
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC IC interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <dac_ic_interface.h>
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

/**
 * @brief DAC type.
 */
typedef struct _dac_ic_wrapper_t_
{
    dac_ic_id_t id;
    bool isEnabled;
} dac_ic_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief DAC driver.
 */
static dac_ic_driver_t driver;

/**
 * @brief DAC map.
 */
static dac_ic_wrapper_t idToDacMap[DAC_IC_ID_COUNT];

/**
 * @brief DAC mutex.
 */
static mutex_t mutex;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void dacIcInit(void)
{
    int i;

    for (i = 0; i < DAC_IC_ID_COUNT; i++)
    {
        idToDacMap[i].id.id     = DAC_IC_ID_NONE;
        idToDacMap[i].id.dac    = NULL_POINTER;
        idToDacMap[i].isEnabled = false;
    }

    mutexInitMutex(&mutex);

    dacIcApplicationInit();
}

//------------------------------------------------------------------------------
dac_ic_error_e dacIcSetDriver(const dac_ic_driver_t* dacDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(dacDriver)              ||
        IS_NULL_POINTER(dacDriver->setEnabled)  ||
        IS_NULL_POINTER(dacDriver->configure)   ||
        IS_NULL_POINTER(dacDriver->channelSet))
    {
        return DAC_IC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *dacDriver;

    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_ic_error_e dacIcAddId(const dac_ic_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if ((id->id == DAC_IC_ID_NONE) || (id->id >= DAC_IC_ID_COUNT))
    {
        return DAC_IC_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToDacMap[id->id].id = *id;

    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_ic_error_e dacIcAddIds(const dac_ic_id_t ids[], const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > DAC_IC_ID_COUNT)
    {
        return DAC_IC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        dac_ic_error_e error = dacIcAddId(&(ids[i]));

#ifdef PLAT4M_DEBUG

        if (error != DAC_IC_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_ic_error_e dacIcSetEnabled(const dac_ic_id_e id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == DAC_IC_ID_NONE) || (id >= DAC_IC_ID_COUNT))
    {
        return DAC_IC_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&mutex, true);

    driver.setEnabled(idToDacMap[id].id.dac, enabled);
    idToDacMap[id].isEnabled = enabled;

    // Unlock mutex
    mutexSetLocked(&mutex, false);

    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_ic_error_e dacIcIsEnabled(const dac_ic_id_e id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isEnabled))
    {
        return DAC_IC_ERROR_PARAMETER_INVALID;
    }

    if ((id == DAC_IC_ID_NONE) || (id >= DAC_IC_ID_COUNT))
    {
        return DAC_IC_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&mutex, true);

    *isEnabled = idToDacMap[id].isEnabled;

    // Unlock mutex
    mutexSetLocked(&mutex, false);

    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_ic_error_e dacIcConfigure(const dac_ic_id_e id, const dac_ic_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(config))
    {
        return DAC_IC_ERROR_PARAMETER_INVALID;
    }

    if ((id == DAC_IC_ID_NONE) || (id >= DAC_IC_ID_COUNT))
    {
        return DAC_IC_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&mutex, true);

    driver.configure(idToDacMap[id].id.dac, config);

    // Unlock mutex
    mutexSetLocked(&mutex, false);

    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
dac_ic_error_e dacIcChannelSet(const dac_ic_id_e id,
                               const dac_ic_channel_t channel,
                               const float voltage)
{
#ifdef PLAT4M_DEBUG

    if ((id == DAC_IC_ID_NONE) || (id >= DAC_IC_ID_COUNT))
    {
        return DAC_IC_ERROR_ID_INVALID;
    }

    if (!(idToDacMap[id].isEnabled))
    {
        return DAC_IC_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&mutex, true);

    driver.channelSet(idToDacMap[id].id.dac, channel, voltage);

    // Unlock mutex
    mutexSetLocked(&mutex, false);

    return DAC_IC_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

