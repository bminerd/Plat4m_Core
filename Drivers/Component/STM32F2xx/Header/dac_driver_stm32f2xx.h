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
 * @file dac_driver_stm32f2xx.h
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC driver layer for the STM32F2xx.
 */

#ifndef _DAC_DRIVER_STM32F2XX_H_
#define _DAC_DRIVER_STM32F2XX_H_

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
 * @brief Enumeration of DACs.
 */
typedef enum _dac_e_
{
    DAC_1 = 0,

    // Do not place values below!
    DAC_COUNT
} dac_e;

/**
 * @brief Enumeration of DAC channels.
 */
typedef enum _dac_channel_e_
{
    DAC_CHANNEL_1 = 0,
    DAC_CHANNEL_2,

    // Do not place values below!
    DAC_CHANNEL_COUNT
} dac_channel_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the DAC driver layer.
 */
void dacDriverStm32f2xxInit(void);

#endif // _DAC_DRIVER_STM32F2XX_H_
