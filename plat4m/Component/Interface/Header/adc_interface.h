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
 * @file adc_interface.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief TODO Comment!
 */

#ifndef _ADC_INTERFACE_H_
#define _ADC_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <adc_driver.h>

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _adc_error_e_
{
    ADC_ERROR_NONE = 0,
    ADC_ERROR_INVALID_ID,

    // Do not place values below!
    ADC_ERROR_COUNT
} adc_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef set_enabled_f adc_driver_set_enabled_f;

/**
 * TODO Comment!
 */
typedef adc_error_e adc_driver_read_f(float* voltage);

/**
 * TODO Comment!
 */
typedef struct _adc_driver_t_
{
    adc_driver_id_e id;
    adc_driver_set_enabled_f* setEnabled;
    adc_driver_read_f* read;
} adc_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void adcInit(void);

/**
 * TODO Comment!
 */
extern bool adcAddDriver(adc_driver_t* adcDriver);

/**
 * TODO Comment!
 */
extern bool adcAddDrivers(adc_driver_t adcDriver[], uint8_t size);

/**
 * TODO Comment!
 */
extern adc_error_e adcSetEnabled(adc_driver_id_e id, bool enabled);

/**
 * TODO Comment!
 */
extern adc_error_e adcRead(adc_driver_id_e id, float* voltage);

#endif // _ADC_INTERFACE_H_
