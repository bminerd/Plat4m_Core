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
 * @file io_expander_driver_stmpe811.h
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief IO expander driver layer for the STMPE811.
 */

#ifndef _IO_EXPANDER_DRIVER_STMPE811_H_
#define _IO_EXPANDER_DRIVER_STMPE811_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define IO_EXPANDER_STMPE811_IO_PORT_COUNT  1
#define IO_EXPANDER_STMPE811_IO_PIN_COUNT   8

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of IO expander registers.
 */
typedef enum _io_expander_stmpe811_reg_e_
{
    IO_EXPANDER_STMPE811_REG_CHIP_ID = 0,
    IO_EXPANDER_STMPE811_REG_ID_VER,
    IO_EXPANDER_STMPE811_REG_SYS_CTRL1,
    IO_EXPANDER_STMPE811_REG_SYS_CTRL2,
    IO_EXPANDER_STMPE811_REG_INT_CTRL,
    IO_EXPANDER_STMPE811_REG_INT_EN,
    IO_EXPANDER_STMPE811_REG_INT_STA,
    IO_EXPANDER_STMPE811_REG_GPIO_EN,
    IO_EXPANDER_STMPE811_REG_GPIO_INT_STA,
    IO_EXPANDER_STMPE811_REG_GPIO_SET_PIN,
    IO_EXPANDER_STMPE811_REG_GPIO_CLR_PIN,
    IO_EXPANDER_STMPE811_REG_GPIO_MP_STA,
    IO_EXPANDER_STMPE811_REG_GPIO_DIR,
    IO_EXPANDER_STMPE811_REG_GPIO_ED,
    IO_EXPANDER_STMPE811_REG_GPIO_RE,
    IO_EXPANDER_STMPE811_REG_GPIO_FE,
    IO_EXPANDER_STMPE811_REG_GPIO_AF,

    // Do not place values below!
    IO_EXPANDER_STMPE811_REG_COUNT
} io_expander_stmpe811_reg_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief IO expander STMPE811 info type.
 */
typedef struct _io_expander_stmpe811_info_t_
{
    uint8_t regValueMap[IO_EXPANDER_STMPE811_REG_COUNT];
} io_expander_stmpe811_info_t;

/**
 * @brief IO expander STMPE811 type.
 */
typedef struct _io_expander_stmpe811_t_
{
    i2c_device_id_t* i2cDeviceId;
    private_data_t private[sizeof(io_expander_stmpe811_info_t) /
                           sizeof(private_data_t)];
} io_expander_stmpe811_t;

/**
 * @brief IO expander configuration type.
 */
typedef struct _io_expander_stmpe811_config_t_
{
    int a;
} io_expander_stmpe811_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the IO expander driver layer.
 */
void ioExpanderDriverStmpe811Init(void);

#endif // _IO_EXPANDER_DRIVER_STMPE811_H_
