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
 * @file adc_driver_template.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief 
 */

#ifndef _ADC_DRIVER_H_
#define _ADC_DRIVER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief
 */
typedef enum _adc_driver_id_e_
{
    // plat4m
    //
    // Add ADC driver IDs here.
    //
    // Example:
    //
    //      ADC_DRIVER_ID_2_CHANNEL_7,
    //      ...
    //
    // Note: first ADC driver ID should be set to 0 (zero).
    //
    // Example:
    //
    //      ADC_DRIVER_ID_1_CHANNEL_0 = 0,
    //      ADC_DRIVER_ID_1_CHANNEL_1,
    //      ...

    // Do not place values below!
    ADC_DRIVER_ID_COUNT
} adc_driver_id_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief
 */
extern void adcDriverInit(void);

#endif // _ADC_DRIVER_H_
