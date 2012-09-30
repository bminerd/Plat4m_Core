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
 * @file setting_driver.c
 * @author Ben Minerd
 * @date 9/6/2012
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <setting_driver.h>
#include <setting_interface.h>
#include <system_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Remove!
 */
static setting_value_t gateMaxValue;

/**
 * TODO Remove!
 */
static setting_value_t gateMinValue;

/**
 * TODO Remove!
 */
static setting_value_t metersYardsValue;

/**
 * TODO Remove!
 */
static setting_value_t degsMilsValue;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static bool systemVersionGet(setting_value_t* value);

/**
 * TODO Comment!
 */
static bool gateMaxGet(setting_value_t* value);

/**
 * TODO Comment!
 */
static bool gateMaxSet(setting_value_t value);

/**
 * TODO Comment!
 */
static bool gateMinGet(setting_value_t* value);

/**
 * TODO Comment!
 */
static bool gateMinSet(setting_value_t value);

/**
 * TODO Comment!
 */
static bool metersYardsGet(setting_value_t* value);

/**
 * TODO Comment!
 */
static bool metersYardsSet(setting_value_t value);

/**
 * TODO Comment!
 */
static bool degsMilsGet(setting_value_t* value);

/**
 * TODO Comment!
 */
static bool degsMilsSet(setting_value_t value);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void settingDriverInit(void)
{
    setting_driver_t settings[] =
    {
        // System version
        {
            .id         = SETTING_ID_SYSTEM_VERSION,
            .type       = SETTING_VALUE_TYPE_STRING,
            .isReadOnly = true,
            .getValue   = systemVersionGet,
            .setValue   = NULL_POINTER
        },
        // Gate max
        {
            .id         = SETTING_ID_GATE_MAX,
            .type       = SETTING_VALUE_TYPE_UINT16,
            .isReadOnly = false,
            .getValue   = gateMaxGet,
            .setValue   = gateMaxSet,
        },
        // Gate min
        {
            .id         = SETTING_ID_GATE_MIN,
            .type       = SETTING_VALUE_TYPE_UINT16,
            .isReadOnly = false,
            .getValue   = gateMinGet,
            .setValue   = gateMinSet,
        },
        // Meters/yards
        {
            .id         = SETTING_ID_METERS_YARDS,
            .type       = SETTING_VALUE_TYPE_UINT8,
            .isReadOnly = false,
            .getValue   = metersYardsGet,
            .setValue   = metersYardsSet
        },
        // Degrees/miliradians
        {
            .id         = SETTING_ID_DEGREES_MILIRADIANS,
            .type       = SETTING_VALUE_TYPE_UINT8,
            .isReadOnly = false,
            .getValue   = degsMilsGet,
            .setValue   = degsMilsSet
        }
    };

    settingAddDrivers(settings, ARRAY_SIZE(settings));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool systemVersionGet(setting_value_t* value)
{
    value->str = SYSTEM_VERSION;

    return true;
}

//------------------------------------------------------------------------------
static bool gateMaxGet(setting_value_t* value)
{
    // For testing only!
    *value = gateMaxValue;

    return true;
}

//------------------------------------------------------------------------------
static bool gateMaxSet(setting_value_t value)
{
    gateMaxValue = value;

    return true;
}

//------------------------------------------------------------------------------
static bool gateMinGet(setting_value_t* value)
{
    // For testing only!
    *value = gateMinValue;

    return true;
}

//------------------------------------------------------------------------------
static bool gateMinSet(setting_value_t value)
{
    gateMinValue = value;

    return true;
}

//------------------------------------------------------------------------------
static bool metersYardsGet(setting_value_t* value)
{
    // For testing only!
    *value = metersYardsValue;

    return true;
}

//------------------------------------------------------------------------------
static bool metersYardsSet(setting_value_t value)
{
    // For testing only!
    metersYardsValue = value;

    return true;
}

//------------------------------------------------------------------------------
static bool degsMilsGet(setting_value_t* value)
{
    // For testing only!
    *value = degsMilsValue;

    return true;
}

//------------------------------------------------------------------------------
static bool degsMilsSet(setting_value_t value)
{
    degsMilsValue = value;

    return true;
}
