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
 * @file dac_driver_stm32f2xx.c
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC driver layer for the STM32F2xx.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <dac_driver_stm32f2xx.h>
#include <dac_interface.h>
#include <gpio_interface.h>

#include <stm32f2xx.h>
#include <stm32f2xx_dac.h>
#include <stm32f2xx_rcc.h>

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
 * @brief DAC channel map.
 */
static const uint32_t channelMap[DAC_CHANNEL_COUNT] =
{
    DAC_Channel_1,  /// DAC_CHANNEL_1
    DAC_Channel_2   /// DAC_CHANNEL_2
};

/**
 * @brief DAC clock map.
 */
static const uint32_t clockMap[DAC_COUNT] =
{
    RCC_APB1Periph_DAC  /// DAC_1
};

/**
 * @brief DAC GPIO map.
 */
static const gpio_t gpioMap[DAC_COUNT][DAC_CHANNEL_COUNT] =
{
    /// DAC_1
    {
        /// DAC_CHANNEL_1
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_4
        },
        /// DAC_CHANNEL_2
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_5
        }
    }
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Sets the given DAC enabled or disabled.
 * @param dac DAC to access.
 * @param enabled Flag that indicates if the DAC should be enabled or disabled.
 */
static void setEnabled(const dac_t* dac, const bool enabled);

/**
 * @brief Configures the given DAC.
 * @param dac DAC to access.
 * @param config DAC configuration.
 */
static void configure(const dac_t* dac, const dac_config_t* config);

/**
 * @brief Sets the given voltage on the given DAC.
 * @param dac DAC to access.
 * @param voltage Voltage to set on the DAC.
 */
static void set(const dac_t* dac, const float voltage);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void dacDriverStm32f2xxInit(void)
{
    dac_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure,
        .set        = set
    };

    dacSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(const dac_t* dac, const bool enabled)
{
    RCC_APB1PeriphClockCmd(clockMap[dac->dac], (FunctionalState) enabled);
    DAC_Cmd(channelMap[dac->channel], (FunctionalState) enabled);
}

//------------------------------------------------------------------------------
static void configure(const dac_t* dac, const dac_config_t* config)
{
    gpio_config_t gpioConfig =
    {
        .mode       = GPIO_MODE_ANALOG,
        .resistor   = GPIO_RESISTOR_NONE
    };

    gpioHardwareSetEnabled(&(gpioMap[dac->dac][dac->channel]), true);
    gpioHardwareConfigure(&(gpioMap[dac->dac][dac->channel]), &gpioConfig);
}

//------------------------------------------------------------------------------
static void set(const dac_t* dac, const float voltage)
{

}
