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
 * @file i2c_driver_avr.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <i2c_interface.h>
#include <i2c_driver_avr.h>

//#include <iom328p.h>
//#include <twi.h>

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
static i2c_error_e i2cMasterStart(void);

/**
 *
 */
static i2c_error_e i2cMasterStop(void);

/**
 *
 */
static i2c_error_e i2cMasterTxAddress(uint8_t address, i2c_master_mode_e mode);

/**
 *
 */
static i2c_error_e i2cTx(uint8_t byte);

/**
 *
 */
static i2c_error_e i2cRx(uint8_t* byte);

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static i2c_error_e i2cMasterStart(void)
{
//	// Send START condition
//	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
//
//	// Wait for TWINT flag to be set
//	while (!(TWCR & (1 << TWINT)));
//
//	// Check value of TWI Status Register
//	if ((TWSR & 0xF8) != START)
//	{
//		// return error
//	}

	return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e i2cMasterStop(void)
{
//	// Send STOP condition
//	TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWSTO);

	return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e i2cMasterTxAddress(uint8_t address, i2c_master_mode_e mode)
{
//	TWDR = (address << 1) | (uint8_t)mode;
//	TWCR = (1 << TWINT) | (1 << TWEN);
//
//	// Wait for TWINT flag to be set
//	while (!(TWCR & (1 << TWINT)));
//
//	// Check value of TWI Status Register
//	if ((TWSR & 0xF8) != MT_SLA_ACK)
//	{
//		// return error
//	}

	return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e i2cTx(uint8_t byte)
{
//	TWDR = byte;
//	TWCR = (1 << TWINT) | (1 << TWEN);
//
//	// Wait for TWINT flag to be set
//	while (!(TWCR & (1 << TWINT)));
//
//	// Check value of TWI Status Register
//	if ((TWSR & 0xF8) != MT_DATA_ACK)
//	{
//		// return error
//	}

	return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e i2cRx(uint8_t* byte)
{
//	// Wait for TWINT flag to be set
//	while (!(TWCR & (1 << TWINT)));
//
//	// Check value of TWI Status Register
//	if ((TWSR & 0xF8) != MR_DATA_ACK)
//	{
//		// return error
//	}
//
//	*byte = TWDR;

	return I2C_ERROR_NONE;
}
