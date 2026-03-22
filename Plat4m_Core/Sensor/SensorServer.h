
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
/// @file SensorServer.h
/// @author Ben Minerd
/// @date 11/11/2024
/// @brief SensorServer class header file.
///

#ifndef PLAT4M_SENSOR_SERVER_H
#define PLAT4M_SENSOR_SERVER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Sensor/Sensor.h>
#include <Plat4m_Core/Sensor/SensorRawSample.h>
#include <Plat4m_Core/Sensor/SensorSample.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/DataObjectBase.h>
#include <Plat4m_Core/DataObjectTopicService.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType, std::uint32_t nDof>
class SensorServer : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_COMMUNICATION_FAILED
    };

    using Error = ErrorTemplate<ErrorCode>;

    struct Config
    {
        bool connectRawSampleTopic;
        bool connectSampleTopic;

        //----------------------------------------------------------------------
        // Config: Public constructors
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        Config() :
            connectRawSampleTopic(false),
            connectSampleTopic(false)
        {
        }

        //----------------------------------------------------------------------
        Config(const bool connectRawSampleTopicIn,
               const bool connectSampleTopicIn) :
            connectRawSampleTopic(connectRawSampleTopicIn),
            connectSampleTopic(connectSampleTopicIn)
        {
        
        }
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    SensorServer(Sensor<ValueType, nDof>& sensor,
                 const TopicBase::Id& rawSampleTopicId,
                 const TopicBase::Id& sampleTopicId,
                 const DataObjectBase::Id& rawSampleDataObjectId,
                 const DataObjectBase::Id& sampleDataObjectId) :
        Module(),
        mySensor(sensor),
        myConfig(),
        myRawSampleCallback(
                        createCallback(this, &SensorServer::rawSampleCallback)),
        mySampleCallback(createCallback(this, &SensorServer::sampleCallback)),
        myRawSampleTopic(Topic<SensorRawSample<nDof>>::create(
                                                             rawSampleTopicId)),
        mySampleTopic(Topic<SensorSample<ValueType, nDof>>::create(
                                                                sampleTopicId)),
        myRawSampleDataObject(rawSampleDataObjectId, rawSampleTopicId),
        mySampleDataObject(sampleDataObjectId, sampleTopicId)
    {
        mySensor.setSampleCallback(
                           createCallback(this, &SensorServer::sampleCallback));
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~SensorServer()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Error setSensorServerConfig(const Config& config)
    {
        myConfig = config;

        return Error(ERROR_CODE_NONE);
    }

    //--------------------------------------------------------------------------
    const Config& getSensorServerConfig() const
    {
        return myConfig;
    }

protected:

    //--------------------------------------------------------------------------
    // Protected virtual methods overridden for Module
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Module::Error subclassSetEnabled(const bool enabled) override
    {
        if (enabled)
        {
            if (myConfig.connectRawSampleTopic)
            {
                mySensor.setRawSampleCallback(myRawSampleCallback);
            }

            if (myConfig.connectSampleTopic)
            {
                mySensor.setSampleCallback(mySampleCallback);
            }
        }

        return Module::Error(Module::ERROR_CODE_NONE);
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Sensor<ValueType, nDof>& mySensor;

    Config myConfig;

    typename Sensor<ValueType, 3>::RawSampleCallback& myRawSampleCallback;

    typename Sensor<ValueType, 3>::SampleCallback& mySampleCallback;

    Topic<SensorRawSample<nDof>>& myRawSampleTopic;

    Topic<SensorSample<ValueType, nDof>>& mySampleTopic;

    Plat4m::DataObjectTopicService<
           SensorRawSample<nDof>,
           Plat4m::DataObjectBase::ACCESS_READ_ONLY_PUSH> myRawSampleDataObject;

    Plat4m::DataObjectTopicService<
              SensorSample<ValueType, nDof>,
              Plat4m::DataObjectBase::ACCESS_READ_ONLY_PUSH> mySampleDataObject;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void rawSampleCallback(const SensorRawSample<nDof>& sample)
    {
        myRawSampleTopic.publish(sample);
    }

    //--------------------------------------------------------------------------
    void sampleCallback(const SensorSample<ValueType, nDof>& sample)
    {
        mySampleTopic.publish(sample);
    }
};

}; // namespace Plat4m

#endif // PLAT4M_SENSOR_SERVER_H
