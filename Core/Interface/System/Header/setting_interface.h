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
 * @file setting_interface.h
 * @author Ben Minerd
 * @date 9/6/2012
 * @brief Setting interface layer.
 */

#ifndef _SETTING_INTERFACE_H_
#define _SETTING_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <setting_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of setting value types.
 */
typedef enum _setting_value_type_e_
{
    // Do not place values above!
    SETTING_VALUE_TYPE_NONE = -1,

    SETTING_VALUE_TYPE_UINT,
    SETTING_VALUE_TYPE_UINT8,
    SETTING_VALUE_TYPE_UINT16,
    SETTING_VALUE_TYPE_UINT32,
    SETTING_VALUE_TYPE_INT,
    SETTING_VALIE_TYPE_INT8,
    SETTING_VALUE_TYPE_INT16,
    SETTING_VALUE_TYPE_INT32,
    SETTING_VALUE_TYPE_FLOAT32,
    SETTING_VALUE_TYPE_STRING
} setting_value_type_e;

/**
 * @brief Enumeration of setting errors.
 */
typedef enum _setting_error_e_
{
    SETTING_ERROR_NONE,
    SETTING_ERROR_PARAMETER_INVALID,
    SETTING_ERROR_INVALID,
    SETTING_ERROR_VALUE_TYPE_INVALID,
    SETTING_ERROR_READ_ONLY
} setting_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Setting value type.
 */
typedef union _setting_value_t_
{
    unsigned int uint;
    uint8_t uint8;
    uint16_t uint16;
    uint32_t uint32;
    int intSigned;
    int8_t int8;
    int16_t int16;
    int32_t int32;
    float float32;
    char* str;
} setting_value_t;

/**
 * @brief Sets a setting value.
 * @param value Setting value.
 * @return Setting error.
 */
typedef setting_error_e setting_value_get_f(setting_value_t* value);

/**
 * @brief Gets a setting value.
 * @param value Setting value.
 * @return Setting error.
 */
typedef setting_error_e setting_value_set_f(const setting_value_t* value);

/**
 * @brief Setting type.
 */
typedef struct _setting_t_
{
    setting_id_e id;
    setting_value_type_e valueType;
    bool isReadOnly;
    setting_value_get_f* getValue;
    setting_value_set_f* setValue;
} setting_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the setting module.
 */
extern void settingInit(void);

/**
 * @brief Adds the given setting.
 */
extern setting_error_e settingAddSetting(const setting_t* setting);

/**
 * @brief Adds the given setting.
 */
extern setting_error_e settingAddSettings(const setting_t settings[],
                                          const unsigned int size);

/**
 * @brief Gets the value of the given setting.
 * @param id Setting to access.
 * @param value Value that was retrieved.
 * @return Setting error.
 */
extern setting_error_e settingValueGet(const setting_id_e id,
                                       setting_value_t* value);

/**
 * @brief Sets the value of the given setting.
 * @param id Setting to access.
 * @param value Value to set.
 * @return Setting error.
 */
extern setting_error_e settingValueSet(const setting_id_e id,
                                       const setting_value_t* value);

#endif // _SETTING_INTERFACE_H_
