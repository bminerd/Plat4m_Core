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
// Copyright (c) 2016-2023 Benjamin Minerd
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

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Sensor/Sensor.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/TimeStamp.h>
#include <Plat4m_Core/Accel/Accel.h>
#include <Plat4m_Core/Gyro/Gyro.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType, std::uint32_t nDofAccel, std::uint32_t nDofGyro>
class Imu : public Sensor<Sample>
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
        ERROR_CODE_COMMUNICATION_FAILED
    };

    using Error = ErrorTemplate<ErrorCode>;

    enum MeasurementMode
    {
        MEASUREMENT_MODE_SINGLE,
        MEASUREMENT_MODE_CONTINUOUS
    };

    struct Sample
    {
        TimeStamp timeStamp;
        Accel<ValueType, nDofAccel>::Sample accelSample;
        Gyro<ValueType, nDofGyro>::Sample gyroSample;
    };

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

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Error setConfig(const Config& config)
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

    //--------------------------------------------------------------------------
    Config getConfig() const
    {
        return myConfig;
    }

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Imu() :
        Module(),
        myConfig()
    {
    }

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Imu();

    //--------------------------------------------------------------------------
    // Protected pure virtual methods (deprecated)
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverGetRawMeasurement(RawMeasurement& measurement) = 0;

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

    
};

}; // namespace Plat4m

#endif // PLAT4M_IMU_H
