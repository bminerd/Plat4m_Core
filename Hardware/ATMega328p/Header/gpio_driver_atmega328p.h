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
 * @file gpio_driver_atmega328p.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief TODO Comment!
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

/**
 * @brief Enumeration of port IDs.
 */
typedef enum _gpio_driver_port_e_
{
    // GPIO driver IDs go here
    // ex.
    //      GPIO_DRIVER_PORT_LED_1
    //
    // Note: first GPIO driver ID should be set to 0 (zero).
    // ex.
    //      GPIO_DRIVER_PORT_LED_1 = 0,
    //      GPIO_DRIVER_PORT_LED_2,z
    //      ...
    
    GPIO_DRIVER_PORT_B = 0,
    GPIO_DRIVER_PORT_C,
    GPIO_DRIVER_PORT_D,

    // Do not place values below!
    GPIO_DRIVER_PORT_COUNT
} gpio_driver_port_e;

/**
 * @brief Enumeration of pin IDs.
 */
typedef enum _gpio_driver_pin_e_
{
    GPIO_DRIVER_PIN_0 = 0,
    GPIO_DRIVER_PIN_1,
    GPIO_DRIVER_PIN_2,
    GPIO_DRIVER_PIN_3,
    GPIO_DRIVER_PIN_4,
    GPIO_DRIVER_PIN_5,
    GPIO_DRIVER_PIN_6,
    GPIO_DRIVER_PIN_7,
    
    // Do not place values below!
    GPIO_DRIVER_PIN_COUNT
} gpio_driver_pin_e;

typedef enum _gpio_driver_alternate_function_e_
{
	GPIO_DRIVER_ALTERNATE_FUNCTION_NONE
} gpio_driver_alternate_function_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the GPIO driver level.
 */
extern void gpioDriverInit(void);

#endif // _GPIO_DRIVER_H_
