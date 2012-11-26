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
 * @brief GPIO driver layer for the STM32F2xx.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_driver_stm32f2xx.h>
#include <gpio_interface.h>

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

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief GPIO port map.
 */
static GPIO_TypeDef* portMap[GPIO_PORT_COUNT] =
{
    GPIOA,  /// GPIO_PORT_A
    GPIOB,  /// GPIO_PORT_B
    GPIOC,  /// GPIO_PORT_C
    GPIOD,  /// GPIO_PORT_D
    GPIOE,  /// GPIO_PORT_E
    GPIOF,  /// GPIO_PORT_F
    GPIOG,  /// GPIO_PORT_G
    GPIOH,  /// GPIO_PORT_H
    GPIOI   /// GPIO_PORT_I
};

/**
 * @brief GPIO pin map.
 */
static uint16_t pinMap[GPIO_PIN_COUNT] =
{
    GPIO_Pin_0,     /// GPIO_PIN_0
    GPIO_Pin_1,     /// GPIO_PIN_1
    GPIO_Pin_2,     /// GPIO_PIN_2
    GPIO_Pin_3,     /// GPIO_PIN_3
    GPIO_Pin_4,     /// GPIO_PIN_4
    GPIO_Pin_5,     /// GPIO_PIN_5
    GPIO_Pin_6,     /// GPIO_PIN_6
    GPIO_Pin_7,     /// GPIO_PIN_7
    GPIO_Pin_8,     /// GPIO_PIN_8
    GPIO_Pin_9,     /// GPIO_PIN_9
    GPIO_Pin_10,    /// GPIO_PIN_10
    GPIO_Pin_11,    /// GPIO_PIN_11
    GPIO_Pin_12,    /// GPIO_PIN_12
    GPIO_Pin_13,    /// GPIO_PIN_13
    GPIO_Pin_14,    /// GPIO_PIN_14
    GPIO_Pin_15     /// GPIO_PIN_15
};

/**
 * @brief GPIO driver clock map.
 */
static const uint32_t clockMap[GPIO_PORT_COUNT] =
{
    RCC_AHB1Periph_GPIOA,   /// GPIO_PORT_A
    RCC_AHB1Periph_GPIOB,   /// GPIO_PORT_B
    RCC_AHB1Periph_GPIOC,   /// GPIO_PORT_C
    RCC_AHB1Periph_GPIOD,   /// GPIO_PORT_D
    RCC_AHB1Periph_GPIOE,   /// GPIO_PORT_E
    RCC_AHB1Periph_GPIOF,   /// GPIO_PORT_F
    RCC_AHB1Periph_GPIOG,   /// GPIO_PORT_G
    RCC_AHB1Periph_GPIOH,   /// GPIO_PORT_H
    RCC_AHB1Periph_GPIOI,   /// GPIO_PORT_I
};

/**
 * @brief GPIO mode map.
 */
static const GPIOMode_TypeDef modeMap[GPIO_MODE_COUNT] =
{
    GPIO_Mode_OUT,  /// GPIO_MODE_DIGITAL_OUTPUT
    GPIO_Mode_IN,   /// GPIO_MODE_DIGITAL_INPUT
    GPIO_Mode_AN,   /// GPIO_MODE_ANALOG
    GPIO_Mode_AF    /// GPIO_MODE_ALTERNATE_FUNCTION
};

/**
 * @brief GPIO resistor map.
 */
static const GPIOPuPd_TypeDef resistorMap[GPIO_RESISTOR_COUNT] =
{
    GPIO_PuPd_NOPULL,   /// GPIO_RESISTOR_NONE
    GPIO_PuPd_UP,       /// GPIO_RESISTOR_PULL_UP
    GPIO_PuPd_DOWN      /// GPIO_RESISTOR_PULL_DOWN
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Sets the given GPIO gpio enabled or disabled.
 * @param gpio GPIO to access.
 * @param enabled Flag for enabled/disabled.
 */
static void setEnabled(const gpio_t* gpio, const bool enabled);

/**
 * @brief Sets the mode for the given GPIO gpio.
 * @param gpio GPIO to access.
 * @param config GPIO configuration.
 */
static void configure(const gpio_t* gpio, const gpio_config_t* config);

/**
 * @brief Sets the level for the given GPIO gpio.
 * @param gpio GPIO gpio to access.
 * @param level GPIO output level to set.
 */
static void setLevel(const gpio_t* gpio, const gpio_level_e level);

/**
 * @brief Gets the level for the given GPIO output gpio.
 * @param gpio GPIO to access.
 * @param level Current GPIO output level.
 */
static void getLevel(const gpio_t* gpio, gpio_level_e* level);

/**
 * @brief Reads the level for the given GPIO input gpio.
 * @param gpio GPIO to access.
 * @param level Current GPIO input level.
 */
static void readLevel(const gpio_t* gpio, gpio_level_e* level);

/**
 * @brief Toggles the level for the given GPIO output gpio.
 * @param gpio GPIO to access.
 */
static void toggleLevel(const gpio_t* gpio);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void gpioDriverStm32f2xxInit(void)
{
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
static void setEnabled(const gpio_t* gpio, const bool enabled)
{
    RCC_AHB1PeriphClockCmd(clockMap[gpio->port], (FunctionalState) enabled);
}

//------------------------------------------------------------------------------
static void configure(const gpio_t* gpio, const gpio_config_t* config)
{
    GPIO_InitTypeDef gpioInit =
    {
        .GPIO_Pin   = pinMap[gpio->pin],
        .GPIO_Mode  = modeMap[config->mode],
        .GPIO_Speed = GPIO_CLOCK_SPEED,
        .GPIO_PuPd  = resistorMap[config->resistor]
    };

    switch (config->mode)
    {
        case GPIO_MODE_DIGITAL_OUTPUT:
        {
            gpioInit.GPIO_OType = GPIO_OType_PP;

            break;
        }
        case GPIO_MODE_ALTERNATE_FUNCTION:
        {
            switch (config->alternateFunction)
            {
                // I2C
                case GPIO_ALTERNATE_FUNCTION_4:
                {
                    gpioInit.GPIO_OType = GPIO_OType_OD;

                    break;
                }
                // UART
                case GPIO_ALTERNATE_FUNCTION_7:
                case GPIO_ALTERNATE_FUNCTION_8:
                {
                    gpioInit.GPIO_OType = GPIO_OType_PP;

                    break;
                }
            }

            GPIO_PinAFConfig(portMap[gpio->port],
                             gpio->pin,
                             config->alternateFunction);

            break;
        }
        default:
        {
            break;
        }
    }

    GPIO_Init(portMap[gpio->port], &gpioInit);
}

//------------------------------------------------------------------------------
static void setLevel(const gpio_t* gpio, const gpio_level_e level)
{
    GPIO_WriteBit(portMap[gpio->port],
                  pinMap[gpio->pin],
                  (BitAction) level);
}

//------------------------------------------------------------------------------
static void getLevel(const gpio_t* gpio, gpio_level_e* level)
{
    *level = (gpio_level_e) GPIO_ReadOutputDataBit(portMap[gpio->port],
                                                   pinMap[gpio->pin]);
}

//------------------------------------------------------------------------------
static void readLevel(const gpio_t* gpio, gpio_level_e* level)
{
    *level = (gpio_level_e) GPIO_ReadInputDataBit(portMap[gpio->port],
                                                  pinMap[gpio->pin]);
}

//------------------------------------------------------------------------------
static void toggleLevel(const gpio_t* gpio)
{
    GPIO_ToggleBits(portMap[gpio->port], pinMap[gpio->pin]);
}
