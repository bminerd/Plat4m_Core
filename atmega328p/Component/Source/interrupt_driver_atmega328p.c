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
 * @file interrupt_driver_stm32f2xx.c
 * @author Ben Minerd
 * @date 8/2/12
 * @brief  TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <interrupt_interface.h>
#include <interrupt_driver_stm32f2xx.h>

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
// Declare local interrupt driver functions here.
//
// Example:
//
//      void button1IntSetEnabled(bool setEnabled);
//      void button1IntHandler(void);
//      ...

/**
 * TODO Comment!
 */
static void sysTickIntSetEnabled(bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void interruptDriverInit(void)
{
    // plat4m
    //
    // Initialize interrupt driver.
    // Add drivers to interrupt interface.
    //
    // Example:
    //
    //      interrupt_driver_t interrupts[] =
    //      {
    //          // Button1
    //          {
    //              .id         = INTERRUPT_DRIVER_ID_BUTTON1,
    //              .setEnabled = button1IntSetEnabled,
    //              ...
    //          },
    //          // Button2
    //          {
    //              .id         = INTERRUPT_DRIVER_ID_BUTTON2,
    //              .setEnabled = button2IntSetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      interruptAddDrivers(interrupts, ARRAY_SIZE(interrupts));
    
    interrupt_driver_t interrupts[] =
    {
        // SysTick
        {
            .id         = INTERRUPT_ID_SYS_TICK,
            .isEnabled  = true,
            .setEnabled = sysTickIntSetEnabled
        }
    };
    
    interruptAddDrivers(interrupts, ARRAY_SIZE(interrupts));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void sysTickIntSetEnabled(bool enabled)
{
    if (enabled)
    {
        SysTick_Config(SystemCoreClock / 1000);
    }
    else
    {

    }
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    interruptHandler(INTERRUPT_ID_SYS_TICK);
}
