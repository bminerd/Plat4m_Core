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
 * @file io_expander_interface.h
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief IO expander interface layer.
 */

#ifndef _IO_EXPANDER_INTERFACE_H_
#define _IO_EXPANDER_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <io_expander_application.h>

#include <gpio_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define IO_EXPANDER_IO_PORT_NONE    -1

#define IO_EXPANDER_IO_PIN_NONE     -1

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of IO expander errors.
 */
typedef enum _io_expander_error_e_
{
    IO_EXPANDER_ERROR_NONE,
    IO_EXPANDER_ERROR_PARAMETER_INVALID,
    IO_EXPANDER_ERROR_ID_INVALID,
    IO_EXPANDER_ERROR_NOT_ENABLED,
    IO_EXPANDER_ERROR_IO_PORT_INVALID,
    IO_EXPANDER_ERROR_IO_PIN_INVALID,
    IO_EXPANDER_ERROR_COMMUNICATION
} io_expander_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief IO expander type.
 */
typedef void io_expander_t;

/**
 * @brief IO expander IO port type.
 */
typedef int io_expander_io_port_t;

/**
 * @brief IO expander IO pin type.
 */
typedef int io_expander_io_pin_t;

/**
 * @brief IO expander configuration type.
 */
typedef void io_expander_config_t;

/**
 * @brief IO expander ID type.
 */
typedef struct _io_expander_id_t_
{
    io_expander_id_e id;
    io_expander_driver_id_e driverId;
    io_expander_t* expander;
} io_expander_id_t;

/**
 * @brief IO expander IO hardware type.
 */
typedef struct _io_expander_io_hardware_t_
{
    io_expander_io_port_t port;
    io_expander_io_pin_t pin;
} io_expander_io_hardware_t;

/**
 * @brief IO expander IO hardware configuration type.
 */
typedef struct _io_expander_io_hardware_config_t_
{
    gpio_mode_e mode;
    gpio_resistor_e resistor;
} io_expander_io_hardware_config_t;

/**
 * @brief Function type that sets the IO expander enabled or disabled.
 * @param expander IO expander to access.
 * @param enabled Flag that indicates if the IO expander should be enabled or
 * disabled.
 * @return IO expander error.
 */
typedef io_expander_error_e io_expander_driver_set_enabled_f(
                                                        io_expander_t* expander,
                                                        const bool enabled);

/**
 * @brief Function type that configures the IO expander.
 * @param expander IO expander to access.
 * @param config IO expander configuration.
 * @return IO expander error.
 */
typedef io_expander_error_e io_expander_driver_configure_f(
                                            io_expander_t* expander,
                                            const io_expander_config_t* config);

/**
 * @brief Function type that configures the given IO expander IO hardware.
 * @param expander IO expander to access.
 * @param hardware IO expander IO hardware to access.
 * @param config IO expander IO hardware configuration.
 * @return IO expander error.
 */
typedef io_expander_error_e io_expander_driver_io_configure_f(
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
typedef io_expander_error_e io_expander_driver_io_set_level_f(
                                    io_expander_t* expander,
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
typedef io_expander_error_e io_expander_driver_io_get_level_f(
                                    io_expander_t* expander,
                                    const io_expander_io_hardware_t* hardware,
                                    gpio_level_e* level);

/**
 * @brief IO expander driver type.
 */
typedef struct _io_expander_driver_t_
{
    io_expander_driver_id_e id;
    io_expander_driver_set_enabled_f* setEnabled;
    io_expander_driver_configure_f* configure;
    io_expander_driver_io_configure_f* ioConfigure;
    io_expander_driver_io_set_level_f* ioSetLevel;
    io_expander_driver_io_get_level_f* ioGetLevel;
} io_expander_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the IO expander module.
 */
void ioExpanderInit(void);

/**
 * @brief Adds the given IO expander driver.
 * @param driver IO expander driver to add.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderAddDriver(
                                const io_expander_driver_t* driver);

/**
 * @brief Adds the given IO expander drivers.
 * @param drivers IO expander drivers to add.
 * @param size Number of IO expander drivers to add.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderAddDrivers(const io_expander_driver_t drivers[],
                                         const unsigned int size);

/**
 * @brief Adds the given IO expander.
 * @param id IO expander ID to add.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderAddId(const io_expander_id_t* id);

/**
 * @brief Adds the given IO expanders.
 * @param ids IO expander IDs to add.
 * @param size Number of IO expanders to add.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderAddIds(const io_expander_id_t ids[],
                                     const unsigned int size);

/**
 * @brief Sets the given IO expander enabled or disabled.
 * @param id IO expander to access.
 * @param enabled Flag that indicates if the IO expander should be enabled or
 * disabled.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderSetEnabled(const io_expander_id_e id,
                                         const bool enabled);

/**
 * @brief Checks to see if the given IO expander is enabled or disabled.
 * @param id IO expander to access.
 * @param enabled Flag that indicates if the IO expander is enabled or disabled.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderIsEnabled(const io_expander_id_e id,
                                        bool* enabled);

/**
 * @brief Configures the given IO expander.
 * @param id IO expander to access.
 * @param config IO expander configuration.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderConfigure(const io_expander_id_e id,
                                        const io_expander_config_t* config);

/**
 * @brief Configures the given IO expander IO hardware.
 * @param id IO expander to access.
 * @param hardware IO expander IO hardware to access.
 * @param config IO expander IO hardware configuration.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderIoConfigure(
                                const io_expander_id_e id,
                                const io_expander_io_hardware_t* hardware,
                                const io_expander_io_hardware_config_t* config);

/**
 * @brief Function type that sets the level for the given IO expander IO
 * hardware.
 * @param id IO expander to access.
 * @param hardware IO expander IO hardware to access.
 * @param level GPIO level to set.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderIoSetLevel(
                                    const io_expander_id_e id,
                                    const io_expander_io_hardware_t* hardware,
                                    const gpio_level_e level);

/**
 * @brief Function type that gets the level for the given IO expander IO
 * hardware.
 * @param id IO expander to access.
 * @param hardware IO expander IO hardware to access.
 * @param level Current GPIO level.
 * @return IO expander error.
 */
io_expander_error_e ioExpanderIoGetLevel(
                                    const io_expander_id_e id,
                                    const io_expander_io_hardware_t* hardware,
                                    gpio_level_e* level);

#endif // _IO_EXPANDER_INTERFACE_H_
