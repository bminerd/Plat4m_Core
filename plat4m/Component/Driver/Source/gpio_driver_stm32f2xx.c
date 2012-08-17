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
 * @file gpio_driver_stm32f2xx.c
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
#include <stm322xg_eval.h>

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

// plat4m
//
// Declare local GPIO driver functions here.
//
// Example:
//
//      static void ledSetEnabled(bool setEnabled);
//      static void ledSetLevel(gpio_level_e level);
//      ...

/**
 * TODO Comment!
 */
static void led1SetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static void led1SetLevel(gpio_level_e level);

/**
 * TODO Comment!
 */
static void led2SetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static void led2SetLevel(gpio_level_e level);

/**
 * TODO Comment!
 */
static void oledRstNSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static inline void oledRstNSetLevel(gpio_level_e level);

/**
 * TODO Comment!
 */
static void oledVccEnSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static inline void oledVccEnSetLevel(gpio_level_e level);

/**
 * TODO Comment!
 */
static void oledPwrEnSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static inline void oledPwrEnSetLevel(gpio_level_e level);

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
    //
    // Example:
    //
    //      gpio_driver_t gpios[] =
    //      {
    //          // LED1
    //          {
    //              .id         = GPIO_DRIVER_ID_LED1,
    //              .setEnabled = led1SetEnabled,
    //              ...
    //          },
    //          // LED2
    //          {
    //              .id         = GPIO_DRIVER_ID_LED2,
    //              .setEnabled = led2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      gpioAddDrivers(gpios, ARRAY_SIZE(gpios));
    
    gpio_driver_t gpios[] =
    {
        // LED_1
        {
            .id         = GPIO_DRIVER_ID_LED_1,
            .setEnabled = led1SetEnabled,
            .setLevel   = led1SetLevel
        },
        // LED_2
        {
            .id         = GPIO_DRIVER_ID_LED_2,
            .setEnabled = led2SetEnabled,
            .setLevel   = led2SetLevel
        },
        // OLED_RST_N
        {
            .id         = GPIO_DRIVER_ID_OLED_RST_N,
            .setEnabled = oledRstNSetEnabled,
            .setLevel   = oledRstNSetLevel
        },
        // OLED_RST_VCC_EN
        {
            .id         = GPIO_DRIVER_ID_OLED_VCC_EN,
            .setEnabled = oledVccEnSetEnabled,
            .setLevel   = oledVccEnSetLevel
        },
        // OLED_RST_PWR_EN
        {
            .id         = GPIO_DRIVER_ID_OLED_PWR_EN,
            .setEnabled = oledPwrEnSetEnabled,
            .setLevel   = oledPwrEnSetLevel
        }
    };
    
    gpioAddDrivers(gpios, ARRAY_SIZE(gpios));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void led1SetEnabled(bool enabled)
{
    if (enabled)
    {
        STM_EVAL_LEDInit(LED1);
    }
    else
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
static void led1SetLevel(gpio_level_e level)
{
    switch (level)
    {
        case GPIO_LEVEL_HIGH:
        {
            STM_EVAL_LEDOn(LED1);
            
            break;
        }
        case GPIO_LEVEL_LOW:
        {
            STM_EVAL_LEDOff(LED1);
            
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void led2SetEnabled(bool enabled)
{
    if (enabled)
    {
        STM_EVAL_LEDInit(LED2);
    }
    else
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
static void led2SetLevel(gpio_level_e level)
{
    switch (level)
    {
        case GPIO_LEVEL_HIGH:
        {
            STM_EVAL_LEDOn(LED2);

            break;
        }
        case GPIO_LEVEL_LOW:
        {
            STM_EVAL_LEDOff(LED2);

            break;
        }
    }
}

//------------------------------------------------------------------------------
static void oledRstNSetEnabled(bool enabled)
{
    GPIO_InitTypeDef gpioInit;

    gpioInit.GPIO_Pin   = GPIO_Pin_8;
    gpioInit.GPIO_Mode  = GPIO_Mode_OUT;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    if (enabled)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
        GPIO_Init(GPIOE, &gpioInit);
    }
    else
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
static inline void oledRstNSetLevel(gpio_level_e level)
{
    GPIO_WriteBit(GPIOG, GPIO_Pin_1, (BitAction) level);
}

//------------------------------------------------------------------------------
static void oledVccEnSetEnabled(bool enabled)
{
    GPIO_InitTypeDef gpioInit;

    gpioInit.GPIO_Pin   = GPIO_Pin_9;
    gpioInit.GPIO_Mode  = GPIO_Mode_OUT;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    if (enabled)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
        GPIO_Init(GPIOE, &gpioInit);
    }
    else
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
static inline void oledVccEnSetLevel(gpio_level_e level)
{
    GPIO_WriteBit(GPIOE, GPIO_Pin_9, (BitAction) level);
}

//------------------------------------------------------------------------------
static void oledPwrEnSetEnabled(bool enabled)
{
    GPIO_InitTypeDef gpioInit;

    gpioInit.GPIO_Pin   = GPIO_Pin_10;
    gpioInit.GPIO_Mode  = GPIO_Mode_OUT;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    if (enabled)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
        GPIO_Init(GPIOE, &gpioInit);
    }
    else
    {

    }
}

//------------------------------------------------------------------------------
static inline void oledPwrEnSetLevel(gpio_level_e level)
{
    GPIO_WriteBit(GPIOE, GPIO_Pin_10, (BitAction) level);
}
