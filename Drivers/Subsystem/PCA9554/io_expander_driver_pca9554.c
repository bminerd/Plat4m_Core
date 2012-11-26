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
 * @file io_expander_driver_pca9554.c
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief IO expander driver layer for the PCA9554.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <io_expander_driver_pca9554.h>
#include <io_expander_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define IO_EXPANDER_REG_INPUT_ADDR      0x00
#define IO_EXPANDER_REG_OUTPUT_ADDR     0x01
#define IO_EXPANDER_REG_POLARITY_ADDR   0x02
#define IO_EXPANDER_REG_CONFIG_ADDR     0x03

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static uint8_t regAddressMap[IO_EXPANDER_PCA9554_REG_COUNT] =
{
    IO_EXPANDER_REG_INPUT_ADDR,     /// IO_EXPANDER_PCA9554_REG_INPUT
    IO_EXPANDER_REG_OUTPUT_ADDR,    /// IO_EXPANDER_PCA9554_REG_OUTPUT
    IO_EXPANDER_REG_POLARITY_ADDR,  /// IO_EXPANDER_PCA9554_REG_POLARITY
    IO_EXPANDER_REG_CONFIG_ADDR     /// IO_EXPANDER_PCA9554_REG_CONFIG
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Sets the IO expander enabled or disabled.
 * @param expander IO expander to access.
 * @param enabled Flag that indicates if the IO expander should be enabled or
 * disabled.
 * @return IO expander error.
 */
static io_expander_error_e setEnabled(io_expander_t* expander,
                                      const bool enabled);

/**
 * @brief Configures the IO expander.
 * @param expander IO expander to access.
 * @param config IO expander configuration.
 * @return IO expander error.
 */
static io_expander_error_e configure(io_expander_t* expander,
                                     const io_expander_config_t* config);

/**
 * @brief Configures the given IO expander IO hardware.
 * @param expander IO expander to access.
 * @param hardware IO expander IO hardware to access.
 * @param config IO expander IO hardware configuration.
 * @return IO expander error.
 */
static io_expander_error_e ioConfigure(
                                io_expander_t* expander,
                                const io_expander_io_hardware_t* hardware,
                                const io_expander_io_hardware_config_t* config);

/**
 * @brief Function type that sets the level for the given IO expander IO
 * hardware.
 * @param expander IO expander to access.
 * @param hardware IO expander IO hardware to access.
 * @param level GPIO level to set.
 * @return IO expander error.
 */
static io_expander_error_e ioSetLevel(io_expander_t* expander,
                                      const io_expander_io_hardware_t* hardware,
                                      const gpio_level_e level);

/**
 * @brief Function type that gets the level for the given IO expander IO
 * hardware.
 * @param expander IO expander to access.
 * @param hardware IO expander IO hardware to access.
 * @param level Current GPIO level.
 * @return IO expander error.
 */
static io_expander_error_e ioGetLevel(io_expander_t* expander,
                                      const io_expander_io_hardware_t* hardware,
                                      gpio_level_e* level);

/**
 * @brief Writes the given value to the given IO expander register.
 * @param expander IO expander to access.
 * @param reg IO expander register to access.
 * @param value Value to write.
 * @return I2C error.
 */
static i2c_error_e regWrite(io_expander_pca9554_t* expander,
                            const io_expander_pca9554_reg_e reg,
                            const uint8_t value);

/**
 * @brief Writes the given value to the given IO expander register bit.
 * @param expander IO expander to access.
 * @param reg IO expander register to access.
 * @param bit Bit index to write to.
 * @param value Value to write.
 * @return I2C error.
 */
static i2c_error_e regWriteBit(io_expander_pca9554_t* expander,
                               const io_expander_pca9554_reg_e reg,
                               const uint8_t bit,
                               const bit_value_e value);

/**
 * @brief Reads values from the given IO expander register.
 * @param expander IO expander to access.
 * @param reg IO expander register to access.
 * @param value Value read.
 * @return I2C error.
 */
static i2c_error_e regRead(io_expander_pca9554_t* expander,
                           const io_expander_pca9554_reg_e reg,
                           uint8_t* value);

/**
 * @brief Reads a value from the given IO expander register bit.
 * @param expander IO expander to access.
 * @param reg IO expander register to access.
 * @param bit Bit index to write to.
 * @param value Value read.
 * @return I2C error.
 */
static i2c_error_e regReadBit(io_expander_pca9554_t* expander,
                              const io_expander_pca9554_reg_e reg,
                              const uint8_t bit,
                              bit_value_e* value);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void ioExpanderDriverPca9554Init(void)
{
    io_expander_driver_t driver =
    {
        .id             = IO_EXPANDER_DRIVER_ID_PCA9554,
        .setEnabled     = setEnabled,
        .configure      = configure,
        .ioConfigure    = ioConfigure,
        .ioSetLevel     = ioSetLevel,
        .ioGetLevel     = ioGetLevel
    };

    ioExpanderAddDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static io_expander_error_e setEnabled(io_expander_t* expander,
                                      const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(expander))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    if (enabled)
    {
        // TODO initialize expander->regValueMap
    }
    else
    {

    }

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
static io_expander_error_e configure(io_expander_t* expander,
                                     const io_expander_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(expander) || IS_NULL_POINTER(config))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
static io_expander_error_e ioConfigure(
                                io_expander_t* expander,
                                const io_expander_io_hardware_t* hardware,
                                const io_expander_io_hardware_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(expander)   ||
        IS_NULL_POINTER(hardware)   ||
        IS_NULL_POINTER(config))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if (hardware->port >= IO_EXPANDER_PCA9554_IO_PORT_COUNT)
    {
        return IO_EXPANDER_ERROR_IO_PORT_INVALID;
    }

    if (hardware->pin >= IO_EXPANDER_PCA9554_IO_PIN_COUNT)
    {
        return IO_EXPANDER_ERROR_IO_PIN_INVALID;
    }

    if ((config->mode > GPIO_MODE_DIGITAL_INPUT) ||
        (config->resistor > GPIO_RESISTOR_PULL_UP))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to this IO expander
    io_expander_pca9554_t* thisExpander = (io_expander_pca9554_t*) expander;

    // Set IO direction
    // GPIO_MODE_DIGITAL_OUTPUT = 0 = output,
    // GPIO_MODE_DIGITAL_INPUT  = 1 = input
    regWriteBit(thisExpander,
                IO_EXPANDER_PCA9554_REG_CONFIG,
                hardware->pin,
                (bit_value_e) config->mode);

    // Internal pull-ups are enabled if in input mode

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
static io_expander_error_e ioSetLevel(io_expander_t* expander,
                                      const io_expander_io_hardware_t* hardware,
                                      const gpio_level_e level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(expander) || IS_NULL_POINTER(hardware))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if (hardware->port >= IO_EXPANDER_PCA9554_IO_PORT_COUNT)
    {
        return IO_EXPANDER_ERROR_IO_PORT_INVALID;
    }

    if (hardware->pin >= IO_EXPANDER_PCA9554_IO_PIN_COUNT)
    {
        return IO_EXPANDER_ERROR_IO_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to this IO expander
    io_expander_pca9554_t* thisExpander = (io_expander_pca9554_t*) expander;

    if (regWriteBit(thisExpander,
                    IO_EXPANDER_PCA9554_REG_OUTPUT,
                    hardware->pin,
                    (bit_value_e) level) != I2C_ERROR_NONE)
    {
        return IO_EXPANDER_ERROR_COMMUNICATION;
    }

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
static io_expander_error_e ioGetLevel(io_expander_t* expander,
                                      const io_expander_io_hardware_t* hardware,
                                      gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(expander)   ||
        IS_NULL_POINTER(hardware)   ||
        IS_NULL_POINTER(level))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if (hardware->port >= IO_EXPANDER_PCA9554_IO_PORT_COUNT)
    {
        return IO_EXPANDER_ERROR_IO_PORT_INVALID;
    }

    if (hardware->pin >= IO_EXPANDER_PCA9554_IO_PIN_COUNT)
    {
        return IO_EXPANDER_ERROR_IO_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to this IO expander
    io_expander_pca9554_t* thisExpander = (io_expander_pca9554_t*) expander;

    if (regReadBit(thisExpander,
                   IO_EXPANDER_PCA9554_REG_INPUT,
                   hardware->pin,
                   (bit_value_e*) level) != I2C_ERROR_NONE)
    {
        return IO_EXPANDER_ERROR_COMMUNICATION;
    }

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e regWrite(io_expander_pca9554_t* expander,
                            const io_expander_pca9554_reg_e reg,
                            const uint8_t value)
{
    io_expander_pca9554_info_t* info =
        (io_expander_pca9554_info_t*)expander->private;

    uint8_t bytes[] = {regAddressMap[reg], value};
    byte_array_t data = BYTE_ARRAY(bytes);

    i2c_error_e error = i2cMasterTx(expander->i2cDeviceId, &data);

    if (error == I2C_ERROR_NONE)
    {
        info->regValueMap[reg] = value;
    }

    return error;
}

//------------------------------------------------------------------------------
static i2c_error_e regWriteBit(io_expander_pca9554_t* expander,
                               const io_expander_pca9554_reg_e reg,
                               const uint8_t bit,
                               const bit_value_e value)
{
    io_expander_pca9554_info_t* info =
        (io_expander_pca9554_info_t*)expander->private;

    uint8_t regValue;

    switch (value)
    {
        case BIT_VALUE_0:
        {
            regValue = info->regValueMap[reg] & ~(1 << bit);

            break;
        }
        case BIT_VALUE_1:
        {
            regValue = info->regValueMap[reg] | (1 << bit);

            break;
        }
    }

    return regWrite(expander, reg, regValue);
}

//------------------------------------------------------------------------------
static i2c_error_e regRead(io_expander_pca9554_t* expander,
                           const io_expander_pca9554_reg_e reg,
                           uint8_t* value)
{
    io_expander_pca9554_info_t* info =
        (io_expander_pca9554_info_t*)expander->private;

    uint8_t txBytes[] = {regAddressMap[reg]};
    byte_array_t txData = BYTE_ARRAY(txBytes);
    byte_array_t rxData = BYTE_ARRAY(value);

    i2c_error_e error = i2cMasterTxRx(expander->i2cDeviceId, &txData, &rxData);

    if (error == I2C_ERROR_NONE)
    {
        info->regValueMap[reg] = *value;
    }

    return error;
}

//------------------------------------------------------------------------------
static i2c_error_e regReadBit(io_expander_pca9554_t* expander,
                              const io_expander_pca9554_reg_e reg,
                              const uint8_t bit,
                              bit_value_e* value)
{
    uint8_t regValue;
    uint8_t mask = ~(1 << bit);

    i2c_error_e error = regRead(expander, reg, &regValue);

    if (error == I2C_ERROR_NONE)
    {
        *value = (bit_value_e) ((regValue & mask) == mask);
    }

    return error;
}
