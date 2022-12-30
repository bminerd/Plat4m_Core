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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file ListUnitTest.cpp
/// @author Ben Minerd
/// @date 12/28/2022
/// @brief ListUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/UnitTest/ListUnitTest.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction ListUnitTest::myTestCallbackFunctions[] =
{
    &ListUnitTest::constructorTest,

    &ListUnitTest::appendTest,
    &ListUnitTest::appendTest2,
    &ListUnitTest::appendTest3,

    &ListUnitTest::prependTest,
    &ListUnitTest::prependTest2,
    &ListUnitTest::prependTest3,

    &ListUnitTest::removeTest,
    &ListUnitTest::removeTest2,
    &ListUnitTest::removeTest3,
    &ListUnitTest::removeTest4,
    &ListUnitTest::removeTest5,
    &ListUnitTest::removeTest6,
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ListUnitTest::ListUnitTest() :
    UnitTest("ListUnitTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ListUnitTest::~ListUnitTest()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ListUnitTest::constructorTest()
{
    List<std::uint8_t> list;

    return UNIT_TEST_REPORT(
             UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(0))  &
             UNIT_TEST_CASE_EQUAL(list.isEmpty(), true)                        &
             UNIT_TEST_CASE_EQUAL(list.first(), static_cast<std::uint8_t*>(0)) &
             UNIT_TEST_CASE_EQUAL(list.last(), static_cast<std::uint8_t*>(0)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::appendTest()
{
    List<std::uint8_t> list;

    std::uint8_t value = 3;

    list.append(value);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(1))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(3)) &
           UNIT_TEST_CASE_EQUAL(list.last(), static_cast<std::uint8_t*>(0)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::appendTest2()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.append(value1);

    std::uint8_t value2 = 5;

    list.append(value2);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(2))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(3)) &
           UNIT_TEST_CASE_EQUAL(*(list.last()), static_cast<std::uint8_t>(5)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::appendTest3()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.append(value1);

    std::uint8_t value2 = 5;

    list.append(value2);

    std::uint8_t value3 = 1;

    list.append(value3);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(3))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(3)) &
           UNIT_TEST_CASE_EQUAL(*(list.last()), static_cast<std::uint8_t>(1)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::prependTest()
{
    List<std::uint8_t> list;

    std::uint8_t value = 3;

    list.prepend(value);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(1))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(3)) &
           UNIT_TEST_CASE_EQUAL(list.last(), static_cast<std::uint8_t*>(0)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::prependTest2()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.prepend(value1);

    std::uint8_t value2 = 5;

    list.prepend(value2);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(2))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(5)) &
           UNIT_TEST_CASE_EQUAL(*(list.last()), static_cast<std::uint8_t>(3)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::prependTest3()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.prepend(value1);

    std::uint8_t value2 = 5;

    list.prepend(value2);

    std::uint8_t value3 = 1;

    list.prepend(value3);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(3))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(1)) &
           UNIT_TEST_CASE_EQUAL(*(list.last()), static_cast<std::uint8_t>(3)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::removeTest()
{
    List<std::uint8_t> list;

    std::uint8_t value = 3;

    list.append(value);

    list.remove(value);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(0))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), true)                          &
           UNIT_TEST_CASE_EQUAL(list.first(), static_cast<std::uint8_t*>(0))   &
           UNIT_TEST_CASE_EQUAL(list.last(), static_cast<std::uint8_t*>(0)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::removeTest2()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.append(value1);

    std::uint8_t value2 = 5;

    list.append(value2);

    // This should move value2 to the first element position
    list.remove(value1);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(1))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(5)) &
           UNIT_TEST_CASE_EQUAL(list.last(), static_cast<std::uint8_t*>(0)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::removeTest3()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.append(value1);

    std::uint8_t value2 = 5;

    list.append(value2);

    // This should leave value1 in the first element position
    list.remove(value2);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(1))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(3)) &
           UNIT_TEST_CASE_EQUAL(list.last(), static_cast<std::uint8_t*>(0)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::removeTest4()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.append(value1);

    std::uint8_t value2 = 5;

    list.append(value2);

    std::uint8_t value3 = 1;

    list.append(value3);

    // This should move value2 to the first element position
    list.remove(value1);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(2))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(5)) &
           UNIT_TEST_CASE_EQUAL(*(list.last()), static_cast<std::uint8_t>(1)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::removeTest5()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.append(value1);

    std::uint8_t value2 = 5;

    list.append(value2);

    std::uint8_t value3 = 1;

    list.append(value3);

    // This should leave value1 in the first element position and value3 in the
    // last element position
    list.remove(value2);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(2))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(3)) &
           UNIT_TEST_CASE_EQUAL(*(list.last()), static_cast<std::uint8_t>(1)));
}

//------------------------------------------------------------------------------
bool ListUnitTest::removeTest6()
{
    List<std::uint8_t> list;

    std::uint8_t value1 = 3;

    list.append(value1);

    std::uint8_t value2 = 5;

    list.append(value2);

    std::uint8_t value3 = 1;

    list.append(value3);

    // This should move value2 to the last element position
    list.remove(value3);

    return UNIT_TEST_REPORT(
           UNIT_TEST_CASE_EQUAL(list.size(), static_cast<std::uint32_t>(2))    &
           UNIT_TEST_CASE_EQUAL(list.isEmpty(), false)                         &
           UNIT_TEST_CASE_EQUAL(*(list.first()), static_cast<std::uint8_t>(3)) &
           UNIT_TEST_CASE_EQUAL(*(list.last()), static_cast<std::uint8_t>(5)));
}
