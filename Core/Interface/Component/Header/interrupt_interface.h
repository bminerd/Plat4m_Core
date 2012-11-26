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
 * @file interrupt_interface.h
 * @author Ben Minerd
 * @date 8/2/12
 * @brief Interrupt interface layer.
 */

#ifndef _INTERRUPT_INTERFACE_H_
#define _INTERRUPT_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <interrupt_application.h>
#include <gpio_interface.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of interrupt errors.
 */
typedef enum _interrupt_error_e_
{
    INTERRUPT_ERROR_NONE,
    INTERRUPT_ERROR_PARAMETER_INVALID,
    INTERRUPT_ERROR_ID_INVALID,
    INTERRUPT_ERROR_INTERRUPT_INVALID,
    INTERRUPT_ERROR_NOT_INITIALIZED,
    INTERRUPT_ERROR_NOT_ENABLED
} interrupt_error_e;

/**
 * @brief Enumeration of interrupt triggers.
 */
typedef enum _interrupt_trigger_e_
{
    INTERRUPT_TRIGGER_RISING,
    INTERRUPT_TRIGGER_FALLING,
    INTERRUPT_TRIGGER_RISING_FALLING,

    // Do not place values below!
    INTERRUPT_TRIGGER_COUNT
} interrupt_trigger_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Interrupt configuration type.
 */
typedef struct _interrupt_config_t_
{
    interrupt_trigger_e trigger;
    gpio_id_t* gpio;
} interrupt_config_t;

/**
 * @brief Function type that handles an interrupt with the given trigger.
 * @param trigger Interrupt trigger that occurred.
 */
typedef void interrupt_handler_f(const interrupt_trigger_e trigger);

/**
 * @brief Interrupt type.
 */
typedef struct _interrupt_info_t_
{
    bool isInitialized;
    bool isEnabled;
    mutex_t mutex;
    interrupt_config_t config;
    interrupt_handler_f* handler;
} interrupt_info_t;

/**
 * @brief Interrupt ID type.
 */
typedef struct _interrupt_id_t_
{
    const interrupt_e interrupt;
    const generic_data_t private[sizeof(interrupt_info_t) /
                                 sizeof(generic_data_t)];
} interrupt_id_t;

/**
 * @brief Function type that sets the given interrupt enabled or disabled.
 * @param interrupt Interrupt to access.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
typedef void interrupt_driver_set_enabled_f(const interrupt_e interrupt,
                                            const bool enabled);

/**
 * @brief Function type that configures the given interrupt.
 * @param interrupt Interrupt to access.
 * @param config Interrupt configuration.
 */
typedef void interrupt_driver_configure_f(const interrupt_e interrupt,
                                          const interrupt_config_t* config);

/**
 * @brief Interrupt driver type.
 */
typedef struct _interrupt_driver_t_
{
    interrupt_driver_set_enabled_f* setEnabled;
    interrupt_driver_configure_f* configure;
} interrupt_driver_t;

/**
 * @brief Function type for an interface interrupt handler.
 * @param interrupt Interrupt to handle.
 */
typedef void interrupt_interface_int_handler_f(const interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the interrupt module.
 */
void interruptInit(void);

/**
 * @brief Sets the driver for the interrupt module.
 * @param interruptDriver Interrupt driver.
 * @return Interrupt error.
 */
interrupt_error_e interruptSetDriver(
                                const interrupt_driver_t* interruptDriver,
                                interrupt_interface_int_handler_f** handler);

/**
 * @brief Adds the given interrupt ID.
 * @param id Interrupt ID to add.
 * @return Interrupt error.
 */
interrupt_error_e interruptInitId(interrupt_id_t* id);

/**
 * @brief Sets the handler for the given interrupt.
 * @param id Interrupt to access.
 * @param handler Interrupt handler.
 * @return Interrupt error.
 */
interrupt_error_e interruptSetHandler(interrupt_id_t* id,
                                      interrupt_handler_f* handler);

/**
 * @brief Sets the given interrupt enabled or disabled.
 * @param id Interrupt to access.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 * @return Interrupt error.
 */
interrupt_error_e interruptSetEnabled(interrupt_id_t* id,
                                      const bool enabled);

/**
 * @brief Checks to see if the given interrupt is enabled or disabled.
 * @param id Interrupt to access.
 * @param isEnabled Flag that indicates if the interrupt is enabled or disabled.
 * @return Interrupt error.
 */
interrupt_error_e interruptIsEnabled(interrupt_id_t* id, bool* isEnabled);

/**
 * @brief Configures the given interrupt.
 * @param id Interrupt to access.
 * @param config Interrupt configuration.
 * @return Interrupt error.
 */
interrupt_error_e interruptConfigure(interrupt_id_t* id,
                                     const interrupt_config_t* config);

#endif // _INTERRUPT_INTERFACE_H_
