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

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef struct _system_procedure_wrapper_t_
{
    system_procedure_t procedure;
    bool isEnabled;
    uint32_t callTimeMs;
} system_procedure_wrapper_t;

/**
 * TODO Comment!
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
 * TODO Comment!
 */
static system_procedure_wrapper_t procedureMap[SYSTEM_PROCEDURE_COUNT];

/**
 * TODO Comment!
 */
static system_state_wrapper_t stateMap[SYSTEM_STATE_COUNT];

/**
 * TODO Comment!
 */
static volatile uint32_t systemTimeMs = 0;

static system_state_e systemState;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static void procedureUpdate(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void systemInit(void)
{
    int i;

    for (i = 0; i < SYSTEM_PROCEDURE_COUNT; i++)
    {
        procedureMap[i].procedure.procedure = SYSTEM_PROCEDURE_NONE;
        procedureMap[i].procedure.callback  = NULL_POINTER;
        procedureMap[i].isEnabled           = false;
        procedureMap[i].callTimeMs          = 0;
    }

    for (i = 0; i < SYSTEM_STATE_COUNT; i++)
    {
        stateMap[i].state.state = SYSTEM_STATE_NONE;
        stateMap[i].state.enter = NULL_POINTER;
        stateMap[i].state.exit  = NULL_POINTER;
        stateMap[i].isEnabled   = false;
    }

    // System-wide inits
    microInit();

    // Driver specific init
    systemApplicationInit();
    
    while (true)
    {
        procedureUpdate();
    }
}

//------------------------------------------------------------------------------
extern system_error_e systemAddProcedure(const system_procedure_t* procedure)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(procedure))
    {
        return SYSTEM_ERROR_PARAMETER_INVALID;
    }

    if ((procedure->procedure == SYSTEM_PROCEDURE_NONE)     ||
        (procedure->procedure >= SYSTEM_PROCEDURE_COUNT)    ||
        IS_NULL_POINTER(procedure->callback))
    {
        return SYSTEM_ERROR_PROCEDURE_INVALID;
    }

#endif // PLAT4M_DEBUG

    procedureMap[procedure->procedure].procedure = *procedure;

    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern system_error_e systemAddProcedures(const system_procedure_t procedures[],
                                          const unsigned int size)
{
#ifdef PLAT4M_DEBUG
    
    if (size > SYSTEM_PROCEDURE_COUNT)
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
extern system_error_e systemAddState(const system_state_t* state)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(state))
    {
        return SYSTEM_ERROR_PARAMETER_INVALID;
    }

    if ((state->state == SYSTEM_STATE_NONE)     ||
        (state->state >= SYSTEM_STATE_COUNT)    ||
        IS_NULL_POINTER(state->enter))
    {
        return SYSTEM_ERROR_STATE_INVALID;
    }

#endif // PLAT4M_DEBUG

    stateMap[state->state].state = *state;

    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern system_error_e systemAddStates(const system_state_t states[],
                                      const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > SYSTEM_STATE_COUNT)
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
extern system_error_e systemProcedureSetEnabled(
                                            const system_procedure_e procedure,
                                            const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((procedure == SYSTEM_PROCEDURE_NONE) ||
        (procedure >= SYSTEM_PROCEDURE_COUNT))
    {
        return SYSTEM_ERROR_PROCEDURE_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    procedureMap[procedure].isEnabled = enabled;
    
    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern system_error_e systemStateSetEnabled(const system_state_e state,
                                            const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((state == SYSTEM_STATE_NONE) || (state >= SYSTEM_STATE_COUNT))
    {
        return SYSTEM_ERROR_STATE_INVALID;
    }
    
#endif // PLAT4M_DEBUG

    stateMap[state].isEnabled = enabled;
    
    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern system_error_e systemStateTransition(const system_state_e state)
{
#ifdef PLAT4M_DEBUG

    if ((state == SYSTEM_STATE_NONE)    ||
        (state >= SYSTEM_STATE_COUNT)   ||
        IS_NULL_POINTER(stateMap[state].state.enter))
    {
        return SYSTEM_ERROR_STATE_INVALID;
    }

    if (!(stateMap[state].isEnabled))
    {
        return SYSTEM_ERROR_STATE_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    if (!IS_NULL_POINTER(stateMap[state].state.exit))
    {
        stateMap[state].state.exit();
    }

    systemState = state;

    stateMap[state].state.enter();

    return SYSTEM_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern system_state_e systemStateGet(void)
{
    return systemState;
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
extern void systemDelayMs(const uint32_t delayMs)
{
    uint32_t delayTimeMs = systemTimeMs + delayMs;

    while (delayTimeMs > systemTimeMs);
}

//------------------------------------------------------------------------------
extern bool systemCheckTimeMs(const uint32_t checkTimeMs)
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

    for (i = 0; i < SYSTEM_PROCEDURE_COUNT; i++)
    {
        if (procedureMap[i].isEnabled &&
            systemCheckTimeMs(procedureMap[i].callTimeMs))
        {
            procedureMap[i].procedure.callback();
        }
    }
}
