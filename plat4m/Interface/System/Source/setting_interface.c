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
 * @file setting_interface.c
 * @author Ben Minerd
 * @date 9/6/2012
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <setting_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static setting_driver_t drivers[SETTING_ID_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static bool settingAddDriver(setting_driver_t* driver);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void settingInit(void)
{
    int i;

    for (i = 0; i < SETTING_ID_COUNT; i++)
    {
        drivers[i].id           = (setting_id_e) i;
        drivers[i].type         = SETTING_VALUE_TYPE_UINT8;
        drivers[i].isReadOnly   = false;
        drivers[i].getValue     = NULL_POINTER;
        drivers[i].setValue     = NULL_POINTER;
    }

    settingDriverInit();
}

//------------------------------------------------------------------------------
extern bool settingAddDrivers(setting_driver_t drivers[], int size)
{
    if (size > SETTING_ID_COUNT)
    {
        return false;
    }

    int i;

    for (i = 0; i < size; i++)
    {
        if (!settingAddDriver(&(drivers[i])))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern setting_error_e settingValueGet(setting_id_e id,
                                       setting_value_t* value)
{
    if (id >= SETTING_ID_COUNT)
    {
        return SETTING_ERROR_INVALID_ID;
    }

    if (!drivers[id].getValue(value))
    {
        // Return error
    }

    return SETTING_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern setting_error_e settingValueSet(setting_id_e id,
                                       setting_value_t value)
{
    if (id >= SETTING_ID_COUNT)
    {
        return SETTING_ERROR_INVALID_ID;
    }

    if (drivers[id].isReadOnly)
    {
        return SETTING_ERROR_READ_ONLY;
    }

    if (!drivers[id].setValue(value))
    {
        // Return error
    }

    return SETTING_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool settingAddDriver(setting_driver_t* driver)
{
    if ((driver->id >= SETTING_ID_COUNT) ||
        IS_NULL_POINTER(driver->getValue))
    {
        return false;
    }

    ADD_DRIVER(drivers, driver);

    return true;
}
