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
 * @file dac_ic_driver_ltc2637.h
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC ID driver layer for the LTC2637.
 */

#ifndef _DAC_IC_DRIVER_LTC2637_H_
#define _DAC_IC_DRIVER_LTC2637_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define DAC_IC_LTC2637_CHANNEL_COUNT 8

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief DAC IC LTC2637 info type.
 */
typedef struct _dac_ic_ltc2637_info_t_
{
    int a; // TODO Add info
} dac_ic_ltc2637_info_t;

/**
 * @brief DAC IC LTC2637 type.
 */
typedef struct _dac_ic_ltc2637_t_
{
    i2c_device_id_t* i2cDeviceId;
    generic_data_t private[sizeof(dac_ic_ltc2637_info_t)];
} dac_ic_ltc2637_t;

/**
 * @brief DAC IC LTC2637 configuration type.
 */
typedef struct _dac_ic_ltc2637_config_t_
{
    int a; // TODO Add configuration
} dac_ic_ltc2637_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the DAC IC driver layer for the LTC2637.
 */
void dacIcDriverLtc2637Init(void);

#endif // _DAC_IC_DRIVER_LTC2637_H_
