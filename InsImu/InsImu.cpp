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

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>

#include <Plat4m_Core/InsImu/InsImu.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Math/Plat4mMath.h>

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
    gyroMeasurementReadyCallback();
    accelMeasurementReadyCallback();
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
Plat4m::Controls::KalmanFilter<Plat4m::RealNumber, 4, 4, 4>&
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
    rotatedAngularVelocityDpsVector =
                                    myRotationMatrix * angularVelocityDpsVector;

    Math::Quaternion<RealNumber> quaternion =
              Math::eulerAnglesToQuaternion(rotatedAngularVelocityDpsVector(0),
                                            rotatedAngularVelocityDpsVector(1),
                                            rotatedAngularVelocityDpsVector(2));
    Eigen::Matrix<RealNumber, 4, 1> uVector;
    uVector(0) = quaternion.x();
    uVector(1) = quaternion.y();
    uVector(2) = quaternion.z();
    uVector(3) = quaternion.w();

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

    Eigen::Matrix<RealNumber, 4, 1> measurementVector;

    AngleDegrees maxAccelAngleDegrees = 85.0;

    // If either the pitch or roll angle is beyond the max accel angle, the
    // calculated angle is unreliable due to being in the near-infinite portion
    // of arctan. Use the predicted angles as the update instead.
    if ((abs(pitchAngleDegrees) > maxAccelAngleDegrees) &&
        (abs(rollAngleDegrees)  > maxAccelAngleDegrees))
    {
//        measurementVector(0) = myKalmanFilter.getXVector()(0);
//        measurementVector(1) = myKalmanFilter.getXVector()(1);
    }
    else
    {
        AngleRadians yawAngleRadians = 0.0;
        Math::Quaternion<RealNumber> quaternion =
                                Math::eulerAnglesToQuaternion(rollAngleRadians,
                                                              pitchAngleRadians,
                                                              yawAngleRadians);
        measurementVector(0) = quaternion.x();
        measurementVector(1) = quaternion.y();
        measurementVector(2) = quaternion.z();
        measurementVector(3) = quaternion.w();
    }

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
    measurement.quaternion.x() = myKalmanFilter.getXVector()(0);
    measurement.quaternion.y() = myKalmanFilter.getXVector()(1);
    measurement.quaternion.z() = myKalmanFilter.getXVector()(2);
    measurement.quaternion.w() = myKalmanFilter.getXVector()(3);

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
