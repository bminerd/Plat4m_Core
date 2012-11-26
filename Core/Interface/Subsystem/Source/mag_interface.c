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
 * @file mag_interface.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Magnetometer interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <mag_interface.h>
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
 * @brief Magnetometer wrapper_type.
 */
typedef struct _mag_wrapper_t_
{
    mag_id_t id;
    bool isEnabled;
    mutex_t mutex;
} mag_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Magnetometer driver.
 */
static mag_driver_t driver;

/**
 * @brief ID to Magnetometer map.
 */
static mag_wrapper_t idToMagMap[MAG_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void magInit(void)
{
    int i;

    for (i = 0; i < MAG_ID_COUNT; i++)
    {
        idToMagMap[i].id.id     = MAG_ID_NONE;
        // Remaining mag_id_t fields are driver specific

        idToMagMap[i].isEnabled = false;

        mutexInitMutex(&(idToMagMap[i].mutex));
    }

	magApplicationInit();
}

//------------------------------------------------------------------------------
mag_error_e magSetDriver(const mag_driver_t* magDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(magDriver))
    {
        return MAG_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *magDriver;

    return MAG_ERROR_NONE;
}

//------------------------------------------------------------------------------
mag_error_e magAddId(const mag_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return MAG_ERROR_PARAMETER_INVALID;
    }

    if ((id->id == MAG_ID_NONE) || (id->id >= MAG_ID_COUNT))
    {
        return MAG_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToMagMap[id->id].id = *id;

    return MAG_ERROR_NONE;
}

//------------------------------------------------------------------------------
mag_error_e magAddIds(const mag_id_t ids[], const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > MAG_ID_COUNT)
    {
        return MAG_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        mag_error_e error = magAddId(&(ids[i]));

#ifdef PLAT4M_DEBUG

        if (error != MAG_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return MAG_ERROR_NONE;
}

//------------------------------------------------------------------------------
mag_error_e magSetEnabled(const mag_id_e id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == MAG_ID_NONE) || (id >= MAG_ID_COUNT))
    {
        return MAG_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), true);

    mag_error_e error = driver.setEnabled(idToMagMap[id].id.mag, enabled);

    if (error == MAG_ERROR_NONE)
    {
        idToMagMap[id].isEnabled = enabled;
    }

    // Unlock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
mag_error_e magIsEnabled(const mag_id_e id, bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(enabled))
    {
        return MAG_ERROR_PARAMETER_INVALID;
    }

    if ((id == MAG_ID_NONE) || (id >= MAG_ID_COUNT))
    {
        return MAG_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), true);

    *enabled = idToMagMap[id].isEnabled;

    // Unlock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), false);

    return MAG_ERROR_NONE;
}

//------------------------------------------------------------------------------
mag_error_e magConfigure(const mag_id_e id, const mag_config_t* magConfig)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(magConfig))
    {
        return MAG_ERROR_PARAMETER_INVALID;
    }

    if ((id == MAG_ID_NONE) || (id >= MAG_ID_COUNT))
    {
        return MAG_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), true);

    mag_error_e error = driver.configure(idToMagMap[id].id.mag, magConfig);

    // Unlock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
mag_error_e magRead(const mag_id_e id, mag_reading_t* reading)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(reading))
    {
        return MAG_ERROR_PARAMETER_INVALID;
    }

    if ((id == MAG_ID_NONE) || (id >= MAG_ID_COUNT))
    {
        return MAG_ERROR_ID_INVALID;
    }

    if (!(idToMagMap[id].isEnabled))
    {
        return MAG_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), true);

    mag_error_e error = driver.read(idToMagMap[id].id.mag, reading);

    // Unlock mutex
    mutexSetLocked(&(idToMagMap[id].mutex), false);

	return error;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
