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
 * @file gyro_driver_itg3200.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Gyroscope driver layer for the ITG3200.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gyro_driver_itg3200.h>
#include <gyro_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of Gyroscope registers.
 */
typedef enum _gyro_reg_e_
{
    GYRO_REG_WHO_AM_I       = (uint8_t) 0x00,
    GYRO_REG_SMPLRT_DIV     = (uint8_t) 0x15,
    GYRO_REG_DLPF_FS        = (uint8_t) 0x16,
    GYRO_REG_TEMP_OUT_H     = (uint8_t) 0x1B,
    GYRO_REG_TEMP_OUT_L     = (uint8_t) 0x1C,
    GYRO_REG_GYRO_XOUT_H    = (uint8_t) 0x1D,
    GYRO_REG_GYRO_XOUT_L    = (uint8_t) 0x1E,
    GYRO_REG_GYRO_YOUT_H    = (uint8_t) 0x1F,
    GYRO_REG_GYRO_YOUT_L    = (uint8_t) 0x20,
    GYRO_REG_GYRO_ZOUT_H    = (uint8_t) 0x21,
    GYRO_REG_GYRO_ZOUT_L    = (uint8_t) 0x22,
    GYRO_REG_PWR_MGM        = (uint8_t) 0x3E
} gyro_reg_e;

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
 * @brief Sets the gyroscope enabled or disabled.
 * @param gyro Gyroscope to access.
 * @param enabled Flag that indicates if the gyroscope should be enabled or
 * disabled.
 * @return Gyroscope error.
 */
static gyro_error_e setEnabled(const gyro_t* gyro, const bool enabled);

/**
 * @brief Configures the gyroscope.
 * @param gyro Gyroscope to access.
 * @param config Gyroscope configuration.
 * @return Gyroscope error.
 */
static gyro_error_e configure(const gyro_t* gyro, const gyro_config_t* config);

/**
 * @brief Gets an gyroscope reading.
 * @param gyro Gyroscope to access.
 * @param reading Gyroscope reading.
 * @return Gyroscope error.
 */
static gyro_error_e read(const gyro_t* gyro, gyro_reading_t* reading);

/**
 * @brief Writes the given value to the given gyroscope register.
 * @param gyro Gyroscope to access.
 * @param reg Gyroscope register to access.
 * @param value Value to write.
 * @return I2C error.
 */
static i2c_error_e regWrite(const gyro_itg3200_t* gyro,
                            const gyro_reg_e reg,
                            const uint8_t value);

/**
 * @brief Reads values from the given gyroscope register.
 * @param gyro Gyroscope to access.
 * @param reg Gyroscope register to access.
 * @param data Values read.
 * @return I2C error.
 */
static i2c_error_e regRead(const gyro_itg3200_t* gyro,
                           const gyro_reg_e reg,
                           byte_array_t* data);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void gyroDriverItg3200Init(void)
{
    gyro_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure,
        .read       = read
    };

    gyroSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static gyro_error_e setEnabled(const gyro_t* gyro, const bool enabled)
{
    // Cast to this gyroscope
    gyro_itg3200_t* thisGyro = (gyro_itg3200_t*) gyro;

    regWrite(thisGyro, GYRO_REG_SMPLRT_DIV, 0x15);
    regWrite(thisGyro, GYRO_REG_DLPF_FS, (0x03 << 3));
    regWrite(thisGyro, GYRO_REG_PWR_MGM, 0x01);

    return GYRO_ERROR_NONE;
}

//------------------------------------------------------------------------------
static gyro_error_e configure(const gyro_t* gyro, const gyro_config_t* config)
{
    return GYRO_ERROR_NONE;
}

//------------------------------------------------------------------------------
static gyro_error_e read(const gyro_t* gyro, gyro_reading_t* reading)
{
    // Cast to this gyroscope
    gyro_itg3200_t* thisGyro = (gyro_itg3200_t*) gyro;

    uint8_t bytes[8];
    byte_array_t data = BYTE_ARRAY(bytes);

    if (regRead(thisGyro, GYRO_REG_TEMP_OUT_H, &data) != I2C_ERROR_NONE)
    {
        return GYRO_ERROR_COMMUNICATION;
    }

    // Temp sample
    reading->temp                       = (((gyro_sample_t) bytes[0]) << 8) |
                                           ((gyro_sample_t) bytes[1]);

    // Yaw sample
    reading->samples[GYRO_DOF_YAW]      = (((gyro_sample_t) bytes[2]) << 8) |
                                           ((gyro_sample_t) bytes[3]);

    // Pitch sample
    reading->samples[GYRO_DOF_PITCH]    = (((gyro_sample_t) bytes[4]) << 8) |
                                           ((gyro_sample_t) bytes[5]);

    // Roll sample
    reading->samples[GYRO_DOF_ROLL]     = (((gyro_sample_t) bytes[6]) << 8) |
                                           ((gyro_sample_t) bytes[7]);

    return GYRO_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e regWrite(const gyro_itg3200_t* gyro,
                            const gyro_reg_e reg,
                            const uint8_t value)
{
    uint8_t bytes[] = {(uint8_t) reg, value};
    byte_array_t data = BYTE_ARRAY(bytes);
    
    return i2cMasterTx(gyro->i2cDeviceId, &data);
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(const gyro_itg3200_t* gyro,
                           const gyro_reg_e reg,
                           byte_array_t* data)
{
    uint8_t txBytes[] = {reg};
    byte_array_t txData = BYTE_ARRAY(txBytes);
    
    return i2cMasterTxRx(gyro->i2cDeviceId, &txData, data);
}
