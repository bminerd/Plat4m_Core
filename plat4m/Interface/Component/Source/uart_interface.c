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
    uart_driver_id_e driverId;
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
static uart_t uartMap[UART_ID_COUNT];

/**
 * @brief UART ID map.
 */
static uart_t* idMap[UART_DRIVER_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief UART interrupt handler.
 */
static void intHandler(const uart_driver_id_e driverId,
                       const uart_interrupt_e interrupt);

/**
 * @brief Adds the given UART ID map to the local map.
 */
static uart_error_e addIdMap(const uart_id_map_t* map);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartInit(void)
{
    int i;

    for (i = 0; i < UART_ID_COUNT; i++)
    {
        uartMap[i].isEnabled    = false;

        BUFFER_INIT(&(uartMap[i].txBuffer), uartMap[i].txBufferMemory);
        BUFFER_INIT(&(uartMap[i].rxBuffer), uartMap[i].rxBufferMemory);
        
        uartMap[i].rxCallback   = NULL_POINTER;
    }

    for (i = 0; i < UART_DRIVER_ID_COUNT; i++)
    {
        idMap[i] = NULL_POINTER;
    }

    uartDriverInit((interface_int_handler_f*) intHandler);
    uartApplicationInit();
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
        return UART_ERROR_INVALID_PARAMETER;
    }

#endif // PLAT4M_DEBUG

    driver = *uartDriver;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartAddIdMaps(const uart_id_map_t uartIdMaps[],
                                  const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > UART_ID_COUNT)
    {
        return UART_ERROR_INVALID_PARAMETER;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        uart_error_e error = addIdMap(&uartIdMaps[i]);

#ifdef PLAT4M_DEBUG

        if (error != UART_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartSetRxCallback(const uart_id_e id,
                                      const data_callback_f* rxCallback)
{
#ifdef PLAT4M_DEBUG

    if ((id >= UART_ID_COUNT) || IS_NULL_POINTER(rxCallback))
    {
        return UART_ERROR_INVALID_PARAMETER;
    }
    
#endif // PLAT4M_DEBUG

    idMap[id]->rxCallback = rxCallback;
    
    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartSetEnabled(const uart_id_e id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (id >= UART_ID_COUNT)
    {
        return UART_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(idMap[id]->driverId, enabled);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartIsEnabled(const uart_id_e id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (id >= UART_ID_COUNT)
    {
        return UART_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = idMap[id]->isEnabled;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartTx(const uart_id_e id, const byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (id >= UART_ID_COUNT)
    {
        return UART_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_WRITE(&(idMap[id]->txBuffer), &(data->bytes[i])))
        {
            break;
        }
    }

    driver.intSetEnabled(idMap[id]->driverId, UART_INTERRUPT_TX, true);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartRead(const uart_id_e id, byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return UART_ERROR_INVALID_PARAMETER;
    }

    if (id >= UART_ID_COUNT)
    {
        return UART_ERROR_INVALID_ID;
    }

    if (!idMap[id]->isEnabled)
    {
        return UART_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_READ(&(idMap[id]->rxBuffer), &(data->bytes[i])))
        {
            break;
        }
    }

    data->size = i;

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern uart_error_e uartBytesAvailable(const uart_id_e id, uint8_t* byteCount)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(byteCount))
    {
        return UART_ERROR_INVALID_PARAMETER;
    }

    if (id >= UART_ID_COUNT)
    {
        return UART_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    bufferCount(&(idMap[id]->rxBuffer), byteCount);

    return UART_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static uart_error_e addIdMap(const uart_id_map_t* map)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(map))
    {
        return UART_ERROR_INVALID_PARAMETER;
    }

    if ((map->id >= UART_ID_COUNT) || (map->driverId >= UART_DRIVER_ID_COUNT))
    {
        return UART_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    uartMap[map->id].driverId = map->driverId;
    idMap[map->driverId] = &(uartMap[map->id]);

    return UART_ERROR_NONE;
}

//------------------------------------------------------------------------------
static void intHandler(const uart_driver_id_e driverId,
                       const uart_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if (driverId >= UART_DRIVER_ID_COUNT)
    {
        return;
    }

#endif // PLAT4M_DEBUG

    uint8_t data;

    switch (interrupt)
    {
        case UART_INTERRUPT_TX:
        {
            if (BUFFER_READ(&(uartMap[driverId].txBuffer), &data))
            {
                driver.tx(driverId, data);
            }
            else
            {
                driver.intSetEnabled(driverId, UART_INTERRUPT_TX, false);
            }

            break;
        }
        case UART_INTERRUPT_RX:
        {
            driver.rx(driverId, &data);

            if (BUFFER_WRITE(&(uartMap[driverId].rxBuffer), &data))
            {
                uartMap[driverId].rxCallback(data);
            }

            break;
        }
    }
}
