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
 * @file imu_driver_template.c
 * @author Ben Minerd
 * @date 11/23/11
 * @brief
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <imu_interface.h>

// plat4m
//
// Replace below with proper IMU driver header file.
//
// Example:
//
//      #include <imu_driver_slyimu.h>

#include <imu_driver_slyimu.h>

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

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local accelerometer driver functions here.
//
// Example:
//
//      static void setBaudRate(uart_baud_rate_e baud);
//      static void setRepRate(imu_rep_rate_e rate);
//      ...

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void imuDriverInit(void)
{
    // plat4m
    //
    // Initialize IMU driver here.
    //
    // Example:
        //
        //      setBaudRate(UART_BAUD_RATE_115200);
        //      setRepRate(IMU_REP_RATE_1000HZ);
        //      ...
}

//------------------------------------------------------------------------------
extern imu_error_e imuGetMeasurement(imu_reading_t* reading)
{
    // plat4m
    //
    // Fill reading with raw and converted float samples.
    //
    // Example:
    //
    //	    // Accel raw
    //	    reading->accelRawSamples[IMU_ACCEL_DOF_X] = 0;
    //	    ...
    //	    // Accel float
    //	    reading->accelSamples[IMU_ACCEL_DOF_X] = 0.0;
    //	    ...
    //
    //	    // Gyro raw
    //	    reading->gyroRawSamples[IMU_GYRO_DOF_ROLL] = 0;
    //	    ...
    //	    // Gyro float
    //	    reading->gyroSamples[IMU_GYRO_DOF_ROLL] = 0.0;
    //	    ...
    //
    //      ...

    return IMU_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local IMU driver functions declared above.
