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
 * @file interrupt_driver.h
 * @author Ben Minerd
 * @date 8/2/2012
 * @brief Interrupt driver layer for the STM32F2xx.
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
 * @brief Enumeration of interrupts.
 */
typedef enum _interrupt_e_
{
    // Do not place values above!
    INTERRUPT_NONE = -1,

    INTERRUPT_EXTERNAL_0,
    INTERRUPT_EXTERNAL_1,
    INTERRUPT_EXTERNAL_2,
    INTERRUPT_EXTERNAL_3,
    INTERRUPT_EXTERNAL_4,
    INTERRUPT_EXTERNAL_5,
    INTERRUPT_EXTERNAL_6,
    INTERRUPT_EXTERNAL_7,
    INTERRUPT_EXTERNAL_8,
    INTERRUPT_EXTERNAL_9,
    INTERRUPT_EXTERNAL_10,
    INTERRUPT_EXTERNAL_11,
    INTERRUPT_EXTERNAL_12,
    INTERRUPT_EXTERNAL_13,
    INTERRUPT_EXTERNAL_14,
    INTERRUPT_EXTERNAL_15,

    // Do not place values below!
    INTERRUPT_COUNT
} interrupt_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type for an interface interrupt handler.
 * @param interrupt Interrupt to handle.
 */
typedef void interface_int_handler_f(const interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the interrupt driver layer.
 */
extern void interruptDriverInit(const interface_int_handler_f* handler);

#endif // _INTERRUPT_DRIVER_H_
