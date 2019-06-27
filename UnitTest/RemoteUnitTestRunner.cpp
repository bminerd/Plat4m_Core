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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file RemoteUnitTestRunner.cpp
/// @author Ben Minerd
/// @date 5/4/2016
/// @brief RemoteUnitTestRunner class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <RemoteUnitTestRunner.h>
#include <AsciiMessageHandlerTemplate.h>
#include <UnitTestRunAsciiMessage.h>
#include <UnitTestRunResultAsciiMessage.h>

using Plat4m::RemoteUnitTestRunner;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const char* RemoteUnitTestRunner::myName        = "REMOTE_UNIT_TEST_RUNNER";
const char* RemoteUnitTestRunner::myProductName = "PLAT4M";
const char* RemoteUnitTestRunner::myVersion     = "0.0.1";

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RemoteUnitTestRunner::RemoteUnitTestRunner(const char* name,
                                                     const char* productName,
                                                     const char* version) :
        UnitTestRunner(name, productName, version),
    myComLink(),
    myComProtocolPlat4mAscii()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RemoteUnitTestRunner::~RemoteUnitTestRunner()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void RemoteUnitTestRunner::runParentApplication(ComInterface& comInterface)
{
    myComLink.setComInterface(comInterface);

	initializeSystem();

	UnitTestRunner::runParentApplication();
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void RemoteUnitTestRunner::initializeSystem()
{
    myComProtocolPlat4mAscii.addMessageHandler(
        createMessageHandler<RemoteUnitTestRunner,
                             UnitTestRunMessage,
                             UnitTestRunResultMessage,
                             UnitTestRunAsciiMessage,
                             UnitTestRunResultAsciiMessage>(
                       this, &RemoteUnitTestRunner::unitTestRunMessageHandler));

    myComLink.addComProtocol(myComProtocolPlat4mAscii);

    myComLink.enable(true);
}

//------------------------------------------------------------------------------
void RemoteUnitTestRunner::unitTestRunMessageHandler(
                                             const UnitTestRunMessage& request,
                                             UnitTestRunResultMessage& response)
{
    bool passed;

    UnitTest::Error error = runTest(request.getModuleIndex(),
                                    request.getTestIndex(),
                                    passed);

    response.setPassed(passed);
}
