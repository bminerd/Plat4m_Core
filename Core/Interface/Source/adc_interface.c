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
 * @file adc_interface.c
 * @author Ben Minerd
 * @date 7/30/2012
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <adc_interface.h>

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
 * @brief ADC type.
 */
typedef struct _adc_t_
{
    adc_hardware_t hardware;
    bool isEnabled;
    adc_config_t config;
} adc_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief ADC driver.
 */
static adc_driver_t driver;

/**
 * @brief ADC map.
 */
static adc_t adcMap[ADC_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void adcInit(void)
{
    int i;

    for (i = 0; i < ADC_COUNT; i++)
    {
        adcMap[i].hardware.adc      = ADC_NONE;
        adcMap[i].hardware.channel  = ADC_CHANNEL_NONE;
        adcMap[i].isEnabled         = false;
    }

    adcDriverInit();
}

//------------------------------------------------------------------------------
extern adc_error_e adcSetDriver(const adc_driver_t* adcDriver)
{
#ifdef PLAT4M_DEBUG
    
    if (IS_NULL_POINTER(adcDriver))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    driver = *adcDriver;

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern adc_error_e adcSetEnabled(const adc_e adc, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (adc >= ADC_COUNT)
    {
        return ADC_ERROR_ADC_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(&(adcMap[adc].hardware), enabled);
    adcMap[adc].isEnabled = enabled;

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern adc_error_e adcIsEnabled(const adc_e adc, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isEnabled))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }

    if (adc >= ADC_COUNT)
    {
        return ADC_ERROR_ADC_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = adcMap[adc].isEnabled;

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern adc_error_e adcConfigure(const adc_e adc, const adc_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(config))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }

    if (adc >= ADC_COUNT)
    {
        return ADC_ERROR_ADC_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.configure(&(adcMap[adc].hardware), config);
    adcMap[adc].config = *config;

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern adc_error_e adcRead(const adc_e adc, float* voltage)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(voltage))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }

    if (adc >= ADC_COUNT)
    {
        return ADC_ERROR_ADC_INVALID;
    }

    if (!(adcMap[adc].isEnabled))
    {
        return ADC_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    driver.read(&(adcMap[adc].hardware), voltage);

    return ADC_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
