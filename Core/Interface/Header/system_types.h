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
 * @file system_types.h
 * @author Ben Minerd
 * @date 7/30/12
 * @brief TODO Comment!
 */

#ifndef _SYSTEM_TYPES_H_
#define _SYSTEM_TYPES_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define NULL_POINTER (NULL)

/**
 * TODO Comment!
 */
#define IS_NULL_POINTER(ptr) !ptr

/**
 * TODO Comment!
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

/**
 * TODO Comment!
 */
#define DRIVER_MAP_SIZE(driverMap) ARRAY_SIZE(driverMap.map)

/**
 * TODO Comment!
 */
#define BYTE_ARRAY(array) {array, ARRAY_SIZE(array)}

/**
 * TODO Comment!
 */
#define BYTE_ARRAY_STRING(str) {str, strlen(str)}

/**
 * TODO Comment!
 */
#define DATA_ARRAY(array) {BYTE_ARRAY(array), sizeof(array[0])}

/**
 * TODO Comment!
 */
#define MEMORY_COPY(destPtr, srcPtr) \
    memcpy((uint8_t*) destPtr, (uint8_t*) srcPtr, sizeof(*srcPtr))

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief ID type.
 */
typedef int id_t;

/**
 * TODO Comment!
 */
typedef void procedure_f(void);

/**
 * TODO Comment!
 */
typedef void set_enabled_f(bool enabled);

/**
 * TODO Comment!
 */
typedef void data_callback_f(uint8_t data);

/**
 * @brief Byte array type.
 */
typedef struct _byte_array_t_
{
    uint8_t* bytes;
    unsigned int size;
} byte_array_t;

/**
 * @brief Data array type.
 */
typedef struct _data_array_t_
{
    byte_array_t data;
    unsigned int wordSize;
} data_array_t;

/**
 * @brief Dimension data array type.
 */
typedef struct _dimension_data_array_t_
{
    data_array_t data;
    int width;
    int height;
} dimension_data_array_t;

#endif // _SYSTEM_TYPES_H_
