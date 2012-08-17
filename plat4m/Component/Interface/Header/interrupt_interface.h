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
 * @file interrupt_interface.h
 * @author Ben Minerd
 * @date 8/2/12
 * @brief TODO Comment!
 */

#ifndef _INTERRUPT_INTERFACE_H_
#define _INTERRUPT_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <interrupt_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _interrupt_error_e_
{
    INTERRUPT_ERROR_NONE = 0,
    INTERRUPT_ERROR_INVALID_ID,

    // Do not add values below!
    INTERRUPT_ERROR_COUNT
} interrupt_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef struct _interrupt_driver_t_
{
    interrupt_driver_id_e id;
    set_enabled_f* setEnabled;
} interrupt_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void interruptInit(void);

/**
 * TODO Comment!
 */
extern bool interruptAddDriver(interrupt_driver_t* interruptDriver);

/**
 * TODO Comment!
 */
extern bool interruptAddDrivers(interrupt_driver_t interruptDrivers[],
                                uint8_t size);

/**
 * TODO Comment!
 */
extern bool interruptAddHandler(interrupt_driver_id_e id, procedure_f* handler);

/**
 * TODO Comment!
 */
extern interrupt_error_e interruptSetEnabled(interrupt_driver_id_e id,
                                             bool enabled);

/**
 * TODO Comment!
 */
extern void interruptHandler(interrupt_driver_id_e id);

/**
 * TODO Comment!
 */
extern void interruptProcedure(void);

#endif // _INTERRUPT_INTERFACE_H_
