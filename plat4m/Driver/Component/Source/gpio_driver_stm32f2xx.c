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
 * @file gpio_stm32f2xx.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_interface.h>
#include <gpio_driver_stm32f2xx.h>

#include <stm32f2xx.h>
#include <stm32f2xx_gpio.h>
#include <stm32f2xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Define for GPIO clock speed.
 */
#define GPIO_CLOCK_SPEED (GPIO_Speed_50MHz)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

typedef struct _gpio_alternate_function_t_
{
    gpio_driver_alternate_function_e id;
    const uint8_t function;
} gpio_alternate_function_t;

/**
 * @brief GPIO alternate function map type.
 */
typedef struct _gpio_alternate_function_map_t_
{
    const gpio_alternate_function_t* alternateFunctions;
    const unsigned int size;
} gpio_alternate_function_map_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief GPIO driver port map.
 */
static GPIO_TypeDef* portMap[] =
{
    GPIOA,  /// GPIO_DRIVER_PORT_ID_A
    GPIOB,  /// GPIO_DRIVER_PORT_ID_B
    GPIOC,  /// GPIO_DRIVER_PORT_ID_C
    GPIOD,  /// GPIO_DRIVER_PORT_ID_D
    GPIOE,  /// GPIO_DRIVER_PORT_ID_E
    GPIOF,  /// GPIO_DRIVER_PORT_ID_F
    GPIOG,  /// GPIO_DRIVER_PORT_ID_G
    GPIOH,  /// GPIO_DRIVER_PORT_ID_H
    GPIOI   /// GPIO_DRIVER_PORT_ID_I
};

/**
 * @brief GPIO driver pin map.
 */
static uint16_t pinMap[] =
{
    GPIO_Pin_0,     /// GPIO_DRIVER_PIN_ID_0
    GPIO_Pin_1,     /// GPIO_DRIVER_PIN_ID_1
    GPIO_Pin_2,     /// GPIO_DRIVER_PIN_ID_2
    GPIO_Pin_3,     /// GPIO_DRIVER_PIN_ID_3
    GPIO_Pin_4,     /// GPIO_DRIVER_PIN_ID_4
    GPIO_Pin_5,     /// GPIO_DRIVER_PIN_ID_5
    GPIO_Pin_6,     /// GPIO_DRIVER_PIN_ID_6
    GPIO_Pin_7,     /// GPIO_DRIVER_PIN_ID_7
    GPIO_Pin_8,     /// GPIO_DRIVER_PIN_ID_8
    GPIO_Pin_9,     /// GPIO_DRIVER_PIN_ID_9
    GPIO_Pin_10,    /// GPIO_DRIVER_PIN_ID_10
    GPIO_Pin_11,    /// GPIO_DRIVER_PIN_ID_11
    GPIO_Pin_12,    /// GPIO_DRIVER_PIN_ID_12
    GPIO_Pin_13,    /// GPIO_DRIVER_PIN_ID_13
    GPIO_Pin_14,    /// GPIO_DRIVER_PIN_ID_14
    GPIO_Pin_15     /// GPIO_DRIVER_PIN_ID_15
};

/**
 * @brief GPIO driver clock map.
 */
static const uint32_t clockMap[] =
{
    RCC_AHB1Periph_GPIOA,   /// GPIO_DRIVER_PORT_ID_A
    RCC_AHB1Periph_GPIOB,   /// GPIO_DRIVER_PORT_ID_B
    RCC_AHB1Periph_GPIOC,   /// GPIO_DRIVER_PORT_ID_C
    RCC_AHB1Periph_GPIOD,   /// GPIO_DRIVER_PORT_ID_D
    RCC_AHB1Periph_GPIOE,   /// GPIO_DRIVER_PORT_ID_E
    RCC_AHB1Periph_GPIOF,   /// GPIO_DRIVER_PORT_ID_F
    RCC_AHB1Periph_GPIOG,   /// GPIO_DRIVER_PORT_ID_G
    RCC_AHB1Periph_GPIOH,   /// GPIO_DRIVER_PORT_ID_H
    RCC_AHB1Periph_GPIOI,   /// GPIO_DRIVER_PORT_ID_I
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local GPIO driver functions here.
//
// Example:
//
//      void ledSetEnabled(bool setEnabled);
//      void ledSetLevel(gpio_level_e level);
//      ...

/**
 * @brief Sets the given GPIO hardware enabled or disabled.
 * @param hardware GPIO hardware to access.
 * @param enabled Flag for enabled/disabled.
 */
static void setEnabled(const gpio_hardware_t* hardware,
                       const bool enabled);

/**
 * @brief Sets the mode for the given GPIO hardware.
 * @param hardware GPIO hardware to access.
 * @param config GPIO configuration.
 */
static void configure(const gpio_hardware_t* hardware,
                      const gpio_config_t* config);

/**
 * @brief Sets the level for the given GPIO hardware.
 * @param hardware GPIO hardware to access.
 * @param level GPIO output level to set.
 */
static void setLevel(const gpio_hardware_t* hardware,
                     const gpio_level_e level);

/**
 * @brief Gets the level for the given GPIO output hardware.
 * @param hardware GPIO hardware to access.
 * @param level Current GPIO output level.
 */
static void getLevel(const gpio_hardware_t* hardware,
                     gpio_level_e* level);

/**
 * @brief Reads the level for the given GPIO input hardware.
 * @param hardware GPIO hardware to access.
 * @param level Current GPIO input level.
 */
static void readLevel(const gpio_hardware_t* hardware,
                      gpio_level_e* level);

/**
 * @brief Toggles the level for the given GPIO output hardware.
 * @param hardware GPIO hardware to access.
 */
static void toggleLevel(const gpio_hardware_t* hardware);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gpioDriverInit(void)
{
    // plat4m
    //
    // Initialize GPIO driver.
    // Add drivers to GPIO interface.
    
    gpio_driver_t driver =
    {
        .setEnabled     = setEnabled,
        .configure      = configure,
        .setLevel       = setLevel,
        .getLevel       = getLevel,
        .readLevel      = readLevel,
        .toggleLevel    = toggleLevel
    };
    
    gpioSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(const gpio_hardware_t* hardware,
                       const bool enabled)
{
    RCC_AHB1PeriphClockCmd(clockMap[hardware->portId],
                           (FunctionalState) enabled);
}

//------------------------------------------------------------------------------
static void configure(const gpio_hardware_t* hardware,
                      const gpio_config_t* config)
{
    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin   = pinMap[hardware->pinId];
    gpioInit.GPIO_Speed = GPIO_CLOCK_SPEED;

    switch (config->mode)
    {
        case GPIO_MODE_OUTPUT:
        {
            gpioInit.GPIO_Mode  = GPIO_Mode_OUT;
            gpioInit.GPIO_OType = GPIO_OType_PP;

            break;
        }
        case GPIO_MODE_INPUT:
        {
            gpioInit.GPIO_Mode = GPIO_Mode_IN;

            break;
        }
        case GPIO_MODE_ALTERNATE_FUNCTION:
        {
            gpioInit.GPIO_Mode = GPIO_Mode_AF;

            GPIO_PinAFConfig(portMap[hardware->portId],
                             hardware->pinId,
                             config->alternateFunction);

            break;
        }
    }

    gpioInit.GPIO_PuPd = (GPIOPuPd_TypeDef) config->resistor;

    GPIO_Init(portMap[hardware->portId], &gpioInit);
}

//------------------------------------------------------------------------------
static void setLevel(const gpio_hardware_t* hardware,
                     const gpio_level_e level)
{
    switch (level)
    {
        case GPIO_LEVEL_LOW:
        {
            portMap[hardware->portId]->BSRRH = pinMap[hardware->pinId];

            break;
        }
        case GPIO_LEVEL_HIGH:
        {
            portMap[hardware->portId]->BSRRL = pinMap[hardware->pinId];

            break;
        }
    }
}

//------------------------------------------------------------------------------
static void getLevel(const gpio_hardware_t* hardware,
                     gpio_level_e* level)
{
    *level =
        (gpio_level_e) ((portMap[hardware->portId]->ODR &
                         pinMap[hardware->pinId]) == pinMap[hardware->pinId]);
}

//------------------------------------------------------------------------------
static void readLevel(const gpio_hardware_t* hardware,
                      gpio_level_e* level)
{
    *level =
        (gpio_level_e) ((portMap[hardware->portId]->IDR &
                         pinMap[hardware->pinId]) == pinMap[hardware->pinId]);
}

//------------------------------------------------------------------------------
static void toggleLevel(const gpio_hardware_t* hardware)
{
    portMap[hardware->portId]->ODR ^= pinMap[hardware->pinId];
}
