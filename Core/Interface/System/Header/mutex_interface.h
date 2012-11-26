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
 * @file mutex_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Mutex interface layer.
 */

#ifndef _MUTEX_INTERFACE_H_
#define _MUTEX_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <mutex_driver.h>

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
 * @brief Function type that initializes the given mutex.
 * @param mutex Mutex to access.
 */
typedef void mutex_driver_init_f(mutex_t* mutex);

/**
 * @brief Function type that locks the given mutex.
 * @param mutex Mutex to access.
 * @param locked Flag that indicates if the mutex should be locked or unlocked.
 */
typedef void mutex_driver_set_locked_f(mutex_t* mutex, const bool locked);

/**
 * @brief Function type that checks to see if the given mutex is locked or
 * unlocked.
 * @param mutex Mutex to access.
 * @param isLocked Flag that indicates if the mutex is locked or unlocked.
 */
typedef void mutex_driver_is_locked_f(const mutex_t* mutex, bool* isLocked);

/**
 * @brief Mutex driver type.
 */
typedef struct _mutex_driver_t_
{
    mutex_driver_init_f* init;
    mutex_driver_set_locked_f* setLocked;
    mutex_driver_is_locked_f* isLocked;
} mutex_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the mutex module.
 */
void mutexInit(void);

/**
 * @brief Sets the driver for the mutex interface.
 * @param mutexDriver Mutex driver.
 * @return True if the function was successful, false if not.
 */
bool mutexSetDriver(const mutex_driver_t* mutexDriver);

/**
 * @brief Initializes the given mutex.
 *
 * @return True if the function was successful, false if not.
 */
bool mutexInitMutex(mutex_t* mutex);

/**
 * @brief Locks on the given mutex.
 *
 * @return True if the function was successful, false if not.
 */
bool mutexSetLocked(mutex_t* mutex, const bool locked);

#endif // _MUTEX_INTERFACE_H_
