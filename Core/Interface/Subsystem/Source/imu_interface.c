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
 * @file imu_interface.c
 * @author Ben Minerd
 * @date 12/30/11
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <imu_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static bool isEnabled = false;

///**
// * TODO Comment!
// */
//static imu_reading_callback_t* readingCallback = 0;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

///**
// * TODO Comment!
// */
//void imuTask(void);

///**
// * TODO Comment!
// */
//void imuParseData(uint8_t data[],
//                         uint8_t size,
//                         imu_reading_t* reading);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void imuInit(void)
{
    imuDriverInit();
    isEnabled = true;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

////------------------------------------------------------------------------------
//void imuTask(void)
//{
//	static bool initialized = false;
//	static imu_reading_t reading;
//
//	if (!initialized)
//	{
//		imuDriverInit();
//		initialized = true;
//	}
//
//	// Wait on IMU data queue
//
//	if (readingCallback)
//	{
//		readingCallback(&reading);
//	}
//}

////------------------------------------------------------------------------------
//void imuParseData(uint8_t data[], uint8_t size, imu_reading_t* reading)
//{
//
//}
