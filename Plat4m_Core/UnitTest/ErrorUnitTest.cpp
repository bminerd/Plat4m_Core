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
/// @file ErrorUnitTest.cpp
/// @author Ben Minerd
/// @date 11/9/2023
/// @brief ErrorUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/UnitTest/ErrorUnitTest.h>
#include <Plat4m_Core/CallbackFunction.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction ErrorUnitTest::myTestCallbackFunctions[] =
{
    &ErrorUnitTest::test1,
    &ErrorUnitTest::test2,
    &ErrorUnitTest::test3,
    &ErrorUnitTest::test4,
    &ErrorUnitTest::test5
};

ErrorUnitTest::Error ErrorUnitTest::myTest1Error;

ErrorUnitTest::TestModule::Error ErrorUnitTest::myTest2Error;

ErrorUnitTest::TestModule::Error ErrorUnitTest::myTest3Error;

ErrorUnitTest::TestModule::Error ErrorUnitTest::myTest4Error;

ErrorUnitTest::TestModule::Error ErrorUnitTest::myTest5Error;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorUnitTest::ErrorUnitTest() :
    UnitTest("ErrorUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ErrorUnitTest::~ErrorUnitTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ErrorUnitTest::test1()
{
    //
    // Procedure: register an error handler and report a high severity error
    //
    // Test: received error object should match one reported
    //
    // Note: the min reporting severity is configured to high in
    // ApplicationUnitTestApp
    //

    Error::registerCallback(createCallback(&ErrorUnitTest::test1ErrorHandler));

    Error error = PLAT4M_REPORT_ERROR_STATIC(ErrorUnitTest::Error,
                                             ErrorUnitTest::ERROR_CODE_TEST,
                                             ErrorBase::SEVERITY_HIGH,
                                             ErrorUnitTest);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(error, myTest1Error));
}

//------------------------------------------------------------------------------
void ErrorUnitTest::test1ErrorHandler(const Error& error)
{
    myTest1Error = error;
}

//------------------------------------------------------------------------------
bool ErrorUnitTest::test2()
{
    //
    // Procedure: register an error handler for a particular instance and report
    // a high severity error
    //
    // Test: received error object should match one reported
    //
    // Note: the min reporting severity is configured to high in
    // ApplicationUnitTestApp
    //

    TestModule testModule;

    TestModule::Error::registerCallback(
                              createCallback(&ErrorUnitTest::test2ErrorHandler),
                              &testModule);

    TestModule::Error error = testModule.reportError1HighSeverity();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(error, myTest2Error));
}

//------------------------------------------------------------------------------
void ErrorUnitTest::test2ErrorHandler(const TestModule::Error& error)
{
    myTest2Error = error;
}

//------------------------------------------------------------------------------
bool ErrorUnitTest::test3()
{
    //
    // Procedure: register an error handler for a particular instance and report
    // a high severity error followed by a low severity error
    //
    // Test: error handler (test3ErrorHandler()) should only be called for the
    // first error
    //
    // Note: the min reporting severity is configured to high in
    // ApplicationUnitTestApp
    //

    TestModule testModule;

    TestModule::Error::registerCallback(
                              createCallback(&ErrorUnitTest::test3ErrorHandler),
                              &testModule);

    TestModule::Error error1 = testModule.reportError1HighSeverity();

    TestModule::Error error2 = testModule.reportError1LowSeverity();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(error1, myTest3Error));
}

//------------------------------------------------------------------------------
void ErrorUnitTest::test3ErrorHandler(const TestModule::Error& error)
{
    myTest3Error = error;
}

//------------------------------------------------------------------------------
bool ErrorUnitTest::test4()
{
    //
    // Procedure: register an error handler for a particular instance and report
    // two different high severity errors
    //
    // Test: error handler (test4ErrorHandler()) should be called once,
    // received error object should match the first error reported
    //

    TestModule testModule1;

    TestModule testModule2;

    TestModule::Error::registerCallback(
                              createCallback(&ErrorUnitTest::test5ErrorHandler),
                              &testModule1);

    TestModule::Error error1 = testModule1.reportError1HighSeverity();

    TestModule::Error error2 = testModule2.reportError2HighSeverity();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(error1, myTest5Error));
}

//------------------------------------------------------------------------------
void ErrorUnitTest::test4ErrorHandler(const TestModule::Error& error)
{
    myTest4Error = error;
}

//------------------------------------------------------------------------------
bool ErrorUnitTest::test5()
{
    //
    // Procedure: register an error handler for all instances and report two
    // different high severity errors
    //
    // Test: error handler (test5ErrorHandler()) should be called twice,
    // received error object should match the second error reported
    //

    TestModule testModule1;

    TestModule testModule2;

    TestModule::Error::registerCallback(
                             createCallback(&ErrorUnitTest::test4ErrorHandler));

    TestModule::Error error1 = testModule1.reportError1HighSeverity();

    TestModule::Error error2 = testModule2.reportError2HighSeverity();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(error2, myTest4Error));
}

//------------------------------------------------------------------------------
void ErrorUnitTest::test5ErrorHandler(const TestModule::Error& error)
{
    myTest5Error = error;
}
