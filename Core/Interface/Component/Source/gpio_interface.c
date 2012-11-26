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
 * @file gpio_interface.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief GPIO interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_interface.h>
#include <mutex_interface.h>

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
 * @brief GPIO driver.
 */
static gpio_driver_t driver;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void gpioInit(void)
{
    gpioApplicationInit();
}

//------------------------------------------------------------------------------
gpio_error_e gpioSetDriver(const gpio_driver_t* gpioDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(gpioDriver)             ||
        IS_NULL_POINTER(gpioDriver->setEnabled) ||
        IS_NULL_POINTER(gpioDriver->configure)  ||
        IS_NULL_POINTER(gpioDriver->setLevel)   ||
        IS_NULL_POINTER(gpioDriver->getLevel)   ||
        IS_NULL_POINTER(gpioDriver->readLevel)  ||
        IS_NULL_POINTER(gpioDriver->toggleLevel))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *gpioDriver;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioInitId(gpio_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (id->gpio.port == GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (id->gpio.pin == GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioSetEnabled(gpio_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return GPIO_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    gpio_error_e error = gpioHardwareSetEnabled(&(id->gpio), enabled);
    info->isEnabled = enabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
gpio_error_e gpioIsEnabled(gpio_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return GPIO_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *isEnabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioConfigure(gpio_id_t* id, const gpio_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return GPIO_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    gpio_error_e error = gpioHardwareConfigure(&(id->gpio), config);
    memcpy((uint8_t*) &(info->config),
           (uint8_t*) config,
           sizeof(*config));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
gpio_error_e gpioSetLevel(gpio_id_t* id, const gpio_level_e level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return GPIO_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (info->config.mode != GPIO_MODE_DIGITAL_OUTPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.setLevel(&(id->gpio), level);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioGetLevel(gpio_id_t* id, gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(level))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return GPIO_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (info->config.mode != GPIO_MODE_DIGITAL_OUTPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.getLevel(&(id->gpio), level);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioReadLevel(gpio_id_t* id, gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(level))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return GPIO_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (info->config.mode != GPIO_MODE_DIGITAL_INPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.readLevel(&(id->gpio), level);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioToggleLevel(gpio_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpio_info_t* info = (gpio_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return GPIO_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (info->config.mode != GPIO_MODE_DIGITAL_OUTPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.toggleLevel(&(id->gpio));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioHardwareSetEnabled(const gpio_t* gpio, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(gpio))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (gpio->port == GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (gpio->pin == GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(gpio, enabled);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
gpio_error_e gpioHardwareConfigure(const gpio_t* gpio,
                                   const gpio_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(gpio) || IS_NULL_POINTER(config))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (gpio->port == GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (gpio->pin == GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.configure(gpio, config);

    return GPIO_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
