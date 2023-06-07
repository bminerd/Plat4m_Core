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
/// @file ApplicationAcceptanceTestLinuxApp.h
/// @author Ben Minerd
/// @date 7/24/2020
/// @brief ApplicationAcceptanceTestLinuxApp class header file.
///

#ifndef PLAT4M_APPLICATION_ACCEPTANCE_TEST_LINUX_APP_H
#define PLAT4M_APPLICATION_ACCEPTANCE_TEST_LINUX_APP_H

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <Plat4m_Core/UnitTest/ApplicationUnitTestApp.h>
#include <Plat4m_Core/Linux/SystemLinux.h>
#include <Plat4m_Core/Linux/ProcessorLinux.h>
#include <Plat4m_Core/AllocationMemoryLite/AllocationMemoryLite.h>
#include <Test/Acceptance_Tests/TopicTest.h>
#include <Test/Acceptance_Tests/TopicSubscriberTest.h>
#include <Test/Acceptance_Tests/TopicSubscriberThreadTest.h>
#include <Test/Acceptance_Tests/ServiceTest.h>
#include <Test/Acceptance_Tests/ServiceClientTest.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ApplicationAcceptanceTestLinuxApp : public ApplicationUnitTestApp
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ApplicationAcceptanceTestLinuxApp();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ApplicationAcceptanceTestLinuxApp();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    AllocationMemoryLite<4096> myAllocationMemory;

    SystemLinux mySystem;

    ProcessorLinux myProcessor;

    TopicTest myTopicTest;

    TopicSubscriberTest myTopicSubscriberTest;

    TopicSubscriberThreadTest myTopicSubscriberThreadTest;

    ServiceTest myServiceTest;

    ServiceClientTest myServiceClientTest;

    //--------------------------------------------------------------------------
    // Private methods implemented from Application
    //--------------------------------------------------------------------------

    void driverRun();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initializeProcessor();

    void initializeSystem();
};

}; // namespace Plat4m

#endif // PLAT4M_APPLICATION_ACCEPTANCE_TEST_LINUX_APP_H
