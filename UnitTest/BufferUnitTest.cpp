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
/// @file BufferUnitTest.cpp
/// @author Ben Minerd
/// @date 5/9/16
/// @brief BufferUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <string.h>

#include <Plat4m_Core/UnitTest/BufferUnitTest.h>

using Plat4m::BufferUnitTest;
using Plat4m::UnitTest;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction BufferUnitTest::myTestCallbackFunctions[] =
{
    &BufferUnitTest::constructor1Test1,

    &BufferUnitTest::copyConstructorTest1,

    &BufferUnitTest::write1Test1,
    &BufferUnitTest::write1Test2,
    &BufferUnitTest::write1Test3,

    &BufferUnitTest::write2Test1,
    &BufferUnitTest::write2Test2,

    &BufferUnitTest::read1Test1,
    &BufferUnitTest::read1Test2,
    &BufferUnitTest::read1Test3,
    &BufferUnitTest::read1Test4,

    &BufferUnitTest::read2Test1,
    &BufferUnitTest::read2Test2
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BufferUnitTest::BufferUnitTest() :
    UnitTest("BufferUnitTest",
             myTestCallbackFunctions,
             ARRAY_SIZE(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BufferUnitTest::~BufferUnitTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool BufferUnitTest::constructor1Test1()
{
    //
    // Procedure: Instantiate a Buffer<uint8_t> object with a 4 item array
    //
    // Test:
    // - Verify buffer.getItems() returns a pointer to items
    // - Verify buffer.getSize() returns 4
    // - Verify buffer.getWriteIndex() returns 0
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 0
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));

    // Operation

    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    // Test

    const uint8_t testItems[4] =
    {
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareValue = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(buffer.getItems(), (uint8_t*) items)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.getSize(), (uint32_t) 4)       &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 0) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 0)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 0)         &&
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::copyConstructorTest1()
{
    //
    // Procedure: Instantiate a Buffer<uint8_t> object as a copy of a 4 item
    // Buffer
    //
    // Test:
    // - Verify buffer.getItems() returns a pointer to items
    // - Verify buffer.getSize() returns 4
    // - Verify buffer.getWriteIndex() returns 0
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 0
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer1(items, ARRAY_SIZE(items));

    // Operation

    Buffer<uint8_t> buffer(buffer1);

    // Test

    const uint8_t testItems[4] =
    {
        0x00,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareValue = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(buffer.getItems(), (uint8_t*) items)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.getSize(), (uint32_t) 4)       &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 0) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 0)  &&
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::write1Test1()
{
    //
    // Procedure: Instantiate a Buffer<uint8_t> object with a 4 item array, then
    // append a single value
    //
    // Test:
    // - Verify buffer.write(0x01) returns true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    // Operation

    bool returnValue = buffer.write(0x01);

    // Test

    const uint8_t testItems[4] =
    {
        0x01,
        0x00,
        0x00,
        0x00
    };

    int memoryCompareValue = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, true)                    &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 1) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 0)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 1)         &&
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::write1Test2()
{
    //
    // Procedure: Instantiate a Buffer<uint8_t> object with a 4 item array, then
    // append a single value
    //
    // Test:
    // - Verify buffer.write(0x01) returns true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    buffer.write(0x02);
    buffer.write(0x03);
    buffer.write(0x04);

    uint8_t value;
    buffer.read(value);

    // Operation

    bool returnValue = buffer.write(0x05);

    // Test

    const uint8_t testItems[4] =
    {
        0x05,
        0x02,
        0x03,
        0x04
    };

    int memoryCompareValue = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, true)                    &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 1) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 1)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 4)         &&
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::write1Test3()
{
    //
    // Procedure: Instantiate a Buffer<uint8_t> object with a 4 item array,
    // append 4 values, then try to append a 5th value
    //
    // Test:
    // - Verify buffer.write(0x01) returns false
    // - Verify buffer.getWriteIndex() returns 0
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 4
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    buffer.write(0x02);
    buffer.write(0x03);
    buffer.write(0x04);

    // Operation

    bool returnValue = buffer.write(0x05);

    // Test

    const uint8_t testItems[4] =
    {
        0x01,
        0x02,
        0x03,
        0x04
    };

    int memoryCompareValue = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, false)                   &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 0) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 0)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 4)         &&
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::write2Test1()
{
    //
    // Procedure: Instantiate a Buffer<uint8_t> object with a 4 item array, then
    // write 1 value
    //
    // Test:
    // - Verify buffer.write(0x01) returns true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    uint8_t writeItems[3] = {0x01, 0x02, 0x03};
    Array<uint8_t> writeArray(writeItems, ARRAY_SIZE(writeItems));

    // Operation

    bool returnValue = buffer.write(writeArray);

    // Test

    const uint8_t testItems[4] =
    {
        0x01,
        0x02,
        0x03,
        0x00
    };

    int memoryCompareValue = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, true)                    &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 3) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 0)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 3)         &&
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::write2Test2()
{
    //
    // Procedure: Instantiate a Buffer<uint8_t> object with a 4 item array,
    // write 2 values, then try to append a 3 value array
    //
    // Test:
    // - Verify buffer.write(0x01) returns true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));
    buffer.write(0x01);
    buffer.write(0x02);

    uint8_t writeItems[3];
    Array<uint8_t> writeArray(writeItems, ARRAY_SIZE(writeItems));

    // Operation

    bool returnValue = buffer.write(writeArray);

    // Test

    const uint8_t testItems[4] =
    {
        0x01,
        0x02,
        0x00,
        0x00
    };

    int memoryCompareValue = memcmp(items, testItems, ARRAY_SIZE(items));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, false)                   &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 2) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 0)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 2)         &&
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::read1Test1()
{
    //
    // Procedure: Read a single item.
    //
    // Test:
    // - Verify returnValue is true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);

    uint8_t value = 0x00;

    // Operation

    bool returnValue = buffer.read(value);

    // Test

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, true)                    &&
                   UNIT_TEST_CASE_EQUAL(value, (uint8_t) 0x01)                &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 1) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 1)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::read1Test2()
{
    //
    // Procedure: Cause a wrap-around of the write index, but not the read
    // index.
    //
    // Test:
    // - Verify returnValue is true
    // - Verify buffer.getWriteIndex() returns 0
    // - Verify buffer.getReadIndex() returns 3
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    buffer.write(0x02);
    buffer.write(0x03);
    buffer.write(0x04);

    uint8_t value;
    buffer.read(value);
    buffer.read(value);

    value = 0x00;

    // Operation

    bool returnValue = buffer.read(value);

    // Test

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, true)                    &&
                   UNIT_TEST_CASE_EQUAL(value, (uint8_t) 0x03)                &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 0) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 3)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 1));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::read1Test3()
{
    //
    // Procedure: Cause a wrap-around of the circular buffer read and write
    // indexes.
    //
    // Test:
    // - Verify returnValue is true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    buffer.write(0x02);
    buffer.write(0x03);
    buffer.write(0x04);

    uint8_t value;
    buffer.read(value);
    buffer.read(value);
    buffer.read(value);
    buffer.read(value);

    buffer.write(0x05);

    value = 0x00;

    // Operation

    bool returnValue = buffer.read(value);

    // Test

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, true)                    &&
                   UNIT_TEST_CASE_EQUAL(value, (uint8_t) 0x05)                &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 1) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 1)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::read1Test4()
{
    //
    // Procedure: Try to read an item from an emptied buffer.
    //
    // Test:
    // - Verify returnValue is true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    uint8_t value;
    buffer.read(value);

    value = 0x00;

    // Operation

    bool returnValue = buffer.read(value);

    // Test

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, false)                   &&
                   UNIT_TEST_CASE_EQUAL(value, (uint8_t) 0x00)                &&
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 1) &&
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 1)  &&
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::read2Test1()
{
    //
    // Procedure: Read an Array of items.
    //
    // Test:
    // - Verify returnValue is true
    // - Verify buffer.getWriteIndex() returns 0
    // - Verify buffer.getReadIndex() returns 3
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    buffer.write(0x02);
    buffer.write(0x03);
    buffer.write(0x04);

    uint8_t readItems[3];
    memset(readItems, 0, sizeof(readItems));
    Array<uint8_t> readArray(readItems, ARRAY_SIZE(readItems), 0);

    // Operation

    bool returnValue = buffer.read(readArray);

    // Test

    const uint8_t testItems[4] =
    {
        0x01,
        0x02,
        0x03
    };

    int memoryCompareValue = memcmp(readItems,
                                    testItems,
                                    ARRAY_SIZE(readItems));

    return UNIT_TEST_REPORT(
                   UNIT_TEST_CASE_EQUAL(returnValue, true)                     &
                   UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 0)  &
                   UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 3)   &
                   UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 1)          &
                   UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::read2Test2()
{
    //
    // Procedure: Read an Array of items that wraps around the end of the
    // buffer.
    //
    // Test:
    // - Verify returnValue is true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    buffer.write(0x02);
    buffer.write(0x03);
    buffer.write(0x04);

    uint8_t value;
    buffer.read(value);
    buffer.read(value);

    buffer.write(0x05);

    uint8_t readItems[3];
    memset(readItems, 0, sizeof(readItems));
    Array<uint8_t> readArray(readItems, ARRAY_SIZE(readItems), 0);

    // Operation

    bool returnValue = buffer.read(readArray);

    // Test

    const uint8_t testItems[4] =
    {
        0x03,
        0x04,
        0x05
    };

    int memoryCompareValue = memcmp(readItems,
                                    testItems,
                                    ARRAY_SIZE(readItems));

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(returnValue, true)                    &
                    UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 1) &
                    UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 1)  &
                    UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 0)         &
                    UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}

//------------------------------------------------------------------------------
bool BufferUnitTest::read2Test3()
{
    //
    // Procedure: Try to read an Array with more items than are left in the
    // buffer
    //
    // Test:
    // - Verify returnValue is true
    // - Verify buffer.getWriteIndex() returns 1
    // - Verify buffer.getReadIndex() returns 0
    // - Verify buffer.count() returns 1
    // - Verify memoryCompareValue is 0
    //

    // Setup

    uint8_t items[4];
    memset(items, 0, sizeof(items));
    Buffer<uint8_t> buffer(items, ARRAY_SIZE(items));

    buffer.write(0x01);
    buffer.write(0x02);
    buffer.write(0x03);
    buffer.write(0x04);

    uint8_t value;
    buffer.read(value);
    buffer.read(value);
    buffer.read(value);

    uint8_t readItems[3];
    memset(readItems, 0, sizeof(readItems));
    Array<uint8_t> readArray(readItems, ARRAY_SIZE(readItems));

    // Operation

    bool returnValue = buffer.read(readArray);

    // Test

    const uint8_t testItems[4] =
    {
        0x00,
        0x00,
        0x00
    };

    int memoryCompareValue = memcmp(readItems,
                                    testItems,
                                    ARRAY_SIZE(readItems));

    return UNIT_TEST_REPORT(
                    UNIT_TEST_CASE_EQUAL(returnValue, false)                   &
                    UNIT_TEST_CASE_EQUAL(buffer.getWriteIndex(), (uint32_t) 0) &
                    UNIT_TEST_CASE_EQUAL(buffer.getReadIndex(), (uint32_t) 4)  &
                    UNIT_TEST_CASE_EQUAL(buffer.count(), (uint32_t) 1)         &
                    UNIT_TEST_CASE_EQUAL(memoryCompareValue, 0));
}
