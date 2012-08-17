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
 * @file uart_interface.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <uart_interface.h>

#include <buffer_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef UART_DRIVER_ID_LIMIT
#define UART_DRIVER_COUNT (UART_DRIVER_ID_LIMIT)
#else
#define UART_DRIVER_COUNT (UART_DRIVER_ID_COUNT)
#endif

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 *
 */
static uart_driver_t drivers[UART_DRIVER_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
static bool uartTxCallback(uart_driver_id_e id);

/**
 *
 */
static bool uartRxCallback(uart_driver_id_e id);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartInit(void)
{
    int i;

    for (i = 0; i < UART_DRIVER_COUNT; i++)
    {
        drivers[i].id = (uart_driver_id_e) i;
        bufferInit(&(drivers[i].txBuffer));
        bufferInit(&(drivers[i].rxBuffer));
        drivers[i].setEnabled = 0;
        drivers[i].tx = 0;
        drivers[i].rx = 0;
        drivers[i].txIntSetEnabled = 0;
        drivers[i].rxIntSetEnabled = 0;
    }

    uartDriverInit();
}

//------------------------------------------------------------------------------
extern bool uartAddDriver(uart_driver_t uartDriver)
{
    if (uartDriver.id >= UART_DRIVER_ID_COUNT ||
       !uartDriver.tx ||
       !uartDriver.rx)
    {
        return false;
    }

    DRIVER_ADD(drivers, uartDriver, uart_driver_t);

    return true;
}

//------------------------------------------------------------------------------
extern bool uartAddDrivers(uart_driver_t uartDrivers[], uint8_t size)
{
    int i;

    if (size > UART_DRIVER_COUNT)
    {
        return false;
    }

    for (i = 0; i < size; i++)
    {
        if (!uartAddDriver(uartDrivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern bool uartTx(uart_driver_id_e id, uint8_t data[], uint8_t size)
{
    int i;

    if (id >= UART_DRIVER_ID_COUNT ||
        !drivers[id].tx ||
        size > UART_DATA_MAX)
    {
        return false;
    }

    // Load UART Tx buffer
    for (i = 0; i < size; i++)
    {
        if (!bufferWrite(&(drivers[id].txBuffer), data[i]))
        {
            // Does this make sense?
            return false;
        }
    }

    drivers[id].txIntSetEnabled(true);
    
    return true;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool uartTxCallback(uart_driver_id_e id)
{
    uint8_t data;

    if (id >= UART_DRIVER_ID_COUNT)
    {
        return false;
    }

    if (!bufferRead(&(drivers[id].txBuffer), &data))
    {
        // Does this make sense?
        drivers[id].txIntSetEnabled(false);

        return false;
    }

    return drivers[id].tx(data);
}

//------------------------------------------------------------------------------
static bool uartRxCallback(uart_driver_id_e id)
{
    uint8_t data;

    if (id >= UART_DRIVER_ID_COUNT)
    {
        return false;
    }

    if (!drivers[id].rx(&data))
    {
        return false;
    }

    if (!bufferWrite(&(drivers[id].rxBuffer), data))
    {
        return false;
    }

    return true;
}
