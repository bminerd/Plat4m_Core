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

struct _i2c_device_wrapper_t_;

/**
 * @brief I2C transfer type.
 */
typedef struct _i2c_transfer_t_
{
    struct _i2c_device_wrapper_t_* device;
    buffer_t txBuffer;
    buffer_t rxBuffer;
} i2c_transfer_t;

/**
 * @brief I2C type.
 */
typedef struct _i2c_t_
{
    i2c_e i2c;
    bool isEnabled;
    i2c_config_t config;
    i2c_transfer_t bufferMemory[I2C_TRANSFER_BUFFER_SIZE];
    buffer_t buffer;
    uint8_t txBufferMemory[I2C_BUFFER_SIZE];
    uint8_t rxBufferMemory[I2C_BUFFER_SIZE];
    volatile i2c_state_e state;
    const data_callback_f* rxCallback; // TODO Is this necessary?
} i2c_t;

/**
 * @brief I2C device wrapper type.
 */
typedef struct _i2c_device_wrapper_t_
{
    i2c_device_t device;
    i2c_t* i2c;
} i2c_device_wrapper_t;

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
static i2c_t i2cMap[I2C_COUNT];

/**
 * @brief I2C device ID map.
 */
static i2c_device_wrapper_t deviceMap[I2C_DEVICE_COUNT];

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C interrupt handler.
 * @param i2c I2C to access.
 * @param interrupt Interrupt to handle.
 */
static void intHandler(const i2c_e i2c, const i2c_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cInit(void)
{
    int i;

    for (i = 0; i < I2C_COUNT; i++)
    {
        i2cMap[i].i2c       = (i2c_e) i;
        i2cMap[i].isEnabled = false;

        BUFFER_INIT(&(i2cMap[i].buffer), i2cMap[i].bufferMemory);

        i2cMap[i].state     = I2C_STATE_IDLE;
    }

    for (i = 0; i < I2C_DEVICE_COUNT; i++)
    {
        deviceMap[i].device.device  = I2C_DEVICE_NONE;
        deviceMap[i].device.i2c     = I2C_NONE;
        deviceMap[i].i2c            = NULL_POINTER;
    }

    i2cDriverInit(intHandler);
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
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *i2cDriver;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cSetEnabled(const i2c_e i2c, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((i2c == I2C_NONE) || (i2c >= I2C_COUNT))
    {
        return I2C_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(i2cMap[i2c].i2c, enabled);
    driver.intSetEnabled(i2c, I2C_INTERRUPT_TX, enabled);
    driver.intSetEnabled(i2c, I2C_INTERRUPT_START, enabled);
    i2cMap[i2c].isEnabled = enabled;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cIsEnabled(const i2c_e i2c, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isEnabled))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

    if ((i2c == I2C_NONE) || (i2c >= I2C_COUNT))
    {
        return I2C_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = i2cMap[i2c].isEnabled;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cConfigure(const i2c_e i2c, const i2c_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(config))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

    if ((i2c == I2C_NONE) || (i2c >= I2C_COUNT))
    {
        return I2C_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.configure(i2c, config);
    i2cMap[i2c].config = *config;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterAddDevice(const i2c_device_t* device)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(device))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

    if (device->address >= 128)
    {
        return I2C_ERROR_ADDRESS_INVALID;
    }

    if ((device->device == I2C_DEVICE_NONE) ||
        (device->device >= I2C_DEVICE_COUNT))
    {
        return I2C_ERROR_DEVICE_INVALID;
    }

    if ((device->i2c == I2C_NONE)|| (device->i2c >= I2C_COUNT))
    {
        return I2C_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    deviceMap[device->device].device    = *device;
    deviceMap[device->device].i2c       = &(i2cMap[device->i2c]);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterAddDevices(const i2c_device_t devices[],
                                       const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > I2C_DEVICE_COUNT)
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        i2c_error_e error = i2cMasterAddDevice(&(devices[i]));

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
extern i2c_error_e i2cMasterTx(const i2c_device_e device,
                               const byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

    if ((device == I2C_DEVICE_NONE) || (device >= I2C_DEVICE_COUNT))
    {
        return I2C_ERROR_DEVICE_INVALID;
    }

    if (!(deviceMap[device].i2c->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    while (i2cMap[deviceMap[device].i2c->i2c].state != I2C_STATE_IDLE);

    i2cMap[deviceMap[device].i2c->i2c].state = I2C_STATE_BUSY;

    i2c_transfer_t transfer =
    {
        .device = &deviceMap[device]
    };

    BUFFER_INIT(&(transfer.txBuffer),
                i2cMap[transfer.device->i2c->i2c].txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer),
                i2cMap[transfer.device->i2c->i2c].rxBufferMemory);
    
    int i;

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
    driver.masterStart(transfer.device->i2c->i2c);

    uint32_t timeoutTimeMs = systemTimeMsGet() + 1000;

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
extern i2c_error_e i2cMasterRx(const i2c_device_e device,
                               byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }
    
    if (device >= I2C_DEVICE_COUNT)
    {
        return I2C_ERROR_INVALID;
    }

    if (!(deviceMap[device].i2c->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    i2c_transfer_t transfer =
    {
        .device = &deviceMap[device]
    };

    BUFFER_INIT(&(transfer.txBuffer),
                i2cMap[transfer.device->i2c->i2c].txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer),
                i2cMap[transfer.device->i2c->i2c].rxBufferMemory);

    if (!BUFFER_WRITE(&(transfer.device->i2c->buffer), &transfer))
    {
        // Error?
    }

    // Start
    driver.masterStart(transfer.device->i2c->i2c);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern i2c_error_e i2cMasterTxRx(const i2c_device_e device,
                                 const byte_array_t* txData,
                                 byte_array_t* rxData)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(txData) || IS_NULL_POINTER(rxData))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

    if (device >= I2C_DEVICE_COUNT)
    {
        return I2C_ERROR_DEVICE_INVALID;
    }

    if (!(deviceMap[device].i2c->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    i2c_transfer_t transfer =
    {
        .device = &deviceMap[device]
    };

    BUFFER_INIT(&(transfer.txBuffer),
                i2cMap[transfer.device->i2c->i2c].txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer),
                i2cMap[transfer.device->i2c->i2c].rxBufferMemory);

    if (!BUFFER_WRITE(&(transfer.device->i2c->buffer), &transfer))
    {
        // Error?
    }

    // Start
    driver.masterStart(transfer.device->i2c->i2c);

    return I2C_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void intHandler(const i2c_e i2c, const i2c_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if ((i2c == I2C_NONE)                   ||
        (i2c >= I2C_COUNT)                  ||
        (interrupt == I2C_INTERRUPT_NONE)   ||
        (interrupt >= I2C_INTERRUPT_COUNT))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    uint8_t data = 0;
    i2c_transfer_t* transfer;

    BUFFER_PEEK(&(i2cMap[i2c].buffer), &transfer);

#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(transfer))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    switch (interrupt)
    {
        case I2C_INTERRUPT_START:
        {
            if (!bufferIsEmpty(&(transfer->txBuffer)))
            {
                driver.masterTxAddress(i2c,
                                       transfer->device->device.address,
                                       I2C_MASTER_MODE_TX);
            }
            else if (!bufferIsEmpty(&(transfer->rxBuffer)))
            {
                driver.masterTxAddress(i2c,
                                       transfer->device->device.address,
                                       I2C_MASTER_MODE_RX);
            }

            break;
        }
        case I2C_INTERRUPT_STOP:
        {
            if (!bufferIsEmpty(&(transfer->txBuffer)) &&
                BUFFER_READ(&(transfer->txBuffer), &data))
            {
                driver.tx(i2c, data);
            }
            else
            {
                driver.masterStop(i2c);
                BUFFER_READ(&(i2cMap[i2c].buffer), transfer);

                i2cMap[i2c].state = I2C_STATE_IDLE;
            }

            break;
        }
        case I2C_INTERRUPT_ADDRESS:
        {
            if (!bufferIsEmpty(&(transfer->txBuffer)) &&
                BUFFER_READ(&(transfer->txBuffer), &data))
            {
                driver.tx(i2c, data);
            }
            else if (!bufferIsEmpty(&(transfer->rxBuffer)))
            {
                // Rx
            }

            break;
        }
        case I2C_INTERRUPT_TX:
        {
            if (BUFFER_READ(&(transfer->txBuffer), &data))
            {
                driver.tx(i2c, data);
            }

            break;
        }
        case I2C_INTERRUPT_RX:
        {
            break;
        }
    }
}
