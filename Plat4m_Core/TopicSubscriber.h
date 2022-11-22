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
// Copyright (c) 2021 Benjamin Minerd
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
/// @file TopicSubscriber.h
/// @author Ben Minerd
/// @date 1/16/2020
/// @brief TopicSubscriber class header file.
///

#ifndef PLAT4M_TOPIC_SUBSCRIBER_H
#define PLAT4M_TOPIC_SUBSCRIBER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/CallbackMethodParameter.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template<typename SampleType>
class TopicSubscriber : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    struct Config
    {
        std::uint32_t downsampleFactor;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    TopicSubscriber(
                   const TopicBase::Id id,
                   const Config config,
                   typename Topic<SampleType>::SampleCallback& sampleCallback) :
        Module(),
        myTopicId(id),
        myConfig(),
        mySampleCallback(&sampleCallback),
        myPrivateSampleCallback(
                        createCallback(this, &TopicSubscriber::sampleCallback)),
        myDownsampleCounter(1)
    {
        setConfig(config);

        Topic<SampleType>::subscribe(id, myPrivateSampleCallback);
    }

    //--------------------------------------------------------------------------
    TopicSubscriber(
                   const TopicBase::Id id,
                   typename Topic<SampleType>::SampleCallback& sampleCallback) :
        Module(),
        myTopicId(id),
        myConfig(),
        mySampleCallback(&sampleCallback),
        myPrivateSampleCallback(
                        createCallback(this, &TopicSubscriber::sampleCallback)),
        myDownsampleCounter(1)
    {
        myConfig.downsampleFactor = 1;

        Topic<SampleType>::subscribe(id, myPrivateSampleCallback);
    }

    //--------------------------------------------------------------------------
    TopicSubscriber(const Config config, const TopicBase::Id id) :
        Module(),
        myTopicId(id),
        myConfig(),
        mySampleCallback(0),
        myPrivateSampleCallback(
                        createCallback(this, &TopicSubscriber::sampleCallback)),
        myDownsampleCounter(1)
    {
        setConfig(config);

        Topic<SampleType>::subscribe(id, myPrivateSampleCallback);
    }

    //--------------------------------------------------------------------------
    TopicSubscriber(const TopicBase::Id id) :
        Module(),
        myTopicId(id),
        myConfig(),
        mySampleCallback(0),
        myPrivateSampleCallback(
                        createCallback(this, &TopicSubscriber::sampleCallback)),
        myDownsampleCounter(1)
    {
        myConfig.downsampleFactor = 1;

        Topic<SampleType>::subscribe(id, myPrivateSampleCallback);
    }

    //--------------------------------------------------------------------------
    TopicSubscriber(const TopicSubscriber<SampleType>& topicSubscriber) :
        Module(),
        myConfig(topicSubscriber.myConfig),
        mySampleCallback(topicSubscriber.mySampleCallback),
        myPrivateSampleCallback(topicSubscriber.myPrivateSampleCallback),
        myDownsampleCounter(topicSubscriber.myDownsampleCounter)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    virtual ~TopicSubscriber()
    {
        Topic<SampleType>::unsubscribe(myTopicId, myPrivateSampleCallback);
    }

    //--------------------------------------------------------------------------
    // Public operator overloads
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    TopicSubscriber<SampleType>& operator=(
                             const TopicSubscriber<SampleType>& topicSubscriber)
    {
        myConfig = topicSubscriber.myConfig;
        mySampleCallback = topicSubscriber.mySampleCallback;
        myDownsampleCounter = topicSubscriber.myDownsampleCounter;

        return (*this);
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void setConfig(const Config config)
    {
        myConfig = config;

        if (myConfig.downsampleFactor == 0)
        {
            myConfig.downsampleFactor = 1;
        }
    }

    //--------------------------------------------------------------------------
    void resubscribe()
    {
        myConfig.downsampleFactor = 1;

        Topic<SampleType>::subscribe(
                        myTopicId,
                        createCallback(this, &TopicSubscriber::sampleCallback));
    }

protected:

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    typename Topic<SampleType>::SampleCallback* getSampleCallback()
    {
        return mySampleCallback;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    TopicBase::Id myTopicId;

    Config myConfig;

    typename Topic<SampleType>::SampleCallback* mySampleCallback;

    typename Topic<SampleType>::SampleCallback& myPrivateSampleCallback;

    std::uint32_t myDownsampleCounter;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual void sampleCallbackInternal(const SampleType& sample)
    {
        // Not implemented by subclass, default implementation

        if (isValidPointer(mySampleCallback))
        {
            mySampleCallback->call(sample);
        }
    }

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void sampleCallback(const SampleType& sample)
    {
        if (isEnabled())
        {
            if (myDownsampleCounter >= myConfig.downsampleFactor)
            {
                sampleCallbackInternal(sample);

                myDownsampleCounter = 1;
            }
            else
            {
                myDownsampleCounter++;
            }
        }
    }

};

}; // namespace Plat4m

#endif // PLAT4M_TOPIC_SUBSCRIBER_H
