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
 * @file i2c_interface.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief I2C interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <i2c_interface.h>

#include <buffer_interface.h>
#include <system_interface.h>

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
 * @brief I2C transfer type.
 */
typedef struct _i2c_transfer_t_
{
    struct _i2c_device_t_* device;
    buffer_t txBuffer;
    buffer_t rxBuffer;
} i2c_transfer_t;

/**
 * @brief I2C type.
 */
typedef struct _i2c_t_
{
    i2c_driver_id_e driverId;
    bool isEnabled;
    i2c_transfer_t bufferMemory[I2C_TRANSFER_BUFFER_SIZE];
    buffer_t buffer;
    const data_callback_f* rxCallback;
} i2c_t;

/**
 * @brief I2C device type.
 */
typedef struct _i2c_device_t_
{
    id_t id;
    i2c_address_t address;
    i2c_t* i2c;
} i2c_device_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C driver.
 */
static i2c_driver_t driver;

/**
 * @brief I2C map.
 */
static i2c_t i2cMap[I2C_ID_COUNT];

/**
 * @brief I2C ID map.
 */
static i2c_t* idMap[I2C_DRIVER_ID_COUNT];

/**
 * @brief I2C device ID map.
 */
static i2c_device_t deviceMap[I2C_DEVICE_ID_MAX];

/**
 * @brief Current I2C state.
 */
static volatile i2c_state_e currentState = I2C_STATE_IDLE;

/**
 * @brief Memory for the Tx buffer;
 */
static uint8_t txBufferMemory[I2C_BUFFER_SIZE];

/**
 * @brief Memory for the Rx buffer;
 */
static uint8_t rxBufferMemory[I2C_BUFFER_SIZE];

/**
 * @brief Current device ID.
 */
static id_t curDeviceId = 0;

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C interrupt handler.
 */
static void intHandler(const i2c_driver_id_e driverId,
                       const i2c_interrupt_e interrupt);

/**
 * @brief Adds the given I2C ID map to the local map.
 */
static i2c_error_e addIdMap(const i2c_id_map_t* map);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cInit(void)
{
    int i;

    for (i = 0; i < I2C_ID_COUNT; i++)
    {
        i2cMap[i].isEnabled = false;

        BUFFER_INIT(&(i2cMap[i].buffer), i2cMap[i].bufferMemory);

        i2cMap[i].rxCallback = NULL_POINTER;
    }

    for (i = 0; i < I2C_DRIVER_ID_COUNT; i++)
    {
        idMap[i] = NULL_POINTER;
    }

    for (i = 0; i < I2C_DEVICE_ID_MAX; i++)
    {
        deviceMap[i].id         = (id_t) i;
        deviceMap[i].address    = 0;
        deviceMap[i].i2c        = NULL_POINTER;
    }

    i2cDriverInit((interface_int_handler_f*) intHandler);
    i2cApplicationInit();
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cSetDriver(const i2c_driver_t* i2cDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(i2cDriver)                  ||
        IS_NULL_POINTER(i2cDriver->setEnabled)      ||
        IS_NULL_POINTER(i2cDriver->masterStart)     ||
        IS_NULL_POINTER(i2cDriver->masterStop)      ||
        IS_NULL_POINTER(i2cDriver->masterTxAddress) ||
        IS_NULL_POINTER(i2cDriver->tx)              ||
        IS_NULL_POINTER(i2cDriver->rx)              ||
        IS_NULL_POINTER(i2cDriver->intSetEnabled))
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

#endif // PLAT4M_DEBUG

    driver = *i2cDriver;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cAddIdMaps(const i2c_id_map_t i2cIdMaps[],
                                const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        i2c_error_e error = addIdMap(&i2cIdMaps[i]);

#ifdef PLAT4M_DEBUG

        if (error != I2C_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cSetRxCallBack(const i2c_id_e id,
                                    const data_callback_f* callback)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(callback))
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

    if (id > I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    idMap[id]->rxCallback = callback;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterAddSlave(const i2c_id_e id,
                                     const i2c_address_t address,
                                     id_t* deviceId)
{
#ifdef PLAT4M_DEBUG

    if (address >= 128)
    {
        *deviceId = -1;

        return I2C_ERROR_INVALID_PARAMETER;
    }

    if (id >= I2C_ID_COUNT)
    {
        *deviceId = -1;

        return I2C_ERROR_INVALID_ID;
    }

    if (curDeviceId >= I2C_DEVICE_ID_MAX)
    {
        *deviceId = -1;

        return I2C_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    deviceMap[curDeviceId].address = address;
    deviceMap[curDeviceId].i2c = idMap[id];
    *deviceId = curDeviceId++;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cSetEnabled(const i2c_id_e id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (id >= I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(idMap[id]->driverId, enabled);
    idMap[id]->isEnabled = enabled;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cIsEnabled(const i2c_id_e id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isEnabled))
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

    if (id >= I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = idMap[id]->isEnabled;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTx(const id_t deviceId,
                               byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

    if (deviceId >= I2C_DEVICE_ID_MAX)
    {
        return I2C_ERROR_INVALID_ID;
    }

    if (!(deviceMap[deviceId].i2c->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    while (currentState != I2C_STATE_IDLE);

    currentState = I2C_STATE_BUSY;

    int i;
    uint32_t timeoutTimeMs = 0;

    i2c_transfer_t transfer =
    {
        .device = &deviceMap[deviceId]
    };

    BUFFER_INIT(&(transfer.txBuffer), txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer), rxBufferMemory);
    
    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_WRITE(&(transfer.txBuffer), &(data->bytes[i])))
        {
            return I2C_ERROR_TX_BUFFER_FULL;
        }
    }

    if (!BUFFER_WRITE(&(transfer.device->i2c->buffer), &transfer))
    {
        return I2C_ERROR_TX_BUFFER_FULL;
    }

    // Start
    driver.masterStart(transfer.device->i2c->driverId);

    timeoutTimeMs = systemTimeMsGet() + 1000;

    while (!bufferIsEmpty((buffer_t*) &(transfer.device->i2c->buffer)))
    {
        if (systemCheckTimeMs(timeoutTimeMs))
        {
            return I2C_ERROR_TIMEOUT;
        }
    }

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterRx(const id_t deviceId,
                               byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }
    
    if (deviceId >= I2C_DEVICE_ID_MAX)
    {
        return I2C_ERROR_INVALID_ID;
    }

    if (!(deviceMap[deviceId].i2c->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    i2c_transfer_t transfer =
    {
        .device = &deviceMap[deviceId]
    };

    BUFFER_INIT(&(transfer.txBuffer), txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer), rxBufferMemory);

    if (!BUFFER_WRITE(&(transfer.device->i2c->buffer), &transfer))
    {
        // Error?
    }

    // Start
    driver.masterStart(transfer.device->i2c->driverId);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTxRx(const id_t deviceId,
                                 byte_array_t* txData,
                                 byte_array_t* rxData)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(txData) || IS_NULL_POINTER(rxData))
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

    if (deviceId >= I2C_DEVICE_ID_MAX)
    {
        return I2C_ERROR_INVALID_ID;
    }

    if (!(deviceMap[deviceId].i2c->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    i2c_transfer_t transfer =
    {
        .device = &deviceMap[deviceId]
    };

    BUFFER_INIT(&(transfer.txBuffer), txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer), rxBufferMemory);

    if (!BUFFER_WRITE(&(transfer.device->i2c->buffer), &transfer))
    {
        // Error?
    }

    // Start
    driver.masterStart(transfer.device->i2c->driverId);

    return I2C_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static i2c_error_e addIdMap(const i2c_id_map_t* map)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(map))
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

    if ((map->id >= I2C_ID_COUNT) || (map->driverId >= I2C_DRIVER_ID_COUNT))
    {
        return I2C_ERROR_INVALID_ID;
    }

#endif // PLAT4M_DEBUG

    i2cMap[map->id].driverId = map->driverId;
    idMap[map->id] = &(i2cMap[map->driverId]);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern void intHandler(const i2c_driver_id_e driverId,
                       const i2c_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if (driverId >= I2C_DRIVER_ID_COUNT)
    {
        return;
    }

#endif // PLAT4M_DEBUG

    uint8_t data = 0;
    i2c_transfer_t* transfer;

    BUFFER_PEEK(&(i2cMap[driverId].buffer), &transfer);

    switch (interrupt)
    {
        case I2C_INTERRUPT_START:
        {
            if (!IS_NULL_POINTER(transfer))
            {
                if (!bufferIsEmpty(&(transfer->txBuffer)))
                {
                    driver.masterTxAddress(driverId,
                                           transfer->device->address,
                                           I2C_MASTER_MODE_TX);
                }
                else if (!bufferIsEmpty(&(transfer->rxBuffer)))
                {
                    driver.masterTxAddress(driverId,
                                           transfer->device->address,
                                           I2C_MASTER_MODE_RX);
                }
            }

            break;
        }
        case I2C_INTERRUPT_STOP:
        {
            driver.masterStop(driverId);
            driver.intSetEnabled(driverId, I2C_INTERRUPT_TX, false);
            BUFFER_READ(&(i2cMap[driverId].buffer), transfer);

            currentState = I2C_STATE_IDLE;

            break;
        }
        case I2C_INTERRUPT_ADDRESS:
        {
            if (!IS_NULL_POINTER(transfer))
            {
                if (!bufferIsEmpty(&(transfer->txBuffer)) &&
                    BUFFER_READ(&(transfer->txBuffer), &data))
                {
                    driver.intSetEnabled(driverId, I2C_INTERRUPT_TX, true);
                    driver.tx(driverId, data);
                }
                else if (!bufferIsEmpty(&(transfer->rxBuffer)))
                {
                    driver.intSetEnabled(driverId, I2C_INTERRUPT_RX, true);
                }
            }

            break;
        }
        case I2C_INTERRUPT_TX:
        {
            if (!IS_NULL_POINTER(transfer))
            {
                if (BUFFER_READ(&(transfer->txBuffer), &data))
                {
                    driver.tx(driverId, data);
                }
            }

            break;
        }
        case I2C_INTERRUPT_RX:
        {
            break;
        }
    }
}
