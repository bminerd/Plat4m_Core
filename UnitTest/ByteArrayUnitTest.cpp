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
/// @file ByteArrayUnitTest.cpp
/// @author Ben Minerd
/// @date 5/9/16
/// @brief ByteArrayUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <string.h>

#include <Plat4m_Core/UnitTest/ByteArrayUnitTest.h>

using Plat4m::ByteArrayUnitTest;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction
                                  ByteArrayUnitTest::myTestCallbackFunctions[] =
{
    &ByteArrayUnitTest::constructor1Test1,

    &ByteArrayUnitTest::constructor2Test1,
    &ByteArrayUnitTest::constructor2Test2,

    &ByteArrayUnitTest::constructor3Test1,

    &ByteArrayUnitTest::constructor4Test1,

    &ByteArrayUnitTest::append1Test1,
    &ByteArrayUnitTest::append1Test2,

    &ByteArrayUnitTest::append2Test1,
    &ByteArrayUnitTest::append2Test2,
    &ByteArrayUnitTest::append2Test3,
    &ByteArrayUnitTest::append2Test4,

    &ByteArrayUnitTest::prependTest1,
    &ByteArrayUnitTest::prependTest2,
    &ByteArrayUnitTest::prependTest3,

    &ByteArrayUnitTest::insertTest1,
    &ByteArrayUnitTest::insertTest2,
    &ByteArrayUnitTest::insertTest3,

    &ByteArrayUnitTest::setValueTest1
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteArrayUnitTest::ByteArrayUnitTest() :
    UnitTest("ByteArrayUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteArrayUnitTest::~ByteArrayUnitTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::constructor1Test1()
{
    //
    // Procedure: Instantiate an ByteArray object without parameters
    //
    // Test: Verify array.getItems() returns a null pointer
    //

    // Setup / Operation

    ByteArray byteArray;

    // Test

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(byteArray.getItems(), (uint8_t*) 0)   &
                    UNIT_TEST_CASE_EQUAL(byteArray.getMaxSize(), (uint32_t) 0) &
                    UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::constructor2Test1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array
    //
    // Test: Verify array.getItems() returns a pointer to items array
    //

    // Setup

    uint8_t items[10];

    // Operation

    ByteArray byteArray(items, ARRAY_SIZE(items));

    // Test

    return UNIT_TEST_REPORT(
                  UNIT_TEST_CASE_EQUAL(byteArray.getItems(), (uint8_t*) items) &
                  UNIT_TEST_CASE_EQUAL(byteArray.getMaxSize(), (uint32_t) 10)  &
                  UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 10));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::constructor2Test2()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 4 items used
    //
    // Test: Verify array.getSize() returns 4
    //

    // Setup

    uint8_t items[10];

    // Operation

    ByteArray byteArray(items, ARRAY_SIZE(items), 4);

    // Test

    return UNIT_TEST_REPORT(
                  UNIT_TEST_CASE_EQUAL(byteArray.getItems(), (uint8_t*) items) &
                  UNIT_TEST_CASE_EQUAL(byteArray.getMaxSize(), (uint32_t) 10)  &
                  UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 4));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::constructor3Test1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array
    //
    // Test: Verify array.getItems() returns a pointer to items array
    //

    const char* string = "Abc123";

    // Setup/operation

    ByteArray byteArray(string);

    // Test

    return UNIT_TEST_REPORT(
                 UNIT_TEST_CASE_EQUAL(byteArray.getItems(), (uint8_t*) string) &
                 UNIT_TEST_CASE_EQUAL(byteArray.getMaxSize(), (uint32_t) 6)  &
                 UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 6));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::constructor4Test1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 4 item Array
    //
    // Test: Verify array.getItems() returns a pointer to items array
    //

    uint32_t items[4];

    Array<uint32_t> array(items, arraySize(items));

    // Setup/operation

    ByteArray byteArray(array);

    // Test

    return UNIT_TEST_REPORT(
                  UNIT_TEST_CASE_EQUAL(byteArray.getItems(), (uint8_t*) items) &
                  UNIT_TEST_CASE_EQUAL(byteArray.getMaxSize(), (uint32_t) 16)  &
                  UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 16));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::append1Test1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 0 used items, then append a value
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray byteArray(items, ARRAY_SIZE(items), 0);

    const char* string = "Abc123";

    // Operation

    bool returnValue = byteArray.append(string);

    // Test

    const uint8_t testItems[10] =
    {
        'A',
        'b',
        'c',
        '1',
        '2',
        '3',
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(returnValue, true)                 &
                       UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 6) &
                       UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::append1Test2()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 0 used items, then append a value
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10] =
    {
        'A',
        'b',
        'c',
        'd',
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    ByteArray byteArray(items, ARRAY_SIZE(items), 4);

    const char* appendString = "123456";

    // Operation

    bool returnValue = byteArray.append(appendString);

    // Test

    const uint8_t testItems[10] =
    {
        'A',
        'b',
        'c',
        'd',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6'
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                      UNIT_TEST_CASE_EQUAL(returnValue, true)                  &
                      UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 10) &
                      UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::append1Test3()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 0 used items, then append a value
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray byteArray(items, ARRAY_SIZE(items), 0);

    const char* appendString = "Abcefg12356";

    // Operation

    bool returnValue = byteArray.append(appendString);

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

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(returnValue, false)                &
                       UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 0) &
                       UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::append2Test1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 0 used items, then append a 5 item array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray array(items, ARRAY_SIZE(items), 0);

    uint8_t appendItems[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    ByteArray appendByteArray(appendItems, ARRAY_SIZE(appendItems));

    // Operation

    bool returnValue = array.append(appendByteArray);

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

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 5) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::append2Test2()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 6 used items, then append a 4 item array
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray array(items, ARRAY_SIZE(items), 6);

    uint8_t appendItems[4] = {0x01, 0x02, 0x03, 0x04};
    ByteArray appendByteArray(appendItems, ARRAY_SIZE(appendItems));

    // Operation

    bool returnValue = array.append(appendByteArray);

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

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, true)              &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::append2Test3()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 0 used items, then try to append an 11 item array
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray array(items, ARRAY_SIZE(items), 0);

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

    ByteArray appendByteArray(appendItems, ARRAY_SIZE(appendItems));

    // Operation

    bool returnValue = array.append(appendByteArray);

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

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, false)            &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 0) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::append2Test4()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 0 used items, then append an 11 item array with greedy parameter set true
    //
    // Test: Verify array[9] returns 0x0A
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray array(items, ARRAY_SIZE(items), 0);

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

    ByteArray appendByteArray(appendItems, ARRAY_SIZE(appendItems));

    // Operation

    bool returnValue = array.append(appendByteArray, true);

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

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(returnValue, false)             &
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::prependTest1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 0 used items, then prepend single value
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray byteArray(items, ARRAY_SIZE(items), 0);

    const char* prependString = "Abc123";

    // Operation

    bool returnValue = byteArray.prepend(prependString);

    // Test

    const uint8_t testItems[10] =
    {
        'A',
        'b',
        'c',
        '1',
        '2',
        '3',
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                       UNIT_TEST_CASE_EQUAL(returnValue, true)                 &
                       UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 6) &
                       UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::prependTest2()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 1 used item, then prepend an item
    //
    // Test: Verify returnValue is true
    //

    // Setup

    uint8_t items[10] =
    {
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        0x00,
        0x00,
        0x00,
        0x00
    };

    ByteArray byteArray(items, ARRAY_SIZE(items), 6);

    // Operation

    bool returnValue = byteArray.prepend("Abcd");

    // Test

    const uint8_t testItems[10] =
    {
        'A',
        'b',
        'c',
        'd',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6'
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                      UNIT_TEST_CASE_EQUAL(returnValue, true)                  &
                      UNIT_TEST_CASE_EQUAL(byteArray.getSize(), (uint32_t) 10) &
                      UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::prependTest3()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 10 used items, then try to prepend an item
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray array(items, ARRAY_SIZE(items), 0);

    const char* prependString = "Abcdef123456";

    // Operation

    bool returnValue = array.prepend(prependString);

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

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, false)            &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 0) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::insertTest1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 10 used items, then try to prepend an item
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray array(items, ARRAY_SIZE(items), 0);

    const char* insertString = "Abc123";

    // Operation

    bool returnValue = array.insert(insertString, 0);

    // Test

    const uint8_t testItems[10] =
    {
        'A',
        'b',
        'c',
        '1',
        '2',
        '3',
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 6) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::insertTest2()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 10 used items, then try to prepend an item
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10] =
    {
        'A',
        '3',
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    ByteArray array(items, ARRAY_SIZE(items), 2);

    const char* insertString = "bc12";

    // Operation

    bool returnValue = array.insert(insertString, 1);

    // Test

    const uint8_t testItems[10] =
    {
        'A',
        'b',
        'c',
        '1',
        '2',
        '3',
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, true)             &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 6) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::insertTest3()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 10 used items, then try to prepend an item
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10] =
    {
        'A',
        '6',
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    ByteArray array(items, ARRAY_SIZE(items), 2);

    const char* insertString = "bcdef12345";

    // Operation

    bool returnValue = array.insert(insertString, 1);

    // Test

    const uint8_t testItems[10] =
    {
        'A',
        '6',
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                           UNIT_TEST_CASE_EQUAL(returnValue, false)            &
                           UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 2) &
                           UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}

//------------------------------------------------------------------------------
bool ByteArrayUnitTest::setValueTest1()
{
    //
    // Procedure: Instantiate an ByteArray object with a 10 item array and
    // 10 used items, then try to prepend an item
    //
    // Test: Verify returnValue is false
    //

    // Setup

    uint8_t items[10];
    memset(items, 0, sizeof(items));
    ByteArray array(items, ARRAY_SIZE(items), ARRAY_SIZE(items));

    // Operation

    array.setValue(0x01);

    // Test

    const uint8_t testItems[10] =
    {
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01
    };

    int memoryCompareResult = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                          UNIT_TEST_CASE_EQUAL(array.getSize(), (uint32_t) 10) &
                          UNIT_TEST_CASE_EQUAL(memoryCompareResult, 0));
}
