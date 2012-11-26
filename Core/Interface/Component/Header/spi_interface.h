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
 * @file spi_interface.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief SPI interface layer.
 */

#ifndef _SPI_INTERFACE_H_
#define _SPI_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <spi_application.h>
#include <gpio_interface.h>
#include <buffer_interface.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of SPI errors.
 */
typedef enum _spi_error_e_
{
    SPI_ERROR_NONE,
    SPI_ERROR_PARAMETER_INVALID,
    SPI_ERROR_SPI_INVALID,
    SPI_ERROR_DEVICE_ID_INVALID,
    SPI_ERROR_NOT_INITIALIZED,
    SPI_ERROR_NOT_ENABLED,
    SPI_ERROR_TX_BUFFER_FULL,
    SPI_ERROR_TIMEOUT
} spi_error_e;

/**
 * @brief Enumeration of SPI states.
 */
typedef enum _spi_state_e_
{
    SPI_STATE_IDLE,
    SPI_STATE_BUSY
} spi_state_e;

/**
 * @brief Enumeration of SPI modes.
 */
typedef enum _spi_mode_e_
{
    SPI_MODE_MASTER,
    SPI_MODE_SLAVE,

    // Do not place values below!
    SPI_MODE_COUNT
} spi_mode_e;

/**
 * @brief Enumeration of SPI data bits.
 */
typedef enum _spi_data_bits_e_
{
    SPI_DATA_BITS_8,
    SPI_DATA_BITS_16,

    // Do not place values below!
    SPI_DATA_BITS_COUNT,
} spi_data_bits_e;

/**
 * @brief Enumeration of SPI clock polarities.
 */
typedef enum _spi_clock_polarity_e_
{
    SPI_CLOCK_POLARITY_LOW,
    SPI_CLOCK_POLARITY_HIGH,

    // Do not place values below!
    SPI_CLOCK_POLARITY_COUNT
} spi_clock_polarity_e;

/**
 * @brief Enumeration of SPI clock phases.
 */
typedef enum _spi_clock_phase_e_
{
    SPI_CLOCK_PHASE_LEADING_EDGE,
    SPI_CLOCK_PHASE_TRAILING_EDGE,

    // Do not place values below!
    SPI_CLOCK_PHASE_COUNT
} spi_clock_phase_e;

/**
 * @brief Enumeration of SPI data orders.
 */
typedef enum _spi_bit_order_e_
{
    SPI_BIT_ORDER_LSB_FIRST,
    SPI_BIT_ORDER_MSB_FIRST,

    // Do not place values below!
    SPI_BIT_ORDER_COUNT
} spi_bit_order_e;

/**
 * @brief Enumeration of SPI transfer modes.
 */
typedef enum _spi_transfer_mode_e_
{
    SPI_TRANSFER_MODE_SEQUENTIAL,
    SPI_TRANSFER_MODE_CONCURRENT
} spi_transfer_mode_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief SPI configuration type.
 */
typedef struct _spi_config_t_
{
    spi_mode_e mode;
    spi_data_bits_e dataBits;
    spi_clock_polarity_e clockPolarity;
    spi_clock_phase_e clockPhase;
    spi_clock_prescaler_e clockPrescaler;
    spi_bit_order_e bitOrder;
    spi_gpio_map_e map;
} spi_config_t;

/**
 * @brief Predefinition of the SPI device wrapper type.
 */
struct _spi_device_id_t_;

/**
 * @brief SPI transfer type.
 */
typedef struct _spi_transfer_t_
{
    const struct _spi_device_id_t_* device;
    const spi_transfer_mode_e mode;
    const unsigned int txCount;
    const unsigned int rxCount;
    buffer_t txBuffer;
    buffer_t rxBuffer;
} spi_transfer_t;

/**
 * @brief SPI info type.
 */
typedef struct _spi_info_t_
{
    bool isInitialized;
    bool isEnabled;
    mutex_t mutex;
    spi_config_t config;
    spi_transfer_t bufferMemory[SPI_TRANSFER_BUFFER_SIZE];
    buffer_t buffer;
    uint8_t txBufferMemory[SPI_TX_BUFFER_SIZE];
    uint8_t rxBufferMemory[SPI_RX_BUFFER_SIZE];
} spi_info_t;

/**
 * @brief SPI ID type.
 */
typedef struct _spi_id_t_
{
    spi_e spi;
    const generic_data_t private[sizeof(spi_info_t) / sizeof(generic_data_t)];
} spi_id_t;

/**
 * @brief SPI device type.
 */
typedef struct _spi_device_id_t_
{
    spi_id_t* spiId;
    gpio_id_t* chipSelectGpioId;
} spi_device_id_t;

/**
 * @brief Function type that sets the given SPI enabled or disabled.
 * @param spi SPI to access.
 * @param enabled Flag that indicates if the SPI should be enabled or disabled.
 */
typedef void spi_driver_set_enabled_f(const spi_e spi, const bool enabled);

/**
 * @brief Function type that configures the given SPI.
 * @param spi SPI to access.
 * @param config SPI configuration.
 */
typedef void spi_driver_configure_f(const spi_e spi,
                                    const spi_config_t* config);

/**
 * @brief Function type that transmits the given data over the given SPI.
 * @param spi SPI to access.
 * @param data Data to transmit.
 * @todo Support 16-bit data.
 */
typedef void spi_driver_tx_f(const spi_e spi, const uint8_t data);

/**
 * @brief Function type that receives data over the given SPI.
 * @param spi SPI to acccess.
 * @param data Data received.
 * @todo Support 16-bit data.
 */
typedef void spi_driver_rx_f(const spi_e spi, uint8_t* data);

/**
 * @brief Function type that sets the given SPI interrupt enabled or disabled.
 * @param spi SPI to access.
 * @param interrupt SPI interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
typedef void spi_driver_int_set_enabled_f(const spi_e spi,
                                          const spi_interrupt_e interrupt,
                                          const bool enabled);

/**
 * @brief SPI driver type.
 */
typedef struct _spi_driver_t_
{
    spi_driver_set_enabled_f* setEnabled;
    spi_driver_configure_f* configure;
    spi_driver_tx_f* tx;
    spi_driver_rx_f* rx;
    spi_driver_int_set_enabled_f* intSetEnabled;
} spi_driver_t;

/**
 * @brief Function type for a SPI interface interrupt handler.
 */
typedef void spi_interface_int_handler_f(const spi_e spi,
                                         const spi_interrupt_e interrupt);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the SPI module.
 */
void spiInit(void);

/**
 * @brief Sets the driver for the SPI interface.
 * @param spiDriver SPI driver.
 * @return SPI error.
 */
spi_error_e spiSetDriver(const spi_driver_t* spiDriver,
                         spi_interface_int_handler_f** handler);

/**
 * @brief Adds the given SPI ID.
 * @param id SPI ID to add.
 * @return SPI error.
 */
spi_error_e spiInitId(spi_id_t* id);

/**
 * @brief Sets the given SPI enabled or disabled.
 * @param id SPI to access.
 * @param enabled Flag that indicates if the SPI should be enabled or disabled.
 * @return SPI error.
 */
spi_error_e spiSetEnabled(spi_id_t* id, const bool enabled);

/**
 * @brief Checks to see if the given SPI is enabled or disabled.
 * @param id SPI to access.
 * @param isEnabled Flag that indicates if the SPI is enabled or disabled.
 * @return SPI error.
 */
spi_error_e spiIsEnabled(spi_id_t* id, bool* isEnabled);

/**
 * @brief Configures the given SPI.
 * @param id SPI to access.
 * @param config SPI configuration.
 * @return SPI error.
 */
spi_error_e spiConfigure(spi_id_t* id, const spi_config_t* config);

/**
 * @brief Transmits the given data over the given SPI.
 * @param id SPI device.
 * @param mode SPI transfer mode.
 * @param data Data to transmit.
 * @return SPI error.
 */
spi_error_e spiMasterTx(const spi_device_id_t* id,
                        const spi_transfer_mode_e mode,
                        const byte_array_t* data);

/**
 * @brief Receives data over the given SPI.
 * @param id SPI device.
 * @param mode SPI transfer mode.
 * @param data Data received.
 * @return SPI error.
 */
spi_error_e spiMasterRx(const spi_device_id_t* id,
                        const spi_transfer_mode_e mode,
                        byte_array_t* data);

/**
 * @brief Transmits the given data over the given SPI.
 * @param id SPI device.
 * @param mode SPI transfer mode.
 * @param txData Data to transmit.
 * @param rxData Data received.
 * @return SPI error.
 */
spi_error_e spiMasterTxRx(const spi_device_id_t* id,
                          const spi_transfer_mode_e mode,
                          const byte_array_t* txData,
                          byte_array_t* rxData);

#endif // _SPI_INTERFACE_H_
