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
 * @file mag_driver_hmc58831.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Magnetometer driver layer for the HMC5883L.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <mag_driver_hmc5883l.h>
#include <mag_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of compass registers.
 */
typedef enum _mag_reg_e_
{
    MAG_REG_CNFG_A      = (uint8_t) 0x00,
    MAG_REG_CNFG_B      = (uint8_t) 0x01,
    MAG_REG_MODE        = (uint8_t) 0x02,
    MAG_REG_X_MSB       = (uint8_t) 0x03,
    MAG_REG_X_LSB       = (uint8_t) 0x04,
    MAG_REG_Z_MSB       = (uint8_t) 0x05,
    MAG_REG_Z_LSB       = (uint8_t) 0x06,
    MAG_REG_Y_MSB       = (uint8_t) 0x07,
    MAG_REG_Y_LSB       = (uint8_t) 0x08
} mag_reg_e;

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
 * @brief Sets the magnetometer enabled or disabled.
 * @param id Magnetometer to access.
 * @param enabled Flag that indicates if the magnetometer should be enabled or
 * disabled.
 * @return Magnetometer error.
 */
static mag_error_e setEnabled(const mag_t* mag, const bool enabled);

/**
 * @brief Configures the magnetometer.
 * @param id Magnetometer to access.
 * @param magConfig Magnetometer configuration.
 * @return Magnetometer error.
 */
static mag_error_e configure(const mag_t* mag, const mag_config_t* magConfig);

/**
 * @brief Gets a magnetometer reading.
 * @param id Magnetometer to access.
 * @param reading Magnetometer reading.
 * @return Magnetometer error.
 */
static mag_error_e read(const mag_t* mag, mag_reading_t* reading);

/**
 * @brief Writes the given value to the given magnetometer register.
 * @param id Magnetometer to access.
 * @param reg Magnetometer register to access.
 * @param data Values to write.
 * @return I2C error.
 */
static i2c_error_e regWrite(const mag_hmc5883l_t* mag,
                            const mag_reg_e reg,
                            const byte_array_t* data);

/**
 * @brief Reads values from the given magnetometer register.
 * @param id Magnetometer to access.
 * @param reg Magnetometer register to access.
 * @param data Values read.
 * @return I2C error.
 */
static i2c_error_e regRead(const mag_hmc5883l_t* mag,
                           const mag_reg_e reg,
                           byte_array_t* data);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void magDriverHmc5883lInit(void)
{
    mag_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure,
        .read       = read
    };

    magSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static mag_error_e setEnabled(const mag_t* mag, const bool enabled)
{
    return MAG_ERROR_NONE;
}

//------------------------------------------------------------------------------
static mag_error_e configure(const mag_t* mag, const mag_config_t* magConfig)
{
    return MAG_ERROR_NONE;
}

//------------------------------------------------------------------------------
static mag_error_e read(const mag_t* mag, mag_reading_t* reading)
{
    // Cast to this magnetometer
    mag_hmc5883l_t* thisMag = (mag_hmc5883l_t*) mag;

    uint8_t bytes[6];
    byte_array_t data = BYTE_ARRAY(bytes);

    if (regRead(thisMag, MAG_REG_X_MSB, &data) != I2C_ERROR_NONE)
    {
        return MAG_ERROR_COMMUNICATION;
    }

    // Azimuth sample
    reading->samples[MAG_DOF_AZIMUTH]   = (((mag_sample_t) bytes[0]) << 8) |
                                           ((mag_sample_t) bytes[1]);

    // Elevation sample
    reading->samples[MAG_DOF_ELEVATION] = (((mag_sample_t) bytes[2]) << 8) |
                                           ((mag_sample_t) bytes[3]);

    // Bank sample
    reading->samples[MAG_DOF_BANK]      = (((mag_sample_t) bytes[4]) << 8) |
                                           ((mag_sample_t) bytes[5]);

    return MAG_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e regWrite(const mag_hmc5883l_t* mag,
                            const mag_reg_e reg,
                            const byte_array_t* data)
{
    // Make large enough to handle multiple (6) sequential register writes.
    uint8_t bytes[7] = {(uint8_t) reg};

    memcpy(&(bytes[1]), data->bytes, data->size);

    byte_array_t txData = BYTE_ARRAY(bytes);
    txData.size = 1 + data->size;

    return i2cMasterTx(mag->i2cDeviceId, &txData);
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(const mag_hmc5883l_t* mag,
                           const mag_reg_e reg,
                           byte_array_t* data)
{
    uint8_t txBytes[] = {reg};
    byte_array_t txData = BYTE_ARRAY(txBytes);

    return i2cMasterTxRx(mag->i2cDeviceId, &txData, data);
}
