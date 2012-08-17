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
 * @file gyro_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief TODO Comment!
 */

#ifndef _GYRO_INTERFACE_H_
#define _GYRO_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define GYRO_DOF (3u)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _gyro_dof_e_
{
	GYRO_DOF_YAW = 0,
	GYRO_DOF_PITCH,
	GYRO_DOF_ROLL
} gyro_dof_e;

/**
 * TODO Comment!
 */
typedef enum _gyro_error_e_
{
    GYRO_ERROR_NONE = 0,
    GYRO_ERROR_COMM,

    // Do not place values below!
    GYRO_ERROR_COUNT
} gyro_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef uint16_t gyro_sample_t;

/**
 * TODO Comment!
 */
typedef union _gyro_reading_t_
{
	gyro_sample_t temp;
	gyro_sample_t samples[GYRO_DOF];
} gyro_reading_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void gyroInit(void);

/**
 * TODO Comment!
 */
extern gyro_error_e gyroRead(gyro_reading_t* reading);

#endif // _GYRO_INTERFACE_H_
