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
// Copyright (c) 2019 Benjamin Minerd
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
/// @file Topic.h
/// @author Ben Minerd
/// @date 11/23/2019
/// @brief Topic class header file.
///

#ifndef PLAT4M_TOPIC_H
#define PLAT4M_TOPIC_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/TimeStamp.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename SampleType>
class Topic
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE
    };

    typedef Callback<void, const SampleType&> SampleCallback;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static void subscribe(const uint32_t id, SampleCallback& sampleCallback)
    {
        typename List<Topic*>::Iterator iterator = myTopicList.iterator();

        while (iterator.hasCurrent())
        {
            Topic* topic = iterator.current();
            
            if (topic->getId() == id)
            {
                topic->subscribe(sampleCallback);

                break;
            }

            iterator.next();
        }
    }

    //--------------------------------------------------------------------------
    static void unsubscribe(const uint32_t id, SampleCallback& sampleCallback)
    {
        typename List<Topic*>::Iterator iterator = myTopicList.iterator();

        while (iterator.hasCurrent())
        {
            Topic* topic = iterator.current();

            if (topic->getId() == id)
            {
                topic->unsubscribe(sampleCallback);

                break;
            }

            iterator.next();
        }
    }

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Topic(const uint32_t id) :
        myId(id),
        mySampleCallbackList(),
        mySequenceIdCounter(0)
    {
        Topic* pointer = this;

        myTopicList.append(pointer);
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    virtual ~Topic()
    {
        Topic* pointer = this;

        myTopicList.remove(pointer);
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    uint32_t getId() const
    {
        return myId;
    }

    //--------------------------------------------------------------------------
    void subscribe(SampleCallback& sampleCallback)
    {
        SampleCallback* pointer = &sampleCallback;

        mySampleCallbackList.append(pointer);
    }

    //--------------------------------------------------------------------------
    void unsubscribe(SampleCallback& sampleCallback)
    {
        SampleCallback* pointer = &sampleCallback;

        mySampleCallbackList.remove(pointer);
    }

    //--------------------------------------------------------------------------
    void publish(const SampleType& sample, const bool insertTimeStamp = true)
    {
        SampleType sampleCopy = sample;

        if (insertTimeStamp)
        {
            sampleCopy.header.timeStamp = System::getTimeStamp();
        }

        publishPrivate(sampleCopy);
    }

    //--------------------------------------------------------------------------
    void publish(const SampleType& sample, const TimeStamp timeStamp)
    {
        SampleType sampleCopy = sample;

        sampleCopy.header.timeStamp = timeStamp;

        publishPrivate(sampleCopy);
    }

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static List<Topic*> myTopicList;

    uint32_t mySequenceIdCounter;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const uint32_t myId;

    List<SampleCallback*> mySampleCallbackList;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void publishPrivate(SampleType& sample)
    {
        sample.header.sequenceId = mySequenceIdCounter;

        typename List<SampleCallback*>::Iterator iterator =
                                                mySampleCallbackList.iterator();

        while (iterator.hasCurrent())
        {
            SampleCallback* sampleCallback = iterator.current();
            sampleCallback->call(sample);

            iterator.next();
        }

        mySequenceIdCounter++;
    }
};

template <typename SampleType>
List< Topic<SampleType>* > Topic<SampleType>::myTopicList;

}; // namespace Plat4m

#endif // PLAT4M_TOPIC_H
