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
/// @file LengthUnitTest.cpp
/// @author Ben Minerd
/// @date 11/8/2023
/// @brief LengthUnitTest class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>
#include <limits>

#include <Plat4m_Core/UnitTest/LengthUnitTest.h>
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Length.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const UnitTest::TestCallbackFunction LengthUnitTest::myTestCallbackFunctions[] =
{
    &LengthUnitTest::fromTimeMsTest,

    &LengthUnitTest::fromTimeMsSignedTest,

    &LengthUnitTest::fromTimeUsTest,

    &LengthUnitTest::fromTimeUsSignedTest,

    &LengthUnitTest::fromTimeNsTest,

    &LengthUnitTest::fromTimeNsSignedTest,

    &LengthUnitTest::fromTimeSFloatTest1,
    &LengthUnitTest::fromTimeSFloatTest2,

    &LengthUnitTest::fromTimeSDoubleTest1,
    &LengthUnitTest::fromTimeSDoubleTest2,

    &LengthUnitTest::toTimeMsTest2,
    &LengthUnitTest::toTimeMsTest3,
    &LengthUnitTest::toTimeMsTest4,

    &LengthUnitTest::toTimeMsLongTest1,
    &LengthUnitTest::toTimeMsLongTest2,

    &LengthUnitTest::toTimeMsSignedTest1,
    &LengthUnitTest::toTimeMsSignedTest2,
    &LengthUnitTest::toTimeMsSignedTest3,
    &LengthUnitTest::toTimeMsSignedTest4,

    &LengthUnitTest::toTimeMsSignedLongTest1,
    &LengthUnitTest::toTimeMsSignedLongTest2,

    &LengthUnitTest::toTimeUsTest1,
    &LengthUnitTest::toTimeUsTest2,
    &LengthUnitTest::toTimeUsTest3,
    &LengthUnitTest::toTimeUsTest4,

    &LengthUnitTest::toTimeUsLongTest1,
    &LengthUnitTest::toTimeUsLongTest2,

    &LengthUnitTest::toTimeUsSignedTest1,
    &LengthUnitTest::toTimeUsSignedTest2,
    &LengthUnitTest::toTimeUsSignedTest3,
    &LengthUnitTest::toTimeUsSignedTest4,

    &LengthUnitTest::toTimeUsSignedLongTest1,
    &LengthUnitTest::toTimeUsSignedLongTest2,

    &LengthUnitTest::toTimeNsTest1,
    &LengthUnitTest::toTimeNsTest2,
    &LengthUnitTest::toTimeNsTest3,
    &LengthUnitTest::toTimeNsTest4,

    &LengthUnitTest::toTimeNsLongTest1,
    &LengthUnitTest::toTimeNsLongTest2,

    &LengthUnitTest::toTimeNsSignedTest1,
    &LengthUnitTest::toTimeNsSignedTest2,
    &LengthUnitTest::toTimeNsSignedTest3,
    &LengthUnitTest::toTimeNsSignedTest4,
    &LengthUnitTest::toTimeNsSignedTest5,

    &LengthUnitTest::toTimeNsSignedLongTest1,
    &LengthUnitTest::toTimeNsSignedLongTest2
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
LengthUnitTest::LengthUnitTest() :
    UnitTest("LengthUnitTest",
             myTestCallbackFunctions,
             arraySize(myTestCallbackFunctions))
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
LengthUnitTest::~LengthUnitTest()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeMsTest()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeMs timeMs = 1250;

    time.fromTimeMs(timeMs);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeMsSignedTest()
{
    Time<std::int32_t, Units::Time::S> time;

    const TimeMsSigned timeMsSigned = -1250;

    time.fromTimeMsSigned(timeMsSigned);

    const Time<std::int32_t, Units::Time::S> expected(-1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeUsTest()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeUs timeUs = 1250000;

    time.fromTimeUs(timeUs);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeUsSignedTest()
{
    Time<std::int32_t, Units::Time::S> time;

    const TimeUsSigned timeUsSigned = -1250000;

    time.fromTimeUsSigned(timeUsSigned);

    const Time<std::int32_t, Units::Time::S> expected(-1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeNsTest()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeNs timeNs = 1250000000;

    time.fromTimeNs(timeNs);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeNsSignedTest()
{
    Time<std::int32_t, Units::Time::S> time;

    const TimeNsSigned timeNsSigned = -1250000000;

    time.fromTimeNsSigned(timeNsSigned);

    const Time<std::int32_t, Units::Time::S> expected(-1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeSFloatTest1()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeSFloat timeS = 1.25;

    time.fromTimeSFloat(timeS);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeSFloatTest2()
{
    Time<std::uint32_t, Units::Time::MS> time;

    const TimeSFloat timeS = 1.25005;

    time.fromTimeSFloat(timeS);

    const Time<std::uint32_t, Units::Time::MS> expected(1250);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeSDoubleTest1()
{
    Time<std::uint32_t, Units::Time::S> time;

    const TimeSDouble timeS = 1.25;

    time.from<TimeSDouble, Units::Time::S>(timeS);

    const Time<std::uint32_t, Units::Time::S> expected(1);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::fromTimeSDoubleTest2()
{
    Time<std::uint32_t, Units::Time::MS> time;

    const TimeSDouble timeS = 1.25005;

    time.fromTimeSDouble(timeS);

    const Time<std::uint32_t, Units::Time::MS> expected(1250);

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(time, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeMs timeMs = time.toTimeMs();

    const TimeMs expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeMs timeMs = time.toTimeMs();

    const TimeMs expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsTest3()
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
bool LengthUnitTest::toTimeMsTest4()
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
bool LengthUnitTest::toTimeMsLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(4500000);

    const TimeMsLong timeMsLong = time.toTimeMsLong();

    const TimeMsLong expected = 4500000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(4500000);

    const TimeMsLong timeMs = time.toTimeMsLong();

    const TimeMsLong expected = 4500000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsSignedTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeMsSigned timeMsSigned = time.toTimeMsSigned();

    const TimeMsSigned expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSigned, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsSignedTest2()
{
    const Time<std::int32_t, Units::Time::S> time(-1);

    const TimeMsSigned timeMsSigned = time.toTimeMsSigned();

    const TimeMsSigned expected = -1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSigned, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsSignedTest3()
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
bool LengthUnitTest::toTimeMsSignedTest4()
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
bool LengthUnitTest::toTimeMsSignedLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(1);

    const TimeMsSignedLong timeMsSignedLong = time.toTimeMsSignedLong();

    const TimeMsSignedLong expected = 1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeMsSignedLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(-1);

    const TimeMsSignedLong timeMsSignedLong = time.toTimeMsSignedLong();

    const TimeMsSignedLong expected = -1000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeMsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeUs timeUs = time.toTimeUs();

    const TimeUs expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeUs timeUs = time.toTimeUs();

    const TimeUs expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsTest3()
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
bool LengthUnitTest::toTimeUsTest4()
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
bool LengthUnitTest::toTimeUsLongTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeUsLong timeUsLong = time.toTimeUsLong();

    const TimeUsLong expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsLongTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeUsLong timeUsLong = time.toTimeUsLong();

    const TimeUsLong expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsSignedTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeUsSigned timeUsSigned = time.toTimeUsSigned();

    const TimeUsSigned expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSigned, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsSignedTest2()
{
    const Time<std::int32_t, Units::Time::S> time(-1);

    const TimeUsSigned timeUsSigned = time.toTimeUsSigned();

    const TimeUsSigned expected = -1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSigned, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsSignedTest3()
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
bool LengthUnitTest::toTimeUsSignedTest4()
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
bool LengthUnitTest::toTimeUsSignedLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(1);

    const TimeUsSignedLong timeUsSignedLong = time.toTimeUsSignedLong();

    const TimeUsSignedLong expected = 1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeUsSignedLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(-1);

    const TimeUsSignedLong timeUsSignedLong = time.toTimeUsSignedLong();

    const TimeUsSignedLong expected = -1000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeUsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeNs timeNs = time.toTimeNs();

    const TimeNs expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeNs timeNs = time.toTimeNs();

    const TimeNs expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsTest3()
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
bool LengthUnitTest::toTimeNsTest4()
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
bool LengthUnitTest::toTimeNsLongTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeNs timeNs = time.toTimeNsLong();

    const TimeNs expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNs, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsLongTest2()
{
    const Time<std::int32_t, Units::Time::S> time(1);

    const TimeNsLong timeNsLong = time.toTimeNsLong();

    const TimeNsLong expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsSignedTest1()
{
    const Time<std::uint32_t, Units::Time::S> time(1);

    const TimeNsSigned timeNsSigned = time.toTimeNsSigned();

    const TimeNsSigned expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSigned, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsSignedTest2()
{
    const Time<std::int32_t, Units::Time::S> time(-1);

    const TimeNsSigned timeNsSigned = time.toTimeNsSigned();

    const TimeNsSigned expected = -1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSigned, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsSignedTest3()
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
bool LengthUnitTest::toTimeNsSignedTest4()
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
bool LengthUnitTest::toTimeNsSignedTest5()
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
bool LengthUnitTest::toTimeNsSignedLongTest1()
{
    const Time<std::uint64_t, Units::Time::S> time(1);

    const TimeNsSignedLong timeNsSignedLong = time.toTimeNsSignedLong();

    const TimeNsSignedLong expected = 1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSignedLong, expected));
}

//------------------------------------------------------------------------------
bool LengthUnitTest::toTimeNsSignedLongTest2()
{
    const Time<std::int64_t, Units::Time::S> time(-1);

    const TimeNsSignedLong timeNsSignedLong = time.toTimeNsSignedLong();

    const TimeNsSignedLong expected = -1000000000;

    return UNIT_TEST_REPORT(UNIT_TEST_CASE_EQUAL(timeNsSignedLong, expected));
}
