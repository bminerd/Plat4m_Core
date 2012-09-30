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
 * @brief TODO Comment!
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

#ifdef  I2C_ID_LIMIT
#define I2C_COUNT (I2C_ID_LIMIT)
#else
#define I2C_COUNT (I2C_ID_COUNT)
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
typedef struct _i2c_transfer_t_
{
    i2c_device_t* device;
    buffer_t txBuffer;
    buffer_t rxBuffer;
} i2c_transfer_t;

/**
 * TODO Comment!
 */
typedef struct _i2c_container_t_
{
    i2c_driver_t driver;
    i2c_transfer_t bufferMemory[I2C_TRANSFER_BUFFER_SIZE];
    buffer_t buffer;
    data_callback_f* rxCallbacks[I2C_RX_CALLBACK_COUNT];
    uint8_t rxCallbackCount;
} i2c_container_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static i2c_container_t containers[I2C_COUNT];

/**
 * TODO Comment!
 */
static volatile i2c_state_e currentState = I2C_STATE_IDLE;

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static bool i2cAddDriver(i2c_driver_t* i2cDriver);

/**
 * TODO Comment!
 */
static bool isDriverEnabled(i2c_id_e id);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cInit(void)
{
    int i, j;

    for (i = 0; i < I2C_COUNT; i++)
    {
        containers[i].driver.id                 = (i2c_id_e) i;
        containers[i].driver.address            = 0;
        containers[i].driver.isEnabled          = false;
        containers[i].driver.setEnabled         = NULL_POINTER;
        containers[i].driver.masterStart        = NULL_POINTER;
        containers[i].driver.masterStop         = NULL_POINTER;
        containers[i].driver.masterTxAddress    = NULL_POINTER;
        containers[i].driver.tx                 = NULL_POINTER;
        containers[i].driver.rx                 = NULL_POINTER;

        BUFFER_INIT(&(containers[i].buffer), containers[i].bufferMemory);

        for (j = 0; j < I2C_RX_CALLBACK_COUNT; j++)
        {
            containers[i].rxCallbacks[j] = 0;
        }

        containers[i].rxCallbackCount = 0;
    }

    i2cDriverInit();
}

//------------------------------------------------------------------------------
extern bool i2cAddDrivers(i2c_driver_t i2cDrivers[], uint8_t size)
{
    int i;

    if (size > I2C_COUNT)
    {
        return false;
    }

    for (i = 0; i < size; i++)
    {
        if (!i2cAddDriver(&i2cDrivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern bool i2cAddRxCallBack(i2c_id_e id, data_callback_f* callback)
{
    if (id > I2C_ID_COUNT || IS_NULL_POINTER(callback))
    {
        return false;
    }

    containers[id].rxCallbacks[containers[id].rxCallbackCount++] = callback;

    return true;
}

//------------------------------------------------------------------------------
extern bool i2cMasterAddSlave(i2c_id_e id,
                              i2c_address_t address,
                              i2c_device_t* device)
{
    if (id >= I2C_ID_COUNT || IS_NULL_POINTER(device))
    {
        return false;
    }

    device->address = address;
    device->driverId = id;

    return true;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cIsEnabled(i2c_id_e id, bool* isEnabled)
{
    if (id >= I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

    *isEnabled = isDriverEnabled(id);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cSetEnabled(i2c_id_e id, bool enabled)
{
    if (id >= I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

    containers[id].driver.setEnabled(enabled);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTx(i2c_device_t* i2cDevice,
                               byte_array_t* data,
                               bool waitUntilIdle)
{
    static uint8_t txBufferMemory[I2C_BUFFER_SIZE];
    static uint8_t rxBufferMemory[I2C_BUFFER_SIZE];

    if (waitUntilIdle)
    {
        while (currentState != I2C_STATE_IDLE);
    }

    currentState = I2C_STATE_BUSY;

    int i;
    uint32_t timeoutTimeMs = 0;

    i2c_transfer_t transfer =
    {
        .device = i2cDevice
    };

    if (i2cDevice->driverId >= I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

    if (!isDriverEnabled(i2cDevice->driverId))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

    BUFFER_INIT(&(transfer.txBuffer), txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer), rxBufferMemory);
    
    for (i = 0; i < data->size; i++)
    {
        if (!BUFFER_WRITE(&(transfer.txBuffer), &(data->bytes[i])))
        {
            return I2C_ERROR_TX_BUFFER_FULL;
        }
    }

    if (!BUFFER_WRITE(&(containers[i2cDevice->driverId].buffer), &transfer))
    {
        return I2C_ERROR_TX_BUFFER_FULL;
    }

    // Start
    containers[i2cDevice->driverId].driver.masterStart();

//    timeoutTimeMs = systemTimeMsGet() + 1000;
//
//    while (!bufferIsEmpty((buffer_t*) &(containers[i2cDevice->driverId].buffer)))
//    {
//        if (systemCheckTime(timeoutTimeMs))
//        {
//            return I2C_ERROR_TIMEOUT;
//        }
//    }

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterRx(i2c_device_t* i2cDevice, byte_array_t* data)
{
    i2c_transfer_t transfer =
    {
        .device = i2cDevice
    };
    
    if (i2cDevice->driverId >= I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

    if (!isDriverEnabled(i2cDevice->driverId))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

    if (!BUFFER_WRITE(&(containers[i2cDevice->driverId].buffer), &transfer))
    {
        // Error?
    }

    // Start
    containers[i2cDevice->driverId].driver.masterStart();

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTxRx(i2c_device_t* i2cDevice,
                                 byte_array_t* txData,
                                 byte_array_t* rxData)
{
    i2c_transfer_t transfer =
    {
        .device = i2cDevice
    };

    if (i2cDevice->driverId >= I2C_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

    if (!isDriverEnabled(i2cDevice->driverId))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

    if (!BUFFER_WRITE(&(containers[i2cDevice->driverId].buffer), &transfer))
    {
        // Error?
    }

    // Start
    containers[i2cDevice->driverId].driver.masterStart();

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern void i2cIntHandler(i2c_id_e id, i2c_interrupt_e interrupt)
{
    uint8_t data = 0;
    i2c_transfer_t* transfer;

    if (id < I2C_ID_COUNT)
    {
        BUFFER_PEEK(&(containers[id].buffer), &transfer);

        switch (interrupt)
        {
            case I2C_INTERRUPT_START:
            {
                if (!IS_NULL_POINTER(transfer))
                {
                    if (!bufferIsEmpty(&(transfer->txBuffer)))
                    {
                        containers[id].driver.
                            masterTxAddress(transfer->device->address,
                                            I2C_MASTER_MODE_TX);
                    }
                    else if (!bufferIsEmpty(&(transfer->rxBuffer)))
                    {
                        containers[id].driver.
                            masterTxAddress(transfer->device->address,
                                            I2C_MASTER_MODE_RX);
                    }
                }

                break;
            }
            case I2C_INTERRUPT_ADDRESS:
            {
                if (!IS_NULL_POINTER(transfer))
                {
                    if (!bufferIsEmpty(&(transfer->txBuffer)) &&
                        BUFFER_READ(&(transfer->txBuffer), &data))
                    {
                        containers[id].driver.txIntSetEnabled(true);
                        containers[id].driver.tx(data);
                    }
                    else if (!bufferIsEmpty(&(transfer->rxBuffer)))
                    {
                        containers[id].driver.rxIntSetEnabled(true);
                    }
                }

                break;
            }
            case I2C_INTERRUPT_STOP:
            {
                containers[id].driver.masterStop();
                containers[id].driver.txIntSetEnabled(false);
                BUFFER_READ(&(containers[id].buffer), transfer);

                currentState = I2C_STATE_IDLE;

                break;
            }
            case I2C_INTERRUPT_TX:
            {
                if (!IS_NULL_POINTER(transfer))
                {
                    if (BUFFER_READ(&(transfer->txBuffer), &data))
                    {
                        containers[id].driver.tx(data);
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
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool i2cAddDriver(i2c_driver_t* i2cDriver)
{
    if (IS_NULL_POINTER(i2cDriver)                  ||
        i2cDriver->id >= I2C_ID_COUNT        ||
        IS_NULL_POINTER(i2cDriver->setEnabled)      ||
        IS_NULL_POINTER(i2cDriver->masterStart)     ||
        IS_NULL_POINTER(i2cDriver->masterStop)      ||
        IS_NULL_POINTER(i2cDriver->masterTxAddress) ||
        IS_NULL_POINTER(i2cDriver->tx)              ||
        IS_NULL_POINTER(i2cDriver->rx))
    {
        return false;
    }

    ADD_CONTAINER(containers, i2cDriver);

    if (containers[i2cDriver->id].driver.isEnabled)
    {
        i2cSetEnabled(i2cDriver->id, true);
    }

    return true;
}

//------------------------------------------------------------------------------
static bool isDriverEnabled(i2c_id_e id)
{
    return containers[id].driver.isEnabled;
}
