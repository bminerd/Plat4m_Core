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
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <i2c_interface.h>

// plat4m
//
// Replace below with proper I2C driver header file.
//
// Example:
//
//      #include <i2c_stm32f10x.h>

#include <i2c_driver_atmega328p.h>

// plat4m
//
// Include necessary manufacturer I2C driver header files here.
//
// Example:
//
//      #include <stm32f10x_i2c.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
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

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local I2C driver functions here.
//
// Example:
//
//      void i2c1SetEnabled(bool enabled);
//      void i2c1MasterStart(void);
//      ...

/**
 * TODO Comment!
 */
static void i2c1SetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static void i2c1MasterStart(void);

/**
 * TODO Comment!
 */
static void i2c1MasterStop(void);

/**
 * TODO Comment!
 */
static void i2c1MasterTxAddress(uint8_t address,
                                       i2c_master_mode_e mode);

/**
 * TODO Comment!
 */
static void i2c1Tx(uint8_t data);

/**
 * TODO Comment!
 */
static void i2c1Rx(uint8_t* data);

/**
 * TODO Comment!
 */
static void i2c1TxIntSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static void i2c1RxIntSetEnabled(bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cDriverInit(void)
{
    // plat4m
    //
    // Initialize I2C driver.
    // Add drivers to I2C interface.
    //
    // Example:
    //
    //      i2c_t i2cs[] =
    //      {
    //          // I2C_1
    //          {
    //              .id             = I2C_ID_1,
    //              .setEnabled     = i2c1SetEnabled,
    //              ...
    //          },
    //          // I2C_2
    //          {
    //              .id             = I2C_ID_2,
    //              .setEnabled     = i2c2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      i2cAddDrivers(i2cs, ARRAY_SIZE(i2cs));


    i2c_driver_t i2cs[] =
    {
        // I2C_1
        {
            .id                 = I2C_ID_1,
            .address            = I2C_OWN_ADDRESS,
            .isEnabled          = true,
            .setEnabled         = i2c1SetEnabled,
            .masterStart        = i2c1MasterStart,
            .masterStop         = i2c1MasterStop,
            .masterTxAddress    = i2c1MasterTxAddress,
            .tx                 = i2c1Tx,
            .rx                 = i2c1Rx,
            .txIntSetEnabled    = i2c1TxIntSetEnabled,
            .rxIntSetEnabled    = i2c1RxIntSetEnabled
        }
    };

    i2cAddDrivers(i2cs, ARRAY_SIZE(i2cs));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local I2C driver functions declared above.

//------------------------------------------------------------------------------
static void i2c1SetEnabled(bool enabled)
{


    if (enabled)
    {

    }
    else
    {

    }
}

//------------------------------------------------------------------------------
static void i2c1MasterStart(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
}

//------------------------------------------------------------------------------
static void i2c1MasterStop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

//------------------------------------------------------------------------------
static void i2c1MasterTxAddress(uint8_t address, i2c_master_mode_e mode)
{
	TWDR = SLA_W;
	TWCR = (1 << TWINT) | (1 << TWEN);
}

//------------------------------------------------------------------------------
static void i2c1Tx(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
}

//------------------------------------------------------------------------------
static void i2c1Rx(uint8_t* data)
{
}

//------------------------------------------------------------------------------
static void i2c1TxIntSetEnabled(bool enabled)
{
    I2C_ITConfig(I2C1, I2C_IT_BUF, (FunctionalState) enabled);
}

//------------------------------------------------------------------------------
static void i2c1RxIntSetEnabled(bool enabled)
{
    // Do nothing for now!
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
            i2cIntHandler(I2C_ID_1, I2C_INTERRUPT_START);

            break;
        }
        case 0x18:
        {
            i2cIntHandler(I2C_ID_1, I2C_INTERRUPT_ADDRESS);

            break;
        }
        case 0x30:
        {
            i2cIntHandler(I2C_ID_1, I2C_INTERRUPT_TX);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            i2cIntHandler(I2C_ID_1, I2C_INTERRUPT_STOP);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void I2C1_ER_IRQHandler(void)
{
}
