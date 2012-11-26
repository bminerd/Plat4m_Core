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
 * @brief Buffer interface layer.
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
 * @brief Wraps bufferInitBuffer() and provides the necessary parameters based
 * on the given buffer and buffer memory.
 * @param buffer Buffer to access.
 * @param bufferMemory Buffer memory to initialize.
 */
#define BUFFER_INIT(buffer, bufferMemory) \
    bufferInitBuffer(buffer, \
                     (generic_data_t*) bufferMemory, \
                     ARRAY_SIZE(bufferMemory), \
                     sizeof(bufferMemory[0]))

/**
 * @brief Wraps bufferWrite() and casts the given element pointer as the generic
 * data type.
 * @param buffer Buffer to access..
 * @param elementPointer Pointer to element.
 */
#define BUFFER_WRITE(buffer, elementPointer) \
    bufferWrite(buffer, (generic_data_t*) elementPointer)

/**
 * @brief Wraps bufferRead() and casts the given element pointer as the generic
 * data type.
 * @param buffer Buffer to access.
 * @param elementPointer Pointer to element.
 */
#define BUFFER_READ(buffer, elementPointer) \
    bufferRead(buffer, (generic_data_t*) elementPointer)

/**
 * @brief Wraps bufferPeek() and casts the given element pointer as the generic
 * data type.
 * @param buffer Buffer to access.
 * @param elementPointer Pointer to element.
 */
#define BUFFER_PEEK(buffer, elementPointer) \
    bufferPeek(buffer, (generic_data_t**) elementPointer)

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Buffer type.
 */
typedef struct _buffer_t_
{
    data_array_t dataArray;
    unsigned int writeIndex;
    unsigned int readIndex;
    unsigned int count;
} buffer_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the buffer module
 */
void bufferInit(void);

/**
 * @brief Initializes the given buffer.
 * @param buffer Buffer to initialize.
 * @param bufferMemory Buffer memory to initialize.
 * @param bufferMemorySize Size of buffer memory.
 * @param elementSize Size of buffer elements in bytes.
 * @return True if the function was successful, false if not.
 */
bool bufferInitBuffer(buffer_t* buffer,
                      generic_data_t* bufferMemory,
                      const unsigned int bufferMemorySize,
                      const unsigned int elementSize);

/**
 * @brief Writes the given data to the given buffer.
 * @param buffer Buffer to access.
 * @param data Data to write to buffer.
 * @return True if the function was successful, false if not.
 */
bool bufferWrite(buffer_t* buffer, const generic_data_t* data);

/**
 * @brief Reads data from the given buffer.
 * @param buffer Buffer to access.
 * @param data Data read from buffer.
 * @return True if the function was successful, false if not.
 */
bool bufferRead(buffer_t* buffer, generic_data_t* data);

/**
 * @brief Returns a copy of the first element in the buffer.
 * @param buffer Buffer to access.
 * @param data Pointer to first element.
 * @return True if the function was successful, false if not.
 */
bool bufferPeek(const buffer_t* buffer, generic_data_t** data);

/**
 * @brief Checks to see if the given queue is empty.
 * @param buffer Buffer to access.
 * @param isEmpty Flag that indicates if the buffer is empty.
 * @return True if the function was successful, false if not.
 */
bool bufferIsEmpty(const buffer_t* buffer, bool* isEmpty);

/**
 * @brief Checks to see if the given buffer is full.
 * @param buffer Buffer to access.
 * @param isFull Flag that indicates if the buffer is full.
 * @return True if the function was successful, false if not.
 */
bool bufferIsFull(const buffer_t* buffer, bool* isFull);

/**
 * @brief Gets the number of elements currently in the given buffer.
 * @param buffer Buffer to access.
 * @param count Number of elements in the buffer.
 * @return True if the function was successful, false if not.
 */
bool bufferCount(const buffer_t* buffer, int* count);

#endif // _BUFFER_INTERFACE_H_
