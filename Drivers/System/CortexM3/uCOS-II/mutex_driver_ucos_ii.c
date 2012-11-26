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
 * @file mutex_driver_no_os.c
 * @author Ben Minerd
 * @date 10/9/2012
 * @brief Mutex driver layer for uCOS-II.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <mutex_interface.h>
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

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Locks the given mutex.
 * @param mutex Mutex to access.
 * @param locked Flag that indicates if the mutex should be locked or unlocked.
 */
static void setLocked(mutex_t* mutex, const bool locked);

/**
 * @brief Checks to see if the given mutex is locked or unlocked.
 * @param mutex Mutex to access.
 * @param isLocked Flag that indicates if the mutex is locked or unlocked.
 */
static void isLocked(const mutex_t* mutex, bool* isLocked);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void mutexDriverInit(void)
{
    mutex_driver_t driver =
    {
        .setLocked  = setLocked,
        .isLocked   = isLocked
    };

    mutexSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setLocked(mutex_t* mutex, const bool locked)
{
    while (mutex->locked);

    mutex->locked = locked;
}

//------------------------------------------------------------------------------
static void isLocked(const mutex_t* mutex, bool* isLocked)
{
    *isLocked = mutex->locked;
}
