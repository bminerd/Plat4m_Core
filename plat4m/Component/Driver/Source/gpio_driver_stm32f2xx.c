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
 * @file gpio_driver_stm32f2xx.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_interface.h>
#include <gpio_driver_stm32f2xx.h>

#include <stm32f2xx.h>
#include <stm322xg_eval.h>

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

/**
 *
 */
static void led1SetEnabled(bool enabled);

/**
 *
 */
static void led1SetLevel(gpio_level_e level);

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
    
    gpio_driver_t gpios[] =
    {
        // LED1
        {
            .id         = GPIO_DRIVER_ID_LED1,
            .setEnabled = led1SetEnabled,
            .setLevel   = led1SetLevel
        }
    };
    
    gpioAddDrivers(gpios, ARRAY_SIZE(gpios, gpio_driver_t));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void led1SetEnabled(bool enabled)
{
    if (enabled)
    {
        STM_EVAL_LEDInit(LED1);
    }
    else
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
static void led1SetLevel(gpio_level_e level)
{
    switch (level)
    {
        case GPIO_LEVEL_HIGH:
        {
            STM_EVAL_LEDOn(LED1);
            
            break;
        }
        case GPIO_LEVEL_LOW:
        {
            STM_EVAL_LEDOff(LED1);
            
            break;
        }
    }
}
