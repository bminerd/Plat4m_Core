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
 * @file accel_driver_adis16209.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Accelerometer driver layer for the ADIS16209.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <accel_driver_adis16209.h>
#include <accel_interface.h>

#include <gpio_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define ACCEL_REG_SIZE (2)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

typedef enum _reg_e_
{
    REG
} reg_e;

/**
 * @brief Enumeration of accelerometer registers.
 */
typedef enum _accel_reg_e_
{
    ACCEL_REG_ENDURANCE     = (uint8_t) 0x00,
    ACCEL_REG_SUPPLY_OUT    = (uint8_t) 0x02,
    ACCEL_REG_XACCL_OUT     = (uint8_t) 0x04,
    ACCEL_REG_YACCL_OUT     = (uint8_t) 0x06,
    ACCEL_REG_AUX_ADC       = (uint8_t) 0x08,
    ACCEL_REG_TEMP_OUT      = (uint8_t) 0x0A,
    ACCEL_REG_XINCL_OUT     = (uint8_t) 0x0C,
    ACCEL_REG_YINCL_OUT     = (uint8_t) 0x0E,
    ACCEL_REG_ROT_OUT       = (uint8_t) 0x10,
    ACCEL_REG_XACCL_NULL    = (uint8_t) 0x12,
    ACCEL_REG_YACCL_NULL    = (uint8_t) 0x14,
    ACCEL_REG_XINCL_NULL    = (uint8_t) 0x16,
    ACCEL_REG_YINCL_NULL    = (uint8_t) 0x18,
    ACCEL_REG_ROT_NULL      = (uint8_t) 0x1A,
    ACCEL_REG_ALM_MAG1      = (uint8_t) 0x20,
    ACCEL_REG_ALM_MAG2      = (uint8_t) 0x22,
    ACCEL_REG_ALM_SMPL1     = (uint8_t) 0x24,
    ACCEL_REG_ALM_SMPL2     = (uint8_t) 0x26,
    ACCEL_REG_ALM_CTRL      = (uint8_t) 0x28,
    ACCEL_REG_AUX_DAC       = (uint8_t) 0x30,
    ACCEL_REG_GPIO_CTRL     = (uint8_t) 0x32,
    ACCEL_REG_MSC_CTRL      = (uint8_t) 0x34,
    ACCEL_REG_SMPL_PRD      = (uint8_t) 0x36,
    ACCEL_REG_AVG_CNT       = (uint8_t) 0x38,
    ACCEL_REG_SLP_CNT       = (uint8_t) 0x3A,
    ACCEL_REG_STATUS        = (uint8_t) 0x3C,
    ACCEL_REG_COMMAND       = (uint8_t) 0x3E,
    ACCEL_REG_PROD_ID       = (uint8_t) 0x4A
} accel_reg_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Sets the accelerometer enabled or disabled.
 * @param accel Accelerometer to access.
 * @param enabled Flag that indicates if the accelerometer should be enabled or
 * disabled.
 * @return Accelerometer error.
 */
static accel_error_e setEnabled(const accel_t* accel, const bool enabled);

/**
 * @brief Configures the accelerometer.
 * @param accel Accelerometer to access.
 * @param config Accelerometer configuration.
 * @return Accelerometer error.
 */
static accel_error_e configure(const accel_t* accel,
                               const accel_config_t* config);

/**
 * @brief Gets an accelerometer reading.
 * @param accel Accelerometer to access.
 * @param reading Accelerometer reading.
 * @return Accelerometer error.
 */
static accel_error_e read(const accel_t* accel, accel_reading_t* reading);

/**
 * @brief Writes the given value to the given accelerometer register.
 * @param accel Accelerometer to access.
 * @param reg Accelerometer register to access.
 * @param data Values to write.
 * @return SPI error.
 */
static spi_error_e regWrite(const accel_t* accel,
                            const accel_reg_e reg,
                            const byte_array_t* data);

/**
 * @brief Reads values from the given accelerometer register.
 * @param accel Accelerometer to access.
 * @param reg Accelerometer register to access.
 * @param data Values read.
 * @return SPI error.
 */
static spi_error_e regRead(const accel_t* accel,
                           const accel_reg_e reg,
                           byte_array_t* data);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void accelDriverAdis16209Init(void)
{
    accel_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure,
        .read       = read
    };

    accelSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static accel_error_e setEnabled(const accel_t* accel, const bool enabled)
{
    return ACCEL_ERROR_NONE;
}

//------------------------------------------------------------------------------
static accel_error_e configure(const accel_t* accel,
                               const accel_config_t* config)
{
    return ACCEL_ERROR_NONE;
}

//------------------------------------------------------------------------------
static accel_error_e read(const accel_t* accel, accel_reading_t* reading)
{
    uint8_t bytes[ACCEL_REG_SIZE];
    byte_array_t data = BYTE_ARRAY(bytes);

    // X sample
    if (regRead(accel, ACCEL_REG_XINCL_OUT, &data) != SPI_ERROR_NONE)
    {
        return ACCEL_ERROR_COMMUNICATION;
    }

    reading->samples[ACCEL_DOF_X] = (((accel_sample_t) bytes[1]) << 8) |
                                     ((accel_sample_t) bytes[0]);

    // Y sample
    if (regRead(accel, ACCEL_REG_YINCL_OUT, &data) != SPI_ERROR_NONE)
    {
        return ACCEL_ERROR_COMMUNICATION;
    }

    reading->samples[ACCEL_DOF_Y] = (((accel_sample_t) bytes[1]) << 8) |
                                     ((accel_sample_t) bytes[0]);

    return ACCEL_ERROR_NONE;
}

//------------------------------------------------------------------------------
static spi_error_e regWrite(const accel_t* accel,
                            const accel_reg_e reg,
                            const byte_array_t* data)
{
    // Cast to this accel
    accel_adis16209_t* thisAccel = (accel_adis16209_t*) accel;

    uint8_t bytes[4] =
    {
        (uint8_t) reg,
        0,
        data->bytes[0],
        data->bytes[1]
    };

    byte_array_t regData = BYTE_ARRAY(bytes);
    
    return spiMasterTx(thisAccel->spiDeviceId,
                       SPI_TRANSFER_MODE_SEQUENTIAL,
                       &regData);
}

//------------------------------------------------------------------------------
static spi_error_e regRead(const accel_t* accel,
                           const accel_reg_e reg,
                           byte_array_t* data)
{
    // Cast to this accel
    accel_adis16209_t* thisAccel = (accel_adis16209_t*) accel;

    uint8_t txBytes[] = {reg};
    byte_array_t txData = BYTE_ARRAY(txBytes);
    
    return spiMasterTxRx(thisAccel->spiDeviceId,
                         SPI_TRANSFER_MODE_SEQUENTIAL,
                         &txData, data);
}
