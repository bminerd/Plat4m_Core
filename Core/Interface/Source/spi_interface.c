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
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <spi_interface.h>
#include <spi_driver.h>

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

struct _spi_device_wrapper_t_;

/**
 * @brief SPI transfer type.
 */
typedef struct _spi_transfer_t_
{
    struct _spi_device_wrapper_t_* device;
    buffer_t txBuffer;
    buffer_t rxBuffer;
} spi_transfer_t;

/**
 * @brief SPI type.
 */
typedef struct _spi_t_
{
    spi_e spi;
    bool isEnabled;
    spi_config_t config;
    spi_transfer_t bufferMemory[SPI_TRANSFER_BUFFER_SIZE];
    buffer_t buffer;
    uint8_t txBufferMemory[SPI_BUFFER_SIZE];
    uint8_t rxBufferMemory[SPI_BUFFER_SIZE];
    volatile spi_state_e state;
} spi_t;

/**
 * @brief SPI device wrapper type.
 */
typedef struct _spi_device_wrapper_t_
{
    spi_device_t device;
    spi_t* spi;
} spi_device_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief SPI driver.
 */
static spi_driver_t driver;

/**
 * @brief SPI map.
 */
static spi_t spiMap[SPI_COUNT];

/**
 * @brief SPI device map.
 */
static spi_device_wrapper_t deviceMap[SPI_DEVICE_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

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
extern void spiInit(void)
{
    int i;

    for (i = 0; i < SPI_COUNT; i++)
    {
        spiMap[i].spi       = SPI_NONE;
        spiMap[i].isEnabled = false;

        BUFFER_INIT(&(spiMap[i].buffer), spiMap[i].bufferMemory);

        spiMap[i].state     = SPI_STATE_IDLE;
    }

    for (i = 0; i < SPI_DEVICE_COUNT; i++)
    {
        deviceMap[i].device.device  = SPI_DEVICE_NONE;
        deviceMap[i].device.spi     = SPI_NONE;
        deviceMap[i].spi            = NULL_POINTER;
    }

    spiDriverInit(intHandler);
}

//------------------------------------------------------------------------------
extern spi_error_e spiSetDriver(const spi_driver_t* spiDriver)
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

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern spi_error_e spiSetEnabled(const spi_e spi, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((spi == SPI_NONE) || (spi >= SPI_COUNT))
    {
        return SPI_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.setEnabled(spi, enabled);
    spiMap[spi].isEnabled = enabled;

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern spi_error_e spiIsEnabled(const spi_e spi, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isEnabled))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

    if ((spi == SPI_NONE) || (spi >= SPI_COUNT))
    {
        return SPI_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = spiMap[spi].isEnabled;

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern spi_error_e spiConfigure(const spi_e spi, const spi_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if ((spi == SPI_NONE) || (spi >= SPI_COUNT))
    {
        return SPI_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver.configure(spi, config);
    spiMap[spi].config = *config;

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern spi_error_e spiMasterAddDevice(const spi_device_t* device)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(device) ||
        IS_NULL_POINTER(device->chipSelectSetEnabled))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

    if ((device->spi == SPI_NONE) || (device->spi >= SPI_COUNT))
    {
        return SPI_ERROR_INVALID;
    }

    if ((device->device ==  SPI_DEVICE_NONE) ||
        (device->device >= SPI_DEVICE_COUNT))
    {
        return SPI_ERROR_DEVICE_INVALID;
    }

#endif // PLAT4M_DEBUG

    deviceMap[device->device].device    = *device;
    deviceMap[device->device].spi       = &(spiMap[device->spi]);

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern spi_error_e spiMasterAddDevices(const spi_device_t devices[],
                                       const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > SPI_DEVICE_COUNT)
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        spi_error_e error = spiMasterAddDevice(&(devices[i]));

#ifdef PLAT4M_DEBUG

        if (error != SPI_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return SPI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern spi_error_e spiMasterTx(const spi_device_e device,
                               const data_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(data))
    {
        return SPI_ERROR_PARAMETER_INVALID;
    }

    if ((device == SPI_DEVICE_NONE) || (device >= SPI_DEVICE_COUNT))
    {
        return SPI_ERROR_DEVICE_INVALID;
    }

    if (!(deviceMap[device].spi->isEnabled))
    {
        return SPI_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    while (spiMap[deviceMap[device].spi->spi].state != SPI_STATE_IDLE);

    spiMap[deviceMap[device].spi->spi].state = SPI_STATE_BUSY;

    spi_transfer_t transfer =
    {
        .device = &deviceMap[device]
    };

    BUFFER_INIT(&(transfer.txBuffer),
                spiMap[transfer.device->spi->spi].txBufferMemory);
    BUFFER_INIT(&(transfer.rxBuffer),
                spiMap[transfer.device->spi->spi].rxBufferMemory);

    int i;

    for (i = 0; i < data->data.size; i++)
    {
        if (!BUFFER_WRITE(&(transfer.txBuffer), &(data->data.bytes[i])))
        {
            return SPI_ERROR_TX_BUFFER_FULL;
        }
    }

    if (!BUFFER_WRITE(&(transfer.device->spi->buffer), &transfer))
    {
        return SPI_ERROR_TX_BUFFER_FULL;
    }

    // Chip select
    transfer.device->device.chipSelectSetEnabled(true);

    // Enable Tx interrupt
    driver.intSetEnabled(transfer.device->spi->spi, SPI_INTERRUPT_TX, true);

    uint32_t timeoutTimeMs = systemTimeMsGet() + 1000;

    while (!bufferIsEmpty((buffer_t*) &(transfer.device->spi->buffer)))
    {
        if (systemCheckTimeMs(timeoutTimeMs))
        {
            return SPI_ERROR_TIMEOUT;
        }
    }

    return SPI_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void intHandler(const spi_e spi, const spi_interrupt_e interrupt)
{
#ifdef PLAT4M_DEBUG

    if ((spi == SPI_NONE)                   ||
        (spi >= SPI_COUNT)                  ||
        (interrupt == SPI_INTERRUPT_NONE)   ||
        (interrupt >= SPI_INTERRUPT_COUNT))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    // TODO Change to support 16bit data
    uint8_t data = 0;
    spi_transfer_t* transfer;

    BUFFER_PEEK(&(spiMap[spi].buffer), &transfer);

#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(transfer))
    {
        return;
    }

#endif // PLAT4M_DEBUG

    switch (interrupt)
    {
        case SPI_INTERRUPT_TX:
        {
            if (BUFFER_READ(&(transfer->txBuffer), &data))
            {
                driver.tx(spi, data);
            }
            else
            {
                driver.intSetEnabled(spi, SPI_INTERRUPT_TX, false);
            }

            break;
        }
        case SPI_INTERRUPT_RX:
        {
            driver.rx(spi, &data);

            if (BUFFER_WRITE(&(transfer->rxBuffer), &data))
            {

            }

            break;
        }
    }
}
