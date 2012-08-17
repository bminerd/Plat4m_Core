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
 * @file imu_interface.h
 * @author Ben Minerd
 * @date 11/23/11
 * @brief
 */

#ifndef _IMU_INTERFACE_H_
#define _IMU_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 *
 */
#define IMU_ACCEL_DOF (3u)

/**
 *
 */
#define IMU_GYRO_DOF (3u)

/**
 *
 */
#define IMU_MAG_DOF (3u)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef enum _imu_accel_dof_e_
{
    // Accelerometer
    IMU_ACCEL_DOF_X = 0,
    IMU_ACCEL_DOF_Y,
    IMU_ACCEL_DOF_Z
} imu_accel_dof_e;

/**
 *
 */
typedef enum _imu_gyro_dof_e_
{
    // Gyroscope
    IMU_GYRO_DOF_ROLL,
    IMU_GYRO_DOF_PITCH,
    IMU_GYRO_DOF_YAW
} imu_gyro_dof_e;

/**
 *
 */
typedef enum _imu_mag_dof_e
{
    // Magnetometer
    IMU_MAG_DOF_AZIMUTH,
    IMU_MAG_DOF_ELEVATION,
    IMU_MAG_DOF_BANK
} imu_mag_dof_e;

/**
 *
 */
typedef enum _imu_error_e_
{
    IMU_ERROR_NONE = 0,
    IMU_ERROR_TIMEOUT,

    // Do not place values below!
    IMU_ERROR_COUNT
} imu_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 *
 */
typedef uint16_t imu_raw_sample_t;

/**
 *
 */
typedef float imu_sample_t;

/**
 *
 */
typedef struct _imu_reading_t_
{
    imu_raw_sample_t accelRawSamples[IMU_ACCEL_DOF];
    imu_sample_t accelSamples[IMU_ACCEL_DOF];
    imu_raw_sample_t gyroRawSamples[IMU_GYRO_DOF];
    imu_sample_t gyroSamples[IMU_GYRO_DOF];
    imu_raw_sample_t magRawSamples[IMU_MAG_DOF];
    imu_sample_t magSamples[IMU_MAG_DOF];
    uint32_t timeStamp;
} imu_reading_t;

/**
 *
 */
typedef void imu_reading_callback_t(imu_reading_t* reading);

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 *
 */
extern void imuInit(void);

/**
 *
 */
extern imu_error_e imuRead(imu_reading_t* reading);

#endif // _IMU_INTERFACE_H_
