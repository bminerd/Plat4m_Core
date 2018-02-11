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
/// @file Imu.h
/// @author Ben Minerd
/// @date 3/30/2016
/// @brief Imu class header file.
///

#ifndef PLAT4M_IMU_H
#define PLAT4M_IMU_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/Accel.h>
#include <Plat4m_Core/Gyro.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Imu : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_NOT_CONFIGURED,
        ERROR_CODE_COMMUNICATION_FAILED,
        ERROR_CODE_ACCEL_MEASUREMENT_RANGE_INVALID,
        ERROR_CODE_ACCEL_MEASUREMENT_RATE_INVALID,
        ERROR_CODE_GYRO_MEASUREMENT_RANGE_INVALID,
        ERROR_CODE_GYRO_MEASUREMENT_RATE_INVALID,
        ERROR_CODE_ACCEL_NOT_PRESENT,
        ERROR_CODE_GYRO_NOT_PRESENT,
        ERROR_CODE_MAG_NOT_PRESENT
    };

    enum MeasurementMode
    {
        MEASUREMENT_MODE_SINGLE,
        MEASUREMENT_MODE_CONTINUOUS
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> AccelMeasurementReadyCallback;

    typedef Callback<> RawAccelMeasurementReadyCallback;

    typedef Callback<> GyroMeasurementReadyCallback;

    typedef Callback<> RawGyroMeasurementReadyCallback;

    typedef Callback<> MagMeasurementCallback;

    typedef Callback<> RawMagMeasurementCallback;

    typedef Callback<> MeasurementCallback;

    typedef Callback<> RawMeasurementCallback;

    typedef Callback<> GyroOffsetCalibrationCompleteCallback;

    struct Config
    {
        MeasurementMode measurementMode;
        AccelerationG accelMeasurementRangeAccelerationG;
        uint32_t accelMeasurementRateHz;
        AngularVelocityDps gyroMeasurementRangeAngularVelocityDps;
        uint32_t gyroMeasurementRateHz;
        float magMeasurementRange;
        uint32_t magMeasurementRateHz;
    };

    struct AccelMeasurement
    {
    	AccelerationG x;
    	AccelerationG y;
    	AccelerationG z;
    };

    struct RawAccelMeasurement
    {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct GyroMeasurement
    {
    	AngularVelocityDps x;
    	AngularVelocityDps y;
    	AngularVelocityDps z;
    };

    struct RawGyroMeasurement
    {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct MagMeasurement
    {
        float x;
        float y;
        float z;
    };

    struct RawMagMeasurement
    {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct Measurement
    {
        AccelerationG xAccelerationG;
        AccelerationG yAccelerationG;
        AccelerationG zAccelerationG;
        AngularVelocityDps xAngularVelocityDps;
        AngularVelocityDps yAngularVelocityDps;
        AngularVelocityDps zAngularVelocityDps;
        float magX;
        float magY;
        float magZ;
        float tempC;
    };

    struct RawMeasurement
    {
        int32_t temp;
        int32_t accelX;
        int32_t accelY;
        int32_t accelZ;
        int32_t gyroX;
        int32_t gyroY;
        int32_t gyroZ;
        int32_t magX;
        int32_t magY;
        int32_t magZ;
    };

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setTempMeasurementReadyCallback(Callback<>& callback);

    void setRawTempMeasurementReadyCallback(Callback<>& callback);

    void setAccelMeasurementReadyCallback(Callback<>& callback);

    void setRawAccelMeasurementReadyCallback(Callback<>& callback);

    void setGyroMeasurementReadyCallback(Callback<>& callback);

    void setRawGyroMeasurementReadyCallback(Callback<>& callback);

    void setMagMeasurementReadyCallback(Callback<>& callback);

    void setRawMagMeasurementReadyCallback(Callback<>& callback);

    void setMeasurementReadyCallback(Callback<>& callback);

    void setRawMeasurementReadyCallback(Callback<>& callback);

    Error setConfig(const Config& config);

    Config getConfig() const;

    Error setAccelConfig(const Accel::Config& config);

    Accel::Config getAccelConfig() const;

    Error setGyroConfig(const Gyro::Config& config);

    Gyro::Config getGyroConfig() const;

    Error setAccelMeasurementRange(const AccelerationG range);

    Error setAccelMeasurementRateHz(const uint32_t rateHz);

    Error setGyroMeasurementRange(const AngularVelocityDps range);

    Error setGyroMeasurementRateHz(const uint32_t rateHz);

    Error setMagMeasurementRange(const float range);

    Error setMagMeasurementRateHz(const uint32_t rateHz);

    Error setMeasurementRateHz(const uint32_t rateHz);

    float getTempSensitivity() const;

    float getAccelSensitivity() const;

    float getGyroSensitivity() const;

    float getMagSensitivity() const;

    Error getTempMeasurement(float& tempC);

    Error getRawTempMeasurement(int32_t& temp);

    Error getAccelMeasurement(AccelMeasurement& measurement);

    Error getRawAccelMeasurement(RawAccelMeasurement& measurement);

    Error getGyroMeasurement(GyroMeasurement& measurement);

    Error getRawGyroMeasurement(RawGyroMeasurement& measurement);

    Error getMagMeasurement(MagMeasurement& measurement);

    Error getRawMagMeasurement(RawMagMeasurement& measurement);

    Error getMeasurement(Measurement& measurement);

    Error getRawMeasurement(RawMeasurement& measurement);

    void setOffsetCalibrationCompleteCallback(Callback<>& callback);

    Error startOffsetCalibration(const uint32_t nSamples);

    bool isOffsetCalibrationComplete() const;

protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    Imu();
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------
    
    virtual ~Imu();
    
    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void setTempSensitivity(const RealNumber sensitivity);

    void setTempOffset(const RealNumber offset);

    void setAccelSensitivity(const RealNumber sensitivity);

    void setGyroSensitivity(const RealNumber sensitivity);

    void setMagSensitivity(const RealNumber sensitivity);

    void tempMeasurementReady();

    void rawTempMeasurementReady();

    void accelMeasurementReady();

    void rawAccelMeasurementReady();

    void gyroMeasurementReady();

    void rawGyroMeasurementReady();

    void magMeasurementReady();

    void rawMagMeasurementReady();

    void measurementReady();

    void rawMeasurementReady();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    RealNumber myTempSensitivity;
    RealNumber myTempOffset;
    RealNumber myAccelSensitivity;
    RealNumber myGyroSensitivity;
    RealNumber myMagSensitivity;

    Callback<>* myTempMeasurementCallback;
    Callback<>* myRawTempMeasurementCallback;

    Callback<>* myAccelMeasurementCallback;
    Callback<>* myRawAccelMeasurementCallback;

    Callback<>* myGyroMeasurementCallback;
    Callback<>* myRawGyroMeasurementCallback;

    Callback<>* myMagMeasurementCallback;
    Callback<>* myRawMagMeasurementCallback;

    Callback<>* myMeasurementCallback;
    Callback<>* myRawMeasurementCallback;

    Callback<>* myOffsetCalibrationCompleteCallback;

    uint32_t myNOffsetCalibrationTotalSamples;
    uint32_t myNOffsetCalibrationCurrentSamples;

    bool myIsOffsetCalibrationRunning;

    AngularVelocityDps myXOffsetSumAngularVelocityDps;
    AngularVelocityDps myYOffsetSumAngularVelocityDps;
    AngularVelocityDps myZOffsetSumAngularVelocityDps;

    AngularVelocityDps myXOffsetAngularVelocityDps;
    AngularVelocityDps myYOffsetAngularVelocityDps;
    AngularVelocityDps myZOffsetAngularVelocityDps;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverGetRawMeasurement(RawMeasurement& measurement) = 0;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverGetRawTempMeasurement(int32_t& temp);

    virtual Error driverGetRawAccelMeasurement(
                                              RawAccelMeasurement& measurement);

    virtual Error driverGetRawGyroMeasurement(RawGyroMeasurement& measurement);

    virtual Error driverGetRawMagMeasurement(RawMagMeasurement& measurement);

    virtual Error driverGetTempMeasurement(float& tempC);

    virtual Error driverGetAccelMeasurement(AccelMeasurement& measurement);

    virtual Error driverGetGyroMeasurement(GyroMeasurement& measurement);

    virtual Error driverGetMagMeasurement(MagMeasurement& measurement);

    virtual Error driverGetMeasurement(Measurement& measurement);

    virtual Error driverStartOffsetCalibration(const uint32_t nSamples);
};

}; // namespace Plat4m

#endif // PLAT4M_IMU_H
