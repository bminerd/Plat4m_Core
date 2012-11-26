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
 * @file baro_interface.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Barometer interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <baro_interface.h>
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
 * @brief Barometer wrapper type.
 */
typedef struct _baro_wrapper_t_
{
    baro_id_t id;
    bool isEnabled;
    mutex_t mutex;
} baro_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Barometer driver.
 */
static baro_driver_t driver;

/**
 * @brief Barometer map.
 */
static baro_wrapper_t idToBaroMap[BARO_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void baroInit(void)
{
    int i;

    for (i = 0; i < BARO_ID_COUNT; i++)
    {
        idToBaroMap[i].id.id        = BARO_ID_NONE;
        // Remaining baro_t fields are driver specific

        idToBaroMap[i].isEnabled    = false;
        mutexInitMutex(&(idToBaroMap[i].mutex));
    }

    baroApplicationInit();
}

//------------------------------------------------------------------------------
baro_error_e baroSetDriver(const baro_driver_t* baroDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(baroDriver)             ||
        IS_NULL_POINTER(baroDriver->setEnabled) ||
        IS_NULL_POINTER(baroDriver->configure)  ||
        IS_NULL_POINTER(baroDriver->read))
    {
        return BARO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *baroDriver;

    return BARO_ERROR_NONE;
}

//------------------------------------------------------------------------------
baro_error_e baroAddId(const baro_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return BARO_ERROR_PARAMETER_INVALID;
    }

    if ((id->id == BARO_ID_NONE) || (id->id >= BARO_ID_COUNT))
    {
        return BARO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToBaroMap[id->id].id = *id;

    return BARO_ERROR_NONE;
}

//------------------------------------------------------------------------------
baro_error_e baroAddIds(const baro_id_t ids[], const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > BARO_ID_COUNT)
    {
        return BARO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        baro_error_e error = baroAddId(&(ids[i]));

#ifdef PLAT4M_DEBUG

        if (error != BARO_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return BARO_ERROR_NONE;
}

//------------------------------------------------------------------------------
baro_error_e baroSetEnabled(const baro_id_e id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == BARO_ID_NONE) || (id >= BARO_ID_COUNT))
    {
        return BARO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), true);

    baro_error_e error = driver.setEnabled(idToBaroMap[id].id.baro, enabled);

    if (error == BARO_ERROR_NONE)
    {
        idToBaroMap[id].isEnabled = enabled;
    }

    // Unlock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
baro_error_e baroIsEnabled(const baro_id_e id, bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(enabled))
    {
        return BARO_ERROR_PARAMETER_INVALID;
    }

    if ((id == BARO_ID_NONE) || (id >= BARO_ID_COUNT))
    {
        return BARO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), true);

    *enabled = idToBaroMap[id].isEnabled;

    // Unlock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), false);

    return BARO_ERROR_NONE;
}

//------------------------------------------------------------------------------
baro_error_e baroConfigure(const baro_id_e id, const baro_config_t* baroConfig)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(baroConfig))
    {
        return BARO_ERROR_PARAMETER_INVALID;
    }

    if ((id == BARO_ID_NONE) || (id >= BARO_ID_COUNT))
    {
        return BARO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), true);

    baro_error_e error = driver.configure(idToBaroMap[id].id.baro, baroConfig);

    // Unlock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
baro_error_e baroRead(const baro_id_e id, baro_reading_t* reading)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(reading))
    {
        return BARO_ERROR_PARAMETER_INVALID;
    }

    if ((id == BARO_ID_NONE) || (id >= BARO_ID_COUNT))
    {
        return BARO_ERROR_ID_INVALID;
    }

    if (!(idToBaroMap[id].isEnabled))
    {
        return BARO_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), true);

    baro_error_e error = driver.read(idToBaroMap[id].id.baro, reading);

    // Unlock mutex
    mutexSetLocked(&(idToBaroMap[id].mutex), false);

    return error;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
