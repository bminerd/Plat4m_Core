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
static setting_t settingMap[SETTING_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void settingInit(void)
{
    int i;

    for (i = 0; i < SETTING_COUNT; i++)
    {
        settingMap[i].setting       = SETTING_NONE;
        settingMap[i].valueType     = SETTING_VALUE_TYPE_NONE;
        settingMap[i].isReadOnly    = false;
        settingMap[i].getValue      = NULL_POINTER;
        settingMap[i].setValue      = NULL_POINTER;
    }

    settingApplicationInit();
}

//------------------------------------------------------------------------------
extern setting_error_e settingAddSetting(const setting_t* setting)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(setting)            ||
        IS_NULL_POINTER(setting->getValue)  ||
        (!setting->isReadOnly && IS_NULL_POINTER(setting->setValue)))
    {
        return SETTING_ERROR_PARAMETER_INVALID;
    }

    if (setting->setting >= SETTING_COUNT)
    {
        return SETTING_ERROR_INVALID;
    }

    if (setting->valueType == SETTING_VALUE_TYPE_NONE)
    {
        return SETTING_ERROR_VALUE_TYPE_INVALID;
    }

#endif // PLAT4M_DEBUG

    settingMap[setting->setting] = *setting;

    return SETTING_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern setting_error_e settingAddSettings(const setting_t settings[],
                                          const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > SETTING_COUNT)
    {
        return SETTING_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        setting_error_e error = settingAddSetting(&(settings[i]));

#ifdef PLAT4M_DEBUG

        if (error != SETTING_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return SETTING_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern setting_error_e settingValueGet(const setting_e setting,
                                       setting_value_t* value)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(value))
    {
        return SETTING_ERROR_PARAMETER_INVALID;
    }

    if (setting >= SETTING_COUNT)
    {
        return SETTING_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    return settingMap[setting].getValue(value);
}

//------------------------------------------------------------------------------
extern setting_error_e settingValueSet(const setting_e setting,
                                       const setting_value_t* value)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(value))
    {
        return SETTING_ERROR_PARAMETER_INVALID;
    }

    if (setting >= SETTING_COUNT)
    {
        return SETTING_ERROR_INVALID;
    }

#endif // PLAT4M_DEBUG

    return settingMap[setting].setValue(value);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
