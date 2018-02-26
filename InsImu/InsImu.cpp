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
//	myImu.setAccelMeasurementReadyCallback(
//				  createCallback(this, &InsImu::accelMeasurementReadyCallback));
//	myImu.setGyroMeasurementReadyCallback(
//				   createCallback(this, &InsImu::gyroMeasurementReadyCallback));

	RealNumber dt = 1.0/104.0;

	myKalmanFilter.getFMatrix() <<
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0;

	myKalmanFilter.getBMatrix() <<
         dt, 0.0, 0.0,
        0.0,  dt, 0.0,
        0.0, 0.0,  dt,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0;

	myKalmanFilter.getHMatrix() <<
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 0.0,
        0.0, 0.0, 0.0;

	RealNumber p = 1.0;

	myKalmanFilter.getPMatrix() <<
          p, 0.0, 0.0, 0.0, 0.0,
        0.0,   p, 0.0, 0.0, 0.0,
        0.0, 0.0,   p, 0.0, 0.0,
        0.0, 0.0, 0.0,   p, 0.0,
        0.0, 0.0, 0.0, 0.0,   p;

	RealNumber q = 0.05;

    myKalmanFilter.getQMatrix() <<
          q, 0.0, 0.0, 0.0, 0.0,
        0.0,   q, 0.0, 0.0, 0.0,
        0.0, 0.0,   q, 0.0, 0.0,
        0.0, 0.0, 0.0,   q, 0.0,
        0.0, 0.0, 0.0, 0.0,   q;

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

    AngleRadians pitchAngleRadians =
                                    atan2(myImuAccelMeasurement.xAccelerationG,
                                          myImuAccelMeasurement.zAccelerationG);
    AngleDegrees pitchAngleDegrees = radiansToDegrees(pitchAngleRadians);

    AngleRadians rollAngleRadians = atan2(myImuAccelMeasurement.yAccelerationG,
                                          myImuAccelMeasurement.zAccelerationG);
    AngleDegrees rollAngleDegrees = radiansToDegrees(rollAngleRadians);

    AngleDegrees yawAngleDegrees = 0.0;

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
	    measurementVector(2) = myKalmanFilter.getXVector()(2);
	}
	else
	{
	    measurementVector(0) = pitchAngleDegrees;
	    measurementVector(1) = rollAngleDegrees;
	    measurementVector(2) = yawAngleDegrees;
	}

	TimeMs startTimeMs = System::getTimeMs();

	myKalmanFilter.update(measurementVector);

	TimeMs deltaTimeMs = System::getTimeMs() - startTimeMs;

	measurementReady();
}

//------------------------------------------------------------------------------
void InsImu::gyroMeasurementReadyCallback()
{
	myImu.getGyroMeasurement(myImuGyroMeasurement);

	Eigen::Matrix<RealNumber, 3, 1> uVector;
	uVector(0) = myImuGyroMeasurement.xAngularVelocityDps;
	uVector(1) = myImuGyroMeasurement.yAngularVelocityDps;
	uVector(2) = myImuGyroMeasurement.zAngularVelocityDps;

	TimeMs startTimeMs = System::getTimeMs();

	myKalmanFilter.predict(uVector);

//    Math::Vector<RealNumber, 3> accelerationGVector;
//    accelerationGVector(0) = myImuAccelMeasurement.xAccelerationG;
//    accelerationGVector(1) = myImuAccelMeasurement.yAccelerationG;
//    accelerationGVector(2) = myImuAccelMeasurement.zAccelerationG;
//
//    Math::Vector<RealNumber, 3> rotatedAcclerationGVector;
//    rotatedAcclerationGVector = myRotationMatrix * accelerationGVector;
//
//    Math::Vector<RealNumber, 3> angularVelocityDpsVector;
//    angularVelocityDpsVector(0) = myImuGyroMeasurement.xAngularVelocityDps;
//    angularVelocityDpsVector(1) = myImuGyroMeasurement.yAngularVelocityDps;
//    angularVelocityDpsVector(2) = myImuGyroMeasurement.zAngularVelocityDps;
//
//    Math::Vector<RealNumber, 3> rotatedAngularVelocityDpsVector;
//    rotatedAngularVelocityDpsVector = myRotationMatrix *
//    								  	  angularVelocityDpsVector;
//
//    myYawAngularVelocityDps   = rotatedAngularVelocityDpsVector(2);
//    myPitchAngularVelocityDps = rotatedAngularVelocityDpsVector(1);
//    myRollAngularVelocityDps  = rotatedAngularVelocityDpsVector(0);
//
//    RealNumber gyroDt = 1.0 / (myImu.getConfig().gyroMeasurementRateHz);
//
//    myIntegratedYawAngleDegrees   += myYawAngularVelocityDps   * gyroDt;
//    myIntegratedPitchAngleDegrees += myPitchAngularVelocityDps * gyroDt;
//    myIntegratedRollAngleDegrees  += myRollAngularVelocityDps  * gyroDt;
//
//    AngleRadians pitchAngleRadians = atan2(rotatedAcclerationGVector(0),
//                                      	   rotatedAcclerationGVector(2));
//    AngleDegrees pitchAngleDegrees = radiansToDegrees(pitchAngleRadians);
//
//    AngleRadians rollAngleRadians = atan2(rotatedAcclerationGVector(1),
//                                     	  rotatedAcclerationGVector(2));
//    AngleDegrees rollAngleDegrees = radiansToDegrees(rollAngleRadians);
//
//    AngleDegrees lastPitchAngleDegrees = myPitchAngleDegrees;
//    AngleDegrees lastRollAngleDegrees  = myRollAngleDegrees;
//
//    if (myImu.isOffsetCalibrationComplete())
//    {
//        myYawAngleDegrees = myIntegratedYawAngleDegrees;
//
//        myPitchAngleDegrees =
//               0.8*pitchAngleDegrees +
//               0.2*(lastPitchAngleDegrees + myPitchAngularVelocityDps * gyroDt);
//        myRollAngleDegrees =
//                 0.8*rollAngleDegrees +
//                 0.2*(lastRollAngleDegrees + myRollAngularVelocityDps * gyroDt);
//    }
//    else
//    {
//        myYawAngleDegrees   = myIntegratedYawAngleDegrees;
//        myPitchAngleDegrees = pitchAngleDegrees;
//        myRollAngleDegrees  = rollAngleDegrees;
//    }
//
//    if (myYawAngleDegrees > 180.0)
//    {
//    	myYawAngleDegrees -= 360.0;
//    }
//    else if (myYawAngleDegrees < -180.0)
//    {
//    	myYawAngleDegrees += 360.0;
//    }
//
//    measurementReady();
}

//------------------------------------------------------------------------------
Plat4m::Controls::KalmanFilter<Plat4m::RealNumber, 5, 3, 3>& InsImu::getKalmanFilter()
{
    return myKalmanFilter;
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
	                                             myKalmanFilter.getXVector()(3);
	measurement.rotationRateYAngularVelocityDps =
	                                             myKalmanFilter.getXVector()(4);
	measurement.rotationRateZAngularVelocityDps =
	                                             myKalmanFilter.getXVector()(5);

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
