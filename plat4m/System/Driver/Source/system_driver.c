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
 * @file system_driver.c
 * @author Ben Minerd
 * @date 3/18/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_interface.h>
#include <system_driver.h>

#include <gpio_interface.h>
#include <uart_interface.h>
#include <interrupt_interface.h>
#include <adc_interface.h>
#include <i2c_interface.h>
#include <display_interface.h>

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static void led1Procedure(void);

/**
 * TODO Comment!
 */
static void uart1RxCallback(uint8_t data);

/**
 * TODO Comment!
 */
static void button1IntHandler(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void systemDriverInit(void)
{
    // plat4m
    //
    // Initialize system driver.
    // Add drivers to SPI interface.
    //
    // Example:
    //
    //      system_procedure_driver_t procedures[] =
    //      {
    //          // LED1 procedure
    //          {
    //              .id         = SYSTEM_DRIVER_PROCEDURE_ID_LED1,
    //              .enabled    = true,
    //              ...
    //          },
    //          // LED2 procedure
    //          {
    //              .id         = SYSTEM_DRIVER_PROCEDURE_ID_LED2,
    //              .enabled    = true,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      systemProcedureAddDrivers(procedures, ARRAY_SIZE(procedures));

    system_procedure_driver_t procedures[] =
    {
        // LED procedure
        {
            .id         = SYSTEM_DRIVER_PROCEDURE_ID_LED,
            .enabled    = true,
            .callback   = led1Procedure
        }
    };

    systemProcedureAddDrivers(procedures, ARRAY_SIZE(procedures));

    interruptInit();
    interruptSetEnabled(INTERRUPT_DRIVER_ID_BUTTON_1, true);
    interruptAddHandler(INTERRUPT_DRIVER_ID_BUTTON_1, button1IntHandler);
    interruptSetEnabled(INTERRUPT_DRIVER_ID_SYS_TICK, true);
    interruptAddHandler(INTERRUPT_DRIVER_ID_SYS_TICK, systemTimeMsHandler);

    gpioInit();
    gpioSetEnabled(GPIO_DRIVER_ID_LED_1, true);
    gpioSetEnabled(GPIO_DRIVER_ID_LED_2, true);
    gpioSetEnabled(GPIO_DRIVER_ID_OLED_RST_N, true);
    gpioSetEnabled(GPIO_DRIVER_ID_OLED_VCC_EN, true);
    gpioSetEnabled(GPIO_DRIVER_ID_OLED_PWR_EN, true);

    uartInit();
    uartSetEnabled(UART_DRIVER_ID_1, true);
    uartAddRxCallback(UART_DRIVER_ID_1, uart1RxCallback);

    adcInit();
    adcSetEnabled(ADC_DRIVER_ID_1, true);

    i2cInit();
    i2cSetEnabled(I2C_DRIVER_ID_1, true);

    displayInit();
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void led1Procedure(void)
{
    static uint32_t blinkTimeMs = 0;
    static gpio_level_e level = GPIO_LEVEL_HIGH;
    float voltage = 0.0f;

    // Test
    if (systemTimeMsGet() > blinkTimeMs)
    {
        gpioSetLevel(GPIO_DRIVER_ID_LED_1, level);

        if (level == GPIO_LEVEL_LOW)
        {
            level = GPIO_LEVEL_HIGH;
        }
        else
        {
            level = GPIO_LEVEL_LOW;
        }

        adcRead(ADC_DRIVER_ID_1, &voltage);

        blinkTimeMs = systemTimeMsGet() + (uint32_t)(voltage * 300.0f);
    }
}

//------------------------------------------------------------------------------
static void uart1RxCallback(uint8_t data)
{
    byte_array_t dataStr = BYTE_ARRAY("\nplat4m> ");

    if (data == '\r')
    {
        uartTx(UART_DRIVER_ID_1, &dataStr);
    }
}

//------------------------------------------------------------------------------
static void button1IntHandler(void)
{
    static gpio_level_e level = GPIO_LEVEL_HIGH;

    gpioSetLevel(GPIO_DRIVER_ID_LED_2, level);

    if (level == GPIO_LEVEL_LOW)
    {
        level = GPIO_LEVEL_HIGH;
        displaySetEnabled(true);
    }
    else
    {
        level = GPIO_LEVEL_LOW;
        displaySetEnabled(false);
    }
}
