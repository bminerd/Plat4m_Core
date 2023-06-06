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
/// @file TopicTest.cpp
/// @author Ben Minerd
/// @date 7/23/2020
/// @brief TopicTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Test/Acceptance_Tests/TopicTest.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TopicManager.h>
#include <Plat4m_Core/CallbackFunctionParameter.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction TopicTest::myTestCallbackFunctions[] =
{
    &TopicTest::acceptanceTest1,
    &TopicTest::acceptanceTest2
};

std::uint8_t TopicTest::acceptanceTest1Sample = 0;

std::uint8_t TopicTest::acceptanceTest1Sample2 = 0;

TopicTest::TestSample TopicTest::acceptanceTest2Sample =
{
    0,
    0
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TopicTest::TopicTest() :
    UnitTest("TopicTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TopicTest::~TopicTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool TopicTest::acceptanceTest1()
{
    //
    // Procedure: Create a Topic with one subscriber and publish a sample
    //
    // Test: Verify subscriber receives sample
    //

    // Setup / Operation

    const TopicBase::Id testTopicId = 1;
    std::uint8_t testTopicSample = 42;

    TopicManager topicManager;

    Topic<std::uint8_t>& testTopic = Topic<std::uint8_t>::create(testTopicId);

    Topic<std::uint8_t>::subscribe(
                                 testTopicId,
                                 createCallback(&acceptanceTest1TopicCallback));

    Topic<std::uint8_t>::subscribe(
                                testTopicId,
                                createCallback(&acceptanceTest1TopicCallback2));

    testTopic.publish(testTopicSample);

    // Test

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample, (std::uint8_t) 42)) &
        UNIT_TEST_CASE_EQUAL(acceptanceTest1Sample2, (std::uint8_t) 42);
}

//------------------------------------------------------------------------------
void TopicTest::acceptanceTest1TopicCallback(
                                        const TopicSample<std::uint8_t>& sample)
{
    acceptanceTest1Sample = sample.data;
}

//------------------------------------------------------------------------------
void TopicTest::acceptanceTest1TopicCallback2(
                                        const TopicSample<std::uint8_t>& sample)
{
    acceptanceTest1Sample2 = sample.data;
}

//------------------------------------------------------------------------------
bool TopicTest::acceptanceTest2()
{
    //
    // Procedure: Create a Topic with one subscriber and publish a sample
    //
    // Test: Verify subscriber receives sample
    //

    // Setup / Operation

    const TopicBase::Id testTopicId = 1;
    TestSample sample;
    sample.sample1 = 42;
    sample.sample2 = 67;

    TopicManager topicManager;

    Topic<TestSample>& testTopic = Topic<TestSample>::create(testTopicId);

    Topic<TestSample>::subscribe(testTopicId,
                                 createCallback(&acceptanceTest2TopicCallback));

    testTopic.publish(sample);

    // Test

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(
                            acceptanceTest2Sample.sample1, (std::uint8_t) 42)) &
        UNIT_TEST_CASE_EQUAL(acceptanceTest2Sample.sample2, (std::uint8_t) 67);
}

//------------------------------------------------------------------------------
void TopicTest::acceptanceTest2TopicCallback(
                                          const TopicSample<TestSample>& sample)
{
    acceptanceTest2Sample = sample.data;
}
