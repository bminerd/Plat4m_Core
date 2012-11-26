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
 * @file compass_interface.c
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief Compass interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <compass_interface.h>
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
 * @brief Compass wrapper type.
 */
typedef struct _compass_wrapper_t_
{
    compass_id_t id;
    bool isEnabled;
    mutex_t mutex;
} compass_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Compass driver.
 */
static compass_driver_t driver;

/**
 * @brief Compass map.
 */
static compass_wrapper_t idToCompassMap[COMPASS_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void compassInit(void)
{
    int i;

    for (i = 0; i < COMPASS_ID_COUNT; i++)
    {
        idToCompassMap[i].id.id     = COMPASS_ID_NONE;
        // Remaining compass_id_t fields are driver specific

        idToCompassMap[i].isEnabled = false;

        mutexInitMutex(&(idToCompassMap[i].mutex));
    }

    compassApplicationInit();
}

//------------------------------------------------------------------------------
compass_error_e compassSetDriver(const compass_driver_t* compassDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(compassDriver)              ||
        IS_NULL_POINTER(compassDriver->setEnabled)  ||
        IS_NULL_POINTER(compassDriver->configure)   ||
        IS_NULL_POINTER(compassDriver->read))
    {
        return COMPASS_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *compassDriver;

    return COMPASS_ERROR_NONE;
}

//------------------------------------------------------------------------------
compass_error_e compassAddId(const compass_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return COMPASS_ERROR_PARAMETER_INVALID;
    }

    if ((id->id == COMPASS_ID_NONE) || (id->id >= COMPASS_ID_COUNT))
    {
        return COMPASS_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToCompassMap[id->id].id = *id;

    return COMPASS_ERROR_NONE;
}

//------------------------------------------------------------------------------
compass_error_e compassAddIds(const compass_id_t ids[], const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > COMPASS_ID_COUNT)
    {
        return COMPASS_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        compass_error_e error = compassAddId(&(ids[i]));

#ifdef PLAT4M_DEBUG

        if (error != COMPASS_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return COMPASS_ERROR_NONE;
}

//------------------------------------------------------------------------------
compass_error_e compassSetEnabled(const compass_id_e id,
                                  const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == COMPASS_ID_NONE) || (id >= COMPASS_ID_COUNT))
    {
        return COMPASS_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), true);

    compass_error_e error = driver.setEnabled(idToCompassMap[id].id.compass,
                                              enabled);

    if (error == COMPASS_ERROR_NONE)
    {
        idToCompassMap[id].isEnabled = enabled;
    }

    // Unlock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
compass_error_e compassIsEnabled(const compass_id_e id, bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(enabled))
    {
        return COMPASS_ERROR_PARAMETER_INVALID;
    }

    if ((id == COMPASS_ID_NONE) || (id >= COMPASS_ID_COUNT))
    {
        return COMPASS_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), true);

    *enabled = idToCompassMap[id].isEnabled;

    // Unlock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), false);

    return COMPASS_ERROR_NONE;
}

//------------------------------------------------------------------------------
compass_error_e compassConfigure(const compass_id_e id,
                                 const compass_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(config))
    {
        return COMPASS_ERROR_PARAMETER_INVALID;
    }

    if ((id == COMPASS_ID_NONE) || (id >= COMPASS_ID_COUNT))
    {
        return COMPASS_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), true);

    compass_error_e error = driver.configure(idToCompassMap[id].id.compass,
                                             config);

    // Unlock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
compass_error_e compassRead(const compass_id_e id, compass_reading_t* reading)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(reading))
    {
        return COMPASS_ERROR_PARAMETER_INVALID;
    }

    if ((id == COMPASS_ID_NONE) || (id >= COMPASS_ID_COUNT))
    {
        return COMPASS_ERROR_ID_INVALID;
    }

    if (!(idToCompassMap[id].isEnabled))
    {
        return COMPASS_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), true);

    compass_error_e error = driver.read(idToCompassMap[id].id.compass,
                                        reading);

    // Unlock mutex
    mutexSetLocked(&(idToCompassMap[id].mutex), false);

    return error;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

