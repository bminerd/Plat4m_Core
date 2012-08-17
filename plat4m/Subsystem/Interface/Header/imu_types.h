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

/*
 * imu_types.h
 *
 *  Created on: Dec 31, 2011
 *      Author: ben
 */

#ifndef IMU_TYPES_H_
#define IMU_TYPES_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

//#define IMU_ACCEL_DOF_MAX 	(3u)
//
//#define IMU_GYRO_DOF_MAX 	(3u)
//
//#define IMU_DMC_DOF_MAX		(3u)

/*------------------------------------------------------------------------------
 * Typedefs
 *----------------------------------------------------------------------------*/

typedef enum
{
	// Accelerometer
	IMU_ACCEL_DOF_X = 0,
	IMU_ACCEL_DOF_Y,
	IMU_ACCEL_DOF_Z,

	// Do not place values below!
	IMU_ACCEL_DOF_MAX
} imu_accel_dof_e;

typedef enum
{
	// Gyroscope
	IMU_GYRO_DOF_ROLL,
	IMU_GYRO_DOF_PITCH,
	IMU_GYRO_DOF_YAW,

	// Do not place values below!
	IMU_GYRO_DOF_MAX
} imu_gyro_dof_e;

typedef enum
{
	// Digital compass
	IMU_DMC_DOF_AZIMUTH,
	IMU_DMC_DOF_ELEVATION,
	IMU_DMC_DOF_BANK,

	// Do not place values below!
	IMU_DMC_DOF_MAX
} imu_dmc_sample_e;

typedef uint16_t imu_raw_sample_t;
typedef float imu_sample_t;

typedef struct
{
	imu_raw_sample_t accelRawSamples[IMU_MAX_ACCEL_DOF];
	imu_sample_t accelSamples[IMU_MAX_ACCEL_DOF];
	imu_raw_sample_t gyroRawSamples[IMU_MAX_GYRO_DOF];
	imu_sample_t gyroSamples[IMU_MAX_GYRO_DOF];
	imu_raw_sample_t dmcRawSamples[IMU_MAX_DMC_DOF];
	imu_sample_t dmcSamples[IMU_MAX_DMC_DOF];
	uint32_t timeStamp;
} imu_measurement_t;

#endif /* IMU_TYPES_H_ */
