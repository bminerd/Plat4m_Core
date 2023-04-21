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
/// @file TopicSubscriberThreadTest.cpp
/// @author Ben Minerd
/// @date 7/24/2020
/// @brief TopicSubscriberThreadTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <iostream>

#include <Test/Acceptance_Tests/TopicSubscriberThreadTest.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TopicSubscriberThread.h>
#include <Plat4m_Core/CallbackFunctionParameter.h>
#include <Plat4m_Core/CallbackFunction.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/TopicManager.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction
                          TopicSubscriberThreadTest::myTestCallbackFunctions[] =
{
    &TopicSubscriberThreadTest::acceptanceTest1
};

TopicSubscriberThreadTest::TestSample
                               TopicSubscriberThreadTest::acceptanceTest1Sample;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TopicSubscriberThreadTest::TopicSubscriberThreadTest() :
    UnitTest("TopicSubscriberThreadTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TopicSubscriberThreadTest::~TopicSubscriberThreadTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool TopicSubscriberThreadTest::acceptanceTest1()
{
    //
    // Procedure: Create a Topic with one subscriber and publish a sample
    //
    // Test: Verify subscriber receives sample
    //

    // Setup / Operation

    const TopicBase::Id testTopicId = 5;

    TopicManager topicManager;

    Topic<TestSample>& testTopic = Topic<TestSample>::create(testTopicId);

    TopicSubscriberThread<TestSample, 8> subscriber(
        testTopicId,
        createCallback(
                    &TopicSubscriberThreadTest::acceptanceTest1SampleCallback));

    subscriber.enable();
    
    TestSample sample;
    sample.sample1 = 51;
    sample.sample2 = 3;

    testTopic.publish(sample);

    System::delayTimeMs(10);

    subscriber.disable();

    // Test

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample.sample1, (std::uint8_t) 51) &
        UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample.sample2, (std::uint8_t) 3));
}

//------------------------------------------------------------------------------
void TopicSubscriberThreadTest::acceptanceTest1SampleCallback(
                                                       const TestSample& sample)
{
    acceptanceTest1Sample = sample;
}
