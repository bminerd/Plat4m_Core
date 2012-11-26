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
 * @brief System interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_interface.h>

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
 * @brief System procedure wrapper type.
 */
typedef struct _system_procedure_wrapper_t_
{
    system_procedure_t procedure;
    bool isEnabled;
    uint32_t callTimeMs;
} system_procedure_wrapper_t;

/**
 * @brief System state wrapper.
 */
typedef struct _system_state_wrapper_t_
{
    system_state_t state;
    bool isEnabled;
} system_state_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief System driver.
 */
static system_driver_t driver;

/**
 * @brief System procedure map.
 */
static system_procedure_wrapper_t idToProcedureMap[SYSTEM_PROCEDURE_ID_COUNT];

/**
 * @brief System state map.
 */
static system_state_wrapper_t idToStateMap[SYSTEM_STATE_ID_COUNT];

/**
 * @brief Current system state.
 */
static system_state_id_e systemState;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Updates all of the system procedures.
 */
static void procedureUpdate(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void systemInit(void)
{
    int i;

    for (i = 0; i < SYSTEM_PROCEDURE_ID_COUNT; i++)
    {
        idToProcedureMap[i].procedure.id        = SYSTEM_PROCEDURE_ID_NONE;
        idToProcedureMap[i].procedure.callback  = NULL_POINTER;
        idToProcedureMap[i].isEnabled           = false;
        idToProcedureMap[i].callTimeMs          = 0;
    }

    for (i = 0; i < SYSTEM_STATE_ID_COUNT; i++)
    {
        idToStateMap[i].state.id    = SYSTEM_STATE_ID_NONE;
        idToStateMap[i].state.enter = NULL_POINTER;
        idToStateMap[i].state.exit  = NULL_POINTER;
        idToStateMap[i].isEnabled   = false;
    }

    // Driver specific init
    systemApplicationInit();
    
    while (true)
    {
        procedureUpdate();
    }
}

//------------------------------------------------------------------------------
system_error_e systemSetDriver(const system_driver_t* systemDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(systemDriver)                   ||
        IS_NULL_POINTER(systemDriver->timeMsHandler)    ||
        IS_NULL_POINTER(systemDriver->timeMsGet)        ||
        IS_NULL_POINTER(systemDriver->delayMs))
    {
        return SYSTEM_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *systemDriver;

    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_error_e systemAddProcedure(const system_procedure_t* procedure)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(procedure) || IS_NULL_POINTER(procedure->callback))
    {
        return SYSTEM_ERROR_PARAMETER_INVALID;
    }

    if ((procedure->id == SYSTEM_PROCEDURE_ID_NONE) ||
        (procedure->id >= SYSTEM_PROCEDURE_ID_COUNT))
    {
        return SYSTEM_ERROR_PROCEDURE_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToProcedureMap[procedure->id].procedure = *procedure;

    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_error_e systemAddProcedures(const system_procedure_t procedures[],
                                   const unsigned int size)
{
#ifdef PLAT4M_DEBUG
    
    if (size > SYSTEM_PROCEDURE_ID_COUNT)
    {
        return SYSTEM_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        system_error_e error = systemAddProcedure(&(procedures[i]));

#ifdef PLAT4M_DEBUG

        if (error != SYSTEM_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }
    
    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_error_e systemAddState(const system_state_t* state)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(state))
    {
        return SYSTEM_ERROR_PARAMETER_INVALID;
    }

    if ((state->id == SYSTEM_STATE_ID_NONE)     ||
        (state->id >= SYSTEM_STATE_ID_COUNT)    ||
        IS_NULL_POINTER(state->enter))
    {
        return SYSTEM_ERROR_STATE_ID_INVALID;
    }

#endif // PLAT4M_DEBUG

    idToStateMap[state->id].state = *state;

    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_error_e systemAddStates(const system_state_t states[],
                               const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > SYSTEM_STATE_ID_COUNT)
    {
        return SYSTEM_ERROR_PARAMETER_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        system_error_e error = systemAddState(&(states[i]));

#ifdef PLAT4M_DEBUG

        if (error != SYSTEM_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }
    
    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_error_e systemProcedureSetEnabled(const system_procedure_id_e id,
                                         const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == SYSTEM_PROCEDURE_ID_NONE) || (id >= SYSTEM_PROCEDURE_ID_COUNT))
    {
        return SYSTEM_ERROR_PROCEDURE_ID_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    idToProcedureMap[id].isEnabled = enabled;
    
    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_error_e systemStateSetEnabled(const system_state_id_e id,
                                     const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((id == SYSTEM_STATE_ID_NONE) || (id >= SYSTEM_STATE_ID_COUNT))
    {
        return SYSTEM_ERROR_STATE_ID_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    idToStateMap[id].isEnabled = enabled;
    
    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_error_e systemStateTransition(const system_state_id_e id)
{
#ifdef PLAT4M_DEBUG

    if ((id == SYSTEM_STATE_ID_NONE) || (id >= SYSTEM_STATE_ID_COUNT))
    {
        return SYSTEM_ERROR_STATE_ID_INVALID;
    }

    if (!(idToStateMap[id].isEnabled))
    {
        return SYSTEM_ERROR_STATE_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    if (!IS_NULL_POINTER(idToStateMap[id].state.exit))
    {
        idToStateMap[id].state.exit();
    }

    systemState = id;

    idToStateMap[id].state.enter();

    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
system_state_id_e systemStateGet(void)
{
    return systemState;
}

//------------------------------------------------------------------------------
void systemTimeMsHandler(void)
{
    driver.timeMsHandler();
}

//------------------------------------------------------------------------------
uint32_t systemTimeMsGet(void)
{
    return driver.timeMsGet();
}

//------------------------------------------------------------------------------
void systemDelayMs(const uint32_t delayMs)
{
    driver.delayMs(delayMs);
}

//------------------------------------------------------------------------------
bool systemCheckTimeMs(const uint32_t checkTimeMs)
{
    return (systemTimeMsGet() >= checkTimeMs);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void procedureUpdate(void)
{
    int i;

    for (i = 0; i < SYSTEM_PROCEDURE_ID_COUNT; i++)
    {
        if (idToProcedureMap[i].isEnabled &&
            systemCheckTimeMs(idToProcedureMap[i].callTimeMs))
        {
            idToProcedureMap[i].procedure.callback();
        }
    }
}
