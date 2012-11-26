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
 * @file accel_interface.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Accelerometer interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <accel_interface.h>

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
 * @brief Accelerometer driver.
 */
static accel_driver_t driver;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void accelInit(void)
{
    accelApplicationInit();
}

//------------------------------------------------------------------------------
accel_error_e accelSetDriver(const accel_driver_t* accelDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(accelDriver)            ||
        IS_NULL_POINTER(accelDriver->configure) ||
        IS_NULL_POINTER(accelDriver->setEnabled))
    {
        return ACCEL_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *accelDriver;

    return ACCEL_ERROR_NONE;
}

//------------------------------------------------------------------------------
accel_error_e accelInitId(accel_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return ACCEL_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    accel_info_t* info = (accel_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    mutexInitMutex(&(info->mutex));

    return ACCEL_ERROR_NONE;
}

//------------------------------------------------------------------------------
accel_error_e accelSetEnabled(accel_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return ACCEL_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    accel_info_t* info = (accel_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ACCEL_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    accel_error_e error = driver.setEnabled(id->accel, enabled);

    if (error == ACCEL_ERROR_NONE)
    {
        info->isEnabled = enabled;
    }

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
accel_error_e accelIsEnabled(accel_id_t* id, bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(enabled))
    {
        return ACCEL_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    accel_info_t* info = (accel_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ACCEL_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *enabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return ACCEL_ERROR_NONE;
}

//------------------------------------------------------------------------------
accel_error_e accelConfigure(accel_id_t* id, const accel_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return ACCEL_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    accel_info_t* info = (accel_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ACCEL_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    accel_error_e error = driver.configure(id->accel, config);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
accel_error_e accelRead(accel_id_t* id, accel_reading_t* reading)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(reading))
    {
        return ACCEL_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    accel_info_t* info = (accel_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ACCEL_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return ACCEL_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    accel_error_e error = driver.read(id->accel, reading);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
