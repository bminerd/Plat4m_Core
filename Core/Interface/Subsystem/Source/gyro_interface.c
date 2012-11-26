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
 * @file gyro_interface.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Gyroscope interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gyro_interface.h>
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
 * @brief Gyroscope wrapper type.
 */
typedef struct _gyro_wrapper_t_
{
    gyro_id_t id;
    bool isEnabled;
    mutex_t mutex;
} gyro_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Gyroscope driver.
 */
static gyro_driver_t driver;

/**
 * @brief Gyroscope map.
 */
static gyro_wrapper_t idToGyroMap[GYRO_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void gyroInit(void)
{
    int i;

    for (i = 0; i < GYRO_ID_COUNT; i++)
    {
        idToGyroMap[i].id.id = GYRO_ID_NONE;
        // Remaining gyro_t fields are driver specific

        idToGyroMap[i].isEnabled   = false;
        mutexInitMutex(&(idToGyroMap[i].mutex));
    }

    gyroApplicationInit();
}

//------------------------------------------------------------------------------
gyro_error_e gyroSetDriver(const gyro_driver_t* gyroDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(gyroDriver)             ||
        IS_NULL_POINTER(gyroDriver->setEnabled) ||
        IS_NULL_POINTER(gyroDriver->configure)  ||
        IS_NULL_POINTER(gyroDriver->read))
    {
        return GYRO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *gyroDriver;

    return GYRO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gyro_error_e gyroAddId(const gyro_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return GYRO_ERROR_PARAMETER_INVALID;
    }

    if ((id->id == GYRO_ID_NONE) || (id->id >= GYRO_ID_COUNT))
    {
        return GYRO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToGyroMap[id->id].id = *id;

    return GYRO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gyro_error_e gyroAddIds(const gyro_id_t ids[], const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > GYRO_ID_COUNT)
    {
        return GYRO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        gyro_error_e error = gyroAddId(&(ids[i]));

#ifdef PLAT4M_DEBUG

        if (error != GYRO_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return GYRO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gyro_error_e gyroSetEnabled(const gyro_id_e id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == GYRO_ID_NONE) || (id >= GYRO_ID_COUNT))
    {
        return GYRO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), true);

    gyro_error_e error = driver.setEnabled(&(idToGyroMap[id].id), enabled);

    if (error == GYRO_ERROR_NONE)
    {
        idToGyroMap[id].isEnabled = enabled;
    }

    // Unlock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
gyro_error_e gyroIsEnabled(const gyro_id_e id, bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(enabled))
    {
        return GYRO_ERROR_PARAMETER_INVALID;
    }

    if ((id == GYRO_ID_NONE) || (id >= GYRO_ID_COUNT))
    {
        return GYRO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), true);

    *enabled = idToGyroMap[id].isEnabled;

    // Unlock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), false);

    return GYRO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gyro_error_e gyroConfigure(const gyro_id_e id, const gyro_config_t* gyroConfig)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(gyroConfig))
    {
        return GYRO_ERROR_PARAMETER_INVALID;
    }

    if ((id == GYRO_ID_NONE) || (id >= GYRO_ID_COUNT))
    {
        return GYRO_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), true);

    gyro_error_e error = driver.configure(&(idToGyroMap[id].id), gyroConfig);

    // Unlock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
gyro_error_e gyroRead(const gyro_id_e id, gyro_reading_t* reading)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(reading))
    {
        return GYRO_ERROR_PARAMETER_INVALID;
    }

    if ((id == GYRO_ID_NONE) || (id >= GYRO_ID_COUNT))
    {
        return GYRO_ERROR_ID_INVALID;
    }

    if (!(idToGyroMap[id].isEnabled))
    {
        return GYRO_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), true);

    gyro_error_e error = driver.read(&(idToGyroMap[id].id), reading);

    // Unlock mutex
    mutexSetLocked(&(idToGyroMap[id].mutex), false);

    return error;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
