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

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C driver.
 */
static i2c_driver_t driver;

/**
 * @brief I2C to I2C ID map.
 */
static i2c_info_t* i2cToInfoMap[I2C_COUNT];

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

/**
 * @brief Performs an I2C transfer.
 * @param id I2C device.
 * @param txData Transmit data.
 * @param rxData Received data.
 * @return I2C error.
 */
static i2c_error_e i2cTransfer(const i2c_device_id_t* id,
                               const byte_array_t* txData,
                               byte_array_t* rxData);

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
void i2cInit(void)
{
    int i;

    // Initialize I2C to ID map
    for (i = 0; i < I2C_COUNT; i++)
    {
        i2cToInfoMap[i] = NULL_POINTER;
    }

    i2cApplicationInit();
}

//------------------------------------------------------------------------------
i2c_error_e i2cSetDriver(const i2c_driver_t* i2cDriver,
                         i2c_interface_int_handler_f** handler)
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
    *handler = intHandler;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
i2c_error_e i2cInitId(i2c_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

    if (id->i2c >= I2C_COUNT)
    {
        return I2C_ERROR_I2C_INVALID;
    }

#endif // PLAT4M_DEBUG

    i2c_info_t* info = (i2c_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;
    info->state         = I2C_STATE_IDLE;

    mutexInitMutex(&(info->mutex));

    BUFFER_INIT(&(info->buffer), info->bufferMemory);

    i2cToInfoMap[id->i2c] = info;

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
i2c_error_e i2cSetEnabled(i2c_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    i2c_info_t* info = (i2c_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.setEnabled(id->i2c, enabled);
    driver.intSetEnabled(id->i2c,
                         I2C_INTERRUPT_MASTER_START_TRANSMITTED,
                         enabled);
    driver.intSetEnabled(id->i2c, I2C_INTERRUPT_BYTE_TRANSMITTED, enabled);
    info->isEnabled = enabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
i2c_error_e i2cIsEnabled(i2c_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    i2c_info_t* info = (i2c_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *isEnabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
i2c_error_e i2cConfigure(i2c_id_t* id, const i2c_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    i2c_info_t* info = (i2c_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.configure(id->i2c, config);
    memcpy((uint8_t*) &(info->config),
           (uint8_t*) config,
           sizeof(*config));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return I2C_ERROR_NONE;
}

//------------------------------------------------------------------------------
i2c_error_e i2cMasterTx(const i2c_device_id_t* id, const byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(data))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    i2c_info_t* info = (i2c_info_t*) id->i2cId->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    byte_array_t rxData =
    {
        .bytes  = NULL_POINTER,
        .size   = 0
    };

    return i2cTransfer(id, data, &rxData);
}

//------------------------------------------------------------------------------
i2c_error_e i2cMasterRx(const i2c_device_id_t* id, byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(data))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    i2c_info_t* info = (i2c_info_t*) id->i2cId->private;

#ifdef PLAT4M_DEBUG
    
    if (!(info->isInitialized))
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    byte_array_t txData =
    {
        .bytes  = NULL_POINTER,
        .size   = 0
    };

    return i2cTransfer(id, &txData, data);
}

//------------------------------------------------------------------------------
i2c_error_e i2cMasterTxRx(const i2c_device_id_t* id,
                          const byte_array_t* txData,
                          byte_array_t* rxData)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(txData) || IS_NULL_POINTER(rxData))
    {
        return I2C_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    i2c_info_t* info = (i2c_info_t*) id->i2cId->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return I2C_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return I2C_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    return i2cTransfer(id, txData, rxData);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static i2c_error_e i2cTransfer(const i2c_device_id_t* id,
                               const byte_array_t* txData,
                               byte_array_t* rxData)
{
    i2c_info_t* info = (i2c_info_t*) id->i2cId->private;

    i2c_transfer_t transfer =
    {
        .device     = id,
        .txCount    = txData->size,
        .rxCount    = rxData->size
    };

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    BUFFER_INIT(&(transfer.txBuffer), info->txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer), info->rxBufferMemory);

    int i;

    for (i = 0; i < txData->size; i++)
    {
        if (!BUFFER_WRITE(&(transfer.txBuffer), &(txData->bytes[i])))
        {
            return I2C_ERROR_TX_BUFFER_FULL;
        }
    }

    if (!BUFFER_WRITE(&(info->buffer), &transfer))
    {
        // Error?
    }

    info->state = I2C_STATE_BUSY;

    // Start
    driver.masterStart(id->i2cId->i2c);

    uint32_t timeoutTimeMs = systemTimeMsGet() + I2C_TIMEOUT_MS;
    i2c_error_e error = I2C_ERROR_NONE;

    do
    {
        if (systemCheckTimeMs(timeoutTimeMs))
        {
            error = I2C_ERROR_TIMEOUT;

            break;
        }
    }
    while (info->state != I2C_STATE_IDLE);

    BUFFER_READ(&(info->buffer), &transfer);

    if (error == I2C_ERROR_NONE)
    {
        for (i = 0; i < rxData->size; i++)
        {
            if (!BUFFER_READ(&(transfer.rxBuffer), &(rxData->bytes[i])))
            {
                // Error?
            }
        }
    }

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
static void intHandler(const i2c_e i2c, const i2c_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(i2cToInfoMap[i2c]))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    // Map I2C back to ID
    i2c_info_t* info = i2cToInfoMap[i2c];

    i2c_transfer_t* transfer;

    BUFFER_PEEK(&(info->buffer), &transfer);

#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(transfer))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    int txBufferCount = 0;
    uint8_t data = 0;

    bufferCount(&(transfer->txBuffer), &txBufferCount);

    switch (interrupt)
    {
        case I2C_INTERRUPT_MASTER_START_TRANSMITTED:
        {
            if (transfer->txCount > 0)
            {
                driver.masterTxAddress(i2c,
                                       transfer->device->address,
                                       I2C_MASTER_MODE_TX);
            }
            else if (transfer->rxCount > 0)
            {
                driver.masterTxAddress(i2c,
                                       transfer->device->address,
                                       I2C_MASTER_MODE_RX);
            }

            break;
        }
        case I2C_INTERRUPT_MASTER_TRANSMITTER_SELECTED:
        {
            if (txBufferCount > 0)
            {
                if (BUFFER_READ(&(transfer->txBuffer), &data))
                {
                    driver.tx(i2c, data);
                }
            }

            break;
        }
        case I2C_INTERRUPT_MASTER_RECEIVER_SELECTED:
        {
            if (transfer->rxCount > 0)
            {
                driver.rx(i2c, (transfer->rxCount == 1));
            }

            break;
        }
        case I2C_INTERRUPT_BYTE_TRANSMITTING:
        {
            if (txBufferCount > 0)
            {
                if (BUFFER_READ(&(transfer->txBuffer), &data))
                {
                    driver.tx(i2c, data);
                }
            }

            break;
        }
        case I2C_INTERRUPT_BYTE_TRANSMITTED:
        {
            if (txBufferCount > 0)
            {
                if (BUFFER_READ(&(transfer->txBuffer), &data))
                {
                    driver.tx(i2c, data);
                }
            }
            else
            {
                if (transfer->rxCount > 0)
                {
                    // Restart condition
                    driver.masterStart(i2c);
                }
                else
                {
                    driver.masterStop(i2c);
                    info->state = I2C_STATE_IDLE;
                }
            }

            break;
        }
        case I2C_INTERRUPT_BYTE_RECEIVED:
        {
            int rxBufferCount = 0;

            driver.read(i2c, &data);

            if (!BUFFER_WRITE(&(transfer->rxBuffer), &data))
            {
                // Error?
            }

            bufferCount(&(transfer->rxBuffer), &rxBufferCount);

            if (rxBufferCount < transfer->rxCount)
            {
                driver.rx(i2c, ((rxBufferCount + 1) == transfer->rxCount));
            }
            else
            {
                driver.masterStop(i2c);
                info->state = I2C_STATE_IDLE;
            }

            break;
        }
        case I2C_INTERRUPT_SLAVE_STOP_RECEIVED:
        {
            if (transfer->txCount > 0)
            {
                if (BUFFER_READ(&(transfer->txBuffer), &data))
                {
                    driver.tx(i2c, data);
                }
                else
                {
                    driver.masterStop(i2c);
                    info->state = I2C_STATE_IDLE;
                }
            }
            else if (transfer->rxCount > 0)
            {

            }

            break;
        }
        case I2C_INTERRUPT_NACK_RECEIVED:
        {
            int a = 0;

            break;
        }
        default:
        {
            break;
        }
    }
}
