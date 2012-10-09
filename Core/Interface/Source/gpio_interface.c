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
 * @brief GPIO wrapper type.
 */
typedef struct _gpio_wrapper_t_
{
    gpio_hardware_t hardware;
    bool isEnabled;
    gpio_config_t config;
} gpio_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief GPIO driver.
 */
static gpio_driver_t driver;

/**
 * @brief GPIO map.
 */
static gpio_wrapper_t gpioWrapperMap[GPIO_PORT_COUNT][GPIO_PIN_COUNT];

/**
 * @brief GPIO ID map.
 */
static gpio_wrapper_t* gpioMap[GPIO_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gpioInit(void)
{
    int i, j;

    for (i = 0; i < GPIO_PORT_COUNT; i++)
    {
        for (j = 0; j < GPIO_PIN_COUNT; j++)
        {
            gpioWrapperMap[i][j].hardware.port             = (gpio_port_e) i;
            gpioWrapperMap[i][j].hardware.pin              = (gpio_pin_e) j;
            gpioWrapperMap[i][j].isEnabled                 = false;
            gpioWrapperMap[i][j].config.mode               =
                GPIO_MODE_DIGITAL_OUTPUT;
            gpioWrapperMap[i][j].config.resistor           = GPIO_RESISTOR_NONE;
            gpioWrapperMap[i][j].config.alternateFunction  =
                GPIO_ALTERNATE_FUNCTION_NONE;
        }
    }

    for (i = 0; i < GPIO_COUNT; i++)
    {
        gpioMap[i] = NULL_POINTER;
    }

    gpioDriverInit();
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioSetDriver(const gpio_driver_t* gpioDriver)
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
extern gpio_error_e gpioAddGpio(const gpio_t* gpio)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(gpio))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (gpio->gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

    if (gpio->hardware.port >= GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (gpio->hardware.pin >= GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    gpioMap[gpio->gpio] = &(gpioWrapperMap[gpio->hardware.port]
                                          [gpio->hardware.pin]);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioAddGpios(const gpio_t gpios[], const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > GPIO_COUNT)
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        gpio_error_e error = gpioAddGpio(&(gpios[i]));

#ifdef PLAT4M_DEBUG

        if (error != GPIO_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioSetEnabled(const gpio_e gpio, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareSetEnabled(&(gpioMap[gpio]->hardware), enabled);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioIsEnabled(const gpio_e gpio, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareIsEnabled(&(gpioMap[gpio]->hardware), isEnabled);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioConfigure(const gpio_e gpio,
                                  const gpio_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareConfigure(&(gpioMap[gpio]->hardware), config);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioSetLevel(const gpio_e gpio, gpio_level_e level)
{
#ifdef PLAT4M_DEBUG

    if (gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

    if (!(gpioMap[gpio]->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[gpio]->config.mode != GPIO_MODE_DIGITAL_OUTPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setLevel(&(gpioMap[gpio]->hardware), level);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioGetLevel(const gpio_e gpio, gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

    if (!(gpioMap[gpio]->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[gpio]->config.mode != GPIO_MODE_DIGITAL_OUTPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.getLevel(&(gpioMap[gpio]->hardware), level);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioReadLevel(const gpio_e gpio, gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

    if (!(gpioMap[gpio]->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[gpio]->config.mode != GPIO_MODE_DIGITAL_INPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.readLevel(&(gpioMap[gpio]->hardware), level);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioToggleLevel(const gpio_e gpio)
{
#ifdef PLAT4M_DEBUG

    if (gpio >= GPIO_COUNT)
    {
        return GPIO_ERROR_INVALID;
    }

    if (!(gpioMap[gpio]->isEnabled))
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[gpio]->config.mode != GPIO_MODE_DIGITAL_OUTPUT)
    {
        return GPIO_ERROR_MODE_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.toggleLevel(&(gpioMap[gpio]->hardware));

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareSetEnabled(const gpio_hardware_t* hardware,
                                           const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (hardware->port >= GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (hardware->pin >= GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(hardware, enabled);
    gpioWrapperMap[hardware->port][hardware->pin].isEnabled = enabled;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareIsEnabled(const gpio_hardware_t* hardware,
                                          bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware) || IS_NULL_POINTER(isEnabled))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (hardware->port >= GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (hardware->pin >= GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = gpioWrapperMap[hardware->port][hardware->pin].isEnabled;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareConfigure(const gpio_hardware_t* hardware,
                                          const gpio_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware) || IS_NULL_POINTER(config))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (hardware->port >= GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (hardware->pin >= GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.configure(hardware, config);
    gpioWrapperMap[hardware->port][hardware->pin].config = *config;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareReadLevel(const gpio_hardware_t* hardware,
                                          gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware) || IS_NULL_POINTER(level))
    {
        return GPIO_ERROR_PARAMETER_INVALID;
    }

    if (hardware->port >= GPIO_PORT_COUNT)
    {
        return GPIO_ERROR_PORT_INVALID;
    }

    if (hardware->pin >= GPIO_PIN_COUNT)
    {
        return GPIO_ERROR_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.readLevel(hardware, level);

    return GPIO_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
