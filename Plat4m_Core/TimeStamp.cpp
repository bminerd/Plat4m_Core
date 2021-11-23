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
// Copyright (c) 2021 Benjamin Minerd
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

#include <Plat4m_Core/Plat4m.h>

#include <Plat4m_Core/TimeStamp.h>

using namespace std;
using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimeStamp::TimeStamp(const TimeS timeS, const TimeNs timeNs) :
    timeS(timeS),
    timeNs(timeNs)
{
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

    if (result.timeNs >= 1000000000)
    {
        result.timeS += 1;
        result.timeNs -= 1000000000;
    }

    return result;
}

//------------------------------------------------------------------------------
TimeStamp& TimeStamp::operator+=(const TimeStamp& timeStamp)
{
    timeS += timeStamp.timeS;

    timeNs += timeStamp.timeNs;

    if (timeNs >= 1000000000)
    {
        timeS += 1;
        timeNs -= 1000000000;
    }

    return (*this);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimeStamp::fromTimeMs(const TimeMs& timeMs)
{
    timeS  = timeMs / 1000;
    timeNs = (timeMs % 1000) * 1000000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeUs(const TimeUs& timeUs)
{
    timeS  = timeUs / 1000000;
    timeNs = (timeUs % 1000000) * 1000;
}

//------------------------------------------------------------------------------
void TimeStamp::fromTimeNs(const TimeNs& timeNs)
{
    timeS  = timeNs / 1000000000;
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
TimeMs TimeStamp::toTimeMs() const
{
    TimeMs timeMs = (timeS * 1000) + integerDivideRound((int) timeNs, 1000000);

    return timeMs;
}

//------------------------------------------------------------------------------
TimeMs TimeStamp::toTimeMs(uint32_t& rollOverCount) const
{
    uint64_t timeMsWithOverflow =
                     (timeS * 1000) + integerDivideRound((int) timeNs, 1000000);

    TimeMs timeMs = timeMsWithOverflow;

    rollOverCount = timeMsWithOverflow / ((uint64_t) timeMs);

    return timeMs;
}

//------------------------------------------------------------------------------
TimeUs TimeStamp::toTimeUs() const
{
    TimeUs timeUs = (timeS * 1000000) + integerDivideRound((int) timeNs, 1000);

    return timeUs;
}

//------------------------------------------------------------------------------
TimeUs TimeStamp::toTimeUs(uint32_t& rollOverCount) const
{
    uint64_t timeUsWithOverflow =
                     (timeS * 1000000) + integerDivideRound((int) timeNs, 1000);

    TimeUs timeUs = timeUsWithOverflow;

    rollOverCount = timeUsWithOverflow / ((uint64_t) timeUs);

    return timeUs;
}

//------------------------------------------------------------------------------
TimeNs TimeStamp::toTimeNs() const
{
    TimeNs timeNs = (timeS * 1000000000) + this->timeNs;

    return timeNs;
}

//------------------------------------------------------------------------------
TimeNs TimeStamp::toTimeNs(uint32_t& rollOverCount) const
{
    uint64_t timeNsWithOverflow = (timeS * 1000000000) + timeNs;

    TimeUs returnTimeNs = timeNsWithOverflow;

    rollOverCount = timeNsWithOverflow / ((uint64_t) returnTimeNs);

    return returnTimeNs;
}
