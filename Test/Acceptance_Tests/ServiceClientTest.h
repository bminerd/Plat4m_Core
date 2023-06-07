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
/// @file ServiceClientTest.h
/// @author Ben Minerd
/// @date 6/7/2023
/// @brief ServiceClientTest class header file.
///

#ifndef PLAT4M_SERVICE_CLIENT_TEST_H
#define PLAT4M_SERVICE_CLIENT_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/UnitTest/UnitTest.h>
#include <Plat4m_Core/TopicSample.h>
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

class ServiceClientTest : public UnitTest
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ServiceClientTest();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ServiceClientTest();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static bool acceptanceTest1();

    static ServiceBase::Error acceptanceTest1ServiceCallback(
                                    const ServiceRequest<std::uint8_t>& request,
                                    ServiceResponse<bool>& response);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const UnitTest::TestCallbackFunction myTestCallbackFunctions[];

    static std::uint8_t acceptanceTest1ServiceValue;
};

}; // namespace Plat4m

#endif // PLAT4M_TOPIC_TEST_H
