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
// Copyright (c) 2020-2023 Benjamin Minerd
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
/// @file TopicSubscriberTest.cpp
/// @author Ben Minerd
/// @date 7/24/2020
/// @brief TopicSubscriberTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Test/Acceptance_Tests/TopicSubscriberTest.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TopicSubscriber.h>
#include <Plat4m_Core/CallbackFunctionParameter.h>
#include <Plat4m_Core/TopicManager.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction
                                TopicSubscriberTest::myTestCallbackFunctions[] =
{
    &TopicSubscriberTest::acceptanceTest1
};

TopicSubscriberTest::TestSample TopicSubscriberTest::acceptanceTest1Sample;

std::uint32_t TopicSubscriberTest::acceptanceTest1SampleCount = 0;

TopicSubscriberTest::TestSample TopicSubscriberTest::acceptanceTest1Sample2;

std::uint32_t TopicSubscriberTest::acceptanceTest1SampleCount2 = 0;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TopicSubscriberTest::TopicSubscriberTest() :
    UnitTest("TopicSubscriberTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TopicSubscriberTest::~TopicSubscriberTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool TopicSubscriberTest::acceptanceTest1()
{
    //
    // Procedure: Create a Topic with one subscriber and publish a sample
    //
    // Test: Verify subscriber receives sample
    //

    // Setup / Operation

    const TopicBase::Id testTopicId = 1;

    TestSample sample;
    sample.sample = 42;

    TestSample sample2;
    sample2.sample = 71;

    TestSample sample3;
    sample3.sample = 23;

    TopicManager topicManager;

    Topic<TestSample>& testTopic = Topic<TestSample>::create(testTopicId);

    TopicSubscriber<TestSample>::Config config;
    config.downsampleFactor = 0;

    TopicSubscriber<TestSample> subscriber(
                                 testTopicId,
                                 config,
                                 createCallback(&acceptanceTest1TopicCallback));
    subscriber.enable();

    config.downsampleFactor = 2;

    TopicSubscriber<TestSample> subscriber2(
                                testTopicId,
                                config,
                                createCallback(&acceptanceTest1TopicCallback2));
    subscriber2.enable();

    testTopic.publish(sample);
    testTopic.publish(sample2);
    testTopic.publish(sample3);

    // Test

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample.sample, (std::uint8_t) 23)  &
        UNIT_TEST_CASE_EQUAL(acceptanceTest1SampleCount, (std::uint32_t) 3)    &
        UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample2.sample, (std::uint8_t) 71) &
        UNIT_TEST_CASE_EQUAL(acceptanceTest1SampleCount2, (std::uint32_t) 1));
}

//------------------------------------------------------------------------------
void TopicSubscriberTest::acceptanceTest1TopicCallback(
                                          const TopicSample<TestSample>& sample)
{
    // acceptanceTest1Sample = *(sample.data);
    acceptanceTest1Sample = sample.data;
    acceptanceTest1SampleCount++;
}

//------------------------------------------------------------------------------
void TopicSubscriberTest::acceptanceTest1TopicCallback2(
                                          const TopicSample<TestSample>& sample)
{
    // acceptanceTest1Sample2 = *(sample.data);
    acceptanceTest1Sample2 = sample.data;
    acceptanceTest1SampleCount2++;
}
