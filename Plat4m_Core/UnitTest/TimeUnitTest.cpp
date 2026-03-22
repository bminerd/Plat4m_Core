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
// Copyright (c) 2022 - 2023 Benjamin Minerd
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
/// @file TimeUnitTest.cpp
/// @author Ben Minerd
/// @date 6/15/2022
/// @brief TimeUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/UnitTest/TimeUnitTest.h>
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Time.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction TimeUnitTest::myTestCallbackFunctions[] =
{
    &TimeUnitTest::operatorGreaterThanTest,

    &TimeUnitTest::operatorLessThanTest,

    &TimeUnitTest::operatorEqualsTest,

    &TimeUnitTest::operatorGreaterThanOrEqualToTest,

    &TimeUnitTest::operatorLessThanOrEqualToTest,

    &TimeUnitTest::operatorAddTest1,
    &TimeUnitTest::operatorAddTest2,
    &TimeUnitTest::operatorAddTest3,

    &TimeUnitTest::operatorAddEqualsTest1,
    &TimeUnitTest::operatorAddEqualsTest2,

    &TimeUnitTest::operatorSubtractTest1,
    &TimeUnitTest::operatorSubtractTest2,
    &TimeUnitTest::operatorSubtractTest3,

    &TimeUnitTest::operatorSubtractEqualsTest1,
    &TimeUnitTest::operatorSubtractEqualsTest2,

    &TimeUnitTest::operatorModulusTest,

    &TimeUnitTest::fromTimeMsTest,

    &TimeUnitTest::fromTimeMsSignedTest,

    &TimeUnitTest::fromTimeUsTest,

    &TimeUnitTest::fromTimeUsSignedTest,

    &TimeUnitTest::fromTimeNsTest,

    &TimeUnitTest::fromTimeNsSignedTest,

    &TimeUnitTest::fromTimeSFloatTest1,
    &TimeUnitTest::fromTimeSFloatTest2,

    &TimeUnitTest::fromTimeSDoubleTest1,
    &TimeUnitTest::fromTimeSDoubleTest2,

    &TimeUnitTest::toTimeMsTest2,
    &TimeUnitTest::toTimeMsTest3,
    &TimeUnitTest::toTimeMsTest4,

    &TimeUnitTest::toTimeMsLongTest1,
    &TimeUnitTest::toTimeMsLongTest2,

    &TimeUnitTest::toTimeMsSignedTest1,
    &TimeUnitTest::toTimeMsSignedTest2,
    &TimeUnitTest::toTimeMsSignedTest3,
    &TimeUnitTest::toTimeMsSignedTest4,

    &TimeUnitTest::toTimeMsSignedLongTest1,
    &TimeUnitTest::toTimeMsSignedLongTest2,

    &TimeUnitTest::toTimeUsTest1,
    &TimeUnitTest::toTimeUsTest2,
    &TimeUnitTest::toTimeUsTest3,
    &TimeUnitTest::toTimeUsTest4,

    &TimeUnitTest::toTimeUsLongTest1,
    &TimeUnitTest::toTimeUsLongTest2,

    &TimeUnitTest::toTimeUsSignedTest1,
    &TimeUnitTest::toTimeUsSignedTest2,
    &TimeUnitTest::toTimeUsSignedTest3,
    &TimeUnitTest::toTimeUsSignedTest4,

    &TimeUnitTest::toTimeUsSignedLongTest1,
    &TimeUnitTest::toTimeUsSignedLongTest2,

    &TimeUnitTest::toTimeNsTest1,
    &TimeUnitTest::toTimeNsTest2,
    &TimeUnitTest::toTimeNsTest3,
    &TimeUnitTest::toTimeNsTest4,

    &TimeUnitTest::toTimeNsLongTest1,
    &TimeUnitTest::toTimeNsLongTest2,

    &TimeUnitTest::toTimeNsSignedTest1,
    &TimeUnitTest::toTimeNsSignedTest2,
    &TimeUnitTest::toTimeNsSignedTest3,
    &TimeUnitTest::toTimeNsSignedTest4,
    &TimeUnitTest::toTimeNsSignedTest5,

    &TimeUnitTest::toTimeNsSignedLongTest1,
    &TimeUnitTest::toTimeNsSignedLongTest2,

    &TimeUnitTest::toTimeSFloatTest1,

    &TimeUnitTest::toTimeSDoubleTest1
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeUnitTest::TimeUnitTest() :
    UnitTest("TimeUnitTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeUnitTest::~TimeUnitTest()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorGreaterThanTest()
{
    const Time<std::uint32_t, Units::Time::S> time1(100);

    const Time<std::uint32_t, Units::Time::S> time2(20);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 > time2, true) &
                            UNIT_TEST_CASE_EQUAL(time2 > time1, false));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorLessThanTest()
{
    const Time<std::uint32_t, Units::Time::S> time1(20);

    const Time<std::uint32_t, Units::Time::S> time2(100);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 < time2, true) &
                            UNIT_TEST_CASE_EQUAL(time2 < time1, false));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorEqualsTest()
{
    const Time<std::uint32_t, Units::Time::S> time1(20);

    const Time<std::uint32_t, Units::Time::S> time2(20);

    const Time<std::uint32_t, Units::Time::S> time3(100);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 == time2, true) &
                            UNIT_TEST_CASE_EQUAL(time1 == time3, false));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorGreaterThanOrEqualToTest()
{
    const Time<std::uint32_t, Units::Time::S> time1(100);

    const Time<std::uint32_t, Units::Time::S> time2(20);

    const Time<std::uint32_t, Units::Time::S> time3(100);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 >= time2, true)  &
                            UNIT_TEST_CASE_EQUAL(time2 >= time1, false) &
                            UNIT_TEST_CASE_EQUAL(time1 >= time3, true));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorLessThanOrEqualToTest()
{
    const Time<std::uint32_t, Units::Time::S> time1(20);

    const Time<std::uint32_t, Units::Time::S> time2(100);

    const Time<std::uint32_t, Units::Time::S> time3(20);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1 <= time2, true)  &
                            UNIT_TEST_CASE_EQUAL(time2 <= time1, false) &
                            UNIT_TEST_CASE_EQUAL(time1 <= time3, true));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorAddTest1()
{
    const Time<std::uint32_t, Units::Time::S> time1(20);

    const Time<std::uint32_t, Units::Time::S> time2(100);

    const Time<std::uint32_t, Units::Time::S> result = time1 + time2;

    const Time<std::uint32_t, Units::Time::S> expected(120);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorAddTest2()
{
    const Time<std::int32_t, Units::Time::S> time1(-20);

    const Time<std::int32_t, Units::Time::S> time2(100);

    const Time<std::int32_t, Units::Time::S> result = time1 + time2;

    const Time<std::int32_t, Units::Time::S> expected(80);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorAddTest3()
{
    const Time<std::int32_t, Units::Time::S> time1(-20);

    const Time<std::int32_t, Units::Time::S> time2(-100);

    const Time<std::int32_t, Units::Time::S> result = time1 + time2;

    const Time<std::int32_t, Units::Time::S> expected(-120);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorAddEqualsTest1()
{
    Time<std::uint32_t, Units::Time::S> time1(20);

    const Time<std::uint32_t, Units::Time::S> time2(100);

    time1 += time2;

    const Time<std::uint32_t, Units::Time::S> expected(120);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorAddEqualsTest2()
{
    Time<std::int32_t, Units::Time::S> time1(-20);

    const Time<std::int32_t, Units::Time::S> time2(100);

    time1 += time2;

    const Time<std::int32_t, Units::Time::S> expected(80);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorSubtractTest1()
{
    const Time<std::uint32_t, Units::Time::S> time1(150);

    const Time<std::uint32_t, Units::Time::S> time2(100);

    const Time<std::uint32_t, Units::Time::S> result = time1 - time2;

    const Time<std::uint32_t, Units::Time::S> expected(50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorSubtractTest2()
{
    const Time<std::int32_t, Units::Time::S> time1(100);

    const Time<std::int32_t, Units::Time::S> time2(150);

    const Time<std::int32_t, Units::Time::S> result = time1 - time2;

    const Time<std::int32_t, Units::Time::S> expected(-50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorSubtractTest3()
{
    const Time<std::int32_t, Units::Time::S> time1(-150);

    const Time<std::int32_t, Units::Time::S> time2(-100);

    const Time<std::int32_t, Units::Time::S> result = time1 - time2;

    const Time<std::int32_t, Units::Time::S> expected(-50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorSubtractEqualsTest1()
{
    Time<std::uint32_t, Units::Time::S> time1(150);

    const Time<std::uint32_t, Units::Time::S> time2(100);

    time1 -= time2;

    const Time<std::uint32_t, Units::Time::S> expected(50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorSubtractEqualsTest2()
{
    Time<std::int32_t, Units::Time::S> time1(100);

    const Time<std::int32_t, Units::Time::S> time2(150);

    time1 -= time2;

    const Time<std::int32_t, Units::Time::S> expected(-50);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time1, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::operatorModulusTest()
{
    const Time<std::uint32_t, Units::Time::S> time1(150);

    const Time<std::uint32_t, Units::Time::S> time2(20);

    const Time<std::uint32_t, Units::Time::S> result = time1 % time2;

    const Time<std::uint32_t, Units::Time::S> expected(10);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(result, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeMsTest()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeMs timeMs = 1250;

    time.fromTimeMs(timeMs);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeMsSignedTest()
{
    Time<std::int32_t, Units::Time::S> time;

    const TimeMsSigned timeMsSigned = -1250;

    time.fromTimeMsSigned(timeMsSigned);

    const Time<std::int32_t, Units::Time::S> expected(-1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeUsTest()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeUs timeUs = 1250000;

    time.fromTimeUs(timeUs);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeUsSignedTest()
{
    Time<std::int32_t, Units::Time::S> time;

    const TimeUsSigned timeUsSigned = -1250000;

    time.fromTimeUsSigned(timeUsSigned);

    const Time<std::int32_t, Units::Time::S> expected(-1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeNsTest()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeNs timeNs = 1250000000;

    time.fromTimeNs(timeNs);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeNsSignedTest()
{
    Time<std::int32_t, Units::Time::S> time;

    const TimeNsSigned timeNsSigned = -1250000000;

    time.fromTimeNsSigned(timeNsSigned);

    const Time<std::int32_t, Units::Time::S> expected(-1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeSFloatTest1()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeSFloat timeS = 1.25;

    time.fromTimeSFloat(timeS);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeSFloatTest2()
{
    Time<std::uint32_t, Units::Time::MS> time;

    const TimeSFloat timeS = 1.25005;

    time.fromTimeSFloat(timeS);

    const Time<std::uint32_t, Units::Time::MS> expected(1250);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeSDoubleTest1()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeSDouble timeS = 1.25;

    time.from<TimeSDouble, Units::Time::S>(timeS);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::fromTimeSDoubleTest2()
{
    Time<std::uint32_t, Units::Time::MS> time;

    const TimeSDouble timeS = 1.25005;

    time.fromTimeSDouble(timeS);

    const Time<std::uint32_t, Units::Time::MS> expected(1250);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeMs timeMs = time.toTimeMs();

    const TimeMs expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeMs timeMs = time.toTimeMs();

    const TimeMs expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsTest3()
{
    const Time<std::uint32_t, Units::Time::S> time(4500000);

    std::uint32_t rollOverCount = 0;

    const TimeMs timeMs = time.toTimeMs(rollOverCount);

    const TimeMs expected = static_cast<TimeMs>(4500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeMs, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsTest4()
{
    const Time<std::uint32_t, Units::Time::S> time(4500000);

    std::uint32_t rollOverCount = 0;

    const TimeMs timeMs = time.toTimeMs(rollOverCount);

    const TimeMs expected = static_cast<TimeMs>(4500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeMs, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(4500000);

    const TimeMsLong timeMsLong = time.toTimeMsLong();

    const TimeMsLong expected = 4500000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(4500000);

    const TimeMsLong timeMs = time.toTimeMsLong();

    const TimeMsLong expected = 4500000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsSignedTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeMsSigned timeMsSigned = time.toTimeMsSigned();

    const TimeMsSigned expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSigned, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsSignedTest2()
{
    const Time<std::int32_t, Units::Time::S> time(-1);

    const TimeMsSigned timeMsSigned = time.toTimeMsSigned();

    const TimeMsSigned expected = -1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSigned, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsSignedTest3()
{
    const Time<std::uint32_t, Units::Time::S> time(2500000);

    std::uint32_t rollOverCount = 0;

    const TimeMsSigned timeMsSigned = time.toTimeMsSigned(rollOverCount);

    const TimeMsSigned expected = static_cast<TimeMsSigned>(2500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeMsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsSignedTest4()
{
    const Time<std::int32_t, Units::Time::S> time(-2500000);

    std::uint32_t rollOverCount = 0;

    const TimeMsSigned timeMsSigned = time.toTimeMsSigned(rollOverCount);

    const TimeMsSigned expected = static_cast<TimeMsSigned>(-2500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeMsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsSignedLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(1);

    const TimeMsSignedLong timeMsSignedLong = time.toTimeMsSignedLong();

    const TimeMsSignedLong expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeMsSignedLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(-1);

    const TimeMsSignedLong timeMsSignedLong = time.toTimeMsSignedLong();

    const TimeMsSignedLong expected = -1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeUs timeUs = time.toTimeUs();

    const TimeUs expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeUs timeUs = time.toTimeUs();

    const TimeUs expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsTest3()
{
    const Time<std::uint32_t, Units::Time::S> time(4500);

    std::uint32_t rollOverCount = 0;

    const TimeUs timeUs = time.toTimeUs(rollOverCount);

    const TimeUs expected = static_cast<TimeUs>(4500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeUs, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsTest4()
{
    const Time<std::int32_t, Units::Time::S> time(4500);

    std::uint32_t rollOverCount = 0;

    const TimeUs timeUs = time.toTimeUs(rollOverCount);

    const TimeUs expected = static_cast<TimeUs>(4500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeUs, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsLongTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeUsLong timeUsLong = time.toTimeUsLong();

    const TimeUsLong expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsLongTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeUsLong timeUsLong = time.toTimeUsLong();

    const TimeUsLong expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsSignedTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeUsSigned timeUsSigned = time.toTimeUsSigned();

    const TimeUsSigned expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSigned, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsSignedTest2()
{
    const Time<std::int32_t, Units::Time::S> time(-1);

    const TimeUsSigned timeUsSigned = time.toTimeUsSigned();

    const TimeUsSigned expected = -1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSigned, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsSignedTest3()
{
    const Time<std::int32_t, Units::Time::S> time(2500);

    std::uint32_t rollOverCount = 0;

    const TimeUsSigned timeUsSigned = time.toTimeUsSigned(rollOverCount);

    const TimeUsSigned expected = static_cast<TimeUsSigned>(2500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeUsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsSignedTest4()
{
    const Time<std::int32_t, Units::Time::S> time(-2500);

    std::uint32_t rollOverCount = 0;

    const TimeUsSigned timeUsSigned = time.toTimeUsSigned(rollOverCount);

    const TimeUsSigned expected = static_cast<TimeUsSigned>(-2500000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeUsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsSignedLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(1);

    const TimeUsSignedLong timeUsSignedLong = time.toTimeUsSignedLong();

    const TimeUsSignedLong expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeUsSignedLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(-1);

    const TimeUsSignedLong timeUsSignedLong = time.toTimeUsSignedLong();

    const TimeUsSignedLong expected = -1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeNs timeNs = time.toTimeNs();

    const TimeNs expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeNs timeNs = time.toTimeNs();

    const TimeNs expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsTest3()
{
    const Time<std::uint32_t, Units::Time::S> time(5);

    std::uint32_t rollOverCount = 0;

    const TimeNs timeNsSigned = time.toTimeNs(rollOverCount);

    const TimeNs expected = static_cast<TimeNs>(5000000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeNsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsTest4()
{
    const Time<std::int32_t, Units::Time::S> time(5);

    std::uint32_t rollOverCount = 0;

    const TimeNs timeNsSigned = time.toTimeNs(rollOverCount);

    const TimeNs expected = static_cast<TimeNs>(5000000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeNsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsLongTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeNs timeNs = time.toTimeNsLong();

    const TimeNs expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNs, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsLongTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeNsLong timeNsLong = time.toTimeNsLong();

    const TimeNsLong expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsSignedTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeNsSigned timeNsSigned = time.toTimeNsSigned();

    const TimeNsSigned expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSigned, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsSignedTest2()
{
    const Time<std::int32_t, Units::Time::S> time(-1);

    const TimeNsSigned timeNsSigned = time.toTimeNsSigned();

    const TimeNsSigned expected = -1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSigned, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsSignedTest3()
{
    const Time<std::int32_t, Units::Time::S> time(3);

    std::uint32_t rollOverCount = 0;

    const TimeNsSigned timeNsSigned = time.toTimeNsSigned(rollOverCount);

    const TimeNsSigned expected = static_cast<TimeNsSigned>(3000000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeNsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsSignedTest4()
{
    const Time<std::int32_t, Units::Time::S> time(-3);

    std::uint32_t rollOverCount = 0;

    const TimeNsSigned timeNsSigned = time.toTimeNsSigned(rollOverCount);

    const TimeNsSigned expected = static_cast<TimeNsSigned>(-3000000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeNsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(1)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsSignedTest5()
{
    const Time<std::int32_t, Units::Time::S> time(5);

    std::uint32_t rollOverCount = 0;

    const TimeNsSigned timeNsSigned = time.toTimeNsSigned(rollOverCount);

    const TimeNsSigned expected = static_cast<TimeNsSigned>(5000000000);

    return UNIT_TEST_REPORT(
            UNIT_TEST_CASE_EQUAL(timeNsSigned, expected) &
            UNIT_TEST_CASE_EQUAL(rollOverCount, static_cast<std::uint32_t>(2)));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsSignedLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(1);

    const TimeNsSignedLong timeNsSignedLong = time.toTimeNsSignedLong();

    const TimeNsSignedLong expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeNsSignedLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(-1);

    const TimeNsSignedLong timeNsSignedLong = time.toTimeNsSignedLong();

    const TimeNsSignedLong expected = -1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeSFloatTest1()
{
    const Time<std::int64_t, Units::Time::NS> time(1000);

    const TimeSFloat timeSFloat = time.toTimeSFloat();

    const TimeSFloat expected = 0.000001;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL_FLOAT(timeSFloat, expected));
}

//------------------------------------------------------------------------------
bool TimeUnitTest::toTimeSDoubleTest1()
{
    const Time<std::int64_t, Units::Time::NS> time(1000);

    const TimeSDouble timeSDouble = time.toTimeSDouble();

    const TimeSDouble expected = 0.000001;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL_FLOAT(timeSDouble, expected));
}
