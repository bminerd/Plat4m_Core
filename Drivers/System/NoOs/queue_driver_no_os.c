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
 * @file queue_driver_no_os.c
 * @author Ben Minerd
 * @date 10/9/2012
 * @brief Queue driver layer for no OS.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <queue_interface.h>
#include <queue_driver.h>

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

/**
 * @brief Initializes the given queue.
 * @param queue Queue to access.
 * @param queueMemory Queue memory.
 * @param queueMemorySize Size of queue memory.
 * @param elementSize Size of queue elements in bytes.
 * @return True if the function was successful, false if not.
 */
static bool init(queue_t* queue,
                 generic_data_t* queueMemory,
                 const unsigned int queueMemorySize,
                 const unsigned int elementSize);

/**
 * @brief Writes the given data to the given queue.
 * @param queue Queue to access.
 * @param data Data to write to queue.
 * @return True if the function was successful, false if not.
 */
static bool write(queue_t* queue, const generic_data_t* data);

/**
 * @brief Reads data from the given queue.
 * @param queue Queue to access.
 * @param data Data read from queue.
 * @return True if the function was successful, false if not.
 */
static bool read(queue_t* queue, generic_data_t* data);

/**
 * @brief Checks to see if the given queue is empty.
 * @param queue Queue to access.
 * @param isEmpty Flag that indicates if the queue is empty.
 * @return True if the function was successful, false if not.
 */
static bool isEmpty(const queue_t* queue, bool* isEmpty);

/**
 * @brief Checks to see if the given queue is full.
 * @param queue Queue to access.
 * @param isFull Flag that indicates if the queue is full.
 * @return True if the function was successfil, false if not.
 */
static bool isFull(const queue_t* queue, bool* isFull);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void queueDriverInit(void)
{
    queue_driver_t driver =
    {
        .init       = init,
        .write      = write,
        .read       = read,
        .isEmpty    = isEmpty,
        .isFull     = isFull
    };

    queueSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool init(queue_t* queue,
                 generic_data_t* queueMemory,
                 unsigned int queueMemorySize,
                 unsigned int elementSize)
{
    return bufferInitBuffer(&(queue->buffer),
                            queueMemory,
                            queueMemorySize,
                            elementSize);
}

//------------------------------------------------------------------------------
static bool write(queue_t* queue, const generic_data_t* data)
{
    return bufferWrite(&(queue->buffer), data);
}

//------------------------------------------------------------------------------
static bool read(queue_t* queue, generic_data_t* data)
{
    return bufferRead(&(queue->buffer), data);
}

//------------------------------------------------------------------------------
static bool isEmpty(const queue_t* queue, bool* isEmpty)
{
    return bufferIsEmpty(&(queue->buffer), isEmpty);
}

//------------------------------------------------------------------------------
static bool isFull(const queue_t* queue, bool* isFull)
{
    return bufferIsFull(&(queue->buffer), isFull);
}
