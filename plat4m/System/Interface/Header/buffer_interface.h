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
 * @file buffer_interface.h
 * @author Ben Minerd
 * @date 3/18/12
 * @brief TODO Comment!
 */

#ifndef _BUFFER_INTERFACE_H_
#define _BUFFER_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define BUFFER_SIZE (255u)

/**
 * TODO Comment!
 */
#define BUFFER_INIT(buffer, bufferMemory) \
    bufferInit(buffer, \
               (generic_data_t*) bufferMemory, \
               ARRAY_SIZE(bufferMemory), \
               sizeof(bufferMemory[0]))

/**
 * TODO Comment!
 */
#define BUFFER_WRITE(buffer, elementPtr) \
    bufferWrite(buffer, (generic_data_t*) elementPtr)

/**
 * TODO Comment!
 */
#define BUFFER_READ(buffer, elementPtr) \
    bufferRead(buffer, (generic_data_t*) elementPtr)

/**
 * TODO Comment!
 */
#define BUFFER_PEEK(buffer, elementPtr) \
    bufferPeek(buffer, (generic_data_t**) elementPtr)

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef uint8_t generic_data_t;

/**
 * TODO Comment!
 */
typedef struct _buffer_t_
{
    data_array_t dataArray;
    unsigned int writeIndex;
    unsigned int readIndex;
} buffer_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern bool bufferInit(buffer_t* buffer,
                       generic_data_t* bufferMemory,
                       unsigned int bufferMemorySize,
                       unsigned int wordSize);

/**
 * TODO Comment!
 */
extern bool bufferWrite(buffer_t* buffer, generic_data_t* data);

/**
 * TODO Comment!
 */
extern bool bufferRead(buffer_t* buffer, generic_data_t* data);

/**
 * TODO Comment!
 */
extern bool bufferPeek(buffer_t* buffer, generic_data_t** data);

/**
 * TODO Comment!
 */
extern bool bufferIsEmpty(buffer_t* buffer);

/**
 * TODO Comment!
 */
extern bool bufferIsFull(buffer_t* buffer);

/**
 * TODO Comment!
 */
extern bool bufferCount(buffer_t* buffer, uint8_t* count);

#endif // _BUFFER_INTERFACE_H_
