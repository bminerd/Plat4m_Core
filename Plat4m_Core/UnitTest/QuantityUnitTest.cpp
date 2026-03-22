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
/// @file QuantityUnitTest.cpp
/// @author Ben Minerd
/// @date 6/15/2022
/// @brief QuantityUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>
#include <limits>

#include <Plat4m_Core/UnitTest/QuantityUnitTest.h>
#include <Plat4m_Core/Quantity.h>
#include <Plat4m_Core/UnitsTime.h>
#include <Plat4m_Core/UnitsLength.h>
#include <Plat4m_Core/UnitsFrequency.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction
                                   QuantityUnitTest::myTestCallbackFunctions[] =
{
    &QuantityUnitTest::operatorGreaterThanTest,

    &QuantityUnitTest::operatorLessThanTest,

    &QuantityUnitTest::operatorEqualsTest,

    &QuantityUnitTest::operatorGreaterThanOrEqualToTest,

    &QuantityUnitTest::operatorLessThanOrEqualToTest,

    &QuantityUnitTest::operatorAddTest1,
    &QuantityUnitTest::operatorAddTest2,
    &QuantityUnitTest::operatorAddTest3,

    &QuantityUnitTest::operatorAddEqualsTest1,
    &QuantityUnitTest::operatorAddEqualsTest2,

    &QuantityUnitTest::operatorSubtractTest1,
    &QuantityUnitTest::operatorSubtractTest2,
    &QuantityUnitTest::operatorSubtractTest3,

    &QuantityUnitTest::operatorSubtractEqualsTest1,
    &QuantityUnitTest::operatorSubtractEqualsTest2,

    &QuantityUnitTest::operatorModulusTest,

    &QuantityUnitTest::fromTest1,
    &QuantityUnitTest::fromTest2,
    &QuantityUnitTest::fromTest3,
    &QuantityUnitTest::fromTest4,
    &QuantityUnitTest::fromTest5,
    &QuantityUnitTest::fromTest6,
    &QuantityUnitTest::fromTest7,
    &QuantityUnitTest::fromTest8,
    &QuantityUnitTest::fromTest9,
    &QuantityUnitTest::fromTest10,
    &QuantityUnitTest::fromTest11,
    &QuantityUnitTest::fromTest12,
    &QuantityUnitTest::fromTest13,
    &QuantityUnitTest::fromTest14,
    &QuantityUnitTest::fromTest15,
    &QuantityUnitTest::fromTest16,
    &QuantityUnitTest::fromTest17,

    &QuantityUnitTest::toTest1,
    &QuantityUnitTest::toTest2,
    &QuantityUnitTest::toTest3,
    &QuantityUnitTest::toTest4,
    &QuantityUnitTest::toTest5,
    &QuantityUnitTest::toTest6,
    &QuantityUnitTest::toTest7,
    &QuantityUnitTest::toTest8,
    &QuantityUnitTest::toTest9,
    &QuantityUnitTest::toTest10,
    &QuantityUnitTest::toTest11,
    &QuantityUnitTest::toTest12,
    &QuantityUnitTest::toTest13,
    &QuantityUnitTest::toTest14,
    &QuantityUnitTest::toTest15,
    &QuantityUnitTest::toTest16,
    &QuantityUnitTest::toTest17
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QuantityUnitTest::QuantityUnitTest() :
    UnitTest("QuantityUnitTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QuantityUnitTest::~QuantityUnitTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorGreaterThanTest()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(100);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(20);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 > time2, true) &
                            UNIT_TEST_CASE_EQUAL(time2 > time1, false));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorLessThanTest()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(100);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 < time2, true) &
                            UNIT_TEST_CASE_EQUAL(time2 < time1, false));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorEqualsTest()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time3(100);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 == time2, true) &
                            UNIT_TEST_CASE_EQUAL(time1 == time3, false));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorGreaterThanOrEqualToTest()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(100);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time3(100);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 >= time2, true)  &
                            UNIT_TEST_CASE_EQUAL(time2 >= time1, false) &
                            UNIT_TEST_CASE_EQUAL(time1 >= time3, true));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorLessThanOrEqualToTest()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(100);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time3(20);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 <= time2, true)  &
                            UNIT_TEST_CASE_EQUAL(time2 <= time1, false) &
                            UNIT_TEST_CASE_EQUAL(time1 <= time3, true));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorAddTest1()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(100);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> result =
                                                                  time1 + time2;

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(120);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorAddTest2()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-20);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> time2(100);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> result =
                                                                  time1 + time2;

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(80);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorAddTest3()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-20);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> time2(-100);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> result =
                                                                  time1 + time2;

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(-120);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorAddEqualsTest1()
{
    Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(100);

    time1 += time2;

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(120);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorAddEqualsTest2()
{
    Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-20);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> time2(100);

    time1 += time2;

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(80);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorSubtractTest1()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(150);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(100);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> result =
                                                                  time1 - time2;

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorSubtractTest2()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(100);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> time2(150);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> result =
                                                                  time1 - time2;

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(-50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorSubtractTest3()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-150);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> time2(-100);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> result =
                                                                  time1 - time2;

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(-50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorSubtractEqualsTest1()
{
    Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(150);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(100);

    time1 -= time2;

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorSubtractEqualsTest2()
{
    Quantity<std::int32_t, Units::Time, Units::Time::S> time1(100);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> time2(150);

    time1 -= time2;

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(-50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::operatorModulusTest()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(150);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2(20);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> result =
                                                                  time1 % time2;

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest1()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(1);

    Quantity<std::uint32_t, Units::Time, Units::Time::MS> time2;

    time2.from(time1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::MS> expected(1000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest2()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(1);

    Quantity<std::uint32_t, Units::Time, Units::Time::US> time2;

    time2.from(time1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::US> expected(
                                                                       1000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest3()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(1);

    Quantity<std::uint32_t, Units::Time, Units::Time::NS> time2;

    time2.from(time1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::NS> expected(
                                                                    1000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest4()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(10);

    Quantity<std::uint64_t, Units::Time, Units::Time::NS> time2;

    time2.from(time1);

    const Quantity<std::uint64_t, Units::Time, Units::Time::NS> expected(
                                                                   10000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest5()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::MS> time1(10000);

    Quantity<std::uint32_t, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest6()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::US> time1(10000000);

    Quantity<std::uint32_t, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest7()
{
    const Quantity<std::uint64_t, Units::Time, Units::Time::NS> time1(
                                                                   10000000000);

    Quantity<std::uint32_t, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest8()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-1);

    Quantity<std::int32_t, Units::Time, Units::Time::MS> time2;

    time2.from(time1);

    const Quantity<std::int32_t, Units::Time, Units::Time::MS> expected(-1000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest9()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-1);

    Quantity<std::int32_t, Units::Time, Units::Time::US> time2;

    time2.from(time1);

    const Quantity<std::int32_t, Units::Time, Units::Time::US> expected(
                                                                      -1000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest10()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-1);

    Quantity<std::int32_t, Units::Time, Units::Time::NS> time2;

    time2.from(time1);

    const Quantity<std::int32_t, Units::Time, Units::Time::NS> expected(
                                                                   -1000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest11()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-10);

    Quantity<std::int64_t, Units::Time, Units::Time::NS> time2;

    time2.from(time1);

    const Quantity<std::int64_t, Units::Time, Units::Time::NS> expected(
                                                                  -10000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest12()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::MS> time1(-10000);

    Quantity<std::int32_t, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(-10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest13()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::US> time1(-10000000);

    Quantity<std::int32_t, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(-10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest14()
{
    const Quantity<std::int64_t, Units::Time, Units::Time::NS> time1(
                                                                  -10000000000);

    Quantity<std::int8_t, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<std::int8_t, Units::Time, Units::Time::S> expected(-10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest15()
{
    const Quantity<std::uint64_t, Units::Time, Units::Time::S> time1(1);

    Quantity<std::int8_t, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<std::int8_t, Units::Time, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest16()
{
    const Quantity<float, Units::Time, Units::Time::S> time1(1.25);

    Quantity<std::uint32_t, Units::Time, Units::Time::MS> time2;

    time2.from(time1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::MS> expected(1250);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::fromTest17()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::MS> time1(1250);

    Quantity<float, Units::Time, Units::Time::S> time2;

    time2.from(time1);

    const Quantity<float, Units::Time, Units::Time::S> expected(1.25);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest1()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::MS> time2 =
                                     time1.to<std::uint32_t, Units::Time::MS>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::MS> expected(1000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest2()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::US> time2 =
                                     time1.to<std::uint32_t, Units::Time::US>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::US> expected(
                                                                       1000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest3()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(1);

    const Quantity<std::uint32_t, Units::Time, Units::Time::NS> time2 =
                                     time1.to<std::uint32_t, Units::Time::NS>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::NS> expected(
                                                                    1000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest4()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time1(10);

    const Quantity<std::uint64_t, Units::Time, Units::Time::NS> time2 =
                                     time1.to<std::uint64_t, Units::Time::NS>();

    const Quantity<std::uint64_t, Units::Time, Units::Time::NS> expected(
                                                                   10000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest5()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::MS> time1(1000);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2 =
                                      time1.to<std::uint32_t, Units::Time::S>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest6()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::US> time1(1000000);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2 =
                                      time1.to<std::uint32_t, Units::Time::S>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest7()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::NS> time1(
                                                                    1000000000);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2 =
                                      time1.to<std::uint32_t, Units::Time::S>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest8()
{
    const Quantity<std::uint64_t, Units::Time, Units::Time::NS> time1(
                                                                   10000000000);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2 =
                                      time1.to<std::uint32_t, Units::Time::S>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest9()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-1);

    const Quantity<std::int32_t, Units::Time, Units::Time::MS> time2 =
                                      time1.to<std::int32_t, Units::Time::MS>();

    const Quantity<std::int32_t, Units::Time, Units::Time::MS> expected(-1000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest10()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-1);

    const Quantity<std::int32_t, Units::Time, Units::Time::US> time2 =
                                      time1.to<std::int32_t, Units::Time::US>();

    const Quantity<std::int32_t, Units::Time, Units::Time::US> expected(
                                                                      -1000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest11()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::S> time1(-1);

    const Quantity<std::int32_t, Units::Time, Units::Time::NS> time2 =
                                      time1.to<std::int32_t, Units::Time::NS>();

    const Quantity<std::int32_t, Units::Time, Units::Time::NS> expected(
                                                                   -1000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest12()
{
    const Quantity<std::int8_t, Units::Time, Units::Time::S> time1(-10);

    const Quantity<std::int64_t, Units::Time, Units::Time::NS> time2 =
                                      time1.to<std::int64_t, Units::Time::NS>();

    const Quantity<std::int64_t, Units::Time, Units::Time::NS> expected(
                                                                  -10000000000);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest13()
{
    const Quantity<std::int32_t, Units::Time, Units::Time::MS> time1(1000);

    const Quantity<std::int32_t, Units::Time, Units::Time::S> time2 =
                                       time1.to<std::int32_t, Units::Time::S>();

    const Quantity<std::int32_t, Units::Time, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest14()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::US> time1(1000000);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2 =
                                      time1.to<std::uint32_t, Units::Time::S>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest15()
{
    const Quantity<std::uint32_t, Units::Time, Units::Time::NS> time1(
                                                                    1000000000);

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> time2 =
                                      time1.to<std::uint32_t, Units::Time::S>();

    const Quantity<std::uint32_t, Units::Time, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest16()
{
    const Quantity<std::uint64_t, Units::Time, Units::Time::NS> time1(
                                                                   10000000000);

    const Quantity<std::uint8_t, Units::Time, Units::Time::S> time2 =
                                      time1.to<std::uint8_t, Units::Time::S>();

    const Quantity<std::uint8_t, Units::Time, Units::Time::S> expected(10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));
}

//------------------------------------------------------------------------------
bool QuantityUnitTest::toTest17()
{
    // const Quantity<std::uint32_t, Units::Time, Units::Time::MS> time1(1250);

    // const Quantity<float, Units::Time, Units::Time::S> time2 =
    //                                           time1.to<float, Units::Time::S>();

    // const Quantity<float, Units::Time, Units::Time::S> expected(1.25);

    // return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time2, expected));

    // const Quantity<std::uint32_t, Units::Length, Units::Length::MM> length1(1250);

    // const Quantity<float, Units::Length, Units::Length::M> length2 =
    //                                      length1.to<float, Units::Length::M>();

    // const Quantity<float, Units::Length, Units::Length::M> expected(1.25);

    // return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(length2, expected));

    const Quantity<std::uint32_t, Units::Frequency, Units::Frequency::HZ>
                                                               frequency1(1250);

    const Quantity<float, Units::Frequency, Units::Frequency::KHZ> frequency2 =
                                  frequency1.to<float, Units::Frequency::KHZ>();

    const Quantity<float, Units::Frequency, Units::Frequency::KHZ>
                                                                 expected(1.25);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(frequency2, expected));
}
