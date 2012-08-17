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
 * @date 7/30/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <adc_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  ADC_DRIVER_ID_LIMIT
#define ADC_DRIVER_COUNT (ADC_DRIVER_ID_LIMIT)
#else
#define ADC_DRIVER_COUNT (ADC_DRIVER_ID_COUNT)
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
static adc_driver_t drivers[ADC_DRIVER_COUNT];

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

    for (i = 0; i < ADC_DRIVER_COUNT; i++)
    {
        drivers[i].id           = (adc_driver_id_e) i;
        drivers[i].setEnabled   = 0;
        drivers[i].read         = 0;
    }

    adcDriverInit();
}

//------------------------------------------------------------------------------
extern bool adcAddDriver(adc_driver_t* adcDriver)
{
    if (adcDriver->id >= ADC_DRIVER_ID_COUNT    ||
        !adcDriver->setEnabled                  ||
        !adcDriver->read)
    {
        return false;
    }

    ADD_DRIVER(drivers, adcDriver);

    return true;
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
extern adc_error_e adcSetEnabled(adc_driver_id_e id, bool enabled)
{
    if (id >= ADC_DRIVER_ID_COUNT)
    {
        return ADC_ERROR_INVALID_ID;
    }

    drivers[id].setEnabled(enabled);

    return ADC_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern adc_error_e adcRead(adc_driver_id_e id, float* voltage)
{
    if (id >= ADC_DRIVER_ID_COUNT)
    {
        return ADC_ERROR_INVALID_ID;
    }

    drivers[id].read(voltage);

    return ADC_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
