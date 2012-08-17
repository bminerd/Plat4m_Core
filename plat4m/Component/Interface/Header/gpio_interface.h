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
 * @file gpio_interface.h
 * @author Ben Minerd
 * @date 2/3/12
 * @brief TODO Comment!
 */

#ifndef _GPIO_INTERFACE_H_
#define _GPIO_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <gpio_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _gpio_error_e
{
    GPIO_ERROR_NONE,
    GPIO_ERROR_INVALID_ID,

    // Do not place values below!
    GPIO_ERROR_COUNT
} gpio_error_e;

/**
 * TODO Comment!
 */
typedef enum _gpio_level_e_
{
	GPIO_LEVEL_LOW = 0,
	GPIO_LEVEL_HIGH,

	// Do not place values below!
	GPIO_LEVEL_MAX
} gpio_level_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef set_enabled_f gpio_driver_set_enabled_f;

/**
 * TODO Comment!
 */
typedef void gpio_driver_set_level_f(gpio_level_e level);

/**
 * TODO Comment!
 */
typedef struct _gpio_driver_t_
{
    gpio_driver_id_e id;
    gpio_driver_set_enabled_f* setEnabled;
    gpio_driver_set_level_f* setLevel;
} gpio_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void gpioInit(void);

/**
 * TODO Comment!
 */
extern bool gpioAddDriver(gpio_driver_t* gpioDriver);

/**
 * TODO Comment!
 */
extern bool gpioAddDrivers(gpio_driver_t gpioDrivers[], uint8_t size);

/**
 * TODO Comment!
 */
extern gpio_error_e gpioSetEnabled(gpio_driver_id_e id, bool enabled);

/**
 * TODO Comment!
 */
extern gpio_error_e gpioSetLevel(gpio_driver_id_e id, gpio_level_e level);

#endif // _GPIO_INTERFACE_H_
