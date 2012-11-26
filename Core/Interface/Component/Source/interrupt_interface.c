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
 * @file interrupt_interface.c
 * @author Ben Minerd
 * @date 8/2/12
 * @brief Interrupt interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <interrupt_interface.h>

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

/**
 * @brief Interrupt driver.
 */
static interrupt_driver_t driver;

/**
 * @brief ID to interrupt map.
 */
static interrupt_info_t* interruptToInfoMap[INTERRUPT_COUNT];

/**
 * @brief Trigger mode map.
 */
static interrupt_trigger_e triggerMap[] =
{
    INTERRUPT_TRIGGER_FALLING,  /// GPIO_LEVEL_LOW
    INTERRUPT_TRIGGER_RISING    /// GPIO_LEVEL_HIGH
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Interrupt handler.
 */
static void intHandler(const interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void interruptInit(void)
{
    int i;

    for (i = 0; i < INTERRUPT_COUNT; i++)
    {
        interruptToInfoMap[i] = NULL_POINTER;
    }

    interruptApplicationInit();
}

//------------------------------------------------------------------------------
interrupt_error_e interruptSetDriver(
                                    const interrupt_driver_t* interruptDriver,
                                    interrupt_interface_int_handler_f** handler)
{
#ifdef PLAT4M_DEBUG
    
    if (IS_NULL_POINTER(interruptDriver)                ||
        IS_NULL_POINTER(interruptDriver->setEnabled)    ||
        IS_NULL_POINTER(interruptDriver->configure))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    driver = *interruptDriver;
    *handler = intHandler;
    
    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
interrupt_error_e interruptInitId(interrupt_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }

    if ((id->interrupt == INTERRUPT_NONE) || (id->interrupt == INTERRUPT_COUNT))
    {
        return INTERRUPT_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to interrupt
    interrupt_info_t* info = (interrupt_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    interruptToInfoMap[id->interrupt] = info;

    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
interrupt_error_e interruptSetHandler(interrupt_id_t* id,
                                      interrupt_handler_f* handler)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(handler))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    // Cast to interrupt
    interrupt_info_t* info = (interrupt_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return INTERRUPT_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    info->handler = handler;
    
    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
interrupt_error_e interruptSetEnabled(interrupt_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to interrupt
    interrupt_info_t* info = (interrupt_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return INTERRUPT_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.setEnabled(id->interrupt, enabled);
    info->isEnabled = true;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
interrupt_error_e interruptIsEnabled(interrupt_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to interrupt
    interrupt_info_t* info = (interrupt_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return INTERRUPT_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *isEnabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
interrupt_error_e interruptConfigure(interrupt_id_t* id,
                                     const interrupt_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to interrupt
    interrupt_info_t* info = (interrupt_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return INTERRUPT_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.configure(id->interrupt, config);
    memcpy((uint8_t*) &(info->config), (uint8_t*) config, sizeof(*config));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return INTERRUPT_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void intHandler(const interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(interruptToInfoMap[interrupt]))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    interrupt_info_t* info = interruptToInfoMap[interrupt];

    gpio_level_e level;

    gpioReadLevel(info->config.gpio, &level);

    info->handler(triggerMap[level]);
}
