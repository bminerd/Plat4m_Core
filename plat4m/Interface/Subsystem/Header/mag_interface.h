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
 * @file mag_interface.h
 * @author Ben Minerd
 * @date 12/31/11
 * @brief TODO Comment!
 */

#ifndef _MAG_INTERFACE_H_
#define _MAG_INTERFACE_H_

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
typedef enum _mag_dof_e_
{
    MAG_DOF_AZIMUTH = 0,
    MAG_DOF_ELEVATION,
    MAG_DOF_BANK,

    // Do not place values below!
    MAG_DOF_COUNT
} mag_dof_e;

/**
 * TODO Comment!
 */
typedef enum _mag_error_e_
{
    MAG_ERROR_NONE,
    MAG_ERROR_NOT_ENABLED,
    MAG_ERROR_COMM
} mag_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef uint16_t mag_sample_t;

/**
 * TODO Comment!
 */
typedef union _mag_reading_t_
{
    mag_sample_t samples[MAG_DOF_COUNT];
} mag_reading_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
void magInit(void);

/**
 * TODO Comment!
 */
mag_error_e magRead(mag_reading_t* reading);

#endif // _MAG_INTERFACE_H_
