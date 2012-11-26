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
 * @file accel_driver_adxl345.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Accelerometer driver layer for the ADXL345.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <accel_driver_adxl345.h>
#include <accel_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of accelerometer registers.
 */
typedef enum _accel_reg_e_
{
    ACCEL_REG_DEVID         = (uint8_t) 0x00,
    ACCEL_REG_OFSX          = (uint8_t) 0x1E,
    ACCEL_REG_OFSY          = (uint8_t) 0x1F,
    ACCEL_REG_OFSZ          = (uint8_t) 0x20,
    ACCEL_REG_BW_RATE       = (uint8_t) 0x2C,
    ACCEL_REG_POWER_CTL     = (uint8_t) 0x2D,
    ACCEL_REG_DATA_FORMAT   = (uint8_t) 0x31,
    ACCEL_REG_DATAX0        = (uint8_t) 0x32,
    ACCEL_REG_DATAX1        = (uint8_t) 0x33,
    ACCEL_REG_DATAY0        = (uint8_t) 0x34,
    ACCEL_REG_DATAY1        = (uint8_t) 0x35,
    ACCEL_REG_DATAZ0        = (uint8_t) 0x36,
    ACCEL_REG_DATAZ1        = (uint8_t) 0x37
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
 * @param accel Accel to access.
 * @param enabled Flag that indicates if the accelerometer should be enabled or
 * disabled.
 * @return Accelerometer error.
 */
static accel_error_e setEnabled(const accel_t* accel, const bool enabled);

/**
 * @brief Configures the accelerometer.
 * @param accel Accel to access.
 * @param config Accelerometer configuration.
 * @return Accelerometer error.
 */
static accel_error_e configure(const accel_t* accel,
                               const accel_config_t* config);

/**
 * @brief Gets an accelerometer reading.
 * @param accel Accel to access.
 * @param reading Accelerometer reading.
 * @return Accelerometer error.
 */
static accel_error_e read(const accel_t* accel, accel_reading_t* reading);

/**
 * @brief Writes the given value to the given accelerometer register.
 * @param accel Accel to access.
 * @param reg Accelerometer register to access.
 * @param value Value to write.
 * @return I2C error.
 */
static i2c_error_e regWrite(const accel_t* accel,
                            const accel_reg_e reg,
                            const uint8_t value);

/**
 * @brief Reads values from the given accelerometer register.
 * @param accel Accel to access.
 * @param reg Accelerometer register to access.
 * @param data Values read.
 * @return I2C error.
 */
static i2c_error_e regRead(const accel_t* accel,
                           const accel_reg_e reg,
                           byte_array_t* data);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void accelDriverAdxl345Init(void)
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
    regWrite(accel, ACCEL_REG_POWER_CTL, 0);
    regWrite(accel, ACCEL_REG_DATA_FORMAT, 0);

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
    uint8_t bytes[6];
    byte_array_t data = BYTE_ARRAY(bytes);

    if (regRead(accel, ACCEL_REG_DATAX0, &data) != I2C_ERROR_NONE)
    {
        return ACCEL_ERROR_COMMUNICATION;
    }

    // X sample
    reading->samples[ACCEL_DOF_X] = (((accel_sample_t) bytes[1]) << 8) |
                                     ((accel_sample_t) bytes[0]);

    // Y sample
    reading->samples[ACCEL_DOF_Y] = (((accel_sample_t) bytes[3]) << 8) |
                                     ((accel_sample_t) bytes[2]);

    // Z sample
    reading->samples[ACCEL_DOF_Z] = (((accel_sample_t) bytes[5]) << 8) |
                                     ((accel_sample_t) bytes[4]);

    return ACCEL_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e regWrite(const accel_t* accel,
                            const accel_reg_e reg,
                            const uint8_t value)
{
    // Cast to this accel
    accel_adxl345_t* thisAccel = (accel_adxl345_t*) accel;

    uint8_t bytes[] = {(uint8_t) reg, value};
    byte_array_t data = BYTE_ARRAY(bytes);
    
    return i2cMasterTx(thisAccel->i2cDeviceId, &data);
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(const accel_t* accel,
                           const accel_reg_e reg,
                           byte_array_t* data)
{
    // Cast to this accel
    accel_adxl345_t* thisAccel = (accel_adxl345_t*) accel;

    uint8_t txBytes[] = {reg};
    byte_array_t txData = BYTE_ARRAY(txBytes);
    
    return i2cMasterTxRx(thisAccel->i2cDeviceId, &txData, data);
}
