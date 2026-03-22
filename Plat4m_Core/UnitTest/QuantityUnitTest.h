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
/// @file QuantityUnitTest.h
/// @author Ben Minerd
/// @date 11/1/2023
/// @brief QuantityUnitTest class header file.
///

#ifndef PLAT4M_QUANTITY_UNIT_TEST_H
#define PLAT4M_QUANTITY_UNIT_TEST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/UnitTest/UnitTest.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class QuantityUnitTest : public UnitTest
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    QuantityUnitTest();

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~QuantityUnitTest();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static bool operatorGreaterThanTest();

    static bool operatorLessThanTest();

    static bool operatorEqualsTest();

    static bool operatorGreaterThanOrEqualToTest();

    static bool operatorLessThanOrEqualToTest();

    static bool operatorAddTest1();
    static bool operatorAddTest2();
    static bool operatorAddTest3();

    static bool operatorAddEqualsTest1();
    static bool operatorAddEqualsTest2();

    static bool operatorSubtractTest1();
    static bool operatorSubtractTest2();
    static bool operatorSubtractTest3();

    static bool operatorSubtractEqualsTest1();
    static bool operatorSubtractEqualsTest2();

    static bool operatorModulusTest();

    static bool fromTest1();
    static bool fromTest2();
    static bool fromTest3();
    static bool fromTest4();
    static bool fromTest5();
    static bool fromTest6();
    static bool fromTest7();
    static bool fromTest8();
    static bool fromTest9();
    static bool fromTest10();
    static bool fromTest11();
    static bool fromTest12();
    static bool fromTest13();
    static bool fromTest14();
    static bool fromTest15();
    static bool fromTest16();
    static bool fromTest17();

    static bool toTest1();
    static bool toTest2();
    static bool toTest3();
    static bool toTest4();
    static bool toTest5();
    static bool toTest6();
    static bool toTest7();
    static bool toTest8();
    static bool toTest9();
    static bool toTest10();
    static bool toTest11();
    static bool toTest12();
    static bool toTest13();
    static bool toTest14();
    static bool toTest15();
    static bool toTest16();
    static bool toTest17();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const UnitTest::TestCallbackFunction myTestCallbackFunctions[];
};

}; // namespace Plat4m

#endif // PLAT4M_QUANTITY_UNIT_TEST_H
