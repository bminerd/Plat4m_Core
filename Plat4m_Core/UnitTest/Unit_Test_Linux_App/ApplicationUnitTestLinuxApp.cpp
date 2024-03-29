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
// Copyright (c) 2019 Benjamin Minerd
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
/// @file ApplicationUnitTestLinuxApp.cpp
/// @author Ben Minerd
/// @date 6/4/2019
/// @brief ApplicationUnitTestLinuxApp class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ApplicationUnitTestLinuxApp.h>

using Plat4m::ApplicationUnitTestLinuxApp;

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ApplicationUnitTestLinuxApp::ApplicationUnitTestLinuxApp() :
    ApplicationUnitTestApp("UNIT_TEST_LINUX_APP", "UNIT_TEST_LINUX", "0.0.1"),
    mySystem(),
    myProcessor(),
    myArrayUnitTest(),
    myArrayNUnitTest(),
    myBufferUnitTest(),
    myByteArrayUnitTest(),
    myModuleUnitTest(),
    myTimeStampUnitTest(),
    myListUnitTest()
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ApplicationUnitTestLinuxApp::~ApplicationUnitTestLinuxApp()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Application
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int ApplicationUnitTestLinuxApp::driverRun()
{
    initializeProcessor();
    initializeSystem();

    return (runParentApplication());
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ApplicationUnitTestLinuxApp::initializeProcessor()
{
    myProcessor.reset();
}

//------------------------------------------------------------------------------
void ApplicationUnitTestLinuxApp::initializeSystem()
{
    addUnitTest(myArrayUnitTest);
    addUnitTest(myArrayNUnitTest);
    addUnitTest(myBufferUnitTest);
    addUnitTest(myByteArrayUnitTest);
    addUnitTest(myModuleUnitTest);
    addUnitTest(myTimeStampUnitTest);
    addUnitTest(myListUnitTest);
}
