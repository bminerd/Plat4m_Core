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
 * @file spi_driver.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief SPI driver layer for the STM32F2xx.
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

    SPI_1,
    SPI_2,
    SPI_3,
    
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
    // TODO Add error interrupt

    // Do not place values below!
    SPI_INTERRUPT_COUNT
} spi_interrupt_e;

/**
 * @brief Enumeration of SPI GPIO map entries.
 */
typedef enum _spi_gpio_map_e_
{
    // Do not place values above!
    SPI_GPIO_MAP_NONE = -1,

    SPI_GPIO_MAP_1,
    SPI_GPIO_MAP_2,

    // Do not place values below!
    SPI_GPIO_MAP_COUNT
} spi_gpio_map_e;

/**
 * @brief Enumeration of SPI clock prescalers.
 */
typedef enum _spi_clock_prescaler_e_
{
    SPI_CLOCK_PRESCALER_2,
    SPI_CLOCK_PRESCALER_4,
    SPI_CLOCK_PRESCALER_8,
    SPI_CLOCK_PRESCALER_16,
    SPI_CLOCK_PRESCALER_32,
    SPI_CLOCK_PRESCALER_64,
    SPI_CLOCK_PRESCALER_128,
    SPI_CLOCK_PRESCALER_256,

    // Do not place values below!
    SPI_CLOCK_PRESCALER_COUNT
} spi_clock_prescaler_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type for a SPI interface interrupt handler.
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
