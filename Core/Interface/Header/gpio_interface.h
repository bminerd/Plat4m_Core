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
 * @file gpio_interface.h
 * @author Ben Minerd
 * @date 2/3/12
 * @brief GPIO interface layer.
 */

#ifndef _GPIO_INTERFACE_H_
#define _GPIO_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <gpio_application.h>
#include <gpio_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of GPIO errors.
 */
typedef enum _gpio_error_e
{
    GPIO_ERROR_NONE,
    GPIO_ERROR_PARAMETER_INVALID,
    GPIO_ERROR_INVALID,
    GPIO_ERROR_PORT_INVALID,
    GPIO_ERROR_PIN_INVALID,
    GPIO_ERROR_NOT_ENABLED,
    GPIO_ERROR_MODE_INVALID
} gpio_error_e;

/**
 * @brief Enumeration of GPIO levels.
 */
typedef enum _gpio_level_e_
{
	GPIO_LEVEL_LOW  = 0,    /// Digital low / logic 0
	GPIO_LEVEL_HIGH = 1     /// Digital high / logic 1
} gpio_level_e;

/**
 * @brief Enumeration of GPIO modes.
 */
typedef enum _gpio_mode_e_
{
    GPIO_MODE_DIGITAL_OUTPUT = 0,
    GPIO_MODE_DIGITAL_INPUT,
    GPIO_MODE_ANALOG_INPUT,
    GPIO_MODE_ALTERNATE_FUNCTION
} gpio_mode_e;

/**
 * @brief Enumeration of GPIO resistor modes.
 */
typedef enum _gpio_resistor_e_
{
    GPIO_RESISTOR_NONE = 0,
    GPIO_RESISTOR_PULL_UP,
    GPIO_RESISTOR_PULL_DOWN
} gpio_resistor_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief GPIO hardware type.
 */
typedef struct _gpio_hardware_t_
{
    gpio_port_e port;
    gpio_pin_e pin;
} gpio_hardware_t;

/**
 * @brief GPIO type.
 */
typedef struct _gpio_t_
{
    gpio_e gpio;
    gpio_hardware_t hardware;
} gpio_t;

/**
 * @brief GPIO configuration type.
 */
typedef struct _gpio_config_t_
{
    gpio_mode_e mode;
    gpio_resistor_e resistor;
    gpio_alternate_function_e alternateFunction;
} gpio_config_t;

/**
 * @brief Function type that sets the given GPIO hardware enabled or disabled.
 * @param hardware GPIO hardware to access.
 * @param enabled Flag that indicates if the GPIO should be enabled or disabled.
 */
typedef void gpio_driver_set_enabled_f(const gpio_hardware_t* hardware,
                                       const bool enabled);

/**
 * @brief Function type that sets the mode for the given GPIO hardware.
 * @param hardware GPIO hardware to access.
 * @param config GPIO configuration.
 */
typedef void gpio_driver_config_f(const gpio_hardware_t* hardware,
                                  const gpio_config_t* config);

/**
 * @brief Function type that sets the level for the given GPIO hardware.
 * @param hardware GPIO hardware to access.
 * @param level GPIO output level to set.
 */
typedef void gpio_driver_set_level_f(const gpio_hardware_t* hardware,
                                     const gpio_level_e level);

/**
 * @brief Function type that gets the level for the given GPIO output hardware.
 * @param hardware GPIO hardware to access.
 * @param level Current GPIO output level.
 */
typedef void gpio_driver_get_level_f(const gpio_hardware_t* hardware,
                                     gpio_level_e* level);

/**
 * @brief Function type that reads the level for the given GPIO input hardware.
 * @param hardware GPIO hardware to access.
 * @param level Current GPIO input level.
 */
typedef void gpio_driver_read_level_f(const gpio_hardware_t* hardware,
                                      gpio_level_e* level);

/**
 * @brief Function type that toggles the level for the given GPIO output
 * hardware.
 * @param hardware GPIO hardware to access.
 */
typedef void gpio_driver_toggle_level_f(const gpio_hardware_t* hardware);

/**
 * @brief Structure that contains all the necessary GPIO driver functions.
 */
typedef struct _gpio_driver_t_
{
    const gpio_driver_set_enabled_f* setEnabled;
    const gpio_driver_config_f* configure;
    const gpio_driver_set_level_f* setLevel;
    const gpio_driver_get_level_f* getLevel;
    const gpio_driver_read_level_f* readLevel;
    const gpio_driver_toggle_level_f* toggleLevel;
} gpio_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the GPIO interface layer.
 */
extern void gpioInit(void);

/**
 * @brief Sets the driver for the GPIO interface.
 * @param gpioDriver GPIO driver.
 * @return GPIO error.
 */
extern gpio_error_e gpioSetDriver(const gpio_driver_t* gpioDriver);

/**
 * @brief Adds the given GPIO.
 * @param gpio GPIO to add.
 * @return GPIO error.
 */
extern gpio_error_e gpioAddGpio(const gpio_t* gpio);

/**
 * @brief Adds the given GPIOs.
 * @param gpios GPIOs to add.
 * @param size Number of GPIOs to add.
 * @return GPIO error.
 */
extern gpio_error_e gpioAddGpios(const gpio_t gpios[], const unsigned int size);

/**
 * @brief Sets the given GPIO enabled.
 * @param id GPIO ID.
 * @param enabled Flag that indicates if the GPIO should be enabled or disabled.
 * @return GPIO error.
 */
extern gpio_error_e gpioSetEnabled(const gpio_e gpio, const bool enabled);

/**
 * @brief Checks to see if the given GPIO is enabled.
 * @param id GPIO ID.
 * @param isEnabled Flag that indicates if the GPIO is enabled or disabled.
 * @return GPIO error.
 */
extern gpio_error_e gpioIsEnabled(const gpio_e gpio, bool* isEnabled);

/**
 * @brief Sets the mode for the given GPIO.
 * @param id GPIO ID.
 * @param config GPIO configuration.
 * @return GPIO error.
 */
extern gpio_error_e gpioConfigure(const gpio_e gpio,
                                  const gpio_config_t* config);

/**
 * @brief Sets the level for the given GPIO port and pin.
 * @param id GPIO ID.
 * @param level GPIO output level to set.
 * @return GPIO error.
 */
extern gpio_error_e gpioSetLevel(const gpio_e gpio, const gpio_level_e level);

/**
 * @brief Gets the level for the given GPIO output port and pin.
 * @param id GPIO ID.
 * @param level Current GPIO output level.
 * @return GPIO error.
 */
extern gpio_error_e gpioGetLevel(const gpio_e gpio, gpio_level_e* level);

/**
 * @brief Reads the level for the given GPIO input port and pin.
 * @param id GPIO ID.
 * @param level Current GPIO input level.
 * @return GPIO error.
 */
extern gpio_error_e gpioReadLevel(const gpio_e gpio, gpio_level_e* level);

/**
 * @brief Toggles the level for the given GPIO output port and pin.
 * @param id GPIO ID.
 * @param pinId Pin ID.
 * @return GPIO error.
 */
extern gpio_error_e gpioToggleLevel(const gpio_e gpio);

/**
 * @brief Sets the given GPIO port and pin enabled.
 * @param hardware GPIO hardware to access.
 * @param enabled Flag that indicates if the GPIO should be enabled or disabled.
 * @return GPIO error.
 */
extern gpio_error_e gpioHardwareSetEnabled(const gpio_hardware_t* hardware,
                                           const bool enabled);

/**
 * @brief Checks to see if the given GPIO is enabled.
 * @param hardware GPIO hardware to access.
 * @param isEnabled Flag that indicates if the GPIO is enabled or disabled.
 * @return GPIO error.
 */
extern gpio_error_e gpioHardwareIsEnabled(const gpio_hardware_t* hardware,
                                          bool* isEnabled);

/**
 * @brief Sets the mode for the given GPIO port and pin.
 * @param hardware GPIO hardware to access.
 * @param config GPIO configuration.
 * @return GPIO error.
 */
extern gpio_error_e gpioHardwareConfigure(const gpio_hardware_t* hardware,
                                          const gpio_config_t* config);

/**
 * @brief Reads the level for the given GPIO input port and pin.
 * @param hardware GPIO hardware.
 * @param level Current GPIO input level.
 * @return GPIO error.
 */
extern gpio_error_e gpioHardwareReadLevel(const gpio_hardware_t* hardware,
                                          gpio_level_e* level);

#endif // _GPIO_INTERFACE_H_
