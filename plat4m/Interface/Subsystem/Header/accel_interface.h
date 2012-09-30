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
 * @file accel_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief TODO Comment!
 */

#ifndef _ACCEL_INTERFACE_H_
#define _ACCEL_INTERFACE_H_

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
typedef enum _accel_dof_e_
{
    ACCEL_DOF_X = 0,
    ACCEL_DOF_Y,
    ACCEL_DOF_Z,

    // Do not place values below!
    ACCEL_DOF_COUNT
} accel_dof_e;

/**
 * TODO Comment!
 */
typedef enum _accel_error_e_
{
    ACCEL_ERROR_NONE,
    ACCEL_ERROR_NOT_ENABLED,
    ACCEL_ERROR_COMM,
} accel_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef uint16_t accel_sample_t;

/**
 * TODO Comment!
 */
typedef union _accel_reading_t_
{
    accel_sample_t samples[ACCEL_DOF_COUNT];
} accel_reading_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
void accelInit(void);

/**
 * TODO Comment!
 */
accel_error_e accelRead(accel_reading_t* reading);

#endif // _ACCEL_INTERFACE_H_
