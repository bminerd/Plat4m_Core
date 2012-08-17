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

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

/**
 * TODO Comment!
 */
#define DRIVER_MAP_SIZE(driverMap) ARRAY_SIZE(driverMap.map)

/**
 * TODO Comment!
 */
#define BYTE_ARRAY(array) {(uint8_t*) array, ARRAY_SIZE(array)}

/**
 * TODO Comment!
 */
#define MEMORY_COPY(destPtr, srcPtr) \
    memcpy((uint8_t*) destPtr, (uint8_t*) srcPtr, sizeof(*srcPtr))

/**
 * TODO Comment!
 */
#define ADD_DRIVER(array, driverPtr) \
    MEMORY_COPY(&array[driverPtr->id], driverPtr)

/**
 * TODO Comment!
 */
#define ADD_CONTAINER(array, driverPtr) \
    MEMORY_COPY(&(array[driverPtr->id].driver), driverPtr)

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

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
 * TODO Comment!
 */
typedef struct _byte_array_t_
{
    uint8_t* bytes;
    unsigned int size;
} byte_array_t;

/**
 * TODO Comment!
 */
typedef struct _data_array_t_
{
    byte_array_t data;
    unsigned int wordSize;
} data_array_t;

#endif // _SYSTEM_TYPES_H_
