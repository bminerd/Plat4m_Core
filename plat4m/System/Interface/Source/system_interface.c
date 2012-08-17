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
 * @file system_interface.c
 * @author Ben Minerd
 * @date 1/21/12
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_interface.h>

#include <micro_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  SYSTEM_DRIVER_PROCEDURE_ID_LIMIT
#define SYSTEM_DRIVER_PROCEDURE_COUNT (SYSTEM_DRIVER_PROCEDURE_ID_LIMIT)
#else
#define SYSTEM_DRIVER_PROCEDURE_COUNT (SYSTEM_DRIVER_PROCEDURE_ID_COUNT)
#endif

#ifdef  SYSTEM_DRIVER_STATE_ID_LIMIT
#define SYSTEM_DRIVER_STATE_COUNT (SYSTEM_DRIVER_STATE_ID_LIMIT)
#else
#define SYSTEM_DRIVER_STATE_COUNT (SYSTEM_DRIVER_STATE_ID_COUNT)
#endif

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
 *
 */
static system_procedure_driver_t procedureDrivers[
                                                 SYSTEM_DRIVER_PROCEDURE_COUNT];

/**
 *
 */
static system_state_driver_t stateDrivers[SYSTEM_DRIVER_STATE_COUNT];

/**
 *
 */
static system_driver_state_id_e currentState;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
static void init(void);

/**
 *
 */
static void procedureUpdate();

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void systemStart(void)
{
    init();
    
    while (true)
    {
        procedureUpdate();
    }
}

//------------------------------------------------------------------------------
extern bool systemProcedureAddDriver(system_procedure_driver_t driver)
{
    if (driver.id >= SYSTEM_DRIVER_PROCEDURE_ID_COUNT ||
        !driver.callback)
    {
        return false;
    }
    
    DRIVER_ADD(procedureDrivers, driver, system_procedure_driver_t);
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemProcedureAddDrivers(system_procedure_driver_t drivers[],
                                      uint8_t size)
{
    int i;
    
    if (size > SYSTEM_DRIVER_PROCEDURE_COUNT)
    {
        return false;
    }
    
    for (i = 0; i < size; i++)
    {
        if (!systemProcedureAddDriver(drivers[i]))
        {
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemProcedureSetEnabled(system_driver_procedure_id_e id,
                                      bool enabled)
{
    if (id >= SYSTEM_DRIVER_PROCEDURE_ID_COUNT ||
        !procedureDrivers[id].callback)
    {
        return false;
    }
    
    procedureDrivers[id].enabled = enabled;
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemStateAddDriver(system_state_driver_t driver)
{
    if (driver.id >= SYSTEM_DRIVER_STATE_ID_COUNT ||
        !driver.enter ||
        !driver.exit)
    {
        return false;
    }
    
    DRIVER_ADD(stateDrivers, driver, system_state_driver_t);
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemStateAddDrivers(system_state_driver_t drivers[],
                                  uint8_t size)
{
    int i;
    
    if (size > SYSTEM_DRIVER_STATE_COUNT)
    {
        return false;
    }
    
    for (i = 0; i < size; i++)
    {
        if (!systemStateAddDriver(drivers[i]))
        {
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemStateSetEnabled(system_driver_state_id_e id, bool enabled)
{
    if (id >= SYSTEM_DRIVER_STATE_ID_COUNT || !stateDrivers[id].enter)
    {
        return false;
    }
    
    stateDrivers[id].enabled = enabled;
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemStateTransition(system_driver_state_id_e newStateId)
{
    if (newStateId >= SYSTEM_DRIVER_STATE_ID_COUNT ||
        !stateDrivers[newStateId].enabled ||
        !stateDrivers[newStateId].enter)
    {
        return false;
    }

    if (stateDrivers[currentState].exit)
    {
        stateDrivers[currentState].exit();
    }

    currentState = newStateId;

    stateDrivers[currentState].enter();

    return true;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void init(void)
{
    int i;

    for (i = 0; i < SYSTEM_DRIVER_PROCEDURE_COUNT; i++)
    {
        procedureDrivers[i].id = (system_driver_procedure_id_e) i;
        procedureDrivers[i].enabled = false;
        procedureDrivers[i].callback = 0;
    }

    for (i = 0; i < SYSTEM_DRIVER_STATE_COUNT; i++)
    {
        stateDrivers[i].id = (system_driver_state_id_e) i;
        stateDrivers[i].enabled = false;
        stateDrivers[i].enter = 0;
        stateDrivers[i].exit = 0;
    }

    // System-wide inits
    microInit();

    // Driver specific init
    systemDriverInit();
}

//------------------------------------------------------------------------------
static void procedureUpdate(void)
{
    int i = 0;

    if (procedureDrivers[i].enabled)
    {
        if (procedureDrivers[i].callback)
        {
            procedureDrivers[i].callback();
        }
        else
        {
            procedureDrivers[i].enabled = false;
        }
    }

    i++;
    
    // Roll over if needed
    if (i  >= SYSTEM_DRIVER_PROCEDURE_COUNT)
    {
        i = 0;
    }
}
