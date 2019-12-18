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

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template<typename SampleType>
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

    typedef Callback<void, SampleType> SampleCallback;

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static void subscribe(const uint32_t& id,
                          const SampleCallback& sampleCallback)
    {
        List< Topic<SampleType>* >::Iterator iterator = myTopicList.iterator();

        while (iterator.hasCurrent())
        {
            Topic<SampleType>* topic = iterator.current();
            
            if (topic.getId() == id)
            {
                topic->subscribe(sampleCallback);

                break;
            }

            iterator.next();
        }

        SampleCallback* pointer = &sampleCallback;

        mySampleCallbackList.append(pointer);
    }

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Topic(const uint32_t id) :
        myId(id),
        mySampleCallbackList()
    {
        myTopicList.append(this);
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    virtual ~Topic()
    {
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
    void subscribe(const SampleCallback& sampleCallback)
    {
        SampleCallback* pointer = &sampleCallback;

        mySampleCallbackList.append(pointer);
    }

    //--------------------------------------------------------------------------
    void publish(const SampleType& sample)
    {
        List<SampleCallback*>::Iterator iterator =
                                                mySampleCallbackList.iterator();

        while (iterator.hasCurrent())
        {
            SampleCallback* sampleCallback = iterator.current();
            sampleCallback->call(sample);

            iterator.next();
        }
    }

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static List< Topic<SampleType>* > myTopicList;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const uint32_t myId;

    List<SampleCallback*> mySampleCallbackList;
};

}; // namespace Plat4m

#endif // PLAT4M_TOPIC_H
