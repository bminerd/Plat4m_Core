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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file SensorSimulatedServer.h
/// @author Ben Minerd
/// @date 12/28/2023
/// @brief SensorSimulatedServer class header file.
///

#ifndef PLAT4M_SENSOR_SIMULATED_SERVER_H
#define PLAT4M_SENSOR_SIMULATED_SERVER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Sensor/SensorSimulated.h>
#include <Plat4m_Core/Sensor/InertialSystemState.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TopicSubscriber.h>
#include <Plat4m_Core/ServiceBase.h>
#include <Plat4m_Core/Service.h>
#include <Plat4m_Core/CallbackMethod.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType, typename SampleType>
class SensorSimulatedServer : public SensorServer<ValueType, SampleType>
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

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    SensorSimulatedServer(
                        SensorSimulated<ValueType, SampleType>& sensorSimulated,
                        const TopicBase::Id& inertialSystemStateTopicId,
                        const TopicBase::Id& sampleTopicId) :
        SensorServer<ValueType, SampleType>(sensorSimulated, sampleTopicId),
        mySensorSimulated(sensorSimulated),
        myInertialSystemStateTopicSubscriber(
            inertialSystemStateTopicId,
            createCallback(
                this,
                &SensorSimulatedServer<ValueType, SampleType>::
                                              inertialSystemStateTopicCallback))
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~SensorSimulatedServer()
    {
    }

protected:

    //--------------------------------------------------------------------------
    // Protected virtual methods overridden for Module
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Module::Error driverSetEnabled(const bool enabled) override
    {
        myInertialSystemStateTopicSubscriber.setEnabled(enabled);

        return (Module::Error(Module::ERROR_CODE_NONE));
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    SensorSimulated<ValueType, SampleType>& mySensorSimulated;

    TopicSubscriber<InertialSystemState<ValueType>>
                                           myInertialSystemStateTopicSubscriber;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void inertialSystemStateTopicCallback(
                       const TopicSample<InertialSystemState<ValueType>>& state)
    {
        mySensorSimulated.generateSample(state);
    }
};

}; // namespace Plat4m

#endif // PLAT4M_SENSOR_SIMULATED_SERVER_H
