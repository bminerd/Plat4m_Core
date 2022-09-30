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
/// @file TimeStampUnitTest.cpp
/// @author Ben Minerd
/// @date 6/15/2022
/// @brief TimeStampUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <limits>

#include <Plat4m_Core/UnitTest/TimeStampUnitTest.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction TimeStampUnitTest::myTestCallbackFunctions[] =
{
    &TimeStampUnitTest::operatorGreaterThanTest,
    &TimeStampUnitTest::operatorLessThanTest,
    &TimeStampUnitTest::operatorEqualsTest,
    &TimeStampUnitTest::operatorGreaterThanOrEqualToTest,
    &TimeStampUnitTest::operatorLessThanOrEqualToTest,
    &TimeStampUnitTest::operatorAddTest1,
    &TimeStampUnitTest::operatorAddTest2,
    &TimeStampUnitTest::operatorAddTest3,
    &TimeStampUnitTest::operatorAddEqualsTest1,
    &TimeStampUnitTest::operatorAddEqualsTest2,
    &TimeStampUnitTest::operatorSubtractTest1,
    &TimeStampUnitTest::operatorSubtractTest2,
    &TimeStampUnitTest::operatorSubtractTest3,
    &TimeStampUnitTest::operatorSubtractEqualsTest1,
    &TimeStampUnitTest::operatorSubtractEqualsTest2,
    &TimeStampUnitTest::fromTimeMsTest,
    &TimeStampUnitTest::fromTimeUsTest,
    &TimeStampUnitTest::fromTimeNsTest,
    &TimeStampUnitTest::fromTimeMsSignedTest,
    &TimeStampUnitTest::fromTimeUsSignedTest,
    &TimeStampUnitTest::fromTimeNsSignedTest,
    &TimeStampUnitTest::toTimeMsSignedTest1,
    &TimeStampUnitTest::toTimeMsSignedTest2,
    &TimeStampUnitTest::toTimeMsSignedTest3,
    &TimeStampUnitTest::toTimeMsSignedTest4,
    &TimeStampUnitTest::toTimeUsSignedTest1,
    &TimeStampUnitTest::toTimeUsSignedTest2,
    &TimeStampUnitTest::toTimeUsSignedTest3,
    &TimeStampUnitTest::toTimeUsSignedTest4,
    &TimeStampUnitTest::toTimeNsSignedTest1,
    &TimeStampUnitTest::toTimeNsSignedTest2,
    &TimeStampUnitTest::toTimeNsSignedTest3,
    &TimeStampUnitTest::toTimeNsSignedTest4
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeStampUnitTest::TimeStampUnitTest() :
    UnitTest("TimeStampUnitTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeStampUnitTest::~TimeStampUnitTest()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorGreaterThanTest()
{
    TimeStamp timeStamp1(1, 250000000);

    TimeStamp timeStamp2(0, 750000000);

    const bool result1 = timeStamp1 > timeStamp2;
    const bool result2 = timeStamp2 > timeStamp1;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result1, true) &
                            UNIT_TEST_CASE_EQUAL(result2, false));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorLessThanTest()
{
    TimeStamp timeStamp1(0, 750000000);

    TimeStamp timeStamp2(1, 250000000);

    const bool result1 = timeStamp1 < timeStamp2;
    const bool result2 = timeStamp2 < timeStamp1;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result1, true) &
                            UNIT_TEST_CASE_EQUAL(result2, false));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorEqualsTest()
{
    TimeStamp timeStamp1(0, 750000000);

    TimeStamp timeStamp2(0, 750000000);

    TimeStamp timeStamp3(1, 250000000);

    const bool result1 = timeStamp1 == timeStamp2;
    const bool result2 = timeStamp1 == timeStamp3;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result1, true) &
                            UNIT_TEST_CASE_EQUAL(result2, false));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorGreaterThanOrEqualToTest()
{
    TimeStamp timeStamp1(1, 250000000);

    TimeStamp timeStamp2(0, 750000000);

    TimeStamp timeStamp3(1, 250000000);

    const bool result1 = timeStamp1 >= timeStamp2;
    const bool result2 = timeStamp2 >= timeStamp1;
    const bool result3 = timeStamp1 >= timeStamp3;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result1, true)  &
                            UNIT_TEST_CASE_EQUAL(result2, false) &
                            UNIT_TEST_CASE_EQUAL(result3, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorLessThanOrEqualToTest()
{
    TimeStamp timeStamp1(0, 750000000);

    TimeStamp timeStamp2(1, 250000000);

    TimeStamp timeStamp3(0, 750000000);

    const bool result1 = timeStamp1 <= timeStamp2;
    const bool result2 = timeStamp2 <= timeStamp1;
    const bool result3 = timeStamp1 <= timeStamp3;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result1, true)  &
                            UNIT_TEST_CASE_EQUAL(result2, false) &
                            UNIT_TEST_CASE_EQUAL(result3, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorAddTest1()
{
    TimeStamp timeStamp1(0, 750000000);

    TimeStamp timeStamp2(1, 250000000);

    const TimeStamp result = timeStamp1 + timeStamp2;

    const TimeStamp expected(2, 0);

    const bool compare = result == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorAddTest2()
{
    TimeStamp timeStamp1(0, -750000000);

    TimeStamp timeStamp2(1, 250000000);

    const TimeStamp result = timeStamp1 + timeStamp2;

    const TimeStamp expected(0, 500000000);

    const bool compare = result == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorAddTest3()
{
    TimeStamp timeStamp1(0, -750000000);

    TimeStamp timeStamp2(-1, -250000000);

    const TimeStamp result = timeStamp1 + timeStamp2;

    const TimeStamp expected(-2, 0);

    const bool compare = result == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}


//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorAddEqualsTest1()
{
    TimeStamp timeStamp1(0, 750000000);

    TimeStamp timeStamp2(1, 250000000);

    timeStamp1 += timeStamp2;

    const TimeStamp expected(2, 0);

    const bool compare = timeStamp1 == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorAddEqualsTest2()
{
    TimeStamp timeStamp1(0, -750000000);

    TimeStamp timeStamp2(1, 250000000);

    timeStamp1 += timeStamp2;

    const TimeStamp expected(0, 500000000);

    const bool compare = timeStamp1 == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorSubtractTest1()
{
    TimeStamp timeStamp1(2, 750000000);

    TimeStamp timeStamp2(1, 250000000);

    const TimeStamp result = timeStamp1 - timeStamp2;

    const TimeStamp expected(1, 500000000);

    const bool compare = result == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorSubtractTest2()
{
    TimeStamp timeStamp1(1, 250000000);

    TimeStamp timeStamp2(2, 0);

    const TimeStamp result = timeStamp1 - timeStamp2;

    const TimeStamp expected(0, -750000000);

    const bool compare = result == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorSubtractTest3()
{
    TimeStamp timeStamp1(-2, -750000000);

    TimeStamp timeStamp2(-1, -250000000);

    const TimeStamp result = timeStamp1 - timeStamp2;

    const TimeStamp expected(-1, -500000000);

    const bool compare = result == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorSubtractEqualsTest1()
{
    TimeStamp timeStamp1(2, 750000000);

    TimeStamp timeStamp2(1, 250000000);

    timeStamp1 -= timeStamp2;

    const TimeStamp expected(1, 500000000);

    const bool compare = timeStamp1 == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::operatorSubtractEqualsTest2()
{
    TimeStamp timeStamp1(1, 250000000);

    TimeStamp timeStamp2(2, 0);

    timeStamp1 -= timeStamp2;

    const TimeStamp expected(0, -750000000);

    const bool compare = timeStamp1 == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::fromTimeMsTest()
{
    TimeStamp timeStamp;

    TimeMs timeMs = 1250;

    timeStamp.fromTimeMs(timeMs);

    const TimeStamp expected(1, 250000000);

    const bool compare = timeStamp == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::fromTimeUsTest()
{
    TimeStamp timeStamp;

    TimeUs timeUs = 1250000;

    timeStamp.fromTimeUs(timeUs);

    const TimeStamp expected(1, 250000000);

    const bool compare = timeStamp == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::fromTimeNsTest()
{
    TimeStamp timeStamp;

    TimeNs timeNs = 1250000000;

    timeStamp.fromTimeNs(timeNs);

    const TimeStamp expected(1, 250000000);

    const bool compare = timeStamp == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::fromTimeMsSignedTest()
{
    TimeStamp timeStamp;

    TimeMsSigned timeMsSigned = -1250;

    timeStamp.fromTimeMsSigned(timeMsSigned);

    const TimeStamp expected(-1, -250000000);

    const bool compare = timeStamp == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::fromTimeUsSignedTest()
{
    TimeStamp timeStamp;

    TimeUsSigned timeUsSigned = -1250000;

    timeStamp.fromTimeUsSigned(timeUsSigned);

    const TimeStamp expected(-1, -250000000);

    const bool compare = timeStamp == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::fromTimeNsSignedTest()
{
    TimeStamp timeStamp;

    TimeNsSigned timeNsSigned = -1250000000;

    timeStamp.fromTimeNsSigned(timeNsSigned);

    const TimeStamp expected(-1, -250000000);

    const bool compare = timeStamp == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeMsSignedTest1()
{
    TimeStamp timeStamp(1, 250000000);

    TimeMillisecondsSigned timeMs = timeStamp.toTimeMs();

    const TimeMillisecondsSigned expected = 1250;

    const bool compare = timeMs == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeMsSignedTest2()
{
    TimeStamp timeStamp(-1, -250000000);

    TimeMillisecondsSigned timeMs = timeStamp.toTimeMs();

    const TimeMillisecondsSigned expected = -1250;

    const bool compare = timeMs == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeMsSignedTest3()
{
    TimeStamp timeStamp(2500000, 150000000);

    std::uint32_t rollOverCount = 0;

    TimeMillisecondsSigned timeMs = timeStamp.toTimeMs(rollOverCount);

    const TimeMillisecondsSigned expected =
        2500000150 - std::numeric_limits<std::int32_t>::max() - 1;

    const bool compare = timeMs == expected;

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(compare, true) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeMsSignedTest4()
{
    TimeStamp timeStamp(-2500000, -150000000);

    std::uint32_t rollOverCount = 0;

    TimeMillisecondsSigned timeMs = timeStamp.toTimeMs(rollOverCount);

    const TimeMillisecondsSigned expected =
        -(2500000150 - std::numeric_limits<std::int32_t>::max() - 1);

    const bool compare = timeMs == expected;

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(compare, true) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeUsSignedTest1()
{
    TimeStamp timeStamp(1, 250000000);

    TimeMicrosecondsSigned timeUs = timeStamp.toTimeUs();

    const TimeMicrosecondsSigned expected = 1250000;

    const bool compare = timeUs == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeUsSignedTest2()
{
    TimeStamp timeStamp(-1, -250000000);

    TimeMicrosecondsSigned timeUs = timeStamp.toTimeUs();

    const TimeMicrosecondsSigned expected = -1250000;

    const bool compare = timeUs == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeUsSignedTest3()
{
    TimeStamp timeStamp(2500, 150000);

    std::uint32_t rollOverCount = 0;

    TimeMicrosecondsSigned timeUs = timeStamp.toTimeUs(rollOverCount);

    const TimeMicrosecondsSigned expected =
                      2500000150 - std::numeric_limits<std::int32_t>::max() - 1;

    const bool compare = timeUs == expected;

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(compare, true) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeUsSignedTest4()
{
    TimeStamp timeStamp(-2500, -150000);

    std::uint32_t rollOverCount = 0;

    TimeMicrosecondsSigned timeUs = timeStamp.toTimeUs(rollOverCount);

    const TimeMicrosecondsSigned expected =
                   -(2500000150 - std::numeric_limits<std::int32_t>::max() - 1);

    const bool compare = timeUs == expected;

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(compare, true) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeNsSignedTest1()
{
    TimeStamp timeStamp(1, 250000000);

    TimeNanosecondsSigned timeNs = timeStamp.toTimeNs();

    const TimeNanosecondsSigned expected = 1250000000;

    const bool compare = timeNs == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeNsSignedTest2()
{
    TimeStamp timeStamp(-1, -250000000);

    TimeNanosecondsSigned timeNs = timeStamp.toTimeNs();

    const TimeNanosecondsSigned expected = -1250000000;

    const bool compare = timeNs == expected;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(compare, true));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeNsSignedTest3()
{
    TimeStamp timeStamp(2, 500000150);

    std::uint32_t rollOverCount = 0;

    TimeNanosecondsSigned timeNs = timeStamp.toTimeNs(rollOverCount);

    const TimeNanosecondsSigned expected =
                      2500000150 - std::numeric_limits<std::int32_t>::max() - 1;

    const bool compare = timeNs == expected;

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(compare, true) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeStampUnitTest::toTimeNsSignedTest4()
{
    TimeStamp timeStamp(-2, -500000150);

    std::uint32_t rollOverCount = 0;

    TimeNanosecondsSigned timeNs = timeStamp.toTimeNs(rollOverCount);

    const TimeNanosecondsSigned expected =
                   -(2500000150 - std::numeric_limits<std::int32_t>::max() - 1);

    const bool compare = timeNs == expected;

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(compare, true) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}
