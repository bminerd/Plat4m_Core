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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file Imu.cpp
/// @author Ben Minerd
/// @date 3/30/16
/// @brief Imu class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Imu.h>

using Plat4m::Imu;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Imu::setTempMeasurementReadyCallback(Callback<>& callback)
{
    myTempMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setRawTempMeasurementReadyCallback(Callback<>& callback)
{
    myRawTempMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setAccelMeasurementReadyCallback(Callback<>& callback)
{
    myAccelMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setRawAccelMeasurementReadyCallback(Callback<>& callback)
{
    myRawAccelMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setGyroMeasurementReadyCallback(Callback<>& callback)
{
    myGyroMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setRawGyroMeasurementReadyCallback(Callback<>& callback)
{
    myRawGyroMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setMagMeasurementReadyCallback(MagMeasurementCallback& callback)
{
    myMagMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setRawMagMeasurementReadyCallback(RawMagMeasurementCallback& callback)
{
    myRawMagMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setMeasurementReadyCallback(MeasurementCallback& callback)
{
    myMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
void Imu::setRawMeasurementReadyCallback(RawMeasurementCallback& callback)
{
    myRawMeasurementCallback = &callback;
}

//------------------------------------------------------------------------------
Imu::Error Imu::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverSetConfig(config);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Config Imu::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
Imu::Error Imu::setAccelMeasurementRange(const float range)
{
    Config config = myConfig;
    config.accelMeasurementRange = range;

    return setConfig(config);
}

//------------------------------------------------------------------------------
Imu::Error Imu::setAccelMeasurementRateHz(const uint32_t rateHz)
{
    Config config = myConfig;
    config.accelMeasurementRateHz = rateHz;

    return setConfig(config);
}

//------------------------------------------------------------------------------
Imu::Error Imu::setGyroMeasurementRange(const float range)
{
    Config config = myConfig;
    config.gyroMeasurementRange = range;

    return setConfig(config);
}

//------------------------------------------------------------------------------
Imu::Error Imu::setGyroMeasurementRateHz(const uint32_t rateHz)
{
    Config config = myConfig;
    config.gyroMeasurementRateHz = rateHz;

    return setConfig(config);
}

//------------------------------------------------------------------------------
Imu::Error Imu::setMagMeasurementRange(const float range)
{
    Config config = myConfig;
    config.magMeasurementRange = range;

    return setConfig(config);
}

//------------------------------------------------------------------------------
Imu::Error Imu::setMagMeasurementRateHz(const uint32_t rateHz)
{
    Config config = myConfig;
    config.accelMeasurementRateHz = rateHz;

    return setConfig(config);
}

//------------------------------------------------------------------------------
Imu::Error Imu::setMeasurementRateHz(const uint32_t rateHz)
{
    // TODO: Implement
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Imu::getTempSensitivity() const
{
    return myTempSensitivity;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Imu::getAccelSensitivity() const
{
    return myAccelSensitivity;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Imu::getGyroSensitivity() const
{
    return myGyroSensitivity;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Imu::getMagSensitivity() const
{
    return myMagSensitivity;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getTempMeasurement(float& tempC)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    float temporaryTempC;
    Error error = driverGetTempMeasurement(temporaryTempC);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        tempC = temporaryTempC;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getRawTempMeasurement(int32_t& temp)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    int32_t temporaryTemp;
    Error error = driverGetRawTempMeasurement(temporaryTemp);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        temp = temporaryTemp;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getAccelMeasurement(AccelMeasurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    AccelMeasurement tempMeasurement;
    Error error = driverGetAccelMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getRawAccelMeasurement(RawAccelMeasurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    RawAccelMeasurement tempMeasurement;
    Error error = driverGetRawAccelMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getGyroMeasurement(GyroMeasurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    GyroMeasurement tempMeasurement;
    Error error = driverGetGyroMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getRawGyroMeasurement(RawGyroMeasurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    RawGyroMeasurement tempMeasurement;
    Error error = driverGetRawGyroMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getMagMeasurement(MagMeasurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    MagMeasurement tempMeasurement;
    Error error = driverGetMagMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getRawMagMeasurement(RawMagMeasurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    RawMagMeasurement tempMeasurement;
    Error error = driverGetRawMagMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getMeasurement(Measurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Measurement tempMeasurement;
    Error error = driverGetMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::getRawMeasurement(RawMeasurement& measurement)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    RawMeasurement tempMeasurement;
    Error error = driverGetRawMeasurement(tempMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement = tempMeasurement;
    }

    return error;
}

//------------------------------------------------------------------------------
void Imu::setOffsetCalibrationCompleteCallback(Callback<>& callback)
{
    myOffsetCalibrationCompleteCallback = &callback;
}

//------------------------------------------------------------------------------
Imu::Error Imu::startOffsetCalibration(const uint32_t nSamples)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverStartOffsetCalibration(nSamples);

    return error;
}

//------------------------------------------------------------------------------
bool Imu::isOffsetCalibrationComplete() const
{
    return (!myIsOffsetCalibrationRunning);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Imu::Imu(const uint32_t accelDof,
         const uint32_t gyroDof,
         const uint32_t magDof) :
	Module(),
	myAccelDof(accelDof),
	myGyroDof(gyroDof),
	myMagDof(magDof),
	myConfig(),
	myTempSensitivity(0.0),
	myTempOffset(0.0),
	myAccelSensitivity(0.0),
	myGyroSensitivity(0.0),
	myMagSensitivity(0.0),
    myTempMeasurementCallback(0),
    myRawTempMeasurementCallback(0),
	myAccelMeasurementCallback(0),
	myRawAccelMeasurementCallback(0),
    myGyroMeasurementCallback(0),
    myRawGyroMeasurementCallback(0),
    myMagMeasurementCallback(0),
    myRawMagMeasurementCallback(0),
    myMeasurementCallback(0),
    myRawMeasurementCallback(0),
    myOffsetCalibrationCompleteCallback(0),
    myNOffsetCalibrationTotalSamples(0),
    myNOffsetCalibrationCurrentSamples(0),
    myIsOffsetCalibrationRunning(false),
    myXOffsetSumAngularVelocityDps(0.0),
    myYOffsetSumAngularVelocityDps(0.0),
    myZOffsetSumAngularVelocityDps(0.0),
    myXOffsetAngularVelocityDps(0.0),
    myYOffsetAngularVelocityDps(0.0),
    myZOffsetAngularVelocityDps(0.0)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Imu::~Imu()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Imu::setTempSensitivity(const float sensitivity)
{
    myTempSensitivity = sensitivity;
}

//------------------------------------------------------------------------------
void Imu::setTempOffset(const float offset)
{
    myTempOffset = offset;
}

//------------------------------------------------------------------------------
void Imu::setAccelSensitivity(const float sensitivity)
{
    myAccelSensitivity = sensitivity;
}

//------------------------------------------------------------------------------
void Imu::setGyroSensitivity(const float sensitivity)
{
    myGyroSensitivity = sensitivity;
}

//------------------------------------------------------------------------------
void Imu::setMagSensitivity(const float sensitivity)
{
    myMagSensitivity = sensitivity;
}

//------------------------------------------------------------------------------
void Imu::tempMeasurementReady()
{
    if (isValidPointer(myTempMeasurementCallback))
    {
        myTempMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::rawTempMeasurementReady()
{
    if (isValidPointer(myRawTempMeasurementCallback))
    {
        myRawTempMeasurementCallback->call();
    }
    else if (isValidPointer(myTempMeasurementCallback))
    {
        myTempMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::accelMeasurementReady()
{
    if (isValidPointer(myAccelMeasurementCallback))
    {
        myAccelMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::rawAccelMeasurementReady()
{
    if (isValidPointer(myRawAccelMeasurementCallback))
    {
        myRawAccelMeasurementCallback->call();
    }
    else if (isValidPointer(myAccelMeasurementCallback))
    {
        myAccelMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::gyroMeasurementReady()
{
    if (isValidPointer(myGyroMeasurementCallback))
    {
        myGyroMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::rawGyroMeasurementReady()
{
    if (isValidPointer(myRawGyroMeasurementCallback))
    {
        myRawGyroMeasurementCallback->call();
    }
    else if (isValidPointer(myGyroMeasurementCallback))
    {
        myGyroMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::magMeasurementReady()
{
    if (isValidPointer(myMagMeasurementCallback))
    {
        myMagMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::rawMagMeasurementReady()
{
    if (isValidPointer(myRawMagMeasurementCallback))
    {
        myRawMagMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::measurementReady()
{
    if (isValidPointer(myMeasurementCallback))
    {
        myMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
void Imu::rawMeasurementReady()
{
    if (isValidPointer(myRawMeasurementCallback))
    {
        myRawMeasurementCallback->call();
    }
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetRawTempMeasurement(int32_t& temp)
{
    // Not implemented by subclass, default behavior

    return Error(ERROR_CODE_ACCEL_NOT_PRESENT);
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetRawAccelMeasurement(RawAccelMeasurement& measurement)
{
    // Not implemented by subclass, default behavior

    return Error(ERROR_CODE_ACCEL_NOT_PRESENT);
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetRawGyroMeasurement(RawGyroMeasurement& measurement)
{
    // Not implemented by subclass, default behavior

    return Error(ERROR_CODE_GYRO_NOT_PRESENT);
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetRawMagMeasurement(RawMagMeasurement& measurement)
{
    // Not implemented by subclass, default behavior

    return Error(ERROR_CODE_MAG_NOT_PRESENT);
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetTempMeasurement(float& tempC)
{
    // Not implemented by subclass, default behavior

    int32_t rawTemp;
    Error error = driverGetRawTempMeasurement(rawTemp);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        tempC = ((float) rawTemp) * myTempSensitivity + myTempOffset;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetAccelMeasurement(AccelMeasurement& measurement)
{
    // Not implemented by subclass, default behavior

    RawAccelMeasurement rawMeasurement;
    Error error = driverGetRawAccelMeasurement(rawMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement.x = ((float) rawMeasurement.x) * myAccelSensitivity;
        measurement.y = ((float) rawMeasurement.y) * myAccelSensitivity;
        measurement.z = ((float) rawMeasurement.z) * myAccelSensitivity;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetGyroMeasurement(GyroMeasurement& measurement)
{
    // Not implemented by subclass, default behavior

    RawGyroMeasurement rawMeasurement;
    Error error = driverGetRawGyroMeasurement(rawMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement.x = ((float) rawMeasurement.x) * myGyroSensitivity;
        measurement.y = ((float) rawMeasurement.y) * myGyroSensitivity;
        measurement.z = ((float) rawMeasurement.z) * myGyroSensitivity;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetMagMeasurement(MagMeasurement& measurement)
{
    // Not implemented by subclass, default behavior

    RawMagMeasurement rawMeasurement;
    Error error = driverGetRawMagMeasurement(rawMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement.x = ((float) rawMeasurement.x) * myMagSensitivity;
        measurement.y = ((float) rawMeasurement.y) * myMagSensitivity;
        measurement.z = ((float) rawMeasurement.z) * myMagSensitivity;
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverGetMeasurement(Measurement& measurement)
{
    // Not implemented by subclass, default behavior

    RawMeasurement rawMeasurement;
    Error error = driverGetRawMeasurement(rawMeasurement);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        measurement.tempC  = rawMeasurement.temp   * myTempSensitivity;
        measurement.xAccelerationG = rawMeasurement.accelX * myAccelSensitivity;
        measurement.yAccelerationG = rawMeasurement.accelY * myAccelSensitivity;
        measurement.zAccelerationG = rawMeasurement.accelZ * myAccelSensitivity;
        measurement.xAngularVelocityDps =
                                    (rawMeasurement.gyroX * myGyroSensitivity) +
                                    myXOffsetAngularVelocityDps;
        measurement.yAngularVelocityDps =
                                    (rawMeasurement.gyroY * myGyroSensitivity) +
                                    myYOffsetAngularVelocityDps;
        measurement.zAngularVelocityDps =
                                    (rawMeasurement.gyroZ * myGyroSensitivity) +
                                    myZOffsetAngularVelocityDps;
        measurement.magX = rawMeasurement.magX * myMagSensitivity;
        measurement.magY = rawMeasurement.magY * myMagSensitivity;
        measurement.magZ = rawMeasurement.magZ * myMagSensitivity;

        if (myIsOffsetCalibrationRunning)
        {
			myXOffsetSumAngularVelocityDps +=
											measurement.xAngularVelocityDps;
			myYOffsetSumAngularVelocityDps +=
											measurement.yAngularVelocityDps;
			myZOffsetSumAngularVelocityDps +=
											measurement.zAngularVelocityDps;

			myNOffsetCalibrationCurrentSamples++;

			if (myNOffsetCalibrationCurrentSamples ==
										   myNOffsetCalibrationTotalSamples)
			{
				myXOffsetAngularVelocityDps =
							-myXOffsetSumAngularVelocityDps /
							((RealNumber) myNOffsetCalibrationTotalSamples);

				myYOffsetAngularVelocityDps =
							-myYOffsetSumAngularVelocityDps /
							((RealNumber) myNOffsetCalibrationTotalSamples);

				myZOffsetAngularVelocityDps =
							-myZOffsetSumAngularVelocityDps /
							((RealNumber) myNOffsetCalibrationTotalSamples);

				myNOffsetCalibrationCurrentSamples = 0;
				myIsOffsetCalibrationRunning = false;
			}
        }
    }

    return error;
}

//------------------------------------------------------------------------------
Imu::Error Imu::driverStartOffsetCalibration(const uint32_t nSamples)
{
    // Not implemented by subclass, default behavior

    myNOffsetCalibrationTotalSamples = nSamples;
    myNOffsetCalibrationCurrentSamples = 0;

    myIsOffsetCalibrationRunning = true;

    myXOffsetSumAngularVelocityDps = 0.0;
    myYOffsetSumAngularVelocityDps = 0.0;
    myZOffsetSumAngularVelocityDps = 0.0;

    return Error(ERROR_CODE_NONE);
}
