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
 * @file gpio_interface.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  GPIO_DRIVER_ID_LIMIT
#define GPIO_DRIVER_COUNT (GPIO_DRIVER_ID_LIMIT)
#else
#define GPIO_DRIVER_COUNT (GPIO_DRIVER_ID_COUNT)
#endif

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
 *
 */
static gpio_driver_t drivers[GPIO_DRIVER_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gpioInit(void)
{
    int i;

    for (i = 0; i < GPIO_DRIVER_COUNT; i++)
    {
        drivers[i].id = (gpio_driver_id_e) i;
        drivers[i].setEnabled = 0;
        drivers[i].setLevel = 0;
    }

    gpioDriverInit();
}

//------------------------------------------------------------------------------
extern bool gpioAddDriver(gpio_driver_t gpioDriver)
{
    if (gpioDriver.id >= GPIO_DRIVER_ID_COUNT ||
        !gpioDriver.setEnabled ||
        !gpioDriver.setLevel)
    {
        return false;
    }

    DRIVER_ADD(drivers, gpioDriver, gpio_driver_t);

    return true;
}

//------------------------------------------------------------------------------
extern bool gpioAddDrivers(gpio_driver_t gpioDrivers[], uint8_t size)
{
    int i;

    if (size > GPIO_DRIVER_COUNT)
    {
        return false;
    }

    for (i = 0; i < size; i++)
    {
        if (!gpioAddDriver(gpioDrivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioSetEnabled(gpio_driver_id_e id, bool enabled)
{
    if (id >= GPIO_DRIVER_ID_COUNT || !drivers[id].setEnabled)
    {
        return GPIO_ERROR_INVALID_ID;
    }

    drivers[id].setEnabled(enabled);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioSetLevel(gpio_driver_id_e id, gpio_level_e level)
{
    if (id >= GPIO_DRIVER_ID_COUNT || !drivers[id].setLevel)
    {
        return GPIO_ERROR_INVALID_ID;
    }

    drivers[id].setLevel(level);

    return GPIO_ERROR_NONE;
}
