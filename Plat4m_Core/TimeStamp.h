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

    TimeStamp operator%(const TimeStamp& timeStamp) const;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void fromTimeMs(const TimeMs& timeMs,
                    const std::uint32_t rollOverCount = 0);

    void fromTimeUs(const TimeUs& timeUs,
                    const std::uint32_t rollOverCount = 0);

    void fromTimeNs(const TimeNs& timeNs,
                    const std::uint32_t rollOverCount = 0);

    void fromTimeMsSigned(const TimeMsSigned& timeMsSigned,
                          const std::uint32_t rollOverCount = 0);

    void fromTimeUsSigned(const TimeUsSigned& timeUsSigned,
                         const std::uint32_t rollOverCount = 0);

    void fromTimeNsSigned(const TimeNsSigned& timeNsSigned,
                          const std::uint32_t rollOverCount = 0);

    void fromTimeSFloat(const TimeSFloat& timeSFloat);

    void fromTimeSFloat(const TimeSFloat& timeSFloat,
                        const std::uint32_t roundingPrecision);

    void fromTimeSDouble(const TimeSDouble& timeSDouble);

    void fromTimeSDouble(const TimeSDouble& timeSDouble,
                         const std::uint32_t roundingPrecision);

    //--------------------------------------------------------------------------
    template <typename ValueType>
    void fromTimeSValueType(const ValueType& timeSValueType)
    {
        timeS = static_cast<TimeSSigned>(timeSValueType);

        timeNs =
            static_cast<TimeSSigned>(
                 (timeSValueType - static_cast<ValueType>(timeS)) * 1000000000);
    }

    //--------------------------------------------------------------------------
    template <typename ValueType>
    void fromTimeSValueType(const ValueType& timeSValueType,
                            const std::uint32_t roundingPrecisionMultiplier)
    {
        timeS = static_cast<TimeSSigned>(timeSValueType);
        ValueType remainder = timeSValueType - static_cast<ValueType>(timeS);

        TimeNsSigned truncatedInt =
            static_cast<TimeNsSigned>(
                remainder * roundingPrecisionMultiplier +
                                                   static_cast<ValueType>(0.5));

        timeNs = truncatedInt * (1000000000 / roundingPrecisionMultiplier);
    }

    float toTimeSFloat() const;

    double toTimeSDouble() const;

    //--------------------------------------------------------------------------
    template <typename ValueType>
    ValueType toTimeSValueType() const
    {
        ValueType timeS =
          static_cast<ValueType>(this->timeS) +
                                  (static_cast<ValueType>(timeNs) / 1000000000);

        return timeS;
    }

    TimeMsSigned toTimeMsSigned() const;

    TimeMsSigned toTimeMsSigned(std::uint32_t& rollOverCount) const;

    TimeUsSigned toTimeUsSigned() const;

    TimeUsSigned toTimeUsSigned(std::uint32_t& rollOverCount) const;

    TimeNsSigned toTimeNsSigned() const;

    TimeNsSigned toTimeNsSigned(std::uint32_t& rollOverCount) const;

    //--------------------------------------------------------------------------
    // Public deprecated methods
    //--------------------------------------------------------------------------

    TimeMsSigned toTimeMs() const;

    TimeMsSigned toTimeMs(std::uint32_t& rollOverCount) const;

    TimeUsSigned toTimeUs() const;

    TimeUsSigned toTimeUs(std::uint32_t& rollOverCount) const;

    TimeNsSigned toTimeNs() const;

    TimeNsSigned toTimeNs(std::uint32_t& rollOverCount) const;

    void reset();

private:

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void checkForOverUnderFlow();

    void checkForSignChange();
};

}; // namespace Plat4m

#endif // PLAT4M_TIME_STAMP_H
