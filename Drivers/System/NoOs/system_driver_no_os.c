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
 * @file system_driver_no_os.c
 * @author Ben Minerd
 * @date 10/25/2012
 * @brief System driver layer for no OS.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_driver_no_os.h>
#include <system_interface.h>

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
 * @brief System run time in milliseconds.
 */
static volatile uint32_t systemTimeMs = 0;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type that handles the system time MS timer.
 */
static void timeMsHandler(void);

/**
 * @brief Function type that gets the current system time in milliseconds.
 * @return System time in milliseconds.
 */
static uint32_t timeMsGet(void);

/**
 * @brief Function type that delays the system for the given millisecond delay.
 * @param delayMs Delay in milliseconds.
 */
static void delayMs(const uint32_t delayMs);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void systemDriverNoOsInit(void)
{
    system_driver_t driver =
    {
        .timeMsHandler  = timeMsHandler,
        .timeMsGet      = timeMsGet,
        .delayMs        = delayMs
    };

    systemSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void timeMsHandler(void)
{
    systemTimeMs++;
}

//------------------------------------------------------------------------------
static uint32_t timeMsGet(void)
{
    return systemTimeMs;
}

//------------------------------------------------------------------------------
static void delayMs(const uint32_t delayMs)
{
    uint32_t delayTimeMs = systemTimeMs + delayMs;

    while (systemTimeMs < delayTimeMs);
}

