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
 * @file gpio_application.c
 * @author Ben Minerd
 * @date 9/21/2012
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_application.h>
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

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gpioApplicationInit(void)
{
    static gpio_id_map_t idMaps[] =
    {
        // GPIO_ID_LED_1
        {
            .id                 = GPIO_ID_LED_1,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_G,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_6
        },
        // GPIO_ID_LED_2
        {
            .id                 = GPIO_ID_LED_2,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_G,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_8
        },
        // GPIO_ID_LED_3
        {
            .id                 = GPIO_ID_LED_3,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_I,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_9
        },
        // GPIO_ID_LED_4
        {
            .id                 = GPIO_ID_LED_4,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_C,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_7
        },
        // GPIO_ID_OLED_RST_N
        {
            .id                 = GPIO_ID_OLED_RST_N,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_E,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_8
        },
        // GPIO_ID_OLED_VCC_EN
        {
            .id                 = GPIO_ID_OLED_VCC_EN,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_E,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_9
        },
        // GPIO_ID_OLED_PWR_EN
        {
            .id                 = GPIO_ID_OLED_PWR_EN,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_E,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_10
        },
        // GPIO_ID_BUTTON_KEY
        {
            .id                 = GPIO_ID_BUTTON_KEY,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_G,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_15
        },
        // GPIO_ID_BUTTON_TAMPER
        {
            .id                 = GPIO_ID_BUTTON_TAMPER,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_C,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_13
        },
        // GPIO_ID_BUTTON_WAKEUP
        {
            .id                 = GPIO_ID_BUTTON_WAKEUP,
            .hardware.portId    = GPIO_DRIVER_PORT_ID_A,
            .hardware.pinId     = GPIO_DRIVER_PIN_ID_0
        }
    };

    // Add GPIOs to interface
    gpioAddIdMaps(idMaps, ARRAY_SIZE(idMaps));

    gpio_config_t config;

    // LED_1
    config.mode = GPIO_MODE_OUTPUT;
    config.resistor = GPIO_RESISTOR_PULL_UP;
    gpioConfigure(GPIO_ID_LED_1, &config);
    gpioSetEnabled(GPIO_ID_LED_1, true);

    // LED_2
    gpioConfigure(GPIO_ID_LED_2, &config);
    gpioSetEnabled(GPIO_ID_LED_2, true);

    // LED_3
    gpioConfigure(GPIO_ID_LED_3, &config);
    gpioSetEnabled(GPIO_ID_LED_3, true);

    // LED_4
    gpioConfigure(GPIO_ID_LED_4, &config);
    gpioSetEnabled(GPIO_ID_LED_4, true);

    // OLED_RST_N
    config.resistor = GPIO_RESISTOR_NONE;
    gpioConfigure(GPIO_ID_OLED_RST_N, &config);
    gpioSetEnabled(GPIO_ID_OLED_RST_N, true);

    // OLED_VCC_EN
    gpioConfigure(GPIO_ID_OLED_VCC_EN, &config);
    gpioSetEnabled(GPIO_ID_OLED_VCC_EN, true);

    // OLED_PWR_EN
    gpioConfigure(GPIO_ID_OLED_PWR_EN, &config);
    gpioSetEnabled(GPIO_ID_OLED_PWR_EN, true);

    // BUTTON_KEY
    config.mode = GPIO_MODE_INPUT;
    gpioConfigure(GPIO_ID_BUTTON_KEY, &config);
    gpioSetEnabled(GPIO_ID_BUTTON_KEY, true);

    // BUTTON_TAMPER
    gpioConfigure(GPIO_ID_BUTTON_TAMPER, &config);
    gpioSetEnabled(GPIO_ID_BUTTON_TAMPER, true);

    // BUTTON_WAKEUP
    gpioConfigure(GPIO_ID_BUTTON_WAKEUP, &config);
    gpioSetEnabled(GPIO_ID_BUTTON_WAKEUP, true);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

