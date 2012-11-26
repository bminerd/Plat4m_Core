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
 * @file io_expander_interface.c
 * @author Ben Minerd
 * @date 10/11/2012
 * @brief IO expander interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <io_expander_interface.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief IO expander wrapper type.
 */
typedef struct _io_expander_wrapper_t_
{
    io_expander_id_t id;
    bool isEnabled;
    mutex_t mutex;
} io_expander_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief IO expander driver.
 */
static io_expander_driver_t idToDriverMap[IO_EXPANDER_DRIVER_ID_COUNT];

/**
 * @brief IO expander map.
 */
static io_expander_wrapper_t idToExpanderMap[IO_EXPANDER_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void ioExpanderInit(void)
{
    int i;

    for (i = 0; i < IO_EXPANDER_DRIVER_ID_COUNT; i++)
    {
        idToDriverMap[i].id             = IO_EXPANDER_DRIVER_ID_NONE;
        idToDriverMap[i].setEnabled     = NULL_POINTER;
        idToDriverMap[i].configure      = NULL_POINTER;
        idToDriverMap[i].ioConfigure    = NULL_POINTER;
        idToDriverMap[i].ioSetLevel     = NULL_POINTER;
        idToDriverMap[i].ioGetLevel     = NULL_POINTER;
    }

    for (i = 0; i < IO_EXPANDER_ID_COUNT; i++)
    {
        idToExpanderMap[i].id.id        = IO_EXPANDER_ID_NONE;
        idToExpanderMap[i].id.driverId  = IO_EXPANDER_DRIVER_ID_NONE;
        idToExpanderMap[i].id.expander  = NULL_POINTER;
        // Remaining io_expander_t fields are driver specific

        idToExpanderMap[i].isEnabled    = false;

        mutexInitMutex(&(idToExpanderMap[i].mutex));
    }

    ioExpanderApplicationInit();
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderAddDriver(
                                const io_expander_driver_t* ioExpanderDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(ioExpanderDriver))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToDriverMap[ioExpanderDriver->id] = *ioExpanderDriver;

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderAddDrivers(const io_expander_driver_t drivers[],
                                         const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > IO_EXPANDER_DRIVER_ID_COUNT)
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        io_expander_error_e error = ioExpanderAddDriver(&(drivers[i]));

#ifdef PLAT4M_DEBUG

        if (error != IO_EXPANDER_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderAddId(const io_expander_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if ((id->id == IO_EXPANDER_ID_NONE) || (id->id >= IO_EXPANDER_ID_COUNT))
    {
        return IO_EXPANDER_ERROR_ID_INVALID;
    }

    // Remaining expander fields are driver specific

#endif // PLAT4M_DEBUG

    idToExpanderMap[id->id].id = *id;

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderAddIds(const io_expander_id_t ids[],
                                     const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > IO_EXPANDER_ID_COUNT)
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        io_expander_error_e error = ioExpanderAddId(&(ids[i]));

#ifdef PLAT4M_DEBUG

        if (error != IO_EXPANDER_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderSetEnabled(const io_expander_id_e id,
                                         const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == IO_EXPANDER_ID_NONE) || (id >= IO_EXPANDER_ID_COUNT))
    {
        return IO_EXPANDER_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    io_expander_driver_id_e driverId = idToExpanderMap[id].id.driverId;

    // Lock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), true);

    io_expander_error_e error =
        idToDriverMap[driverId].setEnabled(&(idToExpanderMap[id].id), enabled);

    if (error == IO_EXPANDER_ERROR_NONE)
    {
        idToExpanderMap[id].isEnabled = enabled;
    }

    // Unlock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderIsEnabled(const io_expander_id_e id,
                                        bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(enabled))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if ((id == IO_EXPANDER_ID_NONE) || (id >= IO_EXPANDER_ID_COUNT))
    {
        return IO_EXPANDER_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), true);

    *enabled = idToExpanderMap[id].isEnabled;

    // Unlock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), false);

    return IO_EXPANDER_ERROR_NONE;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderConfigure(const io_expander_id_e id,
                                        const io_expander_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(config))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if ((id == IO_EXPANDER_ID_NONE) || (id >= IO_EXPANDER_ID_COUNT))
    {
        return IO_EXPANDER_ERROR_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    io_expander_driver_id_e driverId = idToExpanderMap[id].id.driverId;

    // Lock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), true);

    io_expander_error_e error =
        idToDriverMap[driverId].configure(&(idToExpanderMap[id].id), config);

    // Unlock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderIoConfigure(
                                const io_expander_id_e id,
                                const io_expander_io_hardware_t* hardware,
                                const io_expander_io_hardware_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware) || IS_NULL_POINTER(config))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if ((id == IO_EXPANDER_ID_NONE) || (id >= IO_EXPANDER_ID_COUNT))
    {
        return IO_EXPANDER_ERROR_ID_INVALID;
    }

    if (hardware->port == IO_EXPANDER_IO_PORT_NONE)
    {
        return IO_EXPANDER_ERROR_IO_PORT_INVALID;
    }

    if (hardware->pin == IO_EXPANDER_IO_PIN_NONE)
    {
        return IO_EXPANDER_ERROR_IO_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    io_expander_driver_id_e driverId = idToExpanderMap[id].id.driverId;

    // Lock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), true);

    io_expander_error_e error =
        idToDriverMap[driverId].ioConfigure(idToExpanderMap[id].id.expander,
                                            hardware,
                                            config);

    // Unlock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderIoSetLevel(
                                    const io_expander_id_e id,
                                    const io_expander_io_hardware_t* hardware,
                                    const gpio_level_e level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if ((id == IO_EXPANDER_ID_NONE) || (id >= IO_EXPANDER_ID_COUNT))
    {
        return IO_EXPANDER_ERROR_ID_INVALID;
    }

    if (hardware->port == IO_EXPANDER_IO_PORT_NONE)
    {
        return IO_EXPANDER_ERROR_IO_PORT_INVALID;
    }

    if (hardware->pin == IO_EXPANDER_IO_PIN_NONE)
    {
        return IO_EXPANDER_ERROR_IO_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    io_expander_driver_id_e driverId = idToExpanderMap[id].id.driverId;

    // Lock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), true);

    io_expander_error_e error =
        idToDriverMap[driverId].ioSetLevel(idToExpanderMap[id].id.expander,
                                           hardware,
                                           level);

    // Unlock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), false);

    return error;
}

//------------------------------------------------------------------------------
io_expander_error_e ioExpanderIoGetLevel(
                                    const io_expander_id_e id,
                                    const io_expander_io_hardware_t* hardware,
                                    gpio_level_e* level)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(hardware) || IS_NULL_POINTER(level))
    {
        return IO_EXPANDER_ERROR_PARAMETER_INVALID;
    }

    if ((id == IO_EXPANDER_ID_NONE) || (id >= IO_EXPANDER_ID_COUNT))
    {
        return IO_EXPANDER_ERROR_ID_INVALID;
    }

    if (hardware->port == IO_EXPANDER_IO_PORT_NONE)
    {
        return IO_EXPANDER_ERROR_IO_PORT_INVALID;
    }

    if (hardware->pin == IO_EXPANDER_IO_PIN_NONE)
    {
        return IO_EXPANDER_ERROR_IO_PIN_INVALID;
    }

#endif // PLAT4M_DEBUG

    io_expander_driver_id_e driverId = idToExpanderMap[id].id.driverId;

    // Lock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), true);

    io_expander_error_e error =
        idToDriverMap[driverId].ioGetLevel(idToExpanderMap[id].id.expander,
                                           hardware,
                                           level);

    // Unlock mutex
    mutexSetLocked(&(idToExpanderMap[id].mutex), false);

    return error;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

