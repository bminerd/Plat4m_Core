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
#include <system_driver.h>

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

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef procedure_f system_driver_procedure_callback_f;

/**
 * TODO Comment!
 */
typedef procedure_f system_driver_state_enter_f;

/**
 * TODO Comment!
 */
typedef procedure_f system_driver_state_exit_f;

/**
 * TODO Comment!
 */
typedef struct _system_procedure_t_
{
    system_driver_procedure_id_e id;
    bool enabled;
    system_driver_procedure_callback_f* callback;
} system_procedure_driver_t;

/**
 * TODO Comment!
 */
typedef struct _system_state_t_
{
    system_driver_state_id_e id;
    bool enabled;
    system_driver_state_enter_f* enter;
    system_driver_state_exit_f* exit;
} system_state_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void systemStart(void);

/**
 * TODO Comment!
 */
extern bool systemProcedureAddDriver(system_procedure_driver_t* driver);

/**
 * TODO Comment!
 */
extern bool systemProcedureAddDrivers(system_procedure_driver_t drivers[],
                                      uint8_t size);

/**
 * TODO Comment!
 */
extern bool systemProcedureSetEnabled(system_driver_procedure_id_e id,
                                      bool enabled);

/**
 * TODO Comment!
 */
extern bool systemStateAddDriver(system_state_driver_t* driver);

/**
 * TODO Comment!
 */
extern bool systemStateAddDrivers(system_state_driver_t driver[],
                                  uint8_t size);

/**
 * TODO Comment!
 */
extern bool systemStateSetEnabled(system_driver_state_id_e stateId,
                                  bool enabled);

/**
 * TODO Comment!
 */
extern bool systemStateTransition(system_driver_state_id_e newStateId);

/**
 * TODO Comment!
 */
extern void systemTimeMsHandler(void);

/**
 * TODO Comment!
 */
extern uint32_t systemTimeMsGet(void);

/**
 * TODO Comment!
 */
extern void systemDelayMs(uint32_t delayMs);

#endif // _SYSTEM_INTERFACE_H_
