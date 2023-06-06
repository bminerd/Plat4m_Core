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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file TopicSubscriberExternal.h
/// @author Ben Minerd
/// @date 5/13/2022
/// @brief TopicSubscriberExternal class header file.
///

#ifndef PLAT4M_TOPIC_SUBSCRIBER_EXTERNAL_H
#define PLAT4M_TOPIC_SUBSCRIBER_EXTERNAL_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/TopicSubscriber.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/ComProtocol.h>
#include <Plat4m_Core/ByteArrayN.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template<typename DataType, typename SampleFrameType>
class TopicSubscriberExternal : public TopicSubscriber<DataType>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    TopicSubscriberExternal(
                      const TopicBase::Id id,
                      const typename TopicSubscriber<DataType>::Config config,
                      ComProtocol& comProtocol) :
        TopicSubscriber<DataType>(
              id,
              config,
              createCallback(this,
                             &TopicSubscriberExternal<DataType, SampleFrameType>::sampleCallbackInternal)),
        myComProtocol(comProtocol)
    {
    }

    //--------------------------------------------------------------------------
    TopicSubscriberExternal(const TopicBase::Id id, ComProtocol& comProtocol) :
        TopicSubscriber<DataType>(
              id,
              createCallback(this,
                             &TopicSubscriberExternal<DataType, SampleFrameType>::sampleCallbackInternal)),
        myComProtocol(comProtocol)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    virtual ~TopicSubscriberExternal()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ComProtocol& myComProtocol;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Module::Error driverSetEnabled(const bool enabled) override
    {
        Module::Error error;

        // error = TopicSubscriber<DataType>::setEnabled(enabled);

        error = myComProtocol.setEnabled(enabled);

        return error;
    }

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for TopicSubscriber
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual void sampleCallbackInternal(const DataType& sample) override
    {
        DataType copy = sample;

        SampleFrameType frame(copy);

        ByteArrayN<256> bytes;

        frame.toByteArray(bytes);

        myComProtocol.transmitData(bytes);
    }
};

}; // namespace Plat4m

#endif // PLAT4M_TOPIC_SUBSCRIBER_EXTERNAL_H
