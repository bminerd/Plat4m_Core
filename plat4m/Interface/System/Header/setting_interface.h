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
 * @brief TODO Comment!
 */

#ifndef _SETTING_INTERFACE_H_
#define _SETTING_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <setting_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _setting_value_type_e_
{
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
 * TODO Comment!
 */
typedef enum _setting_error_e_
{
    SETTING_ERROR_NONE,
    SETTING_ERROR_INVALID_ID,
    SETTING_ERROR_READ_ONLY
} setting_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
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
 * TODO Comment!
 */
typedef bool setting_value_get_f(setting_value_t* value);

/**
 * TODO Comment!
 */
typedef bool setting_value_set_f(setting_value_t value);

/**
 * TODO Comment!
 */
typedef struct _setting_driver_t_
{
    setting_id_e id;
    setting_value_type_e type;
    bool isReadOnly;
    setting_value_get_f* getValue;
    setting_value_set_f* setValue;
} setting_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void settingInit(void);

/**
 * TODO Comment!
 */
extern bool settingAddDrivers(setting_driver_t drivers[], int size);

/**
 * TODO Comment!
 */
extern setting_error_e settingValueGet(setting_id_e id,
                                       setting_value_t* value);

/**
 * TODO Comment!
 */
extern setting_error_e settingValueSet(setting_id_e id,
                                       setting_value_t value);

#endif // _SETTING_INTERFACE_H_
