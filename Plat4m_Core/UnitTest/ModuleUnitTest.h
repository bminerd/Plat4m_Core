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
/// @file ModuleUnitTest.h
/// @author Ben Minerd
/// @date 4/21/16
/// @brief ModuleUnitTest class header file.
///

#ifndef PLAT4M_MODULE_UNIT_TEST_H
#define PLAT4M_MODULE_UNIT_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/UnitTest/UnitTest.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ModuleUnitTest : public Module, public UnitTest
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ModuleUnitTest();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ModuleUnitTest();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static bool enableTest1();

    static bool enableTest2();

    static bool enableTest3();

    static bool enableTest4();

    static bool enableTest5();

    static bool enableTest6();

    static bool enableTest7();

    static bool enableTest8();

    static bool enableTest9();

    static bool enableTest10();

    static bool enableTest11();

    static bool enableTest12();

    static bool enableTest13();

    static bool enableTest14();

    static bool enableTest15();

    static bool enableTest16();

    static bool enableTest17();

    static bool enableTest18();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const UnitTest::TestCallbackFunction myTestCallbackFunctions[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    bool myWasInterfaceEnableCalled;

    bool myWasDriverEnableCalled;

    bool myWasEnableCallbackCalled;

    Module::Error myInterfaceEnableError;

    Module::Error myDriverEnableError;

    Module::Error myEnableCallbackError;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------
    
    virtual Module::Error interfaceEnable(const bool enable);

    virtual Module::Error driverEnable(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    Module::Error enableCallback(const bool enable);
};

}; // namespace Plat4m

#endif // PLAT4M_MODULE_UNIT_TEST_H
