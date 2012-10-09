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
 * @file adc_interface.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief ADC interface layer.
 */

#ifndef _ADC_INTERFACE_H_
#define _ADC_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <adc_driver.h>

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of ADC errors.
 */
typedef enum _adc_error_e_
{
    ADC_ERROR_NONE,
    ADC_ERROR_PARAMETER_INVALID,
    ADC_ERROR_ADC_INVALID,
    ADC_ERROR_CHANNEL_INVALID,
    ADC_ERROR_NOT_ENABLED
} adc_error_e;

/**
 * @brief Enumeration of ADC conversions.
 */
typedef enum _adc_conversion_mode_e_
{
    ADC_CONVERSION_MODE_SINGLE,
    ADC_CONVERSION_MODE_CONTINUOUS
} adc_conversion_mode_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief ADC configuration type.
 */
typedef struct _adc_config_t_
{
    adc_conversion_mode_e conversionMode;
} adc_config_t;

/**
 * @brief ADC hardware type.
 */
typedef struct _adc_hardware_t_
{
    adc_e adc;
    adc_channel_e channel;
} adc_hardware_t;

/**
 * @brief Sets the given ADC enabled or disabled.
 * @param hardware ADC to access.
 * @param enabled Flag that indicates if the ADC should be enabled or disabled.
 */
typedef void adc_driver_set_enabled_f(const adc_hardware_t* hardware,
                                      const bool enabled);

/**
 * @brief Configures the given ADC.
 * @param hardware ADC to access.
 * @param config ADC configuration.
 */
typedef void adc_driver_config_f(const adc_hardware_t* hardware,
                                 const adc_config_t* config);

/**
 * @brief Reads a value from the given ADC.
 * @param hardware ADC to access.
 * @param voltage Voltage read from the ADC.
 * @return ADC error.
 */
typedef void adc_driver_read_f(const adc_hardware_t* hardware, float* voltage);

/**
 * @brief ADC driver type.
 */
typedef struct _adc_driver_t_
{
    const adc_driver_set_enabled_f* setEnabled;
    const adc_driver_config_f* configure;
    const adc_driver_read_f* read;
} adc_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the ADC module.
 */
extern void adcInit(void);

/**
 * @brief Sets the driver for the ADC interface.
 * @param adcDriver ADC driver.
 * @return ADC error.
 */
extern adc_error_e adcSetDriver(const adc_driver_t* adcDriver);

/**
 * @brief Sets the given ADC enabled or disabled.
 * @param adc ADC to access.
 * @param enabled Flag that indicates if the ADC should be enabled or disabled.
 * @return ADC error.
 */
extern adc_error_e adcSetEnabled(const adc_e adc, const bool enabled);

/**
 * @brief Checks to see if the given ADC is enabled or disabled.
 * @param adc ADC to access.
 * @param isEnabled Flag that indicates if the ADC is enabled or disabled.
 * @return ADC error.
 */
extern adc_error_e adcIsEnabled(const adc_e adc, bool* isEnabled);

/**
 * @brief Configures the given ADC.
 * @param adc ADC to access.
 * @param config ADC configuration.
 * @return ADC error.
 */
extern adc_error_e adcConfigure(const adc_e adc, const adc_config_t* config);

/**
 * @brief Reads a value from the given ADC.
 * @param adc ADC to access.
 * @param voltage Voltage read from the ADC.
 * @return ADC error.
 */
extern adc_error_e adcRead(const adc_e adc, float* voltage);

#endif // _ADC_INTERFACE_H_
