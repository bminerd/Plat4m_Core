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
 * @file io_expander_driver_pca9554.h
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief IO expander driver layer for the PCA9554.
 */

#ifndef _IO_EXPANDER_DRIVER_PCA9554_H_
#define _IO_EXPANDER_DRIVER_PCA9554_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <io_expander_application.h>
#include <i2c_interface.h>
#include <gpio_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define IO_EXPANDER_PCA9554_IO_PORT_COUNT  1
#define IO_EXPANDER_PCA9554_IO_PIN_COUNT   8

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of IO expander registers.
 */
typedef enum _io_expander_pca9554_reg_e_
{
    IO_EXPANDER_PCA9554_REG_INPUT = 0,
    IO_EXPANDER_PCA9554_REG_OUTPUT,
    IO_EXPANDER_PCA9554_REG_POLARITY,
    IO_EXPANDER_PCA9554_REG_CONFIG,

    // Do not place values below!
    IO_EXPANDER_PCA9554_REG_COUNT
} io_expander_pca9554_reg_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief IO expander PCA9554 info type.
 */
typedef struct _io_expander_pca9554_info_t
{
    uint8_t regValueMap[IO_EXPANDER_PCA9554_REG_COUNT];
} io_expander_pca9554_info_t;

/**
 * @brief IO expander type.
 */
typedef struct _io_expander_pca9554_t_
{
    i2c_device_id_t* i2cDeviceId;
    generic_data_t private[sizeof(io_expander_pca9554_info_t)];
} io_expander_pca9554_t;

/**
 * @brief IO expander configuration type.
 */
typedef struct _io_expander_pca9554_config_t_
{
    int a;
} io_expander_pca9554_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the IO expander driver layer for the PCA9554.
 */
void ioExpanderDriverPca9554Init(void);

#endif // _IO_EXPANDER_DRIVER_PCA9554_H_
