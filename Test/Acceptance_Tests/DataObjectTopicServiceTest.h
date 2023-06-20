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
/// @file DataObjectTopicServiceTest.h
/// @author Ben Minerd
/// @date 6/20/2023
/// @brief DataObjectTopicServiceTest class header file.
///

#ifndef PLAT4M_DATA_OBJECT_TOPIC_SERVICE_TEST_H
#define PLAT4M_DATA_OBJECT_TOPIC_SERVICE_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/UnitTest/UnitTest.h>
#include <Plat4m_Core/ServiceBase.h>
#include <Plat4m_Core/ServiceRequest.h>
#include <Plat4m_Core/ServiceResponse.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class DataObjectTopicServiceTest : public UnitTest
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    DataObjectTopicServiceTest();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~DataObjectTopicServiceTest();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static bool acceptanceTest1();

    static void acceptanceTest1DataObjectUpdatedCallback(
                                                      const std::uint8_t& data);

    static void acceptanceTest1DataObjectChangedCallback(
                                                      const std::uint8_t& data);

    static bool acceptanceTest2();

    static ServiceBase::Error acceptanceTest2DataObjectGetDataCallback(
                                       const ServiceRequest<void*>& request,
                                       ServiceResponse<std::uint8_t>& response);

    static bool acceptanceTest3();

    static ServiceBase::Error acceptanceTest3DataObjectSetDataCallback(
                                    const ServiceRequest<std::uint8_t>& request,
                                    ServiceResponse<void*>& response);

    static ServiceBase::Error acceptanceTest3DataObjectGetDataCallback(
                                       const ServiceRequest<void*>& request,
                                       ServiceResponse<std::uint8_t>& response);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const UnitTest::TestCallbackFunction myTestCallbackFunctions[];

    static std::uint8_t acceptanceTest1UpdatedData;

    static std::uint8_t acceptanceTest1ChangedData;

    static std::uint32_t acceptanceTest1DataObjectUpdatedCallbackCallCount;

    static std::uint32_t acceptanceTest1DataObjectChangedCallbackCallCount;

    static std::uint8_t acceptanceTest2Data;

    static std::uint8_t acceptanceTest3Data;
};

}; // namespace Plat4m

#endif // PLAT4M_DATA_OBJECT_TOPIC_SERVICE_TEST_H
