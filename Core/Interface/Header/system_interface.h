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
 * @file system_interface.h
 * @author Ben Minerd
 * @date 1/21/12
 * @brief TODO Comment!
 */

#ifndef _SYSTEM_INTERFACE_H_
#define _SYSTEM_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <system_application.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define PLAT4M_VERSION ("0.0.1")

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of system errors.
 */
typedef enum _system_error_e_
{
    SYSTEM_ERROR_NONE,
    SYSTEM_ERROR_PARAMETER_INVALID,
    SYSTEM_ERROR_PROCEDURE_INVALID,
    SYSTEM_ERROR_PROCEDURE_NOT_ENABLED,
    SYSTEM_ERROR_STATE_INVALID,
    SYSTEM_ERROR_STATE_NOT_ENABLED
} system_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef procedure_f system_procedure_callback_f;

/**
 * TODO Comment!
 */
typedef procedure_f system_state_enter_f;

/**
 * TODO Comment!
 */
typedef procedure_f system_state_exit_f;

/**
 * @brief System procedure type.
 */
typedef struct _system_procedure_t_
{
    system_procedure_e procedure;
    uint32_t timeDelayMs;
    unsigned int priority;
    const system_procedure_callback_f* callback;
} system_procedure_t;

/**
 * @brief System state type.
 */
typedef struct _system_state_t_
{
    system_state_e state;
    const system_state_enter_f* enter;
    const system_state_exit_f* exit;
} system_state_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the system module.
 */
extern void systemInit(void);

/**
 * @brief Adds the given procedures.
 * @param procedures Procedure to add.
 * @return System error.
 */
extern system_error_e systemAddProcedure(const system_procedure_t* procedures);

/**
 * @brief Adds the given procedures.
 * @param procedures Procedures to add.
 * @param size Number of procedures to add.
 * @return System error.
 */
extern system_error_e systemAddProcedures(const system_procedure_t procedures[],
                                          const unsigned int size);

/**
 * @brief Adds the given state.
 * @param states State to add.
 * @return System error.
 */
extern system_error_e systemAddState(const system_state_t* state);

/**
 * @brief Adds the given states.
 * @param states States to add.
 * @param size Number of states to add.
 * @return System error.
 */
extern system_error_e systemAddStates(const system_state_t states[],
                                      const unsigned int size);

/**
 * @brief Sets the given procedure enabled or disabled.
 * @param procedure Procedure to access.
 * @enabled Flag that indicates if the procedure should be enabled or disabled.
 * @return System error.
 */
extern system_error_e systemProcedureSetEnabled(
                                            const system_procedure_e procedure,
                                            const bool enabled);

/**
 * @brief Sets the given state enabled or disabled.
 * @param state State to access.
 * @enabled Flag that indicates if the state should be enabled or disabled.
 * @return System error.
 */
extern system_error_e systemStateSetEnabled(const system_state_e state,
                                            const bool enabled);

/**
 * @brief Performs a system state transition from the previous state to the
 * given state.
 * @param state State to transition to.
 * @return System error.
 */
extern system_error_e systemStateTransition(const system_state_e state);

/**
 * @brief Gets the current system state.
 * @return Current system state.
 */
extern system_state_e systemStateGet(void);

extern void systemTimeMsHandler(void);

/**
 * @brief Gets the current system time in milliseconds.
 * @return System time in milliseconds.
 */
extern uint32_t systemTimeMsGet(void);

/**
 * @brief Delays the system for the given number of milliseconds.
 * @param delayMs Number of milliseconds to delay.
 */
extern void systemDelayMs(const uint32_t delayMs);

/**
 * @brief Checks the given time against the system time and returns true if
 * the current system time is greater than or equal to the given time.
 * @param checkTimeMs Time to check in milliseconds.
 * @return True if the system time has reached/passed the check time, false if
 * not.
 */
extern bool systemCheckTimeMs(const uint32_t checkTimeMs);

#endif // _SYSTEM_INTERFACE_H_
