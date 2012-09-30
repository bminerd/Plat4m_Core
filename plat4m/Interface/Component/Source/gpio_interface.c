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
 * @brief TODO Comment!
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
typedef struct _gpio_t_
{
    gpio_hardware_t hardware;
    bool isEnabled;
    gpio_config_t config;
} gpio_t;

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
static gpio_t gpioMap[GPIO_DRIVER_PORT_ID_COUNT][GPIO_DRIVER_PIN_ID_COUNT];

/**
 * @brief GPIO ID map.
 */
static gpio_t* idMap[GPIO_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Adds the given GPIO to the local info map.
 */
static bool addIdMap(const gpio_id_map_t* map);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gpioInit(void)
{
    int i, j;

    for (i = 0; i < GPIO_DRIVER_PORT_ID_COUNT; i++)
    {
        for (j = 0; j < GPIO_DRIVER_PIN_ID_COUNT; j++)
        {
            gpioMap[i][j].hardware.portId           = (gpio_driver_port_id_e) i;
            gpioMap[i][j].hardware.pinId            = (gpio_driver_pin_id_e) j;
            gpioMap[i][j].isEnabled                 = false;
            gpioMap[i][j].config.mode               = GPIO_MODE_OUTPUT;
            gpioMap[i][j].config.resistor           = GPIO_RESISTOR_NONE;
            gpioMap[i][j].config.alternateFunction  =
                GPIO_DRIVER_ALTERNATE_FUNCTION_NONE;
        }
    }

    for (i = 0; i < GPIO_ID_COUNT; i++)
    {
        idMap[i] = NULL_POINTER;
    }

    gpioDriverInit();
    gpioApplicationInit();
}

//------------------------------------------------------------------------------
extern bool gpioSetDriver(const gpio_driver_t* gpioDriver)
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
        return false;
    }

#endif // PLAT4M_DEBUG

    driver = *gpioDriver;

    return true;
}

//------------------------------------------------------------------------------
extern bool gpioAddIdMaps(gpio_id_map_t gpioIdMaps[], unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > GPIO_ID_COUNT)
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        if (!addIdMap(&gpioIdMaps[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioSetEnabled(const gpio_id_e id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (id >= GPIO_ID_COUNT)
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareSetEnabled(&(idMap[id]->hardware), enabled);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioIsEnabled(const gpio_id_e id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (id >= GPIO_ID_COUNT)
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareIsEnabled(&(idMap[id]->hardware), isEnabled);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioConfigure(const gpio_id_e id,
                                  const gpio_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (id >= GPIO_ID_COUNT)
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareConfigure(&(idMap[id]->hardware), config);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioSetLevel(const gpio_id_e id, gpio_level_e level)
{
#ifdef PLAT4M_DEBUG

    if (id >= GPIO_ID_COUNT)
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareSetLevel(&(idMap[id]->hardware), level);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioGetLevel(const gpio_id_e id, gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (id >= GPIO_ID_COUNT)
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareGetLevel(&(idMap[id]->hardware), level);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioReadLevel(const gpio_id_e id, gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (id >= GPIO_ID_COUNT)
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareReadLevel(&(idMap[id]->hardware), level);
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioToggleLevel(const gpio_id_e id)
{
#ifdef PLAT4M_DEBUG

    if (id >= GPIO_ID_COUNT)
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    return gpioHardwareToggleLevel(&(idMap[id]->hardware));
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareSetEnabled(const gpio_hardware_t* hardware,
                                           const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return GPIO_ERROR_INVALID_PARAMETER;
    }

    if ((hardware->portId >= GPIO_DRIVER_PORT_ID_COUNT) ||
        (hardware->pinId >= GPIO_DRIVER_PIN_ID_COUNT))
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(hardware, enabled);
    gpioMap[hardware->portId][hardware->pinId].isEnabled = enabled;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareIsEnabled(const gpio_hardware_t* hardware,
                                          bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware) || IS_NULL_POINTER(isEnabled))
    {
        return GPIO_ERROR_INVALID_PARAMETER;
    }

    if ((hardware->portId >= GPIO_DRIVER_PORT_ID_COUNT) ||
        (hardware->pinId >= GPIO_DRIVER_PIN_ID_COUNT))
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = gpioMap[hardware->portId][hardware->pinId].isEnabled;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareConfigure(const gpio_hardware_t* hardware,
                                          const gpio_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return GPIO_ERROR_INVALID_PARAMETER;
    }

    if ((hardware->portId >= GPIO_DRIVER_PORT_ID_COUNT) ||
        (hardware->pinId >= GPIO_DRIVER_PIN_ID_COUNT))
    {
        return GPIO_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    driver.configure(hardware, config);
    gpioMap[hardware->portId][hardware->pinId].config = *config;

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareSetLevel(const gpio_hardware_t* hardware,
                                         const gpio_level_e level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return GPIO_ERROR_INVALID_PARAMETER;
    }

    if ((hardware->portId >= GPIO_DRIVER_PORT_ID_COUNT) ||
        (hardware->pinId >= GPIO_DRIVER_PIN_ID_COUNT))
    {
        return GPIO_ERROR_INVALID_ID;
    }

    if (!gpioMap[hardware->portId][hardware->pinId].isEnabled)
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[hardware->portId][hardware->pinId].config.mode ==
            GPIO_MODE_INPUT)
    {
        return GPIO_ERROR_NOT_AN_OUTPUT;
    }

#endif // PLAT4M_DEBUG

    driver.setLevel(hardware, level);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareGetLevel(const gpio_hardware_t* hardware,
                                         gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return GPIO_ERROR_INVALID_PARAMETER;
    }

    if ((hardware->portId >= GPIO_DRIVER_PORT_ID_COUNT) ||
        (hardware->pinId >= GPIO_DRIVER_PIN_ID_COUNT))
    {
        return GPIO_ERROR_INVALID_ID;
    }

    if (!gpioMap[hardware->portId][hardware->pinId].isEnabled)
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[hardware->portId][hardware->pinId].config.mode ==
            GPIO_MODE_INPUT)
    {
        return GPIO_ERROR_NOT_AN_OUTPUT;
    }

#endif // PLAT4M_DEBUG

    driver.getLevel(hardware, level);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareReadLevel(const gpio_hardware_t* hardware,
                                          gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return GPIO_ERROR_INVALID_PARAMETER;
    }

    if ((hardware->portId >= GPIO_DRIVER_PORT_ID_COUNT) ||
        (hardware->pinId >= GPIO_DRIVER_PIN_ID_COUNT))
    {
        return GPIO_ERROR_INVALID_ID;
    }

    if (!gpioMap[hardware->portId][hardware->pinId].isEnabled)
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[hardware->portId][hardware->pinId].config.mode ==
            GPIO_MODE_OUTPUT)
    {
        return GPIO_ERROR_NOT_AN_OUTPUT;
    }

#endif // PLAT4M_DEBUG

    driver.readLevel(hardware, level);

    return GPIO_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern gpio_error_e gpioHardwareToggleLevel(const gpio_hardware_t* hardware)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return GPIO_ERROR_INVALID_PARAMETER;
    }

    if ((hardware->portId >= GPIO_DRIVER_PORT_ID_COUNT) ||
        (hardware->pinId >= GPIO_DRIVER_PIN_ID_COUNT))
    {
        return GPIO_ERROR_INVALID_ID;
    }

    if (!gpioMap[hardware->portId][hardware->pinId].isEnabled)
    {
        return GPIO_ERROR_NOT_ENABLED;
    }

    if (gpioMap[hardware->portId][hardware->pinId].config.mode ==
            GPIO_MODE_INPUT)
    {
        return GPIO_ERROR_NOT_AN_OUTPUT;
    }

#endif // PLAT4M_DEBUG

    driver.toggleLevel(hardware);

    return GPIO_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool addIdMap(const gpio_id_map_t* map)
{
#ifdef PLAT4M_DEBUG

    if ((map->id >= GPIO_ID_COUNT)                          ||
        (map->hardware.pinId >= GPIO_DRIVER_PIN_ID_COUNT)   ||
        (map->hardware.portId >= GPIO_DRIVER_PORT_ID_COUNT))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    idMap[map->id] = &(gpioMap[map->hardware.portId][map->hardware.pinId]);

    return true;
}
