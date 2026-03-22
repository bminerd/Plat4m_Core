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
/// @file ErrorUnitTest.h
/// @author Ben Minerd
/// @date 11/9/2023
/// @brief ErrorUnitTest class header file.
///

#ifndef PLAT4M_ERROR_UNIT_TEST_H
#define PLAT4M_ERROR_UNIT_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/UnitTest/UnitTest.h>
#include <Plat4m_Core/ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ErrorUnitTest : public UnitTest
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_TEST
    };

    using Error = ErrorTemplate<ErrorCode>;

    //--------------------------------------------------------------------------
    // Public nested classes
    //--------------------------------------------------------------------------

    class TestModule
    {
    public:

        //----------------------------------------------------------------------
        // Public types
        //----------------------------------------------------------------------

        enum ErrorCode
        {
            ERROR_CODE_NONE = 0,
            ERROR_CODE_TEST_1,
            ERROR_CODE_TEST_2
        };

        using Error = ErrorTemplate<ErrorCode>;

        //----------------------------------------------------------------------
        // Public methods
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        Error reportError1LowSeverity()
        {
            return PLAT4M_REPORT_ERROR(TestModule::Error,
                                       TestModule::ERROR_CODE_TEST_1,
                                       ErrorBase::SEVERITY_LOW,
                                       this);
        }

        //----------------------------------------------------------------------
        Error reportError1HighSeverity()
        {
            return PLAT4M_REPORT_ERROR(TestModule::Error,
                                       TestModule::ERROR_CODE_TEST_1,
                                       ErrorBase::SEVERITY_HIGH,
                                       this);
        }

        //----------------------------------------------------------------------
        Error reportError2HighSeverity()
        {
            return PLAT4M_REPORT_ERROR(TestModule::Error,
                                       TestModule::ERROR_CODE_TEST_2,
                                       ErrorBase::SEVERITY_HIGH,
                                       this);
        }
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ErrorUnitTest();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ErrorUnitTest();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static bool test1();

    static void test1ErrorHandler(const Error& error);

    static bool test2();

    static void test2ErrorHandler(const TestModule::Error& error);

    static bool test3();

    static void test3ErrorHandler(const TestModule::Error& error);

    static bool test4();

    static void test4ErrorHandler(const TestModule::Error& error);

    static bool test5();

    static void test5ErrorHandler(const TestModule::Error& error);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const UnitTest::TestCallbackFunction myTestCallbackFunctions[];

    static Error myTest1Error;

    static TestModule::Error myTest2Error;

    static TestModule::Error myTest3Error;

    static TestModule::Error myTest4Error;

    static TestModule::Error myTest5Error;
};

}; // namespace Plat4m

#endif // PLAT4M_MODULE_UNIT_TEST_H
