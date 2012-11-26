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
 * @file mutex_interface.c
 * @author Ben Minerd
 * @date 10/9/2012
 * @brief Mutex interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

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
 * @brief Mutex driver.
 */
static mutex_driver_t driver;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void mutexInit(void)
{
    mutexDriverInit();
}

//------------------------------------------------------------------------------
bool mutexSetDriver(const mutex_driver_t* mutexDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(mutexDriver) || IS_NULL_POINTER(mutexDriver->setLocked))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    driver = *mutexDriver;

    return true;
}

//------------------------------------------------------------------------------
bool mutexInitMutex(mutex_t* mutex)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(mutex))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    driver.init(mutex);

    return true;
}

//------------------------------------------------------------------------------
bool mutexSetLocked(mutex_t* mutex, const bool locked)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(mutex))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    driver.setLocked(mutex, locked);

    return true;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

