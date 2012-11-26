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
 * @file io_expander_driver_mcp23008.h
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief IO expander driver layer for the MCP23008.
 */

#ifndef _IO_EXPANDER_DRIVER_MCP23008_H_
#define _IO_EXPANDER_DRIVER_MCP23008_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define IO_EXPANDER_MCP23008_IO_PORT_COUNT  1
#define IO_EXPANDER_MCP23008_IO_PIN_COUNT   8

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of IO expander registers.
 */
typedef enum _io_expander_mcp23008_reg_e_
{
    IO_EXPANDER_MCP23008_REG_IODIR = 0,
    IO_EXPANDER_MCP23008_REG_IPOL,
    IO_EXPANDER_MCP23008_REG_GPINTEN,
    IO_EXPANDER_MCP23008_REG_DEFVAL,
    IO_EXPANDER_MCP23008_REG_INTCON,
    IO_EXPANDER_MCP23008_REG_IOCON,
    IO_EXPANDER_MCP23008_REG_GPPU,
    IO_EXPANDER_MCP23008_REG_INTF,
    IO_EXPANDER_MCP23008_REG_INTCAP,
    IO_EXPANDER_MCP23008_REG_GPIO,
    IO_EXPANDER_MCP23008_REG_OLAT,

    // Do not place values below!
    IO_EXPANDER_MCP23008_REG_COUNT
} io_expander_mcp23008_reg_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief IO expander MCP23008 info type.
 */
typedef struct _io_expander_mcp23008_info_t_
{
    uint8_t regValueMap[IO_EXPANDER_MCP23008_REG_COUNT];
} io_expander_mcp23008_info_t;

/**
 * @brief IO expander MCP23008 type.
 */
typedef struct _io_expander_mcp23008_t_
{
    i2c_device_id_t* i2cDeviceId;
    private_data_t private[sizeof(io_expander_mcp23008_info_t) /
                           sizeof(private_data_t)];
} io_expander_mcp23008_t;

/**
 * @brief IO expander configuration type.
 */
typedef struct _io_expander_mcp23008_config_t_
{
    int a;
} io_expander_mcp23008_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the IO expander driver layer.
 */
void ioExpanderDriverMcp23008Init(void);

#endif // _IO_EXPANDER_DRIVER_MCP23008_H_
