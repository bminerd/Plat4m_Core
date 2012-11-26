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
 * @file dac_ic_driver_ltc2637.c
 * @author Ben Minerd
 * @date 10/23/2012
 * @brief DAC ID driver layer for the LTC2637.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <dac_ic_driver_ltc2637.h>
#include <dac_ic_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define MAX_VALUE        4095
#define MAX_VOLTAGE      2.5f
#define UNITS_PER_VOLT   (((float) MAX_VALUE) / MAX_VOLTAGE)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of LTC2637 commands.
 */
typedef enum _command_e_
{
    COMMAND_WRITE_INPUT_N = 0,
    COMMAND_UPDATE_DAC_N,
    COMMAND_WRITE_N_UPDATE_ALL,
    COMMAND_WRITE_N_UPDATE_N,
    COMMAND_POWER_DOWN_N,
    COMMAND_POWER_DOWN_CHIP,
    COMMAND_INTERNAL_REF,
    COMMAND_EXTERNAL_REF
} command_e;

/**
 * @brief Enumeration of LTC2637 addresses.
 */
typedef enum _address_e_
{
    ADDRESS_DAC_A = 0,
    ADDRESS_DAC_B,
    ADDRESS_DAC_C,
    ADDRESS_DAC_D,
    ADDRESS_DAC_E,
    ADDRESS_DAC_F,
    ADDRESS_DAC_G,
    ADDRESS_DAC_H
} address_e;

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
 * @brief Sets the given DAC IC enabled or disabled.
 * @param dac DAC IC to access.
 * @param enabled Flag that indicates if the DAC should be enabled or disabled.
 */
static dac_ic_error_e setEnabled(const dac_ic_t* dac, const bool enabled);

/**
 * @brief Configures the given DAC IC.
 * @param dac DAC IC to access.
 * @param config DAC configuration.
 */
static dac_ic_error_e configure(const dac_ic_t* dac,
                                const dac_ic_config_t* config);

/**
 * @brief Sets the given voltage on the given DAC IC.
 * @param dac DAC IC to access.
 * @param channel DAC IC channel to access.
 * @param voltage Voltage to set on the DAC.
 */
static dac_ic_error_e channelSet(const dac_ic_t* dac,
                                 const dac_ic_channel_t channel,
                                 const float voltage);

static i2c_error_e write(const dac_ic_ltc2637_t* dac,
                         const command_e command,
                         const address_e address,
                         const uint16_t value);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void dacIcDriverLtc2637Init(void)
{
    dac_ic_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure,
        .channelSet = channelSet
    };

    dacIcSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static dac_ic_error_e setEnabled(const dac_ic_t* dac, const bool enabled)
{
    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
static dac_ic_error_e configure(const dac_ic_t* dac,
                                const dac_ic_config_t* config)
{
    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
static dac_ic_error_e channelSet(const dac_ic_t* dac,
                                 const dac_ic_channel_t channel,
                                 const float voltage)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(dac))
    {
        return DAC_IC_ERROR_PARAMETER_INVALID;
    }

    if (channel >= DAC_IC_LTC2637_CHANNEL_COUNT)
    {
        return DAC_IC_ERROR_CHANNEL_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Cast to this DAC IC
    dac_ic_ltc2637_t* thisDac = (dac_ic_ltc2637_t*) dac;

    uint16_t value = (uint16_t) (voltage * UNITS_PER_VOLT);

    if (write(thisDac,
              COMMAND_WRITE_N_UPDATE_N,
              (address_e) channel,
              value) != I2C_ERROR_NONE)
    {
        return DAC_IC_ERROR_COMMUNICATION;
    }

    return DAC_IC_ERROR_NONE;
}

//------------------------------------------------------------------------------
static i2c_error_e write(const dac_ic_ltc2637_t* dac,
                         const command_e command,
                         const address_e address,
                         const uint16_t value)
{
    uint8_t bytes[] =
    {
        (((uint8_t) command) << 4) | ((uint8_t) address),
        (uint8_t) (value >> 4),
        (uint8_t) (value << 4)
    };
    byte_array_t data = BYTE_ARRAY(bytes);

    return i2cMasterTx(dac->i2cDeviceId, &data);
}
