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

// plat4m
//
// Replace below with proper driver header file.
// ex.
//      #include <adc_stm32f10x.h>

#include <adc_driver_stm32f2xx.h>

// plat4m
//
// Include necessary manufacturer driver header files here.
// ex.
//      #include <stm32f10x_usart.h>

#include <

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

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
 * TODO Comment!
 */
static void adc1SetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static adc_error_e adc1Read(float* voltage);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void adcDriverInit(void)
{
    // plat4m
    //
    // Initialize ADC driver.
    // Add drivers to ADC interface.
    //
    // Example:
    //
    //      adc_t adcs[] =
    //      {
    //          // ADC_1
    //          {
    //              .id         = ADC_ID_1,
    //              .isEnabled  = true,
    //              .setEnabled = adc1SetEnabled,
    //              ...
    //          },
    //          // ADC_2
    //          {
    //              .id         = ADC_ID_2,
    //              .isEnabled  = true,
    //              .setEnabled = adc2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      adcAddDrivers(adcs, ARRAY_SIZE(adcs));

    adc_driver_t adcs[] =
    {
        // ADC_1
        {
            .id         = ADC_ID_1,
            .isEnabled  = true,
            .setEnabled = adc1SetEnabled,
            .read       = adc1Read
        }
    };

    adcAddDrivers(adcs, ARRAY_SIZE(adcs));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void adc1SetEnabled(bool enabled)
{
    // TODO Use GPIO interface!
    GPIO_InitTypeDef gpioInit;
    ADC_CommonInitTypeDef commonAdcInit;
    ADC_InitTypeDef adcInit;

    if (enabled)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

        // GPIO init
        gpioInit.GPIO_Pin                   = GPIO_Pin_9;
        gpioInit.GPIO_Mode                  = GPIO_Mode_AN;
        gpioInit.GPIO_PuPd                  = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOF, &gpioInit);

        // ADC common init
        commonAdcInit.ADC_Mode              = ADC_Mode_Independent;
        commonAdcInit.ADC_Prescaler         = ADC_Prescaler_Div2;
        commonAdcInit.ADC_DMAAccessMode     = ADC_DMAAccessMode_Disabled;
        commonAdcInit.ADC_TwoSamplingDelay  = ADC_TwoSamplingDelay_5Cycles;
        ADC_CommonInit(&commonAdcInit);

        // ADC init
        adcInit.ADC_Resolution              = ADC_Resolution_12b;
        adcInit.ADC_ScanConvMode            = DISABLE;
        adcInit.ADC_ContinuousConvMode      = DISABLE;
        adcInit.ADC_ExternalTrigConvEdge    = ADC_ExternalTrigConvEdge_None;
        adcInit.ADC_ExternalTrigConv        = ADC_ExternalTrigConv_T1_CC1;
        adcInit.ADC_DataAlign               = ADC_DataAlign_Right;
        adcInit.ADC_NbrOfConversion         = 1;
        ADC_Init(ADC3, &adcInit);

        ADC_RegularChannelConfig(ADC3,
                                 ADC_Channel_7,
                                 1,
                                 ADC_SampleTime_3Cycles);
        ADC_Cmd(ADC3, ENABLE);
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, DISABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, DISABLE);
    }
}

//------------------------------------------------------------------------------
static adc_error_e adc1Read(float* voltage)
{
    uint16_t raw = 0;

    ADC_SoftwareStartConv(ADC3);

    while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);

    raw = ADC_GetConversionValue(ADC3);
    *voltage = 3.3f * raw / 4095u;

    return ADC_ERROR_NONE;
}
