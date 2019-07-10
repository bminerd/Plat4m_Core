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
/// @file ArrayUnitTest.cpp
/// @author Ben Minerd
/// @date 5/9/16
/// @brief ArrayUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <string.h>

#include <Plat4m_Core/UnitTest/ArrayUnitTest.h>

using Plat4m::ArrayUnitTest;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction ArrayUnitTest::myTestCallbackFunctions[] =
{
    &ArrayUnitTest::constructor1Test1,

    &ArrayUnitTest::constructor2Test1,
    &ArrayUnitTest::constructor2Test2,

    &ArrayUnitTest::copyConstructorTest1,

    &ArrayUnitTest::setItemsTest1,
    &ArrayUnitTest::setItemsTest2,

    &ArrayUnitTest::setSizeTest1,
    &ArrayUnitTest::setSizeTest2,

    &ArrayUnitTest::append1Test1,
    &ArrayUnitTest::append1Test2,

    &ArrayUnitTest::append2Test1,
    &ArrayUnitTest::append2Test2,
    &ArrayUnitTest::append2Test3,
    &ArrayUnitTest::append2Test4,

    &ArrayUnitTest::append3Test1,
    &ArrayUnitTest::append3Test2,
    &ArrayUnitTest::append3Test3,
    &ArrayUnitTest::append3Test4,

    &ArrayUnitTest::prepend1Test1,
    &ArrayUnitTest::prepend1Test2,
    &ArrayUnitTest::prepend1Test3,

    &ArrayUnitTest::prepend2Test1,
    &ArrayUnitTest::prepend2Test2,
    &ArrayUnitTest::prepend2Test3,
    &ArrayUnitTest::prepend2Test4,

    &ArrayUnitTest::prepend3Test1,
    &ArrayUnitTest::prepend3Test2,
    &ArrayUnitTest::prepend3Test3,
    &ArrayUnitTest::prepend3Test4
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ArrayUnitTest::ArrayUnitTest() :
    UnitTest("ArrayUnitTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ArrayUnitTest::~ArrayUnitTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ArrayUnitTest::constructor1Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object without parameters
    //
    // Test: Verify array.getItems() returns a null pointer
    //

    // Setup / Operation

    Array<uint8_t> array;

    // Test

    return UNIT_TEST_REPORT(
                        UNIT_TEST_CASE_EQUAL(array.getItems(), (uint8_t*) 0) &
                        UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 0)  &
                        UNIT_TEST_CASE_EQUAL(array.getMaxSize(), (uint32_t) 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::constructor2Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array
    //
    // Test: Verify array.getItems() returns a pointer to items array
    //

    // Setup

    uint8_t items[10];

    // Operation

    Array<uint8_t> array(items, arraySize(items));

    // Test

    return UNIT_TEST_REPORT(
                      UNIT_TEST_CASE_EQUAL(array.getItems(), (uint8_t*) items) &
                      UNIT_TEST_CASE_EQUAL(array.getMaxSize(), (uint32_t) 11)  &
                      UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::constructor2Test2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 4 items used
    //
    // Test: Verify array.getSize() returns 4
    //

    // Setup

    uint8_t items[10];

    // Operation

    Array<uint8_t> array(items, arraySize(items), 4);

    // Test

    return UNIT_TEST_REPORT(
                      UNIT_TEST_CASE_EQUAL(array.getItems(), (uint8_t*) items) &
                      UNIT_TEST_CASE_EQUAL(array.getMaxSize(), (uint32_t) 10)  &
                      UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 4));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::copyConstructorTest1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object as a copy of a 10 item
    // Array
    //
    // Test: Verify array.getItems() returns a pointer to items array
    //

    // Setup

    uint8_t items[10];
    Array<uint8_t> array1(items, arraySize(items));

    // Operation

    Array<uint8_t> array(array1);

    // Test

    return UNIT_TEST_REPORT(
                      UNIT_TEST_CASE_EQUAL(array.getItems(), (uint8_t*) items) &
                      UNIT_TEST_CASE_EQUAL(array.getMaxSize(), (uint32_t) 10)  &
                      UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::setItemsTest1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object without parameters, then
    // set item array
    //
    // Test: Verify array.getItems() returns a pointer to items array
    //

    // Setup

    Array<uint8_t> array;

    uint8_t items[10];

    // Operation

    array.setItems(items, arraySize(items));

    // Test

    return UNIT_TEST_REPORT(
                      UNIT_TEST_CASE_EQUAL(array.getItems(), (uint8_t*) items) &
                      UNIT_TEST_CASE_EQUAL(array.getMaxSize(), (uint32_t) 10)  &
                      UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::setItemsTest2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object without parameters, then
    // set item array with 4 used items
    //
    // Test: Verify array.getSize() returns 4
    //

    // Setup

    Array<uint8_t> array;

    uint8_t items[10];

    // Operation

    array.setItems(items, arraySize(items), 4);

    // Test

    return UNIT_TEST_REPORT(
                      UNIT_TEST_CASE_EQUAL(array.getItems(), (uint8_t*) items) &
                      UNIT_TEST_CASE_EQUAL(array.getMaxSize(), (uint32_t) 10)  &
                      UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 4));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::setSizeTest1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array,
    // then set its size to 4
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    Array<uint8_t> array(items, arraySize(items));

    // Operation

    bool returnValue = array.setSize(4);

    // Test

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true) &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 4));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::setSizeTest2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array,
    // then try to set its size to 12
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    Array<uint8_t> array(items, arraySize(items));

    // Operation

    bool returnValue = array.setSize(12);

    // Test

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false) &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append1Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then append a value
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    // Operation

    bool returnValue = array.append(0x01);

    // Test

    const uint8_t testItems[10] =
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 1) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append1Test2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 10 used items, then try to append a value
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), arraySize(items));

    // Operation

    bool returnValue = array.append(0x01);

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false)             &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append2Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then append a 5 item array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t appendItems[5] = {0x01, 0x02, 0x03, 0x04, 0x05};

    // Operation

    bool returnValue = array.append(appendItems, arraySize(appendItems));

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 5) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append2Test2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 6 used items, then append a 4 item array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 6);

    uint8_t appendItems[4] = {0x01, 0x02, 0x03, 0x04};

    // Operation

    bool returnValue = array.append(appendItems, arraySize(appendItems));

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x01,
        0x02,
        0x03,
        0x04
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, true)              &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append2Test3()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then try to append an 11 item array
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t appendItems[11] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B
    };

    // Operation

    bool returnValue = array.append(appendItems, arraySize(appendItems));

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, false)            &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 0) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append2Test4()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then append an 11 item array with greedy parameter set true
    //
    // Test: Verify array[9] returns 0x0A
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t appendItems[11] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B
    };

    // Operation

    bool returnValue = array.append(appendItems, arraySize(appendItems), true);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false)             &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append3Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then append a 5 item Array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t appendItems[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    Array<uint8_t> appendArray(appendItems, arraySize(appendItems));

    // Operation

    bool returnValue = array.append(appendArray);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 5) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append3Test2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 6 used items, then append a 4 item Array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 6);

    uint8_t appendItems[4] = {0x01, 0x02, 0x03, 0x04};
    Array<uint8_t> appendArray(appendItems, arraySize(appendItems));

    // Operation

    bool returnValue = array.append(appendArray);

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x01,
        0x02,
        0x03,
        0x04
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, true)              &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append3Test3()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then try to append an 11 item Array
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t appendItems[11] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B
    };

    Array<uint8_t> appendArray(appendItems, arraySize(appendItems));

    // Operation

    bool returnValue = array.append(appendArray);

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, false)            &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 0) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::append3Test4()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then append an 11 item Array with greedy parameter set true
    //
    // Test: Verify memoryCompateResult is 0
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t appendItems[11] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B
    };

    Array<uint8_t> appendArray(appendItems, arraySize(appendItems));

    // Operation

    bool returnValue = array.append(appendArray, true);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false)             &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend1Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then prepend single value
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    // Operation

    bool returnValue = array.prepend(0x01);

    // Test

    const uint8_t testItems[10] =
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 1) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend1Test2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 1 used item, then prepend an item
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    items[0] = 0x02;
    Array<uint8_t> array(items, arraySize(items), 1);

    // Operation

    bool returnValue = array.prepend(0x01);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 2) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend1Test3()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 10 used items, then try to prepend an item
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), arraySize(items));

    // Operation

    bool returnValue = array.prepend(0x01);

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false)             &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend2Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then prepend a 5 item array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t prependItems[5] = {0x01, 0x02, 0x03, 0x04, 0x05};

    // Operation

    bool returnValue = array.prepend(prependItems, arraySize(prependItems));

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 5) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend2Test2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 6 used items, then prepend a 4 item array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    items[0] = 0x05;
    items[1] = 0x06;
    items[2] = 0x07;
    items[3] = 0x08;
    items[4] = 0x09;
    items[5] = 0x0A;
    Array<uint8_t> array(items, arraySize(items), 6);

    uint8_t prependItems[4] = {0x01, 0x02, 0x03, 0x04};

    // Operation

    bool returnValue = array.prepend(prependItems, arraySize(prependItems));

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, true)              &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend2Test3()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then try to prepend an 11 item array
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t prependItems[11];

    // Operation

    bool returnValue = array.prepend(prependItems, arraySize(prependItems));

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, false)            &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 0) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend2Test4()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then append an 11 item array with greedy parameter set true
    //
    // Test: Verify memoryCompareResult is 0
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t prependItems[11] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B
    };

    // Operation

    bool returnValue = array.prepend(prependItems,
                                     arraySize(prependItems),
                                     true);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false)             &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend3Test1()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then prepend a 5 item Array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t prependItems[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    Array<uint8_t> prependArray(prependItems, arraySize(prependItems));

    // Operation

    bool returnValue = array.prepend(prependArray);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 5) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend3Test2()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 6 used items, then prepend a 4 item Array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    items[0] = 0x05;
    items[1] = 0x06;
    items[2] = 0x07;
    items[3] = 0x08;
    items[4] = 0x09;
    items[5] = 0x0A;
    Array<uint8_t> array(items, arraySize(items), 6);

    uint8_t prependItems[4] = {0x01, 0x02, 0x03, 0x04};
    Array<uint8_t> prependArray(prependItems, arraySize(prependItems));

    // Operation

    bool returnValue = array.prepend(prependArray);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, true)              &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend3Test3()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then try to prepend an 11 item Array
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t prependItems[11] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B
    };

    Array<uint8_t> prependArray(prependItems, arraySize(prependItems));

    // Operation

    bool returnValue = array.prepend(prependArray);

    // Test

    const uint8_t testItems[10] =
    {
        0x00,
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

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, false)            &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 0) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ArrayUnitTest::prepend3Test4()
{
    //
    // Procedure: Instantiate an Array<uint8_t> object with a 10 item array and
    // 0 used items, then prepend an 11 item Array with greedy parameter set
    // true
    //
    // Test: Verify memoryCompareResult is 0
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    Array<uint8_t> array(items, arraySize(items), 0);

    uint8_t prependItems[11] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A,
        0x0B
    };

    Array<uint8_t> prependArray(prependItems, arraySize(prependItems));

    // Operation

    bool returnValue = array.prepend(prependArray, true);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0A
    };

    int memoryCompareResult = memcmp(items, testItems, arraySize(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false)             &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}
