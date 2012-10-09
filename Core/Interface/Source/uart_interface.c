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
 * @brief UART interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <uart_interface.h>

#include <buffer_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief UART type.
 */
typedef struct _uart_t_
{
    uart_e uart;
    bool isEnabled;
    uint8_t txBufferMemory[UART_BUFFER_SIZE];
    buffer_t txBuffer;
    uint8_t rxBufferMemory[UART_BUFFER_SIZE];
    buffer_t rxBuffer;
    const data_callback_f* rxCallback;
} uart_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief UART driver.
 */
static uart_driver_t driver;

/**
 * @brief UART map.
 */
static uart_t uartMap[UART_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief UART interrupt handler.
 */
static void intHandler(const uart_e uart, const uart_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartInit(void)
{
    int i;

    for (i = 0; i < UART_COUNT; i++)
    {
        uartMap[i].isEnabled    = false;

        BUFFER_INIT(&(uartMap[i].txBuffer), uartMap[i].txBufferMemory);
        BUFFER_INIT(&(uartMap[i].rxBuffer), uartMap[i].rxBufferMemory);
        
        uartMap[i].rxCallback   = NULL_POINTER;
    }

    uartDriverInit(intHandler);
}

//------------------------------------------------------------------------------
extern uart_error_e uartSetDriver(const uart_driver_t* uartDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(uartDriver)             ||
        IS_NULL_POINTER(uartDriver->setEnabled) ||
        IS_NULL_POINTER(uartDriver->tx)         ||
        IS_NULL_POINTER(uartDriver->rx)         ||
        IS_NULL_POINTER(uartDriver->intSetEnabled))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *uartDriver;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartSetRxCallback(const uart_e uart,
                                      const data_callback_f* rxCallback)
{
#ifdef PLAT4M_DEBUG

    if ((uart >= UART_COUNT) || IS_NULL_POINTER(rxCallback))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    uartMap[uart].rxCallback = rxCallback;
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartSetEnabled(const uart_e uart, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (uart >= UART_COUNT)
    {
        return UART_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(uartMap[uart].uart, enabled);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartIsEnabled(const uart_e uart, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (uart >= UART_COUNT)
    {
        return UART_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = uartMap[uart].isEnabled;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartTx(const uart_e uart, const byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (uart >= UART_COUNT)
    {
        return UART_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_WRITE(&(uartMap[uart].txBuffer), &(data->bytes[i])))
        {
            break;
        }
    }

    driver.intSetEnabled(uartMap[uart].uart, UART_INTERRUPT_TX, true);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartRead(const uart_e uart, byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

    if (uart >= UART_COUNT)
    {
        return UART_ERROR_INVALID;
    }

    if (!uartMap[uart].isEnabled)
    {
        return UART_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_READ(&(uartMap[uart].rxBuffer), &(data->bytes[i])))
        {
            break;
        }
    }

    data->size = i;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartBytesAvailable(const uart_e uart, uint8_t* byteCount)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(byteCount))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

    if (uart >= UART_COUNT)
    {
        return UART_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    bufferCount(&(uartMap[uart].rxBuffer), byteCount);

    return UART_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void intHandler(const uart_e uart, const uart_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if ((uart == UART_NONE)                 ||
        (uart >= UART_COUNT)                ||
        (interrupt == UART_INTERRUPT_NONE)  ||
        (interrupt >= UART_INTERRUPT_COUNT))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    uint8_t byte;

    switch (interrupt)
    {
        case UART_INTERRUPT_TX:
        {
            if (BUFFER_READ(&(uartMap[uart].txBuffer), &byte))
            {
                driver.tx(uart, byte);
            }
            else
            {
                driver.intSetEnabled(uart, UART_INTERRUPT_TX, false);
            }

            break;
        }
        case UART_INTERRUPT_RX:
        {
            driver.rx(uart, &byte);

            if (BUFFER_WRITE(&(uartMap[uart].rxBuffer), &byte))
            {
                uartMap[uart].rxCallback(byte);
            }

            break;
        }
    }
}
