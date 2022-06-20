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
/// @file TimeStamp.h
/// @author Ben Minerd
/// @date 9/23/2021
/// @brief TimeStamp class header file.
///

#ifndef PLAT4M_TIME_STAMP_H
#define PLAT4M_TIME_STAMP_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Plat4m.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------

struct TimeStamp
{
    //--------------------------------------------------------------------------
    // Public data members
    //--------------------------------------------------------------------------

    TimeSecondsSigned timeS;
    TimeNanosecondsSigned timeNs;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    TimeStamp();

    TimeStamp(const TimeSecondsSigned seconds,
              const TimeNanosecondsSigned nanoseconds);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    ~TimeStamp();

    //--------------------------------------------------------------------------
    // Public operator overloads
    //--------------------------------------------------------------------------

    bool operator>(const TimeStamp& timeStamp) const;

    bool operator<(const TimeStamp& timeStamp) const;

    bool operator==(const TimeStamp& timeStamp) const;

    bool operator!=(const TimeStamp& timeStamp) const;

    bool operator>=(const TimeStamp& timeStamp) const;

    bool operator<=(const TimeStamp& timeStamp) const;

    TimeStamp operator+(const TimeStamp& timeStamp) const;

    TimeStamp& operator+=(const TimeStamp& timeStamp);

    TimeStamp operator-(const TimeStamp& timeStamp) const;

    TimeStamp& operator-=(const TimeStamp& timeStamp);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void fromTimeMs(const TimeMillisecondsSigned& timeMs,
                    const std::uint32_t rollOverCount = 0);

    void fromTimeUs(const TimeMicrosecondsSigned& timeUs,
                    const std::uint32_t rollOverCount = 0);

    void fromTimeNs(const TimeNanosecondsSigned& timeNs,
                    const std::uint32_t rollOverCount = 0);

    float toTimeSFloat() const;

    TimeMillisecondsSigned toTimeMs() const;

    TimeMillisecondsSigned toTimeMs(std::uint32_t& rollOverCount) const;

    TimeMicrosecondsSigned toTimeUs() const;

    TimeMicrosecondsSigned toTimeUs(std::uint32_t& rollOverCount) const;

    TimeNanosecondsSigned toTimeNs() const;

    TimeNanosecondsSigned toTimeNs(std::uint32_t& rollOverCount) const;

private:

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void checkForOverUnderFlow();

    void checkForSignChange();
};

}; // namespace Plat4m

#endif // PLAT4M_TIME_STAMP_H
