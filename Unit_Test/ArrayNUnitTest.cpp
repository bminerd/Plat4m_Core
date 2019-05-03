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
// Permission is hereby granted, free of uint8_tge, to any person obtaining a copy
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
/// @file ArrayNUnitTest.cpp
/// @author Ben Minerd
/// @date 5/13/16
/// @brief ArrayNUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ArrayNUnitTest.h>

using Plat4m::ArrayNUnitTest;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction ArrayNUnitTest::myTestCallbackFunctions[] =
{
    &ArrayNUnitTest::constructorTest1,

    &ArrayNUnitTest::copyConstructorTest1
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ArrayNUnitTest::ArrayNUnitTest() :
    UnitTest("ArrayNUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ArrayNUnitTest::~ArrayNUnitTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ArrayNUnitTest::constructorTest1()
{
    //
    // Procedure: Instantiate an ArrayN<uint8_t, 10> object without parameters
    //
    // Test: Verify array.getSize() returns 0
    //

    // Setup / Operation

    ArrayN<uint8_t, 10> array;

    // Test

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(array.getMaxSize(), (unsigned int) 10) &
                   UNIT_TEST_CASE_EQUAL(array.getSize(), (unsigned int) 0));
}

//------------------------------------------------------------------------------
bool ArrayNUnitTest::copyConstructorTest1()
{
    //
    // Procedure: Instantiate an ArrayN<uint8_t, 10> object as a copy of a 10
    // item ArrayN object
    //
    // Test: Verify array.getItems() returns a pointer to items array
    //

    // Setup

    ArrayN<uint8_t, 10> array1;
    array1.append(0x01);

    // Operation

    ArrayN<uint8_t, 10> array(array1);

    // Test

    const uint8_t testItems[] =
    {
        0x01,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(array.getItems(),
                                     testItems,
                                     ARRAY_SIZE(testItems));

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(array.getSize(), (unsigned int) 1) &
                       UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}
