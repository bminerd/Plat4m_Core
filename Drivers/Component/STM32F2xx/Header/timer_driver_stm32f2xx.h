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
 * @file timer_driver_stm32f2xx.h
 * @author Ben Minerd
 * @date 10/16/2012
 * @brief Timer driver layer for the STM32F2xx.
 */

#ifndef _TIMER_DRIVER_STM32F2XX_H_
#define _TIMER_DRIVER_STM32F2XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of timers.
 */
typedef enum _timer_e_
{
    // Do not place values above!
    TIMER_NONE = -1,

    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_4,
    TIMER_5,
    TIMER_6,
    TIMER_7,
    TIMER_8,
    TIMER_9,
    TIMER_10,
    TIMER_11,
    TIMER_12,
    TIMER_13,
    TIMER_14,

    // Do not place values below!
    TIMER_COUNT
} timer_e;

/**
 * @brief Enumeration of timer channels.
 */
typedef enum _timer_channel_e_
{
    // Do not place values above!
    TIMER_CHANNEL_NONE = -1,

    TIMER_CHANNEL_1,
    TIMER_CHANNEL_2,
    TIMER_CHANNEL_3,
    TIMER_CHANNEL_4,

    // Do not place values below!
    TIMER_CHANNEL_COUNT
} timer_channel_e;

/**
 * @brief Enumeration of timer interrupts.
 */
typedef enum _timer_interrupt_e_
{
    TIMER_INTERRUPT_OUTPUT_COMPARE
} timer_interrupt_e;

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

///**
// * @brief Function type for a timer interface interrupt handler.
// */
//typedef void timer_interface_int_handler_f(const timer_e timer,
//                                           const timer_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the timer driver layer.
 */
void timerDriverStm32f2xxInit(void);

#endif // _TIMER_DRIVER_STM32F2XX_H_
