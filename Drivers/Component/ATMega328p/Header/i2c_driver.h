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
 * @file i2c_driver.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief I2C driver layer for the ATMega328p.
 */

#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

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
 * @brief Enumeration of I2Cs.
 */
typedef enum _i2c_e_
{
    // Do not place values above!
	I2C_NONE = -1,
    
    I2C_1,
    
    // Do not place values below!
    I2C_COUNT
} i2c_e;

/**
 * @brief Enumeration of I2C interrupts.
 */
typedef enum _i2c_interrupt_e_
{
    // Do not place values above!
    I2C_INTERRUPT_NONE = -1,

    I2C_INTERRUPT_START,
    I2C_INTERRUPT_STOP,
    I2C_INTERRUPT_ADDRESS,
    I2C_INTERRUPT_TX,
    I2C_INTERRUPT_RX,

    // Do not place values below!
    I2C_INTERRUPT_COUNT
} i2c_interrupt_e;

/**
 * @brief Enumeration of I2C GPIO map entries.
 */
typedef enum _i2c_gpio_map_e_
{
    // Do not place values above!
    I2C_GPIO_MAP_NONE = -1,

    I2C_GPIO_MAP_1,

    // Do not place values below!
    I2C_GPIO_MAP_COUNT
} i2c_gpio_map_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type for an I2C interface interrupt handler.
 * @param i2c I2C that caused an interrupt.
 * @param interrupt Interrupt that occurred.
 */
typedef void i2c_interface_int_handler_f(const i2c_e i2c,
										 const i2c_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the I2C driver layer.
 */
void i2cDriverInit(const i2c_interface_int_handler_f* handler);

#endif // _I2C_DRIVER_H_
