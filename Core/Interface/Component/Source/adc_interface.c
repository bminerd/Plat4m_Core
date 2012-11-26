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
 * @brief ADC interface layer.
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

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief ADC driver.
 */
static adc_driver_t driver;

/**
 * @broef ADC info map.
 */
static adc_info_t* adcToInfoMap[ADC_COUNT][ADC_CHANNEL_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void adcInit(void)
{
    int i, j;

    for (i = 0; i < ADC_COUNT; i++)
    {
        for (j = 0; j < ADC_CHANNEL_COUNT; j++)
        {
            adcToInfoMap[i][j] = NULL_POINTER;
        }
    }

    adcApplicationInit();
}

//------------------------------------------------------------------------------
adc_error_e adcSetDriver(const adc_driver_t* adcDriver)
{
#ifdef PLAT4M_DEBUG
    
    if (IS_NULL_POINTER(adcDriver)              ||
        IS_NULL_POINTER(adcDriver->setEnabled)  ||
        IS_NULL_POINTER(adcDriver->configure)   ||
        IS_NULL_POINTER(adcDriver->read))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    driver = *adcDriver;

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
adc_error_e adcInitId(adc_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }

    if (id->adc.adc == ADC_COUNT)
    {
        return ADC_ERROR_ADC_INVALID;
    }

    if (id->adc.channel == ADC_CHANNEL_COUNT)
    {
        return ADC_ERROR_ADC_INVALID;
    }

#endif // PLAT4M_DEBUG

    adc_info_t* info = (adc_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    mutexInitMutex(&(info->mutex));

    adcToInfoMap[id->adc.adc][id->adc.channel] = (adc_info_t*) id->private;

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
adc_error_e adcSetEnabled(adc_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return ADC_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    adc_info_t* info = (adc_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ADC_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.setEnabled(&(id->adc), enabled);
    info->isEnabled = enabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
adc_error_e adcIsEnabled(adc_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    adc_info_t* info = (adc_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ADC_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *isEnabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
adc_error_e adcConfigure(adc_id_t* id, const adc_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    adc_info_t* info = (adc_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ADC_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.configure(&(id->adc), config);
    memcpy((uint8_t*) &(info->config), (uint8_t*) config, sizeof(*config));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
adc_error_e adcRead(adc_id_t* id, float* voltage)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(voltage))
    {
        return ADC_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    adc_info_t* info = (adc_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return ADC_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return ADC_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.read(&(id->adc), voltage);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return ADC_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
