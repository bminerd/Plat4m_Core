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
 * @file buffer_interface.c
 * @author Ben Minerd
 * @date 8/2/12
 * @brief Buffer interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <buffer_interface.h>

#include <stdlib.h>

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

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
bool bufferInitBuffer(buffer_t* buffer,
                      generic_data_t* bufferMemory,
                      const unsigned int bufferMemorySize,
                      const unsigned int elementSize)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(buffer)         ||
        IS_NULL_POINTER(bufferMemory)   ||
        (bufferMemorySize == 0)         ||
        (elementSize == 0))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    buffer->dataArray.data.bytes    = bufferMemory;
    buffer->dataArray.data.size     = bufferMemorySize;
    buffer->dataArray.elementSize   = elementSize;
    buffer->writeIndex              = 0;
    buffer->readIndex               = 0;
    buffer->count                   = 0;

    return true;
}

//------------------------------------------------------------------------------
bool bufferWrite(buffer_t* buffer, const generic_data_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(buffer) || IS_NULL_POINTER(data))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    bool isFull = true;

    bufferIsFull(buffer, &isFull);

    if (isFull)
    {
        return false;
    }
    
    memcpy((uint8_t*) &buffer->dataArray.data.bytes[buffer->writeIndex++ *
                                                    buffer->dataArray.elementSize],
           data,
           buffer->dataArray.elementSize);
    
    buffer->count++;

    if (buffer->writeIndex >= buffer->dataArray.data.size)
    {
        buffer->writeIndex = 0;
    }
    
    return true;
}

//------------------------------------------------------------------------------
bool bufferRead(buffer_t* buffer, generic_data_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(buffer) || IS_NULL_POINTER(data))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    bool isEmpty = true;

    bufferIsEmpty(buffer, &isEmpty);

    if (isEmpty)
    {
        return false;
    }
    
    memcpy(data,
           (uint8_t*) &buffer->dataArray.data.bytes[buffer->readIndex++ *
                                                    buffer->dataArray.elementSize],
           buffer->dataArray.elementSize);
    
    buffer->count--;

    if (buffer->readIndex >= buffer->dataArray.data.size)
    {
        buffer->readIndex = 0;
    }
    
    return true;
}

//------------------------------------------------------------------------------
bool bufferPeek(const buffer_t* buffer, generic_data_t** data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(buffer) || IS_NULL_POINTER(data))
    {
        *data = 0;

        return false;
    }

#endif // PLAT4M_DEBUG

    bool isEmpty = true;

    bufferIsEmpty(buffer, &isEmpty);

    if (isEmpty)
    {
        return false;
    }

    *data = &(buffer->dataArray.data.bytes[buffer->readIndex *
                                           buffer->dataArray.elementSize]);

    return true;
}

//------------------------------------------------------------------------------
bool bufferIsEmpty(const buffer_t* buffer, bool* isEmpty)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(buffer) || IS_NULL_POINTER(isEmpty))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    *isEmpty = (buffer->count == 0);

    return true;
}

//------------------------------------------------------------------------------
bool bufferIsFull(const buffer_t* buffer, bool* isFull)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(buffer) || IS_NULL_POINTER(isFull))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    *isFull = (buffer->count >= buffer->dataArray.data.size);

    return true;
}

//------------------------------------------------------------------------------
bool bufferCount(const buffer_t* buffer, int* count)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(buffer) || IS_NULL_POINTER(count))
    {
        return false;
    }
    
#endif // PLAT4M_DEBUG

    *count = buffer->count;
    
    return true;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
