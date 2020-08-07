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
// Permission is hereby granted, free of uint8_tge, to any person obtaining a copy
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
#include <Plat4m_Core/TopicSubscriber.h>
#include <Plat4m_Core/CallbackFunctionParameter.h>

using Plat4m::TopicSubscriberTest;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction
                                TopicSubscriberTest::myTestCallbackFunctions[] =
{
    &TopicSubscriberTest::acceptanceTest1
};

uint8_t TopicSubscriberTest::acceptanceTest1Sample;

uint8_t TopicSubscriberTest::acceptanceTest1SampleCount = 0;

uint8_t TopicSubscriberTest::acceptanceTest1Sample2;

uint8_t TopicSubscriberTest::acceptanceTest1SampleCount2 = 0;

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

    const uint32_t testTopicID = 1;
    const uint8_t sample  = 42;
    const uint8_t sample2 = 71;
    const uint8_t sample3 = 23;

    Topic<uint8_t> testTopic(testTopicID);

    TopicSubscriber<uint8_t>::Config config;
    config.downsampleFactor = 0;

    TopicSubscriber<uint8_t> subscriber(
                                 testTopicID,
                                 config,
                                 createCallback(&acceptanceTest1TopicCallback));

    config.downsampleFactor = 2;

    TopicSubscriber<uint8_t> subscriber2(
                                testTopicID,
                                config,
                                createCallback(&acceptanceTest1TopicCallback2));

    testTopic.publish(sample);
    testTopic.publish(sample2);
    testTopic.publish(sample3);

    // Test

    return UNIT_TEST_REPORT(
                UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample, (uint8_t) 23)     &
                UNIT_TEST_CASE_EQUAL(acceptanceTest1SampleCount, (uint8_t) 3) &
                UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample2, (uint8_t) 71)    &
                UNIT_TEST_CASE_EQUAL(acceptanceTest1SampleCount2, (uint8_t) 1));
}

//------------------------------------------------------------------------------
void TopicSubscriberTest::acceptanceTest1TopicCallback(const uint8_t sample)
{
    acceptanceTest1Sample = sample;
    acceptanceTest1SampleCount++;
}

//------------------------------------------------------------------------------
void TopicSubscriberTest::acceptanceTest1TopicCallback2(const uint8_t sample)
{
    acceptanceTest1Sample2 = sample;
    acceptanceTest1SampleCount2++;
}
