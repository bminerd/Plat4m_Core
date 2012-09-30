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
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_interface.h>

#include <micro_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  SYSTEM_PROCEDURE_ID_LIMIT
#define SYSTEM_PROCEDURE_COUNT (SYSTEM_PROCEDURE_ID_LIMIT)
#else
#define SYSTEM_PROCEDURE_COUNT (SYSTEM_PROCEDURE_ID_COUNT)
#endif

#ifdef  SYSTEM_STATE_ID_LIMIT
#define SYSTEM_STATE_COUNT (SYSTEM_STATE_ID_LIMIT)
#else
#define SYSTEM_STATE_COUNT (SYSTEM_STATE_ID_COUNT)
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
typedef struct _system_procedure_container_t_
{
    system_procedure_driver_t driver;
    bool isEnabled;
    uint32_t callTimeMs;
} system_procedure_container_t;

/**
 * TODO Comment!
 */
typedef struct _system_state_container_t_
{
    system_state_driver_t driver;
    bool isEnabled;
} system_state_container_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static system_procedure_container_t procedureContainers[SYSTEM_PROCEDURE_COUNT];

/**
 * TODO Comment!
 */
static system_state_container_t stateContainers[SYSTEM_STATE_COUNT];

/**
 * TODO Comment!
 */
static system_state_id_e systemStateId = (system_state_id_e) 0;

/**
 * TODO Comment!
 */
static volatile uint32_t systemTimeMs = 0;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static void init(void);

/**
 * TODO Comment!
 */
static bool addProcedureDriver(system_procedure_driver_t* driver);

/**
 * TODO Comment!
 */
static bool addStateDriver(system_state_driver_t* driver);

/**
 * TODO Comment!
 */
static void procedureUpdate(void);

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
extern bool systemAddProcedureDrivers(system_procedure_driver_t drivers[],
                                      uint8_t size)
{
    int i;
    
    if (size > SYSTEM_PROCEDURE_COUNT)
    {
        return false;
    }
    
    for (i = 0; i < size; i++)
    {
        if (!addProcedureDriver(&drivers[i]))
        {
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemProcedureSetEnabled(system_procedure_id_e id, bool enabled)
{
    if ((id >= SYSTEM_PROCEDURE_ID_COUNT))
    {
        return false;
    }
    
    procedureContainers[id].isEnabled = enabled;
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemAddStateDrivers(system_state_driver_t drivers[],
                                  uint8_t size)
{
    int i;
    
    if (size > SYSTEM_STATE_COUNT)
    {
        return false;
    }
    
    for (i = 0; i < size; i++)
    {
        if (!addStateDriver(&drivers[i]))
        {
            return false;
        }
    }
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemStateSetEnabled(system_state_id_e id, bool enabled)
{
    if ((id >= SYSTEM_STATE_ID_COUNT))
    {
        return false;
    }
    
    stateContainers[id].isEnabled = enabled;
    
    return true;
}

//------------------------------------------------------------------------------
extern bool systemStateTransition(system_state_id_e newStateId)
{
    if ((newStateId >= SYSTEM_STATE_ID_COUNT)    ||
        !stateContainers[newStateId].isEnabled             ||
        IS_NULL_POINTER(stateContainers[newStateId].driver.enter))
    {
        return false;
    }

    if (!IS_NULL_POINTER(stateContainers[systemStateId].driver.exit))
    {
        stateContainers[systemStateId].driver.exit();
    }

    systemStateId = newStateId;

    stateContainers[systemStateId].driver.enter();

    return true;
}

//------------------------------------------------------------------------------
extern system_state_id_e systemStateGet(void)
{
    return systemStateId;
}

//------------------------------------------------------------------------------
extern void systemTimeMsHandler(void)
{
    systemTimeMs++;
}

//------------------------------------------------------------------------------
extern uint32_t systemTimeMsGet(void)
{
    return systemTimeMs;
}

//------------------------------------------------------------------------------
extern void systemDelayMs(uint32_t delayMs)
{
    uint32_t delayTimeMs = systemTimeMs + delayMs;

    while (delayTimeMs > systemTimeMs);
}

//------------------------------------------------------------------------------
extern bool systemCheckTimeMs(uint32_t checkTimeMs)
{
    return (systemTimeMsGet() >= checkTimeMs);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void init(void)
{
    int i;

    for (i = 0; i < SYSTEM_PROCEDURE_COUNT; i++)
    {
        procedureContainers[i].driver.id        = (system_procedure_id_e) i;
        procedureContainers[i].driver.callback  = NULL_POINTER;
        procedureContainers[i].isEnabled        = false;
        procedureContainers[i].callTimeMs       = 0;
    }

    for (i = 0; i < SYSTEM_STATE_COUNT; i++)
    {
        stateContainers[i].driver.id        = (system_state_id_e) i;
        stateContainers[i].driver.enter     = NULL_POINTER;
        stateContainers[i].driver.exit      = NULL_POINTER;
        stateContainers[i].isEnabled        = false;
    }

    // System-wide inits
    microInit();

    // Driver specific init
    systemDriverInit();

    if (stateContainers[0].isEnabled)
    {
        stateContainers[0].driver.enter();
        systemStateId = (system_state_id_e) 0;
    }
}

//------------------------------------------------------------------------------
static bool addProcedureDriver(system_procedure_driver_t* driver)
{
    if (IS_NULL_POINTER(driver)                     ||
        (driver->id >= SYSTEM_PROCEDURE_ID_COUNT)   ||
        IS_NULL_POINTER(driver->callback))
    {
        return false;
    }

    ADD_CONTAINER(procedureContainers, driver);

    procedureContainers[driver->id].isEnabled = true;

    return true;
}

//------------------------------------------------------------------------------
static bool addStateDriver(system_state_driver_t* driver)
{
    if (IS_NULL_POINTER(driver)                 ||
        (driver->id >= SYSTEM_STATE_ID_COUNT)   ||
        IS_NULL_POINTER(driver->enter))
    {
        return false;
    }

    ADD_CONTAINER(stateContainers, driver);

    stateContainers[driver->id].isEnabled = true;

    return true;
}

//------------------------------------------------------------------------------
static void procedureUpdate(void)
{
    int i;

    for (i = 0; i < SYSTEM_PROCEDURE_ID_COUNT; i++)
    {
        if (procedureContainers[i].isEnabled &&
            systemCheckTimeMs(procedureContainers[i].callTimeMs))
        {
            procedureContainers[i].driver.callback();
        }
    }
}
