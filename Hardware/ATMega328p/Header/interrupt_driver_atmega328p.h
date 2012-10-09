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
 * @file interrupt_stm32f2xx.h
 * @author Ben Minerd
 * @date Aug 2, 2012
 * @brief TODO Comment!
 */

#ifndef _INTERRUPT_DRIVER_H_
#define _INTERRUPT_DRIVER_H_

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
 * TODO Comment!
 */
typedef enum _interrupt_id_e_
{
    // plat4m
    //
    // Add interrupt driver IDs here.
    //
    // Example:
    //
    //      INTERRUPT_ID_BUTTON_2,
    //      ...
    //
    // Note: first interrupt driver ID should be set to 0 (zero).
    //
    // Example:
    //
    //      INTERRUPT_ID_BUTTON_1 = 0,
    //      INTERRUPT_ID_BUTTON_2,
    //      ...
    INTERRUPT_ID_SYS_TICK = 0,

    // Do not place values below!
    INTERRUPT_ID_COUNT
} interrupt_id_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void interruptDriverInit(void);

#endif // _INTERRUPT_DRIVER_H_
