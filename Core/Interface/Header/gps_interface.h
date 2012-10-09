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
 * @file gps_interface.h
 * @author Ben Minerd
 * @date 11/23/11
 * @brief TODO Comment!
 */

#ifndef _GPS_INTERFACE_H_
#define _GPS_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <gps_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _gps_error_e_
{
    GPS_ERROR_NONE,
    GPS_ERROR_PARAMETER_INVALID,
    GPS_ERROR_NOT_ENABLED,
    GPS_ERROR_COMMUNICATION
} gps_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef struct _gps_reading_t_
{
	float latitude;
	float longitude;
	float elevation;
	float speed;
	float heading;
	uint32_t timeStamp;
} gps_reading_t;

/**
 * TODO Comment!
 */
typedef void gps_reading_callback_t(gps_reading_t* gps_measurement);

/*------------------------------------------------------------------------------
 * Global functions
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
void gpsInit(void);

#endif // _GPS_INTERFACE_H_
