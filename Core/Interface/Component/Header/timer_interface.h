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
 * @file timer_interface.h
 * @author Ben Minerd
 * @date 10/16/2012
 * @brief Timer interface layer.
 */

#ifndef _TIMER_INTERFACE_H_
#define _TIMER_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <timer_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of timer errors.
 */
typedef enum _timer_error_e_
{
    TIMER_ERROR_NONE,
    TIMER_ERROR_PARAMETER_INVALID,
    TIMER_ERROR_INVALID,
    TIMER_ERROR_NOT_ENABLED
} timer_error_e;

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Timer configuration type.
 */
typedef struct _timer_config_t_
{
    int a;
} timer_config_t;

/**
 * @brief Timer hardware type.
 */
typedef struct _timer_t_
{
    timer_e timer;
    timer_channel_e channel;
} timer_t;

/**
 * @brief Function type that sets the given timer enabled or disabled.
 * @param timer Timer to access.
 * @param enabled Flag that indicates if the timer should be enabled or
 * disabled.
 * @return Timer error.
 */
typedef timer_error_e timer_driver_set_enabled_f(const timer_t* timer,
                                                 const bool enabled);

/**
 * @brief Function type that configures the given timer.
 * @param timer Timer to access.
 * @param config Timer configuration.
 * @return Timer error.
 */
typedef timer_error_e timer_driver_configure_f(const timer_t* timer,
                                               const timer_config_t* config);

/**
 * @brief Function type that sets the given timer interrupt enabled or disabled.
 * @param timer Timer to access.
 * @param interrupt Interrupt to access.
 * @param enabled Flag that indicates if the given timer interrupt should be
 * enabled or disabled.
 * @return Timer error.
 */
typedef timer_error_e timer_driver_int_set_enabled_f(
                                            const timer_t* timer,
                                            const timer_interrupt_e interrupt,
                                            const bool enabled);

/**
 * @brief Timer driver type.
 */
typedef struct _timer_driver_t_
{
    timer_driver_set_enabled_f* setEnabled;
    timer_driver_configure_f* configure;
    timer_driver_int_set_enabled_f* intSetEnabled;
} timer_driver_t;

/**
 * @brief Function type for a timer interface interrupt handler.
 */
typedef void timer_interface_int_handler_f(const timer_e timer,
                                           const timer_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the timer module.
 */
void timerInit(void);

/**
 * @brief Sets the driver for the timer interface.
 * @param timerDriver Timer driver.
 * @return Timer error.
 */
timer_error_e timerSetDriver(const timer_driver_t* timerDriver);

/**
 * @brief Sets the given timer enabled or disabled.
 * @param timer Timer to access.
 * @param enabled Flag that indicates if the timer should be enabled or
 * disabled.
 * @return Timer error.
 */
timer_error_e timerSetEnabled(const timer_e timer, const bool enabled);

/**
 * @brief Checks to see if the given timer is enabled or disabled.
 * @param timer Timer to access.
 * @param isEnabled Flag that indicates if the timer is enabled or disabled.
 * @return Timer error.
 */
timer_error_e timerIsEnabled(const timer_e timer, bool* isEnabled);

/**
 * @brief Configures the given timer.
 * @param timer Timer to access.
 * @param config Timer configuration.
 * @return Timer error.
 */
timer_error_e timerConfigure(const timer_e timer, const timer_config_t* config);

#endif // _TIMER_INTERFACE_H_
