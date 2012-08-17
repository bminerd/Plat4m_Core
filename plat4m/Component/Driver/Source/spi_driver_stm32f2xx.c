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
 * @file spi_driver_template.c
 * @author Ben Minerd
 * @date 8/1/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <spi_interface.h>

// plat4m
//
// Replace below with proper SPI driver header file.
// ex.
//      #include <spi_driver_stm32f10x.h>

#include <spi_driver_stm32f2xx.h>

// plat4m
//
// Include necessary manufacturer driver header files here.
// ex.
//      #include <stm32f10x_spi.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

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
// Declare local SPI driver functions here.
//
// Example:
//
//      static void spi1SetEnabled(bool setEnabled);
//      static void spi1Tx(uint8_t data[], uint8_t size);
//      ...

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void spiDriverInit(void)
{
    // plat4m
    //
    // Initialize SPI driver.
    // Add drivers to SPI interface.
    //
    // Example:
    //
    //      spi_driver_t spis[] =
    //      {
    //          // UART1
    //          {
    //              .id         = SPI_DRIVER_ID_1,
    //              .setEnabled = spi1SetEnabled,
    //              ...
    //          },
    //          // UART2
    //          {
    //              .id         = SPI_DRIVER_ID_2,
    //              .setEnabled = spi2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      spiAddDrivers(spis, ARRAY_SIZE(spis));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local SPI driver functions declared above.
