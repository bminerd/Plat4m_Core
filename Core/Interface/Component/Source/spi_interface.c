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
 * @file spi_interface.c
 * @author Ben Minerd
 * @date 7/30/12
 * @brief SPI interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <spi_interface.h>
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
 * @brief SPI driver.
 */
static spi_driver_t driver;

/**
 * @brief SPI to ID map.
 */
static spi_info_t* spiToInfoMap[SPI_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Performs a SPI transfer.
 * @param id SPI device.
 * @param mode SPI transfer mode.
 * @param txData Data to transmit.
 * @param rxData Data received.
 * @return SPI error.
 */
static spi_error_e spiTransfer(const spi_device_id_t* id,
                               const spi_transfer_mode_e mode,
                               const byte_array_t* txData,
                               byte_array_t* rxData);

/**
 * @brief SPI interrupt handler.
 * @param spi SPI to access.
 * @param interrupt Interrupt to handle.
 */
static void intHandler(const spi_e spi, const spi_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void spiInit(void)
{
    int i;

    // Initialize SPI to ID map
    for (i = 0; i < SPI_COUNT; i++)
    {
        spiToInfoMap[i] = NULL_POINTER;
    }

    spiApplicationInit();
}

//------------------------------------------------------------------------------
spi_error_e spiSetDriver(const spi_driver_t* spiDriver,
                         spi_interface_int_handler_f** handler)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(spiDriver)              ||
        IS_NULL_POINTER(spiDriver->setEnabled)  ||
        IS_NULL_POINTER(spiDriver->configure)   ||
        IS_NULL_POINTER(spiDriver->tx)          ||
        IS_NULL_POINTER(spiDriver->rx)          ||
        IS_NULL_POINTER(spiDriver->intSetEnabled))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *spiDriver;
    *handler = intHandler;

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
spi_error_e spiInitId(spi_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

    if (id->spi >= SPI_COUNT)
    {
        return SPI_ERROR_SPI_INVALID;
    }

#endif // PLAT4M_DEBUG

    spi_info_t* info = (spi_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    mutexInitMutex(&(info->mutex));

    BUFFER_INIT(&(info->buffer), info->bufferMemory);

    spiToInfoMap[id->spi] = (spi_info_t*) id->private;

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
spi_error_e spiSetEnabled(spi_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    spi_info_t* info = (spi_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return SPI_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.setEnabled(id->spi, enabled);
    info->isEnabled = enabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
spi_error_e spiIsEnabled(spi_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    spi_info_t* info = (spi_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return SPI_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *isEnabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
spi_error_e spiConfigure(spi_id_t* id, const spi_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    spi_info_t* info = (spi_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return SPI_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    driver.configure(id->spi, config);
    memcpy((uint8_t*) &(info->config), (uint8_t*) config, sizeof(*config));

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
spi_error_e spiMasterTx(const spi_device_id_t* id,
                        const spi_transfer_mode_e mode,
                        const byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(data))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    spi_info_t* info = (spi_info_t*) id->spiId->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return SPI_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return SPI_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    byte_array_t rxData =
    {
        .size = 0
    };

    return spiTransfer(id, mode, data, &rxData);
}

//------------------------------------------------------------------------------
spi_error_e spiMasterRx(const spi_device_id_t* id,
                        const spi_transfer_mode_e mode,
                        byte_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(data))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    spi_info_t* info = (spi_info_t*) id->spiId->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return SPI_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return SPI_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    byte_array_t txData =
    {
        .size = 0
    };

    return spiTransfer(id, mode, &txData, data);
}

//------------------------------------------------------------------------------
spi_error_e spiMasterTxRx(const spi_device_id_t* id,
                          const spi_transfer_mode_e mode,
                          const byte_array_t* txData,
                          byte_array_t* rxData)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id)     ||
        IS_NULL_POINTER(txData) ||
        IS_NULL_POINTER(rxData))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    spi_info_t* info = (spi_info_t*) id->spiId->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return SPI_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return SPI_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    return spiTransfer(id, mode, txData, rxData);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static spi_error_e spiTransfer(const spi_device_id_t* id,
                               const spi_transfer_mode_e mode,
                               const byte_array_t* txData,
                               byte_array_t* rxData)
{
    spi_info_t* info = (spi_info_t*) id->spiId->private;

    spi_transfer_t transfer =
    {
        .device     = id,
        .mode       = mode,
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
            return SPI_ERROR_TX_BUFFER_FULL;
        }
    }

    if (!BUFFER_WRITE(&(info->buffer), &transfer))
    {
        return SPI_ERROR_TX_BUFFER_FULL;
    }

    // Chip select
    gpioSetLevel(transfer.device->chipSelectGpioId, GPIO_LEVEL_LOW);

    // Enable Tx and Rx interrupts
    driver.intSetEnabled(id->spiId->spi, SPI_INTERRUPT_TX, true);
    driver.intSetEnabled(id->spiId->spi, SPI_INTERRUPT_RX, true);

    uint32_t timeoutTimeMs = systemTimeMsGet() + SPI_TIMEOUT_MS;
    bool isEmpty = false;
    spi_error_e error = SPI_ERROR_NONE;

    do
    {
        if (systemCheckTimeMs(timeoutTimeMs))
        {
            error = SPI_ERROR_TIMEOUT;

            break;
        }

        bufferIsEmpty(&(info->buffer), &isEmpty);
    }
    while (!isEmpty);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
static void intHandler(const spi_e spi, const spi_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(spiToInfoMap[spi]))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    // Map SPI back to ID
    spi_info_t* info = spiToInfoMap[spi];

    spi_transfer_t* transfer;

    BUFFER_PEEK(&(info->buffer), &transfer);

#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(transfer))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    // TODO Change to support 16bit data
    // TODO Verify this scheme works!
    uint8_t data = 0;
    bool txIsEmpty, rxIsEmpty;

    bufferIsEmpty(&(transfer->txBuffer), &txIsEmpty);
    bufferIsEmpty(&(transfer->rxBuffer), &rxIsEmpty);

    if (txIsEmpty && rxIsEmpty)
    {
        driver.intSetEnabled(spi, SPI_INTERRUPT_TX, false);
        driver.intSetEnabled(spi, SPI_INTERRUPT_RX, false);
        BUFFER_READ(&(info->buffer), transfer);
    }
    else
    {
        switch (interrupt)
        {
            case SPI_INTERRUPT_TX:
            {
                switch (transfer->mode)
                {
                    case SPI_TRANSFER_MODE_SEQUENTIAL:
                    {
                        if (!txIsEmpty)
                        {
                            if (BUFFER_READ(&(transfer->txBuffer), &data))
                            {
                                driver.tx(spi, data);
                            }
                        }
                        else
                        {
                            driver.tx(spi, 0);
                        }

                        break;
                    }
                    case SPI_TRANSFER_MODE_CONCURRENT:
                    {
                        if (BUFFER_READ(&(transfer->txBuffer), &data))
                        {
                            driver.tx(spi, data);
                        }

                        break;
                    }
                }

                break;
            }
            case SPI_INTERRUPT_RX:
            {
                driver.rx(spi, &data);

                switch (transfer->mode)
                {
                    case SPI_TRANSFER_MODE_SEQUENTIAL:
                    {
                        if (rxIsEmpty)
                        {
                            if (!BUFFER_WRITE(&(transfer->rxBuffer), &data))
                            {
                                // Error?
                            }
                        }

                        break;
                    }
                    case SPI_TRANSFER_MODE_CONCURRENT:
                    {
                        if (!BUFFER_WRITE(&(transfer->rxBuffer), &data))
                        {
                            // Error?
                        }

                        break;
                    }
                }

                break;
            }
        }
    }
}
