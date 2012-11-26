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
 * @file queue_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief Queue interface layer.
 */

#ifndef _QUEUE_INTERFACE_H_
#define _QUEUE_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

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

/**
 * @brief Function type that initializes the given queue.
 * @param queue Queue to access.
 * @param queueMemory Queue memory.
 * @param queueMemorySize Size of queue memory.
 * @param elementSize Size of queue elements in bytes.
 * @return True if the function was successful, false if not.
 */
typedef bool queue_driver_init_f(queue_t* queue,
                                 generic_data_t* queueMemory,
                                 const unsigned int queueMemorySize,
                                 const unsigned int elementSize);

/**
 * @brief Function type that writes the given data to the given queue.
 * @param queue Queue to access.
 * @param data Data to write to queue.
 * @return True if the function was successful, false if not.
 */
typedef bool queue_driver_write_f(queue_t* queue, const generic_data_t* data);

/**
 * @brief Function type that reads data from to the given queue.
 * @param queue Queue to access.
 * @param data Data read from queue.
 * @return True if the function was successful, false if not.
 */
typedef bool queue_driver_read_f(queue_t* queue, generic_data_t* data);

/**
 * @brief Function type that checks to see if the given queue is empty.
 * @param queue Queue to access.
 * @param isEmpty Flag that indicates if the queue is empty.
 * @return True if the function was successful, false if not.
 */
typedef bool queue_driver_is_empty_f(const queue_t* queue, bool* isEmpty);

/**
 * @brief Checks to see if the given queue is full.
 * @param queue Queue to access.
 * @param isFull Flag that indicates if the queue is full.
 * @return True if the function was successful, false if not.
 */
typedef bool queue_driver_is_full_f(const queue_t* queue, bool* isFull);

/**
 * @brief Queue driver type.
 */
typedef struct _queue_driver_t_
{
    queue_driver_init_f* init;
    queue_driver_write_f* write;
    queue_driver_read_f* read;
    queue_driver_is_empty_f* isEmpty;
    queue_driver_is_full_f* isFull;
} queue_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the Queue module.
 */
void queueInit(void);

/**
 * @brief Sets the driver for the queue interface.
 * @param queueDriver Queue driver.
 * @return True if the function was successful, false if not.
 */
bool queueSetDriver(const queue_driver_t* queueDriver);

/**
 * @brief Initializes the given queue.
 * @param queue Queue to access.
 * @param queueMemory Queue memory.
 * @param queueMemorySize Size of queue memory.
 * @param elementSize Size of queue elements in bytes.
 * @return True if the function was successful, false if not.
 */
bool queueInitQueue(queue_t* queue,
                    generic_data_t* queueMemory,
                    const unsigned int queueMemorySize,
                    const unsigned int elementSize);

/**
 * @brief Writes the given data to the given queue.
 * @param queue Queue to access.
 * @param data Data to write to queue.
 * @return True if the function was successful, false if not.
 */
bool queueWrite(queue_t* queue, const generic_data_t* data);

/**
 * @brief Reads data from the given queue.
 * @param queue Queue to access.
 * @param data Data read from queue.
 * @return True if the function was successful, false if not.
 */
bool queueRead(queue_t* queue, generic_data_t* data);

/**
 * @brief Checks to see if the given queue is empty.
 * @param queue Queue to access.
 * @param isEmpty Flag that indicates if the queue is empty.
 * @return True if the function was successful, false if not.
 */
bool queueIsEmpty(const queue_t* queue, bool* isEmpty);

/**
 * @brief Checks to see if the given queue is full.
 * @param queue Queue to access.
 * @param isFull Flag that indicates if the queue is full.
 * @return True if the function was successful, false if not.
 */
bool queueIsFull(const queue_t* queue, bool* isFull);

#endif // _QUEUE_INTERFACE_H_
