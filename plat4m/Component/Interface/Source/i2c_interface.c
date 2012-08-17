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

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  I2C_DRIVER_ID_LIMIT
#define I2C_DRIVER_COUNT (I2C_DRIVER_ID_LIMIT)
#else
#define I2C_DRIVER_COUNT (I2C_DRIVER_ID_COUNT)
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
    uint8_t txBufferMemory[I2C_DRIVER_BUFFER_SIZE];
    buffer_t txBuffer;
    uint8_t rxBufferMemory[I2C_DRIVER_BUFFER_SIZE];
    buffer_t rxBuffer;
} i2c_transfer_t;

/**
 * TODO Comment!
 */
typedef struct _i2c_driver_container_t_
{
    i2c_driver_t driver;
    i2c_transfer_t bufferMemory[I2C_DRIVER_TRANSFER_BUFFER_SIZE];
    buffer_t buffer;
    data_callback_f* rxCallbacks[I2C_DRIVER_RX_CALLBACK_COUNT];
    uint8_t rxCallbackCount;
} i2c_driver_container_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static i2c_driver_container_t containers[I2C_DRIVER_COUNT];

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cInit(void)
{
    int i, j;

    for (i = 0; i < I2C_DRIVER_COUNT; i++)
    {
        containers[i].driver.id                 = (i2c_driver_id_e) i;
        containers[i].driver.address            = 0;
        containers[i].driver.masterStart        = 0;
        containers[i].driver.masterStop         = 0;
        containers[i].driver.masterTxAddress    = 0;
        containers[i].driver.tx                 = 0;
        containers[i].driver.rx                 = 0;

        BUFFER_INIT(&(containers[i].buffer), containers[i].bufferMemory);

        for (j = 0; j < I2C_DRIVER_RX_CALLBACK_COUNT; j++)
        {
            containers[i].rxCallbacks[j] = 0;
        }

        containers[i].rxCallbackCount = 0;
    }

    i2cDriverInit();
}

//------------------------------------------------------------------------------
extern bool i2cAddDriver(i2c_driver_t* i2cDriver)
{
    if (i2cDriver->id >= I2C_DRIVER_ID_COUNT    ||
        !i2cDriver->masterStart                 ||
        !i2cDriver->masterStop                  ||
        !i2cDriver->masterTxAddress             ||
        !i2cDriver->tx                          ||
        !i2cDriver->rx)
    {
        return false;
    }

    ADD_CONTAINER(containers, i2cDriver);

    return true;
}

//------------------------------------------------------------------------------
extern bool i2cAddDrivers(i2c_driver_t i2cDrivers[], uint8_t size)
{
    int i;

    if (size > I2C_DRIVER_COUNT)
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
extern bool i2cAddRxCallBack(i2c_driver_id_e id, data_callback_f* callback)
{
    if (id > I2C_DRIVER_ID_COUNT || !callback)
    {
        return false;
    }

    containers[id].rxCallbacks[containers[id].rxCallbackCount++] = callback;

    return true;
}

//------------------------------------------------------------------------------
extern bool i2cMasterAddSlave(i2c_driver_id_e id,
                              i2c_address_t address,
                              i2c_device_t* device)
{
    if (id >= I2C_DRIVER_ID_COUNT || !device)
    {
        return false;
    }

    device->address = address;
    device->driverId = id;

    return true;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cSetEnabled(i2c_driver_id_e id, bool enabled)
{
    if (id >= I2C_DRIVER_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

    containers[id].driver.setEnabled(enabled);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTx(i2c_device_t* i2cDevice, byte_array_t* data)
{
    int i;
    uint32_t timeoutTimeMs = 0;

    i2c_transfer_t transfer =
    {
        .device = i2cDevice
    };

    if (!i2cDevice || !data)
    {
        return I2C_ERROR_INVALID_PARAMETER;
    }

    if (i2cDevice->driverId >= I2C_DRIVER_ID_COUNT)
    {
        return I2C_ERROR_INVALID_ID;
    }

    BUFFER_INIT(&(transfer.txBuffer), transfer.txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer), transfer.rxBufferMemory);
    
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

    timeoutTimeMs = systemTimeMsGet() + 1000;

    while (!bufferIsEmpty(&(containers[i2cDevice->driverId].buffer)))
    {
        if (systemTimeMsGet() > timeoutTimeMs)
        {
            return I2C_ERROR_TIMEOUT;
        }
    }

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterRx(i2c_device_t* i2cDevice, byte_array_t* data)
{
    i2c_transfer_t transfer =
    {
        .device = i2cDevice
    };
    
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

    if (!BUFFER_WRITE(&(containers[i2cDevice->driverId].buffer), &transfer))
    {
        // Error?
    }

    // Start
    containers[i2cDevice->driverId].driver.masterStart();

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern void i2cIntHandler(i2c_driver_id_e id, i2c_interrupt_e interrupt)
{
    uint8_t data = 0;
    i2c_transfer_t* transfer;

    if (id < I2C_DRIVER_ID_COUNT)
    {
        BUFFER_PEEK(&(containers[id].buffer), &transfer);

        switch (interrupt)
        {
            case I2C_INTERRUPT_START:
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

                break;
            }
            case I2C_INTERRUPT_ADDRESS:
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

                break;
            }
            case I2C_INTERRUPT_STOP:
            {

                break;
            }
            case I2C_INTERRUPT_TX:
            {
                if (BUFFER_READ(&(transfer->txBuffer), &data))
                {
                    containers[id].driver.tx(data);
                }
                else
                {
                    containers[id].driver.masterStop();
                    containers[id].driver.txIntSetEnabled(false);
                    BUFFER_READ(&(containers[id].buffer), transfer);
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
