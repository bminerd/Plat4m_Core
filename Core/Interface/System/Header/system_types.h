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
 * @brief plat4m system types.
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
 * @brief Defines a more verbose null pointer.
 */
#define NULL_POINTER NULL

/**
 * @brief Checks to see if the given pointer is null.
 * @param pointer Pointer to check if null.
 */
#define IS_NULL_POINTER(pointer) !pointer

/**
 * @brief Returns the number of elements of any given array.
 * @param array Array to get number of elements.
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define ARRAY_SIZE_2D(array) (sizeof(array) / sizeof(array[0][0]))

/**
 * @brief Initializes a byte array type with the given byte array.
 * @param array Array of bytes to put into the byte array type.
 */
#define BYTE_ARRAY(array) { .bytes = array, .size = ARRAY_SIZE(array) }

/**
 * @brief Initializes a byte array type with the given string.
 * @param string String to put into the byte array type.
 */
#define BYTE_ARRAY_STRING(string) { .bytes = string, .size = strlen(string) }

/**
 * @brief Initializes a data array type with the given array of bytes.
 * @param array Array of bytes to put into the data array type.
 */
#define DATA_ARRAY(array) \
    { .data = BYTE_ARRAY(array), .elementSize = sizeof(array[0])}

/*------------------------------------------------------------------------------
 * Enums
 *----------------------------------------------------------------------------*/

typedef enum _bit_value_e_
{
    BIT_VALUE_0 = 0,
    BIT_VALUE_1
} bit_value_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

typedef int id_t;

typedef uint32_t time_ms_t;

/**
 * @brief Generic data type.
 */
typedef uint8_t generic_data_t;

/**
 * @brief
 */
//typedef int private_data_t;
typedef uint8_t private_data_t;

/**
 * @brief Function type for a procedure.
 */
typedef void procedure_f(void);

/**
 * @brief Function type that sets a value enabled or disabled.
 * @param enabled Flag that indicates if the value should be enabled or
 * disabled.
 */
typedef void set_enabled_f(bool enabled);

/**
 * @brief Function type for a data callback.
 * @param data Byte to use.
 */
typedef void data_callback_f(const uint8_t data);

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
    unsigned int elementSize;
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
