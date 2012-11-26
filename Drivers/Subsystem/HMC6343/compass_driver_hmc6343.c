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
 * @file compass_driver_hmc6343.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief Compass driver layer for the HMC6343.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <compass_driver_hmc6343.h>
#include <compass_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Compass reading conversion factor.
 */
#define COMPASS_READING_CONVERSION_FACTOR 0.1f

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of compass registers.
 */
typedef enum _compass_reg_e_
{
    COMPASS_REG_SLAVE_ADDRESS   = (uint8_t) 0x00,
    COMPASS_REG_SW_VERSION      = (uint8_t) 0x02,
    COMPASS_REG_OP_MODE_1       = (uint8_t) 0x04,
    COMPASS_REG_OP_MODE_2       = (uint8_t) 0x05,
    COMPASS_REG_SN_LSB          = (uint8_t) 0x06,
    COMPASS_REG_SN_MSB          = (uint8_t) 0x07,
    COMPASS_REG_DATE_YY         = (uint8_t) 0x08,
    COMPASS_REG_DATE_WW         = (uint8_t) 0x09,
    COMPASS_REG_DEVIATION_LSB   = (uint8_t) 0x0A,
    COMPASS_REG_DEVIATION_MSB   = (uint8_t) 0x0B,
    COMPASS_REG_VARIATION_LSB   = (uint8_t) 0x0C,
    COMPASS_REG_VARIATION_MSB   = (uint8_t) 0x0D,
    COMPASS_REG_X_OFFSET_LSB    = (uint8_t) 0x0E,
    COMPASS_REG_X_OFFSET_MSB    = (uint8_t) 0x0F,
    COMPASS_REG_Y_OFFSET_LSB    = (uint8_t) 0x10,
    COMPASS_REG_Y_OFFSET_MSB    = (uint8_t) 0x11,
    COMPASS_REG_Z_OFFSET_LSB    = (uint8_t) 0x12,
    COMPASS_REG_Z_OFFSET_MSB    = (uint8_t) 0x13,
    COMPASS_REG_FILTER_LSB      = (uint8_t) 0x14,
    COMPASS_REG_FILTER_MSB      = (uint8_t) 0x15
} compass_reg_e;

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
 * @brief Sets the given compass enabled or disabled.
 * @param compass Compass to access.
 * @param enabled Flag that indicates if the compass should be enabled or
 * disabled.
 * @return Compass error.
 */
static compass_error_e setEnabled(const compass_t* compass, const bool enabled);

/**
 * @brief Configures the given compass.
 * @param compass Compass to access.
 * @param magConfig Compass configuration.
 * @return Compass error.
 */
static compass_error_e configure(const compass_t* compass,
                                 const compass_config_t* magConfig);

/**
 * @brief Gets a reading from the given compass.
 * @param compass Compass to access.
 * @param reading Compass reading.
 * @return Compass error.
 */
static compass_error_e read(const compass_t* compass,
                            compass_reading_t* reading);

/**
 * @brief Writes the given value to the given compass register.
 * @param compass Compass to access.
 * @param reg Compass register to access.
 * @param data Values to write.
 * @return I2C error.
 */
static i2c_error_e regWrite(const compass_hmc6343_t* compass,
                            const compass_reg_e reg,
                            const byte_array_t* data);

/**
 * @brief Reads values from the given compass register.
 * @param compass Compass to access.
 * @param reg Compass register to access.
 * @param data Values read.
 * @return I2C error.
 */
static i2c_error_e regRead(const compass_hmc6343_t* compass,
                           const compass_reg_e reg,
                           byte_array_t* data);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void compassDriverHmc6343Init(void)
{
    compass_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure,
        .read       = read
    };

    compassSetDriver(&driver);

    // TODO Initialize compass registers
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static compass_error_e setEnabled(const compass_t* compass, const bool enabled)
{
    return COMPASS_ERROR_NONE;
}

//------------------------------------------------------------------------------
static compass_error_e configure(const compass_t* compass,
                                 const compass_config_t* magConfig)
{
    return COMPASS_ERROR_NONE;
}

//------------------------------------------------------------------------------
static compass_error_e read(const compass_t* compass,
                            compass_reading_t* reading)
{
    // Cast to this compass
    compass_hmc6343_t* thisCompass = (compass_hmc6343_t*) compass;

    uint8_t bytes[2];
    byte_array_t data = BYTE_ARRAY(bytes);

    if (regRead(thisCompass, COMPASS_REG_DEVIATION_LSB, &data) !=
            I2C_ERROR_NONE)
    {
        return COMPASS_ERROR_COMMUNICATION;
    }

    *reading = (compass_reading_t) ((((uint16_t) bytes[1]) << 8) |
                                     ((uint16_t) bytes[0])) *
                                   COMPASS_READING_CONVERSION_FACTOR;

    return COMPASS_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e regWrite(const compass_hmc6343_t* compass,
                            const compass_reg_e reg,
                            const byte_array_t* data)
{
    // Make large enough to handle multiple (6) sequential register writes.
    uint8_t bytes[7] = {(uint8_t) reg};

    memcpy(&(bytes[1]), data->bytes, data->size);

    byte_array_t txData = BYTE_ARRAY(bytes);
    txData.size = 1 + data->size;

    return i2cMasterTx(compass->i2cDeviceId, &txData);
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(const compass_hmc6343_t* compass,
                           const compass_reg_e reg,
                           byte_array_t* data)
{
    uint8_t txBytes[] = {reg};
    byte_array_t txData = BYTE_ARRAY(txBytes);

    return i2cMasterTxRx(compass->i2cDeviceId, &txData, data);
}
