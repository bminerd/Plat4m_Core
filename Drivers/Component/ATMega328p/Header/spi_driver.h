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
 * @file spi_driver_stm32f2xx.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief TODO Comment!
 */

#ifndef _SPI_DRIVER_H_
#define _SPI_DRIVER_H_

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
 * @brief Enumeration of SPIs.
 */
typedef enum _spi_e_
{
    // Do not place values above!
	SPI_NONE = -1,

    SPI_1 = 0,
    
    // Do not place values below!
    SPI_COUNT
} spi_e;

/**
 * @brief Enumeration of SPI interrupts.
 */
typedef enum _spi_interrupt_e_
{
	// Do not place values above!
	SPI_INTERRUPT_NONE = -1,

	SPI_INTERRUPT_TX,
	SPI_INTERRUPT_RX,

	// Do not place values below!
	SPI_INTERRUPT_COUNT
} spi_interrupt_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type for a SPI interface interrupt handler.
 * @param spi SPI that caused an interrupt.
 * @param interrupt Interrupt that occurred.
 */
typedef void spi_interface_int_handler_f(const spi_e spi,
										 const spi_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the SPI driver layer.
 */
extern void spiDriverInit(const spi_interface_int_handler_f* handler);

#endif // _SPI_DRIVER_H_
