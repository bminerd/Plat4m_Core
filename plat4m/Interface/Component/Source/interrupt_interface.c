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
 * @file interrupt_interface.c
 * @author Ben Minerd
 * @date 8/2/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <interrupt_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  INTERRUPT_ID_LIMIT
#define INTERRUPT_COUNT (INTERRUPT_ID_LIMIT)
#else
#define INTERRUPT_COUNT (INTERRUPT_ID_COUNT)
#endif

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef struct _interrupt_container_t_
{
    interrupt_driver_t driver;
    procedure_f* handler;
} interrupt_container_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static interrupt_container_t containers[INTERRUPT_COUNT];

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static bool interruptAddDriver(interrupt_driver_t* interruptDriver);

/**
 * TODO Comment!
 */
static bool isDriverEnabled(interrupt_id_e id);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void interruptInit(void)
{
    int i;
    
    for (i = 0; i < INTERRUPT_COUNT; i++)
    {
        containers[i].driver.id         = (interrupt_id_e) i;
        containers[i].driver.isEnabled  = false;
        containers[i].driver.setEnabled = NULL_POINTER;
        containers[i].handler           = NULL_POINTER;
    }
    
    interruptDriverInit();
}

//------------------------------------------------------------------------------
extern bool interruptAddDrivers(interrupt_driver_t interruptDrivers[],
                                uint8_t size)
{
    int i;
    
    if (size > INTERRUPT_COUNT)
    {
        return false;
    }
    
    for (i = 0; i < size; i++)
    {
        if (!interruptAddDriver(&interruptDrivers[i]))
        {
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern bool interruptAddHandler(interrupt_id_e id, procedure_f* handler)
{
    if ((id >= INTERRUPT_ID_COUNT) || IS_NULL_POINTER(handler))
    {
        return false;
    }
    
    containers[id].handler = handler;
    
    return true;
}

//------------------------------------------------------------------------------
extern interrupt_error_e interruptIsEnabled(interrupt_id_e id,
                                            bool* isEnabled)
{
    if (id >= INTERRUPT_ID_COUNT)
    {
        return INTERRUPT_ERROR_INVALID_ID;
    }

    *isEnabled = isDriverEnabled(id);

    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern interrupt_error_e interruptSetEnabled(interrupt_id_e id,
                                             bool enabled)
{
    if (id >= INTERRUPT_ID_COUNT)
    {
        return INTERRUPT_ERROR_INVALID_ID;
    }

    containers[id].driver.setEnabled(enabled);

    return INTERRUPT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern void interruptHandler(interrupt_id_e id)
{
    if (id < INTERRUPT_ID_COUNT)
    {
        containers[id].handler();
    }
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern bool interruptAddDriver(interrupt_driver_t* interruptDriver)
{
    if (IS_NULL_POINTER(interruptDriver)                ||
        (interruptDriver->id >= INTERRUPT_ID_COUNT)     ||
        IS_NULL_POINTER(interruptDriver->setEnabled))
    {
        return false;
    }

    ADD_CONTAINER(containers, interruptDriver);

    if (containers[interruptDriver->id].driver.isEnabled)
    {
        interruptSetEnabled(interruptDriver->id, true);
    }

    return true;
}

//------------------------------------------------------------------------------
static bool isDriverEnabled(interrupt_id_e id)
{
    return containers[id].driver.isEnabled;
}
