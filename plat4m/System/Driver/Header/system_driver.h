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
 * @file system_driver.h
 * @author Ben Minerd
 * @date 2/3/12
 * @brief TODO Comment!
 */

#ifndef _SYSTEM_DRIVER_H_
#define _SYSTEM_DRIVER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _system_driver_procedure_id_e_
{
    SYSTEM_DRIVER_PROCEDURE_ID_LED = 0,
    
    // Do not place values below!
    SYSTEM_DRIVER_PROCEDURE_ID_COUNT
} system_driver_procedure_id_e;

/**
 * TODO Comment!
 */
typedef enum _system_driver_state_id_e_
{
    SYSTEM_DRIVER_STATE_ID_OFF = 0,
    SYSTEM_DRIVER_STATE_ID_ON,

    // Do not place values below!
    SYSTEM_DRIVER_STATE_ID_COUNT
} system_driver_state_id_e;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void systemDriverInit(void);

#endif // _SYSTEM_DRIVER_H_
