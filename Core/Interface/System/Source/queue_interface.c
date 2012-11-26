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
 * @file queue_interface.c
 * @author Ben Minerd
 * @date 10/9/2012
 * @brief Queue interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <queue_interface.h>

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

/**
 * @brief Queue driver.
 */
static queue_driver_t driver;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
 void queueInit(void)
{
    queueDriverInit();
}

//------------------------------------------------------------------------------
bool queueSetDriver(const queue_driver_t* queueDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(queueDriver)        ||
        IS_NULL_POINTER(queueDriver->write) ||
        IS_NULL_POINTER(queueDriver->read))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    driver = *queueDriver;

    return true;
}

//------------------------------------------------------------------------------
bool queueInitQueue(queue_t* queue,
                    generic_data_t* queueMemory,
                    const unsigned int queueMemorySize,
                    const unsigned int wordSize)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(queue))
    {
        return false;
    }

#endif // PLAT4M_DEBUG



    return true;
}

//------------------------------------------------------------------------------
bool queueWrite(queue_t* queue, const generic_data_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(queue) || IS_NULL_POINTER(data))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    driver.write(queue, data);

    return true;
}

//------------------------------------------------------------------------------
bool queueRead(queue_t* queue, generic_data_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(queue) || IS_NULL_POINTER(data))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    driver.read(queue, data);

    return true;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

