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
 * @brief System interface layer.
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
 * @brief plat4m Version.
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
    SYSTEM_ERROR_PROCEDURE_ID_INVALID,
    SYSTEM_ERROR_PROCEDURE_NOT_ENABLED,
    SYSTEM_ERROR_STATE_ID_INVALID,
    SYSTEM_ERROR_STATE_NOT_ENABLED
} system_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Function type for a system procedure callback.
 */
typedef procedure_f system_procedure_callback_f;

/**
 * @brief Function type for a system state enter procedure.
 */
typedef procedure_f system_state_enter_f;

/**
 * @brief Function type for a system state exit procedure.
 */
typedef procedure_f system_state_exit_f;

/**
 * @brief System procedure type.
 */
typedef struct _system_procedure_t_
{
    system_procedure_id_e id;
    uint32_t timeDelayMs;
    unsigned int priority;
    system_procedure_callback_f* callback;
} system_procedure_t;

/**
 * @brief System state type.
 */
typedef struct _system_state_t_
{
    system_state_id_e id;
    system_state_enter_f* enter;
    system_state_exit_f* exit;
} system_state_t;

/**
 * @brief Function type that handles the system time MS timer.
 */
typedef void system_driver_time_ms_handler_f(void);

/**
 * @brief Function type that gets the current system time in milliseconds.
 * @return System time in milliseconds.
 */
typedef uint32_t system_driver_time_ms_get_f(void);

/**
 * @brief Function type that delays the system for the given millisecond delay.
 * @param delayMs Delay in milliseconds.
 */
typedef void system_driver_delay_ms_f(const uint32_t delayMs);

/**
 * @brief System driver type.
 */
typedef struct _system_driver_t_
{
    system_driver_time_ms_handler_f* timeMsHandler;
    system_driver_time_ms_get_f* timeMsGet;
    system_driver_delay_ms_f* delayMs;
} system_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the system module.
 */
void systemInit(void);

/**
 * @brief Sets the driver for the System interface.
 * @param driver System driver.
 * @return System error.
 */
system_error_e systemSetDriver(const system_driver_t* systemDriver);

/**
 * @brief Adds the given procedures.
 * @param procedures Procedure to add.
 * @return System error.
 */
system_error_e systemAddProcedure(const system_procedure_t* procedures);

/**
 * @brief Adds the given procedures.
 * @param procedures Procedures to add.
 * @param size Number of procedures to add.
 * @return System error.
 */
system_error_e systemAddProcedures(const system_procedure_t procedures[],
                                   const unsigned int size);

/**
 * @brief Adds the given state.
 * @param states State to add.
 * @return System error.
 */
system_error_e systemAddState(const system_state_t* state);

/**
 * @brief Adds the given states.
 * @param states States to add.
 * @param size Number of states to add.
 * @return System error.
 */
system_error_e systemAddStates(const system_state_t states[],
                               const unsigned int size);

/**
 * @brief Sets the given procedure enabled or disabled.
 * @param id Procedure to access.
 * @enabled Flag that indicates if the procedure should be enabled or disabled.
 * @return System error.
 */
system_error_e systemProcedureSetEnabled(const system_procedure_id_e id,
                                          const bool enabled);

/**
 * @brief Sets the given state enabled or disabled.
 * @param id State to access.
 * @enabled Flag that indicates if the state should be enabled or disabled.
 * @return System error.
 */
system_error_e systemStateSetEnabled(const system_state_id_e id,
                                     const bool enabled);

/**
 * @brief Performs a system state transition from the previous state to the
 * given state.
 * @param id State to transition to.
 * @return System error.
 */
system_error_e systemStateTransition(const system_state_id_e id);

/**
 * @brief Gets the current system state.
 * @return Current system state.
 */
system_state_id_e systemStateGet(void);

/**
 * @brief Handles the time MS timer.
 */
void systemTimeMsHandler(void);

/**
 * @brief Gets the current system time in milliseconds.
 * @return System time in milliseconds.
 */
uint32_t systemTimeMsGet(void);

/**
 * @brief Delays the system for the given number of milliseconds.
 * @param delayMs Number of milliseconds to delay.
 */
void systemDelayMs(const uint32_t delayMs);

/**
 * @brief Checks the given time against the system time and returns true if
 * the current system time is greater than or equal to the given time.
 * @param checkTimeMs Time to check in milliseconds.
 * @return True if the system time has reached/passed the check time, false if
 * not.
 */
bool systemCheckTimeMs(const uint32_t checkTimeMs);

#endif // _SYSTEM_INTERFACE_H_
