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
 * @file system_driver.c
 * @author Ben Minerd
 * @date 3/18/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_interface.h>
#include <system_driver.h>

#include <gpio_interface.h>

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
static void led1Procedure(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void systemDriverInit(void)
{
    // plat4m
    //
    // Initialize system driver.
    // Add drivers to SPI interface.
    //
    // Example:
    //
    //      system_procedure_driver_t procedures[] =
    //      {
    //          // LED1 procedure
    //          {
    //              .id         = SYSTEM_DRIVER_PROCEDURE_ID_LED1,
    //              .enabled    = true,
    //              ...
    //          },
    //          // LED2 procedure
    //          {
    //              .id         = SYSTEM_DRIVER_PROCEDURE_ID_LED2,
    //              .enabled    = true,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      systemProcedureAddDrivers(procedures,
    //                                ARRAY_SIZE(procedures,
    //                                           system_procedure_driver_t));
    
    system_procedure_driver_t procedures[] =
    {
        // LED1 procedure
        {
            .id         = SYSTEM_DRIVER_PROCEDURE_ID_LED,
            .enabled    = true,
            .callback   = led1Procedure
        }
    };
    
    systemProcedureAddDrivers(procedures,
                              ARRAY_SIZE(procedures,
                                         system_procedure_driver_t));
    
    gpioInit();
    gpioSetEnabled(GPIO_DRIVER_ID_LED1, true);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void led1Procedure(void)
{
    static uint32_t counter = 0;
    static gpio_level_e level = GPIO_LEVEL_LOW;

    // Test
    if (counter++ >= 1000000)
    {
        gpioSetLevel(GPIO_DRIVER_ID_LED1, level);
        
        if (level == GPIO_LEVEL_LOW)
        {
            level = GPIO_LEVEL_HIGH;
        }
        else
        {
            level = GPIO_LEVEL_LOW;
        }
        
        counter = 0;
    }
}
