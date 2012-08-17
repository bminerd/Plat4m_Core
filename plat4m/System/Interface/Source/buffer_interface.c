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
 * @brief TODO Comment!
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
extern bool bufferInit(buffer_t* buffer,
                       generic_data_t* bufferMemory,
                       unsigned int bufferMemorySize,
                       unsigned int wordSize)
{
    if (!buffer                 ||
        !bufferMemory           ||
        (bufferMemorySize == 0) ||
        (wordSize == 0))
    {
        return false;
    }

    buffer->dataArray.data.bytes    = bufferMemory;
    buffer->dataArray.data.size     = bufferMemorySize;
    buffer->dataArray.wordSize      = wordSize;
    buffer->writeIndex              = 0;
    buffer->readIndex               = 0;

    return true;
}

//------------------------------------------------------------------------------
extern bool bufferWrite(buffer_t* buffer, generic_data_t* data)
{
    if (!buffer                 ||
        bufferIsFull(buffer)    ||
        !data                   ||
        (buffer->dataArray.wordSize == 0))
    {
        return false;
    }
    
    memcpy((uint8_t*) &buffer->dataArray.data.bytes[buffer->writeIndex++ *
                                                    buffer->dataArray.wordSize],
           data,
           buffer->dataArray.wordSize);
    
    if (buffer->writeIndex >= buffer->dataArray.data.size)
    {
        buffer->writeIndex = 0;
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern bool bufferRead(buffer_t* buffer, generic_data_t* data)
{
    if (!buffer                 ||
        bufferIsEmpty(buffer)   ||
        !data                   ||
        (buffer->dataArray.wordSize == 0))
    {
        return false;
    }
    
    memcpy(data,
           (uint8_t*) &buffer->dataArray.data.bytes[buffer->readIndex++ *
                                                    buffer->dataArray.wordSize],
           buffer->dataArray.wordSize);
    
    if (buffer->readIndex >= buffer->dataArray.data.size)
    {
        buffer->readIndex = 0;
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern bool bufferPeek(buffer_t* buffer, generic_data_t** data)
{
    if (!buffer                 ||
        bufferIsEmpty(buffer)   ||
        !data                   ||
        (buffer->dataArray.wordSize == 0))
    {
        data = 0;

        return false;
    }

    *data = &(buffer->dataArray.data.bytes[buffer->readIndex *
                                           buffer->dataArray.wordSize]);

    return true;
}

//------------------------------------------------------------------------------
extern bool bufferIsEmpty(buffer_t* buffer)
{
    return (buffer && (buffer->writeIndex == buffer->readIndex));
}

//------------------------------------------------------------------------------
extern bool bufferIsFull(buffer_t* buffer)
{
    return (buffer                                              &&
            (((buffer->writeIndex + 1) == buffer->readIndex)    ||
            ((buffer->writeIndex == BUFFER_SIZE) && (buffer->readIndex == 0))));
}

//------------------------------------------------------------------------------
extern bool bufferCount(buffer_t* buffer, uint8_t* count)
{
    if (!buffer)
    {
        return false;
    }
    
    *count = abs(buffer->writeIndex - buffer->readIndex);
    
    return true;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
