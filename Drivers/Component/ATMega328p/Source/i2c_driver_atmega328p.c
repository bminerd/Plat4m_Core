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
 * @file i2c_driver_atmega328p.c
 * @author Ben Minerd
 * @date 9/19/12
 * @brief I2C driver layer for the ATMega328p.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <i2c_interface.h>

#include <i2c_driver.h>

// plat4m
//
// Include necessary manufacturer I2C driver header files here.
//
// Example:
//
//      #include <stm32f10x_i2c.h>

#include <avr/io.h>
#include <util/twi.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C own address.
 */
#define I2C_OWN_ADDRESS (0xA0)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static i2c_interface_int_handler_f* intHandler;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type that sets the given I2C enabled or disabled.
 * @param i2c I2C driver ID.
 * @param enabled Flag that indicates if the I2C should be enabled or disabled.
 */
static void setEnabled(const i2c_e i2c, const bool enabled);

/**
 * @brief Function type that configures the given I2C.
 * @param i2c I2C driver ID.
 * @param config I2C configuration.
 */
static void configure(const i2c_e i2c, const i2c_config_t* config);

/**
 * @brief Function type that performs a start on the given I2C.
 * @param i2c I2C driver ID.
 */
static void masterStart(const i2c_e i2c);

/**
 * @brief Function type that performs a stop on the given I2C.
 * @param i2c I2C driver ID.
 */
static void masterStop(const i2c_e i2c);

/**
 * @brief Function type that transmits a slave address over the given I2C.
 * @param i2c I2C driver ID.
 * @param address I2C slave address.
 * @param mode I2C master mode.
 */
static void masterTxAddress(const i2c_e i2c,
                            const i2c_address_t address,
                            const i2c_master_mode_e mode);

/**
 * @brief Function type that transmits a byte over the given I2C.
 * @param i2c I2C driver ID.
 * @param data Byte to transmit.
 */
static void tx(const i2c_e i2c, const uint8_t data);

/**
 * @brief Function type that receives a byte over the given I2C.
 * @param i2c I2C driver ID.
 * @param data Byte received.
 */
static void rx(const i2c_e i2c, uint8_t* data);

/**
 * @brief Function type that sets the given I2C interrupt enabled or disabled.
 * @param i2c I2C driver ID.
 * @param interrupt I2C interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
static void intSetEnabled(const i2c_e i2c,
                          const i2c_interrupt_e interrupt,
                          const bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void i2cDriverInit(const i2c_interface_int_handler_f* handler)
{
    i2c_driver_t driver =
    {
        .setEnabled 		= setEnabled,
        .configure			= configure,
        .masterStart		= masterStart,
        .masterStop			= masterStop,
        .masterTxAddress	= masterTxAddress,
        .tx					= tx,
        .rx					= rx,
        .intSetEnabled		= intSetEnabled
    };

    i2cSetDriver(&driver);

    intHandler = handler;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local I2C driver functions declared above.

//------------------------------------------------------------------------------
static void setEnabled(const i2c_e i2c, const bool enabled)
{

}

//------------------------------------------------------------------------------
static void configure(const i2c_e i2c, const i2c_config_t* config)
{

}

//------------------------------------------------------------------------------
static void masterStart(const i2c_e i2c)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
}

//------------------------------------------------------------------------------
static void masterStop(const i2c_e i2c)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

//------------------------------------------------------------------------------
static void masterTxAddress(const i2c_e i2c,
                            const i2c_address_t address,
                            const i2c_master_mode_e mode)
{
//	TWDR = SLA_W;
	TWCR = (1 << TWINT) | (1 << TWEN);
}

//------------------------------------------------------------------------------
static void tx(const i2c_e i2c, const uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
}

//------------------------------------------------------------------------------
static void rx(const i2c_e i2c, uint8_t* data)
{
}

//------------------------------------------------------------------------------
static void intSetEnabled(const i2c_e i2c,
                          const i2c_interrupt_e interrupt,
                          const bool enabled)
{

}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ISR(TWI_vect)
{
    switch (TWSR)
    {
        case 0x08:
        {
            intHandler(I2C_1, I2C_INTERRUPT_START);

            break;
        }
        case 0x18:
        {
            intHandler(I2C_1, I2C_INTERRUPT_ADDRESS);

            break;
        }
        case 0x30:
        {
            intHandler(I2C_1, I2C_INTERRUPT_TX);

            break;
        }
        case 0x31:
        {
            intHandler(I2C_1, I2C_INTERRUPT_STOP);

            break;
        }
    }
}
