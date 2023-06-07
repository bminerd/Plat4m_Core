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
/// @file ServiceClientTest.cpp
/// @author Ben Minerd
/// @date 6/7/2023
/// @brief ServiceClientTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Test/Acceptance_Tests/ServiceClientTest.h>
#include <Plat4m_Core/Service.h>
#include <Plat4m_Core/ServiceManager.h>
#include <Plat4m_Core/CallbackFunction.h>
#include <Plat4m_Core/ServiceClient.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction ServiceClientTest::myTestCallbackFunctions[] =
{
    &ServiceClientTest::acceptanceTest1
};

std::uint8_t ServiceClientTest::acceptanceTest1ServiceValue = 0;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ServiceClientTest::ServiceClientTest() :
    UnitTest("ServiceClientTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ServiceClientTest::~ServiceClientTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ServiceClientTest::acceptanceTest1()
{
    //
    // Procedure: Create a Service, register a callback, and interface to the
    // Service via ServiceClient
    //
    // Test: Verify service callback is called and values are correct
    //

    // Setup / Operation

    const ServiceBase::Id testServiceId = 1;

    ServiceManager serviceManager;

    Service<std::uint8_t, bool>& testService =
                  createService(testServiceId, &acceptanceTest1ServiceCallback);

    ServiceClient<std::uint8_t, bool> serviceClient(testServiceId);

    std::uint8_t request = 42;

    bool response;

    serviceClient.request(request, response);

    // Test

    return UNIT_TEST_REPORT(
          UNIT_TEST_CASE_EQUAL(acceptanceTest1ServiceValue, (std::uint8_t) 42) &
          UNIT_TEST_CASE_EQUAL(response, true));
}

//------------------------------------------------------------------------------
ServiceBase::Error ServiceClientTest::acceptanceTest1ServiceCallback(
                                    const ServiceRequest<std::uint8_t>& request,
                                    ServiceResponse<bool>& response)
{
    acceptanceTest1ServiceValue = request.data;
    response.data = true;

    return ServiceBase::Error(ServiceBase::ERROR_CODE_NONE);
}
