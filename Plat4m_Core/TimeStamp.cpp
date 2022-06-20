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
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimeStamp::fromTimeMs(const TimeMillisecondsSigned& timeMs,
                           const std::uint32_t rollOverCount)
{
    timeS  = (timeMs / 1000) +
              (std::numeric_limits<std::int32_t>::max() / 1000) * rollOverCount;
    timeNs = (timeMs % 1000) * 1000000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeUs(const TimeMicrosecondsSigned& timeUs,
                           const std::uint32_t rollOverCount)
{
    timeS  = (timeUs / 1000000) +
           (std::numeric_limits<std::int32_t>::max() / 1000000) * rollOverCount;
    timeNs = (timeUs % 1000000) * 1000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeNs(const TimeNanosecondsSigned& timeNs,
                           const std::uint32_t rollOverCount)
{
    timeS  = (timeNs / 1000000000) +
        (std::numeric_limits<std::int32_t>::max() / 1000000000) * rollOverCount;
    this->timeNs = (timeNs % 1000000000);
}

//------------------------------------------------------------------------------
float TimeStamp::toTimeSFloat() const
{
    float timeS =
          static_cast<float>(this->timeS) +
                                      (static_cast<float>(timeNs) / 1000000000);

    return timeS;
}

//------------------------------------------------------------------------------
TimeMillisecondsSigned TimeStamp::toTimeMs() const
{
    uint32_t rollOverCount = 0;

    return toTimeMs(rollOverCount);
}

//------------------------------------------------------------------------------
TimeMillisecondsSigned TimeStamp::toTimeMs(uint32_t& rollOverCount) const
{
    int64_t timeMsWithOverflow =
           ((int64_t) timeS * 1000) + integerDivideRound((int) timeNs, 1000000);

    TimeMillisecondsSigned timeMs = timeMsWithOverflow;

    if ((timeMs < 0) && ((timeS > 0) || (timeNs > 0)))
    {
        timeMs += std::numeric_limits<std::int32_t>::max();
        timeMs += 1;
    }
    else if ((timeMs > 0) && ((timeS < 0) || (timeNs < 0)))
    {
        timeMs -= std::numeric_limits<std::int32_t>::max();
        timeMs -= 1;
    }

    rollOverCount =
        std::abs(timeMsWithOverflow /
                      ((int64_t) std::numeric_limits<std::int32_t>::max() + 1));

    return timeMs;
}

//------------------------------------------------------------------------------
TimeMicrosecondsSigned TimeStamp::toTimeUs() const
{
    uint32_t rollOverCount = 0;

    return toTimeUs(rollOverCount);
}

//------------------------------------------------------------------------------
TimeMicrosecondsSigned TimeStamp::toTimeUs(uint32_t& rollOverCount) const
{
    int64_t timeUsWithOverflow =
           ((int64_t) timeS * 1000000) + integerDivideRound((int) timeNs, 1000);

    TimeMicrosecondsSigned timeUs = timeUsWithOverflow;

    if ((timeUs < 0) && ((timeS > 0) || (timeNs > 0)))
    {
        timeUs += std::numeric_limits<std::int32_t>::max();
        timeUs += 1;
    }
    else if ((timeUs > 0) && ((timeS < 0) || (timeNs < 0)))
    {
        timeUs -= std::numeric_limits<std::int32_t>::max();
        timeUs -= 1;
    }

    rollOverCount =
        std::abs(timeUsWithOverflow /
                      ((int64_t) std::numeric_limits<std::int32_t>::max() + 1));

    return timeUs;
}

//------------------------------------------------------------------------------
TimeNanosecondsSigned TimeStamp::toTimeNs() const
{
    uint32_t rollOverCount = 0;

    return toTimeNs(rollOverCount);
}

//------------------------------------------------------------------------------
TimeNanosecondsSigned TimeStamp::toTimeNs(uint32_t& rollOverCount) const
{
    int64_t timeNsWithOverflow = ((int64_t) timeS * 1000000000) + timeNs;

    TimeNanosecondsSigned returnTimeNs = timeNsWithOverflow;

    if ((returnTimeNs < 0) && ((timeS > 0) || (timeNs > 0)))
    {
        returnTimeNs += std::numeric_limits<std::int32_t>::max();
        returnTimeNs += 1;
    }
    else if ((returnTimeNs > 0) && ((timeS < 0) || (timeNs < 0)))
    {
        returnTimeNs -= std::numeric_limits<std::int32_t>::max();
        returnTimeNs -= 1;
    }

    rollOverCount =
        std::abs(timeNsWithOverflow /
                      ((int64_t) std::numeric_limits<std::int32_t>::max() + 1));

    return returnTimeNs;
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
