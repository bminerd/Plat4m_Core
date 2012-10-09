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
#include <spi_driver.h>

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
    SPI_ERROR_INVALID,
    SPI_ERROR_DEVICE_INVALID,
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

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type that sets a SPI chip select enabled or disabled.
 */
typedef set_enabled_f spi_chip_select_set_enabled_f;

/**
 * @brief SPI device type.
 */
typedef struct _spi_device_t_
{
    spi_device_e device;
    spi_e spi;
    spi_chip_select_set_enabled_f* chipSelectSetEnabled;
} spi_device_t;

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
typedef void spi_driver_config_f(const spi_e spi, const spi_config_t* config);

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
    const spi_driver_set_enabled_f* setEnabled;
    const spi_driver_config_f* configure;
    const spi_driver_tx_f* tx;
    const spi_driver_rx_f* rx;
    const spi_driver_int_set_enabled_f* intSetEnabled;
} spi_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the SPI module.
 */
extern void spiInit(void);

/**
 * @brief Sets the driver for the SPI interface.
 * @param spiDriver SPI driver.
 * @return SPI error.
 */
extern spi_error_e spiSetDriver(const spi_driver_t* spiDriver);

/**
 * @brief Sets the given SPI enabled or disabled.
 * @param spi SPI to access.
 * @param enabled Flag that indicates if the SPI should be enabled or disabled.
 * @return SPI error.
 */
extern spi_error_e spiSetEnabled(const spi_e spi, const bool enabled);

/**
 * @brief Checks to see if the given SPI is enabled or disabled.
 * @param spi SPI to access.
 * @param isEnabled Flag that indicates if the SPI is enabled or disabled.
 * @return SPI error.
 */
extern spi_error_e spiIsEnabled(const spi_e spi, bool* isEnabled);

/**
 * @brief Configures the given SPI.
 * @param spi SPI to access.
 * @param config SPI configuration.
 * @return SPI error.
 */
extern spi_error_e spiConfigure(const spi_e spi, const spi_config_t* config);

/**
 * @brief Adds the given SPI device.
 * @param device Device to add.
 * @return SPI error.
 */
extern spi_error_e spiMasterAddDevice(const spi_device_t* device);

/**
 * @brief Adds the given SPI devices.
 * @param devices Devices to add.
 * @param size Number of devices to add.
 * @return SPI error.
 */
extern spi_error_e spiMasterAddDevices(const spi_device_t devices[],
                                       const unsigned int size);

/**
 * @brief Transmits the given data over the given SPI.
 * @param device SPI device.
 * @param data Data to transmit.
 * @return SPI error.
 */
extern spi_error_e spiMasterTx(const spi_device_e device,
                               const data_array_t* data);

#endif // _SPI_INTERFACE_H_
