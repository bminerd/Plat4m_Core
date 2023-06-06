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

#include <new>

#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/TimeStamp.h>
#include <Plat4m_Core/MemoryAllocator.h>
#include <Plat4m_Core/TopicManager.h>
#include <Plat4m_Core/TopicSample.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename DataType>
class Topic : public TopicBase
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    typedef Callback<void, const TopicSample<DataType>&> SampleCallback;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static Topic& create(const TopicBase::Id id)
    {
        Topic* topic = findOrCreate(id);

        return (*topic);
    }

    //--------------------------------------------------------------------------
    static void subscribe(const TopicBase::Id id,
                          SampleCallback& sampleCallback)
    {
        Topic* topic = findOrCreate(id);

        topic->subscribe(sampleCallback);
    }

    //--------------------------------------------------------------------------
    static void unsubscribe(const TopicBase::Id id,
                            SampleCallback& sampleCallback)
    {
        Topic* topic = static_cast<Topic*>(TopicManager::find(id));

        if (isValidPointer(topic))
        {
            topic->unsubscribe(sampleCallback);
        }
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~Topic()
    {
        Topic* pointer = this;

        TopicManager::remove(*pointer);
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

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
    void publish(const DataType& sample)
    {
        TopicSample<DataType> topicSample(sample);

        publishPrivate(topicSample);
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    List<SampleCallback*> mySampleCallbackList;

    std::uint32_t mySequenceIdCounter;

    //--------------------------------------------------------------------------
    // Private static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static Topic* find(const TopicBase::Id id)
    {
        Topic* topic = 0;

        TopicBase* topicBase = TopicManager::find(id);

        if (isValidPointer(topicBase))
        {
            topic = dynamic_cast<Topic*>(topicBase);

            if (isNullPointer(topic))
            {
                Error error(TopicBase::ERROR_CODE_TOPIC_TYPE_ID_MISMATCH);
            }
        }

        return topic;
    }

    //--------------------------------------------------------------------------
    static Topic* createPrivate(const TopicBase::Id id)
    {
        void* memoryPointer = AllocationMemory::allocate(sizeof(Topic));

        Topic* topic = new(memoryPointer) Topic(id);

        return topic;
    }

    //--------------------------------------------------------------------------
    static Topic* findOrCreate(const TopicBase::Id id)
    {
        Topic* topic = find(id);

        if (isNullPointer(topic))
        {
            topic = createPrivate(id);
        }

        return topic;
    }

    //--------------------------------------------------------------------------
    // Private constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Topic(const TopicBase::Id id) :
        TopicBase(id),
        mySampleCallbackList(),
        mySequenceIdCounter(0)
    {
        Topic* pointer = this;

        TopicManager::add(*pointer);
    }

    //--------------------------------------------------------------------------
    Topic(const Topic& topic) :
        TopicBase(topic.myId),
        mySampleCallbackList(topic.mySampleCallbackList),
        mySequenceIdCounter(topic.mySequenceIdCounter)
    {
    }

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    void publishPrivate(TopicSample<DataType>& sample)
    {
        sample.sequenceId = mySequenceIdCounter;
        sample.timeStamp = System::getTimeStamp();

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

}; // namespace Plat4m

#endif // PLAT4M_TOPIC_H
