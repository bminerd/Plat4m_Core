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

/**
 * @brief Interrupt type.
 */
typedef struct _interrupt_t_
{
    interrupt_e id;
    bool isEnabled;
    interrupt_config_t config;
    const interrupt_handler_f* handler;
} interrupt_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Interrupt driver.
 */
static interrupt_driver_t driver;

/**
 * @brief Interrupt map.
 */
static interrupt_t interruptMap[INTERRUPT_COUNT];

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
static void interruptHandler(const interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void interruptInit(void)
{
    int i;
    
    for (i = 0; i < INTERRUPT_COUNT; i++)
    {
        interruptMap[i].id          = (interrupt_e) i;
        interruptMap[i].isEnabled   = false;
        interruptMap[i].handler     = NULL_POINTER;
    }
    
    interruptDriverInit(interruptHandler);
}

//------------------------------------------------------------------------------
extern interrupt_error_e interruptSetDriver(
                                    const interrupt_driver_t* interruptDriver)
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
    
    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern interrupt_error_e interruptSetHandler(
                                            const interrupt_e interrupt,
                                            const interrupt_handler_f* handler)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(handler))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }

    if (interrupt >= INTERRUPT_COUNT)
    {
        return INTERRUPT_ERROR_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    interruptMap[interrupt].handler = handler;
    
    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern interrupt_error_e interruptSetEnabled(const interrupt_e interrupt,
                                             const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (interrupt >= INTERRUPT_COUNT)
    {
        return INTERRUPT_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(interrupt, enabled);

    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern interrupt_error_e interruptIsEnabled(const interrupt_e interrupt,
                                            bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isEnabled))
    {
        return INTERRUPT_ERROR_PARAMETER_INVALID;
    }

    if (interrupt >= INTERRUPT_COUNT)
    {
        return INTERRUPT_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = interruptMap[interrupt].isEnabled;

    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern interrupt_error_e interruptConfigure(const interrupt_e interrupt,
                                            const interrupt_config_t* config)
{
#ifdef PLAT4M_DEBUG

#endif // PLAT4M_DEBUG

    driver.configure(interrupt, config);
    interruptMap[interrupt].config = *config;

    return INTERRUPT_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void interruptHandler(const interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if ((interrupt == INTERRUPT_NONE)   ||
        (interrupt >= INTERRUPT_COUNT)  ||
        IS_NULL_POINTER(interruptMap[interrupt].handler))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    gpio_level_e level;

    gpioHardwareReadLevel(&(interruptMap[interrupt].config.hardware), &level);

    interruptMap[interrupt].handler(triggerMap[level]);
}
