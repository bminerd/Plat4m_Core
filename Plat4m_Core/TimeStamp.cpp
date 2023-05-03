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
// Copyright (c) 2021 - 2023 Benjamin Minerd
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
/// @file TimeStamp.cpp
/// @author Ben Minerd
/// @date 9/23/2021
/// @brief TimeStamp class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdlib>
#include <limits>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/TimeStamp.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeStamp::TimeStamp() :
    timeS(0),
    timeNs(0)
{
}

//------------------------------------------------------------------------------
TimeStamp::TimeStamp(const TimeSecondsSigned seconds,
                     const TimeNanosecondsSigned nanoseconds) :
    timeS(seconds),
    timeNs(nanoseconds)
{
    checkForOverUnderFlow();
    checkForSignChange();
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeStamp::~TimeStamp()
{
}

//------------------------------------------------------------------------------
// Public operator overloads
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool TimeStamp::operator>(const TimeStamp& timeStamp) const
{
    if (timeS > timeStamp.timeS)
    {
        return true;
    }
    else if (timeS == timeStamp.timeS)
    {
        return (timeNs > timeStamp.timeNs);
    }

    return false;
}

//------------------------------------------------------------------------------
bool TimeStamp::operator<(const TimeStamp& timeStamp) const
{
    if (timeS < timeStamp.timeS)
    {
        return true;
    }
    else if (timeS == timeStamp.timeS)
    {
        return (timeNs < timeStamp.timeNs);
    }

    return false;
}

//------------------------------------------------------------------------------
bool TimeStamp::operator==(const TimeStamp& timeStamp) const
{
    return ((timeS == timeStamp.timeS) && (timeNs == timeStamp.timeNs));
}

//------------------------------------------------------------------------------
bool TimeStamp::operator!=(const TimeStamp& timeStamp) const
{
    return ((timeS != timeStamp.timeS) || (timeNs != timeStamp.timeNs));
}

//------------------------------------------------------------------------------
bool TimeStamp::operator>=(const TimeStamp& timeStamp) const
{
    if (timeS > timeStamp.timeS)
    {
        return true;
    }
    else if (timeS == timeStamp.timeS)
    {
        return (timeNs >= timeStamp.timeNs);
    }

    return false;
}

//------------------------------------------------------------------------------
bool TimeStamp::operator<=(const TimeStamp& timeStamp) const
{
    if (timeS < timeStamp.timeS)
    {
        return true;
    }
    else if (timeS == timeStamp.timeS)
    {
        return (timeNs <= timeStamp.timeNs);
    }

    return false;
}

//------------------------------------------------------------------------------
TimeStamp TimeStamp::operator+(const TimeStamp& timeStamp) const
{
    TimeStamp result;

    result.timeS = timeS + timeStamp.timeS;

    result.timeNs = timeNs + timeStamp.timeNs;

    result.checkForOverUnderFlow();
    result.checkForSignChange();

    return result;
}

//------------------------------------------------------------------------------
TimeStamp& TimeStamp::operator+=(const TimeStamp& timeStamp)
{
    timeS += timeStamp.timeS;

    timeNs += timeStamp.timeNs;

    checkForOverUnderFlow();
    checkForSignChange();

    return (*this);
}

//------------------------------------------------------------------------------
TimeStamp TimeStamp::operator-(const TimeStamp& timeStamp) const
{
    TimeStamp result;

    result.timeS = timeS - timeStamp.timeS;

    result.timeNs = timeNs - timeStamp.timeNs;

    result.checkForOverUnderFlow();
    result.checkForSignChange();

    return result;
}

//------------------------------------------------------------------------------
TimeStamp& TimeStamp::operator-=(const TimeStamp& timeStamp)
{
    timeS -= timeStamp.timeS;

    timeNs -= timeStamp.timeNs;

    checkForOverUnderFlow();
    checkForSignChange();

    return (*this);
}

//------------------------------------------------------------------------------
TimeStamp TimeStamp::operator%(const TimeStamp& timeStamp) const
{
    TimeStamp result;

    int64_t ns = static_cast<int64_t>(timeS) * 1000000000 +
                                                   static_cast<int64_t>(timeNs);

    int64_t timeStampNs = static_cast<int64_t>(timeStamp.timeS) * 1000000000 +
                                         static_cast<int64_t>(timeStamp.timeNs);

    int64_t modulus = ns % timeStampNs;

    result.timeS = modulus / 1000000000;
    result.timeNs = modulus % 1000000000;

    return result;
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimeStamp::fromTimeMs(const TimeMs& timeMs,
                           const std::uint32_t rollOverCount)
{
    timeS  = (timeMs / 1000) +
                    (std::numeric_limits<TimeMs>::max() / 1000) * rollOverCount;
    timeNs = (timeMs % 1000) * 1000000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeUs(const TimeUs& timeUs,
                           const std::uint32_t rollOverCount)
{
    timeS  = (timeUs / 1000000) +
                 (std::numeric_limits<TimeUs>::max() / 1000000) * rollOverCount;
    timeNs = (timeUs % 1000000) * 1000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeNs(const TimeNs& timeNs,
                           const std::uint32_t rollOverCount)
{
    timeS  = (timeNs / 1000000000) +
              (std::numeric_limits<TimeNs>::max() / 1000000000) * rollOverCount;
    this->timeNs = (timeNs % 1000000000);
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeMsSigned(const TimeMsSigned& timeMsSigned,
                                 const std::uint32_t rollOverCount)
{
    timeS  = (timeMsSigned / 1000) +
              (std::numeric_limits<TimeMsSigned>::max() / 1000) * rollOverCount;
    timeNs = (timeMsSigned % 1000) * 1000000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeUsSigned(const TimeUsSigned& timeUsSigned,
                                 const std::uint32_t rollOverCount)
{
    timeS  = (timeUsSigned / 1000000) +
           (std::numeric_limits<TimeUsSigned>::max() / 1000000) * rollOverCount;
    timeNs = (timeUsSigned % 1000000) * 1000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeNsSigned(const TimeNsSigned& timeNsSigned,
                                 const std::uint32_t rollOverCount)
{
    timeS  = (timeNsSigned / 1000000000) +
        (std::numeric_limits<TimeNsSigned>::max() / 1000000000) * rollOverCount;
    this->timeNs = (timeNsSigned % 1000000000);
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeSFloat(const TimeSFloat& timeSFloat)
{
    fromTimeSValueType<float>(timeSFloat);
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeSFloat(const TimeSFloat& timeSFloat,
                               const std::uint32_t roundingPrecision)
{
    fromTimeSValueType<float>(timeSFloat, roundingPrecision);
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeSDouble(const TimeSDouble& timeSDouble)
{
    fromTimeSValueType<double>(timeSDouble);
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeSDouble(const TimeSDouble& timeSDouble,
                                const std::uint32_t roundingPrecision)
{
    fromTimeSValueType<double>(timeSDouble, roundingPrecision);
}

//------------------------------------------------------------------------------
float TimeStamp::toTimeSFloat() const
{
    return (toTimeSValueType<float>());
}

//------------------------------------------------------------------------------
double TimeStamp::toTimeSDouble() const
{
    return (toTimeSValueType<double>());
}

//------------------------------------------------------------------------------
TimeMillisecondsSigned TimeStamp::toTimeMsSigned() const
{
    std::uint32_t rollOverCount = 0;

    return toTimeMsSigned(rollOverCount);
}

//------------------------------------------------------------------------------
TimeMsSigned TimeStamp::toTimeMsSigned(std::uint32_t& rollOverCount) const
{
    std::int64_t timeMsSignedWithOverflow =
        (static_cast<std::int64_t>(timeS) * 1000) +
                                      integerDivideRound((int) timeNs, 1000000);

    TimeMsSigned timeMsSigned = timeMsSignedWithOverflow;

    if ((timeMsSigned < 0) && ((timeS > 0) || (timeNs > 0)))
    {
        timeMsSigned += std::numeric_limits<TimeMsSigned>::max();
        timeMsSigned += 1;
    }
    else if ((timeMsSigned > 0) && ((timeS < 0) || (timeNs < 0)))
    {
        timeMsSigned -= std::numeric_limits<TimeMsSigned>::max();
        timeMsSigned -= 1;
    }

    rollOverCount =
        std::abs(timeMsSignedWithOverflow /
            (static_cast<std::int64_t>(
                                std::numeric_limits<TimeMsSigned>::max()) + 1));

    return timeMsSigned;
}

//------------------------------------------------------------------------------
TimeUsSigned TimeStamp::toTimeUsSigned() const
{
    std::uint32_t rollOverCount = 0;

    return toTimeUsSigned(rollOverCount);
}

//------------------------------------------------------------------------------
TimeUsSigned TimeStamp::toTimeUsSigned(uint32_t& rollOverCount) const
{
    std::int64_t timeUsSignedWithOverflow =
        (static_cast<std::int64_t>(timeS) * 1000000) +
                                         integerDivideRound((int) timeNs, 1000);

    TimeUsSigned timeUsSigned = timeUsSignedWithOverflow;

    if ((timeUsSigned < 0) && ((timeS > 0) || (timeNs > 0)))
    {
        timeUsSigned += std::numeric_limits<TimeUsSigned>::max();
        timeUsSigned += 1;
    }
    else if ((timeUsSigned > 0) && ((timeS < 0) || (timeNs < 0)))
    {
        timeUsSigned -= std::numeric_limits<TimeUsSigned>::max();
        timeUsSigned -= 1;
    }

    rollOverCount =
        std::abs(timeUsSignedWithOverflow /
            (static_cast<std::int64_t>(
                                std::numeric_limits<TimeUsSigned>::max()) + 1));

    return timeUsSigned;
}

//------------------------------------------------------------------------------
TimeNsSigned TimeStamp::toTimeNsSigned() const
{
    std::uint32_t rollOverCount = 0;

    return toTimeNsSigned(rollOverCount);
}

//------------------------------------------------------------------------------
TimeNsSigned TimeStamp::toTimeNsSigned(std::uint32_t& rollOverCount) const
{
    std::int64_t timeNsSignedWithOverflow =
                       (static_cast<std::int64_t>(timeS) * 1000000000) + timeNs;

    TimeNsSigned timeNsSigned = timeNsSignedWithOverflow;

    if ((timeNsSigned < 0) && ((timeS > 0) || (timeNs > 0)))
    {
        timeNsSigned += std::numeric_limits<TimeNsSigned>::max();
        timeNsSigned += 1;
    }
    else if ((timeNsSigned > 0) && ((timeS < 0) || (timeNs < 0)))
    {
        timeNsSigned -= std::numeric_limits<TimeNsSigned>::max();
        timeNsSigned -= 1;
    }

    rollOverCount =
        std::abs(timeNsSignedWithOverflow /
            (static_cast<std::int64_t>(
                                std::numeric_limits<TimeNsSigned>::max()) + 1));

    return timeNsSigned;
}

//------------------------------------------------------------------------------
// Public deprecated methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeMsSigned TimeStamp::toTimeMs() const
{
    return toTimeMsSigned();
}

//------------------------------------------------------------------------------
TimeMsSigned TimeStamp::toTimeMs(std::uint32_t& rollOverCount) const
{
    return toTimeMsSigned(rollOverCount);
}

//------------------------------------------------------------------------------
TimeUsSigned TimeStamp::toTimeUs() const
{
    return toTimeUsSigned();
}

//------------------------------------------------------------------------------
TimeUsSigned TimeStamp::toTimeUs(std::uint32_t& rollOverCount) const
{
    return toTimeUsSigned(rollOverCount);
}

//------------------------------------------------------------------------------
TimeNsSigned TimeStamp::toTimeNs() const
{
    return toTimeNsSigned();
}

//------------------------------------------------------------------------------
TimeNsSigned TimeStamp::toTimeNs(std::uint32_t& rollOverCount) const
{
    return toTimeNsSigned(rollOverCount);
}

//------------------------------------------------------------------------------
void TimeStamp::reset()
{
    timeNs = 0;
    timeS = 0;
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimeStamp::checkForOverUnderFlow()
{
    if (timeNs <= -1000000000)
    {
        timeS -= 1;
        timeNs += 1000000000;
    }
    else if (timeNs >= 1000000000)
    {
        timeS += 1;
        timeNs -= 1000000000;
    }
}

//------------------------------------------------------------------------------
void TimeStamp::checkForSignChange()
{
    if ((timeS < 0) && (timeNs > 0))
    {
        timeS += 1;
        timeNs -= 1000000000;
    }
    else if ((timeS > 0) && (timeNs < 0))
    {
        timeS -= 1;
        timeNs += 1000000000;
    }
}
