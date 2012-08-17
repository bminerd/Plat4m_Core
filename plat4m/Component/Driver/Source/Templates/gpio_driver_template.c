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
 * @file gpio_driver_template.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_interface.h>

// plat4m
//
// Replace below with proper GPIO driver header file.
//
// Example:
//
//      #include <gpio_driver_stm32f10x.h>

#include <gpio_driver_template.h>

// plat4m
//
// Include necessary manufacturer GPIO driver header files here.
//
// Example:
//
//      #include <stm32f10x_gpio.h>

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

// plat4m
//
// Declare local GPIO driver functions here.
//
// Example:
//
//      static void ledSetEnabled(bool setEnabled);
//      static void ledSetLevel(gpio_level_e level);
//      ...

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gpioDriverInit(void)
{
    // plat4m
    //
    // Initialize GPIO driver.
    // Add drivers to GPIO interface.
    //
    // Example:
    //
    //      gpio_driver_t gpios[] =
    //      {
    //          // LED1
    //          {
    //              .id         = GPIO_DRIVER_ID_LED1,
    //              .setEnabled = led1SetEnabled,
    //              ...
    //          },
    //          // LED2
    //          {
    //              .id         = GPIO_DRIVER_ID_LED2,
    //              .setEnabled = led2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      gpioAddDrivers(gpios, ARRAY_SIZE(gpios, gpio_driver_t));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local GPIO driver functions declared above.
