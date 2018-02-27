//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2016 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file InsImu.cpp
/// @author Ben Minerd
/// @date 3/30/2016
/// @brief InsImu class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Eigen/Dense>
#include <Eigen/SVD>

#include <Plat4m_Core/InsImu/InsImu.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/System.h>

using Plat4m::InsImu;
using Plat4m::Ins;
using Plat4m::Controls::KalmanFilter;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InsImu::InsImu(Imu& imu) :
    Ins(),
    myImu(imu),
	myRotationMatrix(),
    myIntegratedYawAngleDegrees(0.0),
    myIntegratedPitchAngleDegrees(0.0),
    myIntegratedRollAngleDegrees(0.0),
    myYawAngleDegrees(0.0),
    myPitchAngleDegrees(0.0),
    myRollAngleDegrees(0.0),
    myYawAngularVelocityDps(0.0),
    myPitchAngularVelocityDps(0.0),
    myRollAngularVelocityDps(0.0),
    myEulerAnglesVector(),
    myQuaternion(),
	myImuAccelMeasurement(),
	myImuGyroMeasurement(),
	myKalmanFilter()
{
	RealNumber dt = 1.0/104.0;

	myKalmanFilter.getFMatrix() <<
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0;

	myKalmanFilter.getBMatrix() <<
         dt, 0.0, 0.0,
        0.0,  dt, 0.0,
        0.0, 0.0,  dt;

	myKalmanFilter.getHMatrix() <<
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0;

	RealNumber p = 1.0;

	myKalmanFilter.getPMatrix() <<
          p, 0.0, 0.0,
        0.0,   p, 0.0,
        0.0, 0.0,   p;

	RealNumber q = 0.05;

    myKalmanFilter.getQMatrix() <<
          q, 0.0, 0.0,
        0.0,   q, 0.0,
        0.0, 0.0,   q;

    RealNumber r = 0.6;

    myKalmanFilter.getRMatrix() <<
          r, 0.0, 0.0,
        0.0,   r, 0.0,
        0.0, 0.0,   r;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InsImu::~InsImu()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void InsImu::imuMeasurementReadyCallback()
{
    Imu::Measurement measurement;

    Imu::Error error = myImu.getMeasurement(measurement);

    Math::Vector<RealNumber, 3> accelerationGVector;
    accelerationGVector(0) = measurement.xAccelerationG;
    accelerationGVector(1) = measurement.yAccelerationG;
    accelerationGVector(2) = measurement.zAccelerationG;

    Math::Vector<RealNumber, 3> rotatedAcclerationGVector;
    rotatedAcclerationGVector = myRotationMatrix * accelerationGVector;

    Math::Vector<RealNumber, 3> angularVelocityDpsVector;
    angularVelocityDpsVector(0) = measurement.xAngularVelocityDps;
    angularVelocityDpsVector(1) = measurement.yAngularVelocityDps;
    angularVelocityDpsVector(2) = measurement.zAngularVelocityDps;

    Math::Vector<RealNumber, 3> rotatedAngularVelocityDpsVector;
    rotatedAngularVelocityDpsVector = myRotationMatrix *
    								  	  angularVelocityDpsVector;

    myYawAngularVelocityDps   = rotatedAngularVelocityDpsVector(2);
    myPitchAngularVelocityDps = rotatedAngularVelocityDpsVector(1);
    myRollAngularVelocityDps  = rotatedAngularVelocityDpsVector(0);

    RealNumber gyroDt = 1.0 / (myImu.getConfig().gyroMeasurementRateHz);

    myIntegratedYawAngleDegrees   += myYawAngularVelocityDps   * gyroDt;
    myIntegratedPitchAngleDegrees += myPitchAngularVelocityDps * gyroDt;
    myIntegratedRollAngleDegrees  += myRollAngularVelocityDps  * gyroDt;

    AngleRadians pitchAngleRadians = atan2(rotatedAcclerationGVector(0),
                                      	   -rotatedAcclerationGVector(2));
    AngleDegrees pitchAngleDegrees = radiansToDegrees(pitchAngleRadians);

    AngleRadians rollAngleRadians = atan2(-rotatedAcclerationGVector(1),
                                     	  -rotatedAcclerationGVector(2));
    AngleDegrees rollAngleDegrees = radiansToDegrees(rollAngleRadians);

    AngleDegrees lastPitchAngleDegrees = myPitchAngleDegrees;
    AngleDegrees lastRollAngleDegrees  = myRollAngleDegrees;

    if (myImu.isOffsetCalibrationComplete())
    {
        myYawAngleDegrees = myIntegratedYawAngleDegrees;

        myPitchAngleDegrees =
               0.8*pitchAngleDegrees +
               0.2*(lastPitchAngleDegrees + myPitchAngularVelocityDps * gyroDt);
        myRollAngleDegrees =
                 0.8*rollAngleDegrees +
                 0.2*(lastRollAngleDegrees + myRollAngularVelocityDps * gyroDt);
    }
    else
    {
        myYawAngleDegrees   = myIntegratedYawAngleDegrees;
        myPitchAngleDegrees = pitchAngleDegrees;
        myRollAngleDegrees  = rollAngleDegrees;
    }

    if (myYawAngleDegrees > 180.0)
    {
    	myYawAngleDegrees -= 360.0;
    }
    else if (myYawAngleDegrees < -180.0)
    {
    	myYawAngleDegrees += 360.0;
    }
}

//------------------------------------------------------------------------------
void InsImu::accelMeasurementReadyCallback()
{
	myImu.getAccelMeasurement(myImuAccelMeasurement);

    setImuAccelMeasurement(myImuAccelMeasurement);
}

//------------------------------------------------------------------------------
void InsImu::gyroMeasurementReadyCallback()
{
	myImu.getGyroMeasurement(myImuGyroMeasurement);

    setImuGyroMeasurement(myImuGyroMeasurement);
}

//------------------------------------------------------------------------------
void InsImu::setRotationAngles(const AngleRadians xRotationAngleRadians,
                               const AngleRadians yRotationAngleRadians,
                               const AngleRadians zRotationAngleRadians)
{
    RealNumber xRotationMatrixValues[3][3] =
    {
        {1.0,                         0.0,                         0.0},
        {0.0,  cos(xRotationAngleRadians), -sin(xRotationAngleRadians)},
        {0.0,  sin(xRotationAngleRadians),  cos(xRotationAngleRadians)}
    };
    Math::RotationMatrix<RealNumber> xRotationMatrix(xRotationMatrixValues);

    RealNumber yRotationMatrixValues[3][3] =
    {
        { cos(yRotationAngleRadians), 0.0,  sin(yRotationAngleRadians)},
        {                        0.0, 1.0,                         0.0},
        {-sin(yRotationAngleRadians), 0.0,  cos(yRotationAngleRadians)}
    };
    Math::RotationMatrix<RealNumber> yRotationMatrix(yRotationMatrixValues);

    RealNumber zRotationMatrixValues[3][3] =
    {
        { cos(zRotationAngleRadians), -sin(zRotationAngleRadians), 0.0},
        { sin(zRotationAngleRadians),  cos(zRotationAngleRadians), 0.0},
        {                        0.0,                         0.0, 1.0}
    };
    Math::RotationMatrix<RealNumber> zRotationMatrix(zRotationMatrixValues);

    myRotationMatrix = xRotationMatrix * yRotationMatrix * zRotationMatrix;
}

//------------------------------------------------------------------------------
Plat4m::Controls::KalmanFilter<Plat4m::RealNumber, 3, 3, 3>&
                                                       InsImu::getKalmanFilter()
{
    return myKalmanFilter;
}

//------------------------------------------------------------------------------
void InsImu::setImuGyroMeasurement(
                                 const Imu::GyroMeasurement& imuGyroMeasurement)
{
    myImuGyroMeasurement = imuGyroMeasurement;

    Math::Vector<RealNumber, 3> angularVelocityDpsVector;
    angularVelocityDpsVector(0) = myImuGyroMeasurement.xAngularVelocityDps;
    angularVelocityDpsVector(1) = myImuGyroMeasurement.yAngularVelocityDps;
    angularVelocityDpsVector(2) = myImuGyroMeasurement.zAngularVelocityDps;

    Math::Vector<RealNumber, 3> rotatedAngularVelocityDpsVector;
    rotatedAngularVelocityDpsVector = myRotationMatrix *
                                          angularVelocityDpsVector;

    Eigen::Matrix<RealNumber, 3, 1> uVector;
    uVector(0) = rotatedAngularVelocityDpsVector(0);
    uVector(1) = rotatedAngularVelocityDpsVector(1);
    uVector(2) = rotatedAngularVelocityDpsVector(2);

    myKalmanFilter.predict(uVector);
}

//------------------------------------------------------------------------------
void InsImu::setImuAccelMeasurement(
                               const Imu::AccelMeasurement& imuAccelMeasurement)
{
    myImuAccelMeasurement = imuAccelMeasurement;

    Math::Vector<RealNumber, 3> accelerationGVector;
    accelerationGVector(0) = myImuAccelMeasurement.xAccelerationG;
    accelerationGVector(1) = myImuAccelMeasurement.yAccelerationG;
    accelerationGVector(2) = myImuAccelMeasurement.zAccelerationG;

    // Rotate the accelerations into the orientation of the IMU
    Math::Vector<RealNumber, 3> rotatedAcclerationGVector;
    rotatedAcclerationGVector = myRotationMatrix * accelerationGVector;

    AngleRadians pitchAngleRadians = atan2(rotatedAcclerationGVector(0),
                                           -rotatedAcclerationGVector(2));
    AngleDegrees pitchAngleDegrees = radiansToDegrees(pitchAngleRadians);

    AngleRadians rollAngleRadians = atan2(-rotatedAcclerationGVector(1),
                                          -rotatedAcclerationGVector(2));
    AngleDegrees rollAngleDegrees = radiansToDegrees(rollAngleRadians);

    Eigen::Matrix<RealNumber, 3, 1> measurementVector;

    RealNumber maxAccelAngle = 85.0;

    // If either the pitch or roll angle is beyond the max accel angle, the
    // calculated angle is unreliable due to being in the near-infinite portion
    // of arctan. Use the predicted angles as the update instead.
    if ((abs(pitchAngleDegrees) > maxAccelAngle) &&
        (abs(rollAngleDegrees)  > maxAccelAngle))
    {
        measurementVector(0) = myKalmanFilter.getXVector()(0);
        measurementVector(1) = myKalmanFilter.getXVector()(1);
    }
    else
    {
        measurementVector(0) = rollAngleDegrees;
        measurementVector(1) = pitchAngleDegrees;
    }

    // Propagate the gyro yaw angle integration, we have no "truth" sensor
    measurementVector(2) = myKalmanFilter.getXVector()(2);

    myKalmanFilter.update(measurementVector);

    measurementReady();
}

//------------------------------------------------------------------------------
// Private methods implemented from Ins
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Ins::Error InsImu::driverSetConfig(const Config& config)
{
    return Ins::Error(Ins::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Ins::Error InsImu::driverGetMeasurement(Measurement& measurement)
{
	measurement.rotationXAngleDegrees = myKalmanFilter.getXVector()(0);
	measurement.rotationYAngleDegrees = myKalmanFilter.getXVector()(1);
	measurement.rotationZAngleDegrees = myKalmanFilter.getXVector()(2);
	measurement.rotationRateXAngularVelocityDps =
	                                             myKalmanFilter.getUVector()(0);
	measurement.rotationRateYAngularVelocityDps =
	                                             myKalmanFilter.getUVector()(1);
	measurement.rotationRateZAngularVelocityDps =
	                                             myKalmanFilter.getUVector()(2);

    return Ins::Error(Ins::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Ins::Error InsImu::driverGetEulerAnglesDegrees(AngleDegrees& yawAngleDegrees,
                                               AngleDegrees& pitchAngleDegrees,
                                               AngleDegrees& rollAngleDegrees)
{
    yawAngleDegrees = myYawAngleDegrees;
    pitchAngleDegrees = myPitchAngleDegrees;
    rollAngleDegrees  = myRollAngleDegrees;

    return Ins::Error(Ins::ERROR_CODE_NONE);
}
