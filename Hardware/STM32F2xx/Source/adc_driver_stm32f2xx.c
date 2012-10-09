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
 * @file adc_stm32f2xx.c
 * @author Ben Minerd
 * @date 8/9/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <adc_interface.h>
#include <adc_driver.h>

#include <gpio_interface.h>

// plat4m
//
// Include necessary manufacturer driver header files here.
// ex.
//      #include <stm32f10x_usart.h>

#include <stm32f2xx.h>
#include <stm32f2xx_adc.h>
#include <stm32f2xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief ADC map.
 */
static ADC_TypeDef* adcMap[ADC_COUNT] =
{
    ADC1,   /// ADC_1
    ADC2,   /// ADC_2
    ADC3    /// ADC_3
};

/**
 * @brief ADC channel map.
 */
static const uint8_t channelMap[ADC_CHANNEL_COUNT] =
{
    ADC_Channel_0,  /// ADC_CHANNEL_0
    ADC_Channel_1,  /// ADC_CHANNEL_1
    ADC_Channel_2,  /// ADC_CHANNEL_2
    ADC_Channel_3,  /// ADC_CHANNEL_3
    ADC_Channel_4,  /// ADC_CHANNEL_4
    ADC_Channel_5,  /// ADC_CHANNEL_5
    ADC_Channel_6,  /// ADC_CHANNEL_6
    ADC_Channel_7,  /// ADC_CHANNEL_7
    ADC_Channel_8,  /// ADC_CHANNEL_8
    ADC_Channel_9,  /// ADC_CHANNEL_9
    ADC_Channel_10, /// ADC_CHANNEL_10
    ADC_Channel_11, /// ADC_CHANNEL_11
    ADC_Channel_12, /// ADC_CHANNEL_12
    ADC_Channel_13, /// ADC_CHANNEL_13
    ADC_Channel_14, /// ADC_CHANNEL_14
    ADC_Channel_15  /// ADC_CHANNEL_15
};

/**
 * @brief ADC clock map.
 */
static const uint32_t clockMap[ADC_COUNT] =
{
    RCC_APB2Periph_ADC1,    /// ADC_1
    RCC_APB2Periph_ADC2,    /// ADC_2
    RCC_APB2Periph_ADC3     /// ADC_3
};

static const gpio_hardware_t gpioMap[ADC_COUNT][ADC_CHANNEL_COUNT] =
{
    /// ADC_1
    {
        /// ADC_CHANNEL_0
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_1
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_2
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_2
        },
        /// ADC_CHANNEL_3
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_3
        },
        /// ADC_CHANNEL_4
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_4
        },
        /// ADC_CHANNEL_5
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_5
        },
        /// ADC_CHANNEL_6
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_6
        },
        /// ADC_CHANNEL_7
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_7
        },
        /// ADC_CHANNEL_8
        {
            .port   = GPIO_PORT_B,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_9
        {
            .port   = GPIO_PORT_B,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_10
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_11
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_12
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_2
        },
        /// ADC_CHANNEL_13
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_3
        },
        /// ADC_CHANNEL_14
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_4
        },
        /// ADC_CHANNEL_15
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_5
        }
    },
    /// ADC_2
    {
        /// ADC_CHANNEL_0
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_1
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_2
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_2
        },
        /// ADC_CHANNEL_3
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_3
        },
        /// ADC_CHANNEL_4
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_4
        },
        /// ADC_CHANNEL_5
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_5
        },
        /// ADC_CHANNEL_6
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_6
        },
        /// ADC_CHANNEL_7
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_7
        },
        /// ADC_CHANNEL_8
        {
            .port   = GPIO_PORT_B,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_9
        {
            .port   = GPIO_PORT_B,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_10
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_11
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_12
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_2
        },
        /// ADC_CHANNEL_13
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_3
        },
        /// ADC_CHANNEL_14
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_4
        },
        /// ADC_CHANNEL_15
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_5
        }
    },
    /// ADC_3
    {
        /// ADC_CHANNEL_0
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_1
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_2
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_2
        },
        /// ADC_CHANNEL_3
        {
            .port   = GPIO_PORT_A,
            .pin    = GPIO_PIN_3
        },
        /// ADC_CHANNEL_4
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_6
        },
        /// ADC_CHANNEL_5
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_7
        },
        /// ADC_CHANNEL_6
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_8
        },
        /// ADC_CHANNEL_7
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_9
        },
        /// ADC_CHANNEL_8
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_10
        },
        /// ADC_CHANNEL_9
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_3
        },
        /// ADC_CHANNEL_10
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_0
        },
        /// ADC_CHANNEL_11
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_1
        },
        /// ADC_CHANNEL_12
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_2
        },
        /// ADC_CHANNEL_13
        {
            .port   = GPIO_PORT_C,
            .pin    = GPIO_PIN_3
        },
        /// ADC_CHANNEL_14
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_4
        },
        /// ADC_CHANNEL_15
        {
            .port   = GPIO_PORT_F,
            .pin    = GPIO_PIN_5
        }
    }
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local ADC driver functions here.
//
// Example:
//
//      void adc1SetEnabled(bool enabled);
//      void adc1Read(float* voltage);
//      ...

/**
 * @brief Sets the given ADC enabled or disabled.
 * @param hardware ADC to access.
 * @param enabled Flag that indicates if the ADC should be enabled or disabled.
 */
static void setEnabled(const adc_hardware_t* hardware, const bool enabled);

/**
 * @brief Configures the given ADC.
 * @param hardware ADC to access.
 * @param config ADC configuration.
 */
static void configure(const adc_hardware_t* hardware,
                      const adc_config_t* config);

/**
 * @brief Reads a value from the given ADC.
 * @param hardware ADC to access.
 * @param voltage Voltage read from the ADC.
 * @return ADC error.
 */
static void read(const adc_hardware_t* hardware, float* voltage);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void adcDriverInit(void)
{
    // plat4m
    //
    // Initialize ADC driver.
    //
    // Example:

    adc_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure,
        .read       = read
    };

    adcSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(const adc_hardware_t* hardware, const bool enabled)
{
    RCC_APB2PeriphClockCmd(clockMap[hardware->adc], (FunctionalState) enabled);
    ADC_Cmd(adcMap[hardware->adc], (FunctionalState) enabled);
}

//------------------------------------------------------------------------------
static void configure(const adc_hardware_t* hardware,
                      const adc_config_t* config)
{
    gpio_config_t gpioConfig =
    {
        .mode       = GPIO_MODE_ANALOG_INPUT,
        .resistor   = GPIO_RESISTOR_NONE
    };

    gpioHardwareSetEnabled(&(gpioMap[hardware->adc][hardware->channel]), true);
    gpioHardwareConfigure(&(gpioMap[hardware->adc][hardware->channel]),
                          &gpioConfig);

    // ADC common init
    ADC_CommonInitTypeDef commonAdcInit =
    {
        .ADC_Mode               = ADC_Mode_Independent,
        .ADC_Prescaler          = ADC_Prescaler_Div2,
        .ADC_DMAAccessMode      = ADC_DMAAccessMode_Disabled,
        .ADC_TwoSamplingDelay   = ADC_TwoSamplingDelay_5Cycles
    };

    ADC_CommonInit(&commonAdcInit);

    // ADC init
    ADC_InitTypeDef adcInit =
    {
        adcInit.ADC_Resolution              = ADC_Resolution_12b,
        adcInit.ADC_ScanConvMode            = DISABLE,
        adcInit.ADC_ContinuousConvMode      =
            (FunctionalState) config->conversionMode,
        adcInit.ADC_ExternalTrigConvEdge    = ADC_ExternalTrigConvEdge_None,
        adcInit.ADC_ExternalTrigConv        = ADC_ExternalTrigConv_T1_CC1,
        adcInit.ADC_DataAlign               = ADC_DataAlign_Right,
        adcInit.ADC_NbrOfConversion         = 1
    };

    ADC_Init(adcMap[hardware->adc], &adcInit);

    ADC_RegularChannelConfig(adcMap[hardware->adc],
                             channelMap[hardware->channel],
                             1,
                             ADC_SampleTime_3Cycles);
}

//------------------------------------------------------------------------------
static void read(const adc_hardware_t* hardware, float* voltage)
{
    uint16_t raw = 0;

    ADC_SoftwareStartConv(ADC3);

    while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);

    raw = ADC_GetConversionValue(ADC3);
    *voltage = 3.3f * raw / 4095u;
}
