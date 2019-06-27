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
/// @file ModuleUnitTest.cpp
/// @author Ben Minerd
/// @date 3/22/13
/// @brief ModuleUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/UnitTest/ModuleUnitTest.h>
#include <Plat4m_Core/CallbackMethodParameter.h>

using Plat4m::ModuleUnitTest;
using Plat4m::Module;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction ModuleUnitTest::myTestCallbackFunctions[] =
{
    &ModuleUnitTest::enableTest1,
    &ModuleUnitTest::enableTest2,
    &ModuleUnitTest::enableTest3,
    &ModuleUnitTest::enableTest4,
    &ModuleUnitTest::enableTest5,
    &ModuleUnitTest::enableTest6,
    &ModuleUnitTest::enableTest7,
    &ModuleUnitTest::enableTest8,
    &ModuleUnitTest::enableTest9,
    &ModuleUnitTest::enableTest10,
    &ModuleUnitTest::enableTest11,
    &ModuleUnitTest::enableTest12,
    &ModuleUnitTest::enableTest13,
    &ModuleUnitTest::enableTest14,
    &ModuleUnitTest::enableTest15,
    &ModuleUnitTest::enableTest16,
    &ModuleUnitTest::enableTest17,
    &ModuleUnitTest::enableTest18
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ModuleUnitTest::ModuleUnitTest() :
    Module(),
    UnitTest("ModuleUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions)),
    myWasInterfaceEnableCalled(false),
    myWasDriverEnableCalled(false),
    myWasEnableCallbackCalled(false),
    myInterfaceEnableError(Module::ERROR_CODE_NONE),
    myDriverEnableError(Module::ERROR_CODE_NONE),
    myEnableCallbackError(Module::ERROR_CODE_NONE)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ModuleUnitTest::~ModuleUnitTest()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest1()
{
    //
    // Procedure: call enable(true)
    //
    // Test: error code should be ERROR_CODE_NONE
    //

    ModuleUnitTest module;

    Module::Error error = module.enable(true);

    return UNIT_TEST_REPORT(
                UNIT_TEST_CASE_EQUAL(error.getCode(), Module::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest2()
{
    //
    // Procedure: call enable(true)
    //
    // Test: module.myWasInterfaceEnableCalled should be true
    //

    ModuleUnitTest module;

    module.enable(true);

    return UNIT_TEST_REPORT(
                 UNIT_TEST_CASE_EQUAL(module.myWasInterfaceEnableCalled, true));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest3()
{
    //
    // Procedure: call enable(true)
    //
    // Test: module.myWasDriverEnableCalled should be true
    //

    ModuleUnitTest module;

    module.enable(true);

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(module.myWasDriverEnableCalled, true));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest4()
{
    //
    // Procedure: call enable(true) with callback
    //
    // Test: module.myWasEnableCallbackCalled should be true
    //

    ModuleUnitTest module;

    CallbackMethodParameter<ModuleUnitTest, Module::Error, bool>
                             callback(&module, &ModuleUnitTest::enableCallback);
    module.setEnableCallback(callback);

    module.enable(true);

    return UNIT_TEST_REPORT(
                  UNIT_TEST_CASE_EQUAL(module.myWasEnableCallbackCalled, true));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest5()
{
    //
    // Procedure: call enable(true)
    //
    // Test: isEnabled should be true
    //

    ModuleUnitTest module;

    module.enable(true);

    bool isEnabled = module.isEnabled();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(isEnabled, true));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest6()
{
    //
    // Procedure: call enable(true)
    //
    // Test: isEnabled should be true
    //

    ModuleUnitTest module;

    CallbackMethodParameter<ModuleUnitTest, Module::Error, bool>
                             callback(&module, &ModuleUnitTest::enableCallback);
    module.setEnableCallback(callback);

    module.enable(true);

    bool isEnabled = module.isEnabled();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(isEnabled, true));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest7()
{
    //
    // Procedure: call enable(true) after already enabled
    //
    // Test: module.myWasInterfaceEnableCalled should be false
    //

    ModuleUnitTest module;

    module.enable(true);
    module.myWasInterfaceEnableCalled = false;

    module.enable(true);

    return UNIT_TEST_REPORT(
                UNIT_TEST_CASE_EQUAL(module.myWasInterfaceEnableCalled, false));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest8()
{
    //
    // Procedure: call enable(true) after already enabled
    //
    // Test: module.myWasDriverEnableCalled should be false
    //

    ModuleUnitTest module;

    module.enable(true);
    module.myWasDriverEnableCalled = false;

    module.enable(true);

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(module.myWasDriverEnableCalled, false));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest9()
{
    //
    // Procedure: call enable(true) after already enabled
    //
    // Test: module.myWasEnableCallbackCalled should be false
    //

    ModuleUnitTest module;

    module.enable(true);
    module.myWasEnableCallbackCalled = false;

    module.enable(true);

    return UNIT_TEST_REPORT(
                 UNIT_TEST_CASE_EQUAL(module.myWasEnableCallbackCalled, false));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest10()
{
    //
    // Procedure: call enable(true) with driver error
    //
    // Test: error.getCode() should be ERROR_CODE_ENABLE_FAILED
    //

    ModuleUnitTest module;
    module.myDriverEnableError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    Module::Error error = module.enable(true);

    return UNIT_TEST_REPORT(
                        UNIT_TEST_CASE_EQUAL(error.getCode(),
                                             Module::ERROR_CODE_ENABLE_FAILED));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest11()
{
    //
    // Procedure: call enable(true) with driver error
    //
    // Test: isEnabled should be false
    //

    ModuleUnitTest module;
    module.myDriverEnableError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    module.enable(true);

    bool isEnabled = module.isEnabled();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(isEnabled, false));
}


//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest12()
{
    //
    // Procedure: call enable(true) with driver error
    //
    // Test: module.myWasInterfaceEnableCalled should be false
    //

    ModuleUnitTest module;
    module.myDriverEnableError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    module.enable(true);

    return UNIT_TEST_REPORT(
                UNIT_TEST_CASE_EQUAL(module.myWasInterfaceEnableCalled, false));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest13()
{
    //
    // Procedure: call enable(true) with driver error
    //
    // Test: module.myWasEnableCallbackCalled should be false
    //

    ModuleUnitTest module;
    module.myDriverEnableError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    module.enable(true);

    return UNIT_TEST_REPORT(
                 UNIT_TEST_CASE_EQUAL(module.myWasEnableCallbackCalled, false));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest14()
{
    //
    // Procedure: call enable(true) with interface error
    //
    // Test: error.getCode() should be ERROR_CODE_ENABLE_FAILED
    //

    ModuleUnitTest module;
    module.myInterfaceEnableError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    Module::Error error = module.enable(true);

    return UNIT_TEST_REPORT(
                        UNIT_TEST_CASE_EQUAL(error.getCode(),
                                             Module::ERROR_CODE_ENABLE_FAILED));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest15()
{
    //
    // Procedure: call enable(true) with interface error
    //
    // Test: isEnabled should be false
    //

    ModuleUnitTest module;
    module.myInterfaceEnableError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    module.enable(true);

    bool isEnabled = module.isEnabled();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(isEnabled, false));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest16()
{
    //
    // Procedure: call enable(true) with interface error
    //
    // Test: module.myWasDriverEnableCalled should be false
    //

    ModuleUnitTest module;
    module.myInterfaceEnableError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    module.enable(true);

    return UNIT_TEST_REPORT(
                 UNIT_TEST_CASE_EQUAL(module.myWasEnableCallbackCalled, false));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest17()
{
    //
    // Procedure: call enable(true) with callback error
    //
    // Test: error.getCode() should be ERROR_CODE_ENABLE_FAILED
    //

    ModuleUnitTest module;

    CallbackMethodParameter<ModuleUnitTest, Module::Error, bool>
                             callback(&module, &ModuleUnitTest::enableCallback);
    module.setEnableCallback(callback);

    module.myEnableCallbackError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    Module::Error error = module.enable(true);

    return UNIT_TEST_REPORT(
                        UNIT_TEST_CASE_EQUAL(error.getCode(),
                                             Module::ERROR_CODE_ENABLE_FAILED));
}

//------------------------------------------------------------------------------
bool ModuleUnitTest::enableTest18()
{
    //
    // Procedure: call enable(true) with callback error
    //
    // Test: isEnabled should be false
    //

    ModuleUnitTest module;

    CallbackMethodParameter<ModuleUnitTest, Module::Error, bool>
                             callback(&module, &ModuleUnitTest::enableCallback);
    module.setEnableCallback(callback);

    module.myEnableCallbackError =
                                Module::Error(Module::ERROR_CODE_ENABLE_FAILED);

    module.enable(true);

    bool isEnabled = module.isEnabled();

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(isEnabled, false));
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ModuleUnitTest::interfaceEnable(const bool enable)
{
    myWasInterfaceEnableCalled = true;

    return myInterfaceEnableError;
}

//------------------------------------------------------------------------------
Module::Error ModuleUnitTest::driverEnable(const bool enable)
{
    myWasDriverEnableCalled = true;

    return myDriverEnableError;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ModuleUnitTest::enableCallback(const bool enable)
{
    myWasEnableCallbackCalled = true;

    return myEnableCallbackError;
}
