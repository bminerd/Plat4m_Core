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
 * @file gpio_driver_template.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief
 */

#ifndef _GPIO_DRIVER_H_
#define _GPIO_DRIVER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

typedef enum _gpio_driver_id_e_
{
    // plat4m
    //
    // Add GPIO driver IDs here.
    //
    // Example:
    //
    //      GPIO_DRIVER_ID_LED_1,
    //      ...
    //
    // Note: first GPIO driver ID should be set to 0 (zero).
    //
    // Example:
    //
    //      GPIO_DRIVER_ID_LED_1 = 0,
    //      GPIO_DRIVER_ID_LED_2,
    //      ...

    // Do not place values below!
    GPIO_DRIVER_ID_COUNT
} gpio_driver_id_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
extern void gpioDriverInit(void);

#endif // _GPIO_DRIVER_H_
