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

#ifdef  ADC_ID_LIMIT
#define ADC_COUNT (ADC_ID_LIMIT)
#else
#define ADC_COUNT (ADC_ID_COUNT)
#endif

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
 * TODO Comment!
 */
static adc_driver_t drivers[ADC_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static bool adcAddDriver(adc_driver_t* adcDriver);

/**
 * TODO Comment!
 */
static bool isDriverEnabled(adc_id_e id);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void adcInit(void)
{
    int i;

    for (i = 0; i < ADC_COUNT; i++)
    {
        drivers[i].id           = (adc_id_e) i;
        drivers[i].isEnabled    = false;
        drivers[i].setEnabled   = NULL_POINTER;
        drivers[i].read         = NULL_POINTER;
    }

    adcDriverInit();
}

//------------------------------------------------------------------------------
extern bool adcAddDrivers(adc_driver_t adcDrivers[], uint8_t size)
{
    int i;
    
    for (i = 0; i < size; i++)
    {
        if (!adcAddDriver(&adcDrivers[i]))
        {
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern adc_error_e adcIsEnabled(adc_id_e id, bool* isEnabled)
{
    if (id >= ADC_ID_COUNT)
    {
        return ADC_ERROR_INVALID_ID;
    }

    *isEnabled = isDriverEnabled(id);

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern adc_error_e adcSetEnabled(adc_id_e id, bool enabled)
{
    if (id >= ADC_ID_COUNT)
    {
        return ADC_ERROR_INVALID_ID;
    }

    drivers[id].setEnabled(enabled);

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern adc_error_e adcRead(adc_id_e id, float* voltage)
{
    if (id >= ADC_ID_COUNT || IS_NULL_POINTER(voltage))
    {
        return ADC_ERROR_INVALID_ID;
    }

    if (!isDriverEnabled(id))
    {
        return ADC_ERROR_NOT_ENABLED;
    }

    drivers[id].read(voltage);

    return ADC_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool adcAddDriver(adc_driver_t* adcDriver)
{
    if (IS_NULL_POINTER(adcDriver)              ||
        (adcDriver->id >= ADC_ID_COUNT)  ||
        IS_NULL_POINTER(adcDriver->setEnabled)  ||
        IS_NULL_POINTER(adcDriver->read))
    {
        return false;
    }

    ADD_DRIVER(drivers, adcDriver);

    if (drivers[adcDriver->id].isEnabled)
    {
        adcSetEnabled(adcDriver->id, true);
    }

    return true;
}

//------------------------------------------------------------------------------
static bool isDriverEnabled(adc_id_e id)
{
    return drivers[id].isEnabled;
}
