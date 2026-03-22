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
// Copyright (c) 2024 Benjamin Minerd
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
/// @file Sensor.h
/// @author Ben Minerd
/// @date 11/11/2024
/// @brief Sensor class header file.
///

#ifndef PLAT4M_SENSOR_H
#define PLAT4M_SENSOR_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>
#include <cstring>

#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/Sensor/SensorBase.h>
#include <Plat4m_Core/Sensor/SensorRawSample.h>
#include <Plat4m_Core/Sensor/SensorSample.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType, std::uint32_t nDof>
class Sensor : public SensorBase
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    using RawSampleCallback = Callback<void, const SensorRawSample<nDof>&>;

    using SampleCallback = Callback<void, const SensorSample<ValueType, nDof>&>;

    struct Traits
    {
        ValueType scaleFactors[nDof];

        //----------------------------------------------------------------------
        // Public constructors
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        Traits() :
            scaleFactors{0}
        {
        }

        //----------------------------------------------------------------------
        Traits(const ValueType (&scaleFactorsIn)[nDof]) :
            scaleFactors()
        {
            std::memcpy(scaleFactors,
                        scaleFactorsIn,
                        sizeof(scaleFactorsIn));
        }
    };

    struct Config
    {
        std::uint32_t outputRateHz;

        //----------------------------------------------------------------------
        // Public constructors
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        Config() :
            outputRateHz(0)
        {
        }

        //----------------------------------------------------------------------
        Config(const std::uint32_t outputRateHzIn) :
            outputRateHz(outputRateHzIn)
        {
        }
    };

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    const Traits& getTraits() const
    {
        return myTraits;
    }

    //--------------------------------------------------------------------------
    Error setSensorConfig(const Config& config)
    {
        Error error = subclassSetSensorConfig(config);

        if (error.getCode() != ERROR_CODE_NONE)
        {
            return error;
        }

        myConfig = config;

        return Error(ERROR_CODE_NONE);
    }

    //--------------------------------------------------------------------------
    const Config& getSensorConfig() const
    {
        return myConfig;
    }

    //--------------------------------------------------------------------------
    void setRawSampleCallback(RawSampleCallback& callback)
    {
        myCurrentRawSampleCallback = &callback;
    }

    //--------------------------------------------------------------------------
    void setSampleCallback(SampleCallback& callback)
    {
        myCurrentSampleCallback = &callback;
    }

    //--------------------------------------------------------------------------
    const SensorRawSample<nDof>& getCurrentRawSample() const
    {
        return myCurrentRawSample;
    }

    //--------------------------------------------------------------------------
    const SensorSample<ValueType, nDof>& getCurrentSample() const
    {
        return myCurrentSample;
    }

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Sensor() :
        SensorBase(),
        myTraits(),
        myConfig(),
        myCurrentRawSampleCallback(0),
        myCurrentSampleCallback(0),
        myCurrentRawSample(),
        myCurrentSample()
    {
    }

    //--------------------------------------------------------------------------
    Sensor(const Traits& traits) :
        SensorBase(),
        myTraits(traits),
        myConfig(),
        myCurrentRawSampleCallback(0),
        myCurrentSampleCallback(0),
        myCurrentRawSample(),
        myCurrentSample()
    {
    }

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~Sensor()
    {
    }

    //--------------------------------------------------------------------------
    // Protected pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error subclassSetSensorConfig(const Config& config) = 0;

    //--------------------------------------------------------------------------
    // Protected virtual methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual void rawSampleReady(const SensorRawSample<nDof>& sample)
    {
        if (isValidPointer(myCurrentRawSampleCallback))
        {
            myCurrentRawSampleCallback->call(sample);
        }

        myCurrentRawSample = sample;

        SensorSample<ValueType, nDof> newSample;
        newSample.timeOfValidity = sample.timeOfValidity;
        newSample.status = sample.status;

        for (std::uint32_t i = 0; i < nDof; i++)
        {
            newSample.values[i] = sample.values[i] * myTraits.scaleFactors[i];
        }

        subclassApplySampleCorrections(newSample);

        sampleReady(newSample);
    }

    //--------------------------------------------------------------------------
    virtual void sampleReady(const SensorSample<ValueType, nDof>& sample)
    {
        if (isValidPointer(myCurrentSampleCallback))
        {
            myCurrentSampleCallback->call(sample);
        }

        myCurrentSample = sample;
    }

    //--------------------------------------------------------------------------
    virtual void subclassApplySampleCorrections(
                                          SensorSample<ValueType, nDof>& sample)
    {
        // Not implemented by subclass, do nothing
    }

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void setTraits(const Traits& traits)
    {
        myTraits = traits;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Traits myTraits;

    Config myConfig;

    RawSampleCallback* myCurrentRawSampleCallback;

    SampleCallback* myCurrentSampleCallback;

    SensorRawSample<nDof> myCurrentRawSample;

    SensorSample<ValueType, nDof> myCurrentSample;
};

}; // namespace Plat4m

#endif // PLAT4M_SENSOR_H