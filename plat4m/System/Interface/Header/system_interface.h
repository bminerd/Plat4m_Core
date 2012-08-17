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
 * @brief
 */

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <system_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define PLAT4M_VERSION ("0.0.1")

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef procedure_f system_driver_procedure_callback_f;

/**
 *
 */
typedef procedure_f system_driver_state_enter_f;

/**
 *
 */
typedef procedure_f system_driver_state_exit_f;

/**
 *
 */
typedef struct _system_procedure_t_
{
    system_driver_procedure_id_e id;
    bool enabled;
    system_driver_procedure_callback_f* callback;
} system_procedure_driver_t;

/**
 *
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
 *
 */
extern void systemStart(void);

/**
 *
 */
extern bool systemProcedureAddDriver(system_procedure_driver_t driver);

/**
 *
 */
extern bool systemProcedureAddDrivers(system_procedure_driver_t drivers[],
                                      uint8_t size);

/**
 *
 */
extern bool systemProcedureSetEnabled(system_driver_procedure_id_e id,
                                      bool enabled);

/**
 *
 */
extern bool systemStateAddDriver(system_state_driver_t driver);

/**
 *
 */
extern bool systemStateAddDrivers(system_state_driver_t driver[],
                                  uint8_t size);

/**
 *
 */
extern bool systemStateSetEnabled(system_driver_state_id_e stateId,
                                  bool enabled);

/**
 *
 */
extern bool systemStateTransition(system_driver_state_id_e newStateId);

#endif // _SYSTEM_INTERFACE_H_
