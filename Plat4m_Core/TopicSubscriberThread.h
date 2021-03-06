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
// Copyright (c) 2020 Benjamin Minerd
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
/// @file TopicSubscriberThread.h
/// @author Ben Minerd
/// @date 7/24/2020
/// @brief TopicSubscriberThread class header file.
///

#ifndef PLAT4M_TOPIC_SUBSCRIBER_THREAD_H
#define PLAT4M_TOPIC_SUBSCRIBER_THREAD_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/TopicSubscriber.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/CallbackMethodParameter.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Queue.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template<typename SampleType, uint32_t nQueueValues>
class TopicSubscriberThread : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    TopicSubscriberThread(
                   const uint32_t id,
                   const typename TopicSubscriber<SampleType>::Config config,
                   typename Topic<SampleType>::SampleCallback& sampleCallback) :
        Module(),
        mySampleCallback(sampleCallback),
        myTopicSubscriber(
                  id,
                  config,
                  createCallback(this, &TopicSubscriberThread::sampleCallback)),
        myThread(System::createThread(
                 createCallback(this, &TopicSubscriberThread::threadCallback))),
        myQueue(System::createQueue<SampleType>(nQueueValues, myThread))
    {
    }

    //--------------------------------------------------------------------------
    TopicSubscriberThread(
                   const uint32_t id,
                   typename Topic<SampleType>::SampleCallback& sampleCallback) :
        Module(),
        mySampleCallback(sampleCallback),
        myTopicSubscriber(
                  id,
                  createCallback(this, &TopicSubscriberThread::sampleCallback)),
        myThread(System::createThread(
                 createCallback(this, &TopicSubscriberThread::threadCallback))),
        myQueue(System::createQueue<SampleType>(nQueueValues, myThread))
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    virtual ~TopicSubscriberThread()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    Thread& getThread()
    {
        return myThread;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    typename Topic<SampleType>::SampleCallback& mySampleCallback;

    TopicSubscriber<SampleType> myTopicSubscriber;

    Thread& myThread;

    Queue<SampleType>& myQueue;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Module::Error driverSetEnabled(const bool enabled)
    {
        Module::Error error = myThread.setEnabled(enabled);

        if (!enabled)
        {
            myQueue.clear();
        }

        return error;
    }

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void sampleCallback(const SampleType& sample)
    {
        myQueue.enqueue(sample);
    }

    //--------------------------------------------------------------------------
    void threadCallback()
    {
        SampleType sample;

        if (myQueue.dequeue(sample))
        {
            mySampleCallback.call(sample);
        }
    }
};

}; // namespace Plat4m

#endif // PLAT4M_TOPIC_SUBSCRIBER_THREAD_H
