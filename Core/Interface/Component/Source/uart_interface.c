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
#include <mutex_interface.h>

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

/**
 * @brief UART driver.
 */
static uart_driver_t driver;

/**
 * @brief ID to UART map.
 */
static uart_info_t* uartToInfoMap[UART_COUNT];

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
void uartInit(void)
{
    int i;

    // Initialize ID to UART map
    for (i = 0; i < UART_COUNT; i++)
    {
        uartToInfoMap[i] = NULL_POINTER;
    }

    uartApplicationInit();
}

//------------------------------------------------------------------------------
uart_error_e uartSetDriver(const uart_driver_t* uartDriver,
                           uart_interface_int_handler_f** handler)
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
    *handler = intHandler;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartInitId(uart_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

    if (id->uart == UART_COUNT)
    {
        return UART_ERROR_UART_INVALID;
    }

    if (!IS_NULL_POINTER(uartToInfoMap[id->uart]))
    {
        return UART_ERROR_ID_DUPLICATE;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    mutexInitMutex(&(info->mutex));

    BUFFER_INIT(&(info->txBuffer), info->txBufferMemory);
    BUFFER_INIT(&(info->rxBuffer), info->rxBufferMemory);

    uartToInfoMap[id->uart] = (uart_info_t*) id->private;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartSetRxCallback(uart_id_t* id,
                               uart_rx_data_callback_f* rxCallback)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(rxCallback))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

#ifdef PLAT4M_DEBUG
    
    if (!(info->isInitialized))
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    info->rxCallback = rxCallback;
    
    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartSetEnabled(uart_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.setEnabled(id->uart, enabled);
    driver.intSetEnabled(id->uart, UART_INTERRUPT_RX, enabled);

    info->isEnabled = enabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartIsEnabled(uart_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *isEnabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartConfigure(uart_id_t* id, const uart_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.configure(id->uart, config);
    memcpy((uint8_t*) &(info->config), (uint8_t*) config, sizeof(*config));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartTx(uart_id_t* id, const byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(data))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    int i;

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_WRITE(&(info->txBuffer), &(data->bytes[i])))
        {
            break;
        }
    }

    driver.intSetEnabled(id->uart, UART_INTERRUPT_TX, true);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartRead(uart_id_t* id, byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return UART_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    int i;

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_READ(&(info->rxBuffer), &(data->bytes[i])))
        {
            break;
        }
    }

    data->size = i;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
uart_error_e uartBytesAvailable(uart_id_t* id, int* byteCount)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(byteCount))
    {
        return UART_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    uart_info_t* info = (uart_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UART_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return UART_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    bufferCount(&(info->rxBuffer), byteCount);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return UART_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void intHandler(const uart_e uart, const uart_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(uartToInfoMap[uart]))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    // Map UART back to ID
    uart_info_t* info = uartToInfoMap[uart];

    uint8_t byte;

    switch (interrupt)
    {
        case UART_INTERRUPT_TX:
        {
            if (BUFFER_READ(&(info->txBuffer), &byte))
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

            if (BUFFER_WRITE(&(info->rxBuffer), &byte))
            {
                info->rxCallback(byte);
            }

            break;
        }
        default:
        {
            break;
        }
    }
}
