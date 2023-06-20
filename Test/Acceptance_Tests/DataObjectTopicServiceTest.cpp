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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file DataObjectTopicServiceTest.cpp
/// @author Ben Minerd
/// @date 6/20/2023
/// @brief DataObjectTopicServiceTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Test/Acceptance_Tests/DataObjectTopicServiceTest.h>
#include <Plat4m_Core/TopicBase.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TopicManager.h>
#include <Plat4m_Core/CallbackFunction.h>
#include <Plat4m_Core/DataObjectManager.h>
#include <Plat4m_Core/DataObjectBase.h>
#include <Plat4m_Core/DataObjectTopicService.h>
#include <Plat4m_Core/ServiceBase.h>
#include <Plat4m_Core/Service.h>
#include <Plat4m_Core/ServiceManager.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction DataObjectTopicServiceTest::myTestCallbackFunctions[] =
{
    &DataObjectTopicServiceTest::acceptanceTest1,
    &DataObjectTopicServiceTest::acceptanceTest2,
    &DataObjectTopicServiceTest::acceptanceTest3
};

std::uint8_t DataObjectTopicServiceTest::acceptanceTest1UpdatedData = 0;

std::uint8_t DataObjectTopicServiceTest::acceptanceTest1ChangedData = 0;

std::uint32_t DataObjectTopicServiceTest::
                          acceptanceTest1DataObjectUpdatedCallbackCallCount = 0;

std::uint32_t DataObjectTopicServiceTest::
                          acceptanceTest1DataObjectChangedCallbackCallCount = 0;

std::uint8_t DataObjectTopicServiceTest::acceptanceTest2Data = 42;

std::uint8_t DataObjectTopicServiceTest::acceptanceTest3Data = 0;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DataObjectTopicServiceTest::DataObjectTopicServiceTest() :
    UnitTest("DataObjectTopicServiceTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DataObjectTopicServiceTest::~DataObjectTopicServiceTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool DataObjectTopicServiceTest::acceptanceTest1()
{
    //
    // Procedure: Create a Topic with one subscriber and publish a sample
    //
    // Test: Verify subscriber receives sample
    //

    // Setup / Operation

    const TopicBase::Id topicId = 1;
    std::uint8_t topicSample = 42;

    TopicManager topicManager;
    DataObjectManager dataObjectManager;

    Topic<std::uint8_t>& topic = Topic<std::uint8_t>::create(topicId);

    const DataObjectBase::Id dataObjectId = 11;

    DataObjectTopicService<std::uint8_t, DataObjectBase::ACCESS_READ_ONLY_PUSH>
                                              dataObject(dataObjectId, topicId);

    dataObject.addDataUpdatedCallback(
        createCallback(&DataObjectTopicServiceTest::
                                     acceptanceTest1DataObjectUpdatedCallback));

    dataObject.addDataChangedCallback(
        createCallback(&DataObjectTopicServiceTest::
                                     acceptanceTest1DataObjectChangedCallback));

    dataObject.enable();

    topic.publish(topicSample);
    topic.publish(topicSample);

    topicSample = 93;

    topic.publish(topicSample);

    dataObject.disable();

    // Test

    return UNIT_TEST_REPORT(
        UNIT_TEST_CASE_EQUAL(acceptanceTest1UpdatedData, (std::uint8_t) 93) &
        UNIT_TEST_CASE_EQUAL(acceptanceTest1ChangedData, (std::uint8_t) 93) &
        UNIT_TEST_CASE_EQUAL(
                        acceptanceTest1DataObjectUpdatedCallbackCallCount,
                        (std::uint32_t) 3)                                  &
        UNIT_TEST_CASE_EQUAL(
                        acceptanceTest1DataObjectChangedCallbackCallCount,
                        (std::uint32_t) 2));
}

//------------------------------------------------------------------------------
void DataObjectTopicServiceTest::acceptanceTest1DataObjectUpdatedCallback(
                                                       const std::uint8_t& data)
{
    acceptanceTest1UpdatedData = data;
    acceptanceTest1DataObjectUpdatedCallbackCallCount++;
}

//------------------------------------------------------------------------------
void DataObjectTopicServiceTest::acceptanceTest1DataObjectChangedCallback(
                                                       const std::uint8_t& data)
{
    acceptanceTest1ChangedData = data;
    acceptanceTest1DataObjectChangedCallbackCallCount++;
}

//------------------------------------------------------------------------------
bool DataObjectTopicServiceTest::acceptanceTest2()
{
    //
    // Procedure: Create a Topic with one subscriber and publish a sample
    //
    // Test: Verify subscriber receives sample
    //

    // Setup / Operation

    const ServiceBase::Id serviceId = 1;

    ServiceManager serviceManager;
    DataObjectManager dataObjectManager;

    Service<void*, std::uint8_t>& service =
        createService(
            serviceId,
            &DataObjectTopicServiceTest::
                                      acceptanceTest2DataObjectGetDataCallback);

    const DataObjectBase::Id dataObjectId = 11;

    DataObjectTopicService<std::uint8_t, DataObjectBase::ACCESS_READ_ONLY_PULL>
                                            dataObject(dataObjectId, serviceId);

    dataObject.enable();

    std::uint8_t data = 0;

    dataObject.read(data);

    dataObject.disable();

    // Test

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(data, acceptanceTest2Data));
}

//------------------------------------------------------------------------------
ServiceBase::Error
    DataObjectTopicServiceTest::acceptanceTest2DataObjectGetDataCallback(
                                        const ServiceRequest<void*>& request,
                                        ServiceResponse<std::uint8_t>& response)
{
    response.data = acceptanceTest2Data;

    return ServiceBase::Error(ServiceBase::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
bool DataObjectTopicServiceTest::acceptanceTest3()
{
    //
    // Procedure: Create a Topic with one subscriber and publish a sample
    //
    // Test: Verify subscriber receives sample
    //

    // Setup / Operation

    const ServiceBase::Id setServiceId = 1;
    const ServiceBase::Id getServiceId = 2;

    ServiceManager serviceManager;
    DataObjectManager dataObjectManager;

    Service<std::uint8_t, void*>& setService =
        createService(
            setServiceId,
            &DataObjectTopicServiceTest::
                                      acceptanceTest3DataObjectSetDataCallback);

    Service<void*, std::uint8_t>& getService =
        createService(
            getServiceId,
            &DataObjectTopicServiceTest::
                                      acceptanceTest3DataObjectGetDataCallback);

    const DataObjectBase::Id dataObjectId = 11;

    DataObjectTopicService<std::uint8_t, DataObjectBase::ACCESS_READ_WRITE>
                                                       dataObject(dataObjectId,
                                                                  setServiceId,
                                                                  getServiceId);

    dataObject.enable();

    std::uint8_t data = 42;

    dataObject.write(data);

    std::uint8_t dataRead1 = 0;

    dataObject.read(dataRead1);

    dataObject.disable();

    // Test

    return UNIT_TEST_REPORT(
                  UNIT_TEST_CASE_EQUAL(acceptanceTest3Data, (std::uint8_t) 42) &
                  UNIT_TEST_CASE_EQUAL(dataRead1,           (std::uint8_t) 42));
}

//------------------------------------------------------------------------------
ServiceBase::Error
    DataObjectTopicServiceTest::acceptanceTest3DataObjectSetDataCallback(
                                    const ServiceRequest<std::uint8_t>& request,
                                    ServiceResponse<void*>& response)
{
    acceptanceTest3Data = request.data;

    return ServiceBase::Error(ServiceBase::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ServiceBase::Error
    DataObjectTopicServiceTest::acceptanceTest3DataObjectGetDataCallback(
                                        const ServiceRequest<void*>& request,
                                        ServiceResponse<std::uint8_t>& response)
{
    response.data = acceptanceTest3Data;

    return ServiceBase::Error(ServiceBase::ERROR_CODE_NONE);
}
