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
 * @brief TODO Comment!
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

/**
 * TODO Comment!
 */
typedef struct _uart_driver_container_t_
{
    uart_driver_t driver;
    uint8_t txBufferMemory[UART_DRIVER_BUFFER_SIZE];
    buffer_t txBuffer;
    uint8_t rxBufferMemory[UART_DRIVER_BUFFER_SIZE];
    buffer_t rxBuffer;
    data_callback_f* rxCallbacks[UART_DRIVER_RX_CALLBACK_COUNT];
    uint8_t rxCallbackCount;
} uart_driver_container_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static uart_driver_container_t containers[UART_DRIVER_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static void callRxCallbacks(uart_driver_id_e id, uint8_t data);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartInit(void)
{
    int i, j;

    for (i = 0; i < UART_DRIVER_COUNT; i++)
    {
        containers[i].driver.id                 = (uart_driver_id_e) i;
        containers[i].driver.setEnabled         = 0;
        containers[i].driver.tx                 = 0;
        containers[i].driver.rx                 = 0;
        containers[i].driver.txIntSetEnabled    = 0;
        containers[i].driver.rxIntSetEnabled    = 0;

        BUFFER_INIT(&(containers[i].txBuffer), containers[i].txBufferMemory);
        BUFFER_INIT(&(containers[i].rxBuffer), containers[i].rxBufferMemory);
        
        for (j = 0; j < UART_DRIVER_RX_CALLBACK_COUNT; j++)
        {
            containers[i].rxCallbacks[j] = 0;
        }
        
        containers[i].rxCallbackCount = 0;
    }

    uartDriverInit();
}

//------------------------------------------------------------------------------
extern bool uartAddDriver(uart_driver_t* uartDriver)
{
    if (uartDriver->id >= UART_DRIVER_ID_COUNT   ||
       !uartDriver->tx                           ||
       !uartDriver->rx)
    {
        return false;
    }

    ADD_CONTAINER(containers, uartDriver);

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
        if (!uartAddDriver(&uartDrivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern bool uartAddRxCallback(uart_driver_id_e id,
                              data_callback_f* rxCallback)
{
    if (id >= UART_DRIVER_ID_COUNT || !rxCallback)
    {
        return false;
    }
    
    containers[id].rxCallbacks[containers[id].rxCallbackCount++] = rxCallback;
    
    return true;
}

//------------------------------------------------------------------------------
extern uart_error_e uartSetEnabled(uart_driver_id_e id, bool enabled)
{
    if (id >= UART_DRIVER_ID_COUNT || !containers[id].driver.setEnabled)
    {
        return UART_ERROR_INVALID_ID;
    }
    
    containers[id].driver.setEnabled(enabled);
    containers[id].driver.rxIntSetEnabled(enabled);
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartTx(uart_driver_id_e id, byte_array_t* data)
{
    int i;

    if (id >= UART_DRIVER_ID_COUNT || !containers[id].driver.tx)
    {
        return UART_ERROR_INVALID_ID;
    }

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_WRITE(&(containers[id].txBuffer), &(data->bytes[i])))
        {
            return UART_ERROR_TX_BUFFER_FULL;
        }
    }
    
    containers[id].driver.txIntSetEnabled(true);
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartRead(uart_driver_id_e id, byte_array_t* data)
{
    int i;

    if (id >= UART_DRIVER_ID_COUNT)
    {
        return UART_ERROR_INVALID_ID;
    }

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_READ(&(containers[id].rxBuffer), &(data->bytes[i])))
        {
            break;
        }
    }

    data->size = i;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartBytesAvailable(uart_driver_id_e id, uint8_t* byteCount)
{
    if (id >= UART_DRIVER_ID_COUNT)
    {
        return UART_ERROR_INVALID_ID;
    }
    
    bufferCount(&(containers[id].rxBuffer), byteCount);
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern void uartIntHandler(uart_driver_id_e id, uart_interrupt_e interrupt)
{
    uint8_t data;

    if (id < UART_DRIVER_ID_COUNT)
    {
        switch (interrupt)
        {
            case UART_INTERRUPT_TX:
            {
                if (BUFFER_READ(&(containers[id].txBuffer), &data))
                {
                    containers[id].driver.tx(data);
                }
                else
                {
                    containers[id].driver.txIntSetEnabled(false);
                }
                
                break;
            }
            case UART_INTERRUPT_RX:
            {
                containers[id].driver.rx(&data);

                if (BUFFER_WRITE(&(containers[id].rxBuffer), &data))
                {
                    callRxCallbacks(id, data);
                }
                
                break;
            }
        }
    }
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void callRxCallbacks(uart_driver_id_e id, uint8_t data)
{
    int i;
    
    for (i = 0; i < containers[id].rxCallbackCount; i++)
    {
        containers[id].rxCallbacks[i](data);
    }
}
