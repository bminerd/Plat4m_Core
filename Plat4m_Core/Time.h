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
/// @file Time.h
/// @author Ben Minerd
/// @date 9/23/2021
/// @brief Time class header file.
///

#ifndef PLAT4M_TIME_H
#define PLAT4M_TIME_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Quantity.h>
#include <Plat4m_Core/UnitsTime.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType, Units::Time::Type UnitsTimeOption>
class Time : public Quantity<ValueType, Units::Time, UnitsTimeOption>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Time() :
        Quantity<ValueType, Units::Time, UnitsTimeOption>()
    {
    }

    //--------------------------------------------------------------------------
    explicit Time(const ValueType& time) :
        Quantity<ValueType, Units::Time, UnitsTimeOption>(time)
    {
    }

    //--------------------------------------------------------------------------
    // Public copy constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Time(const QuantityInterface<ValueType>& quantity) :
        Quantity<ValueType, Units::Time, UnitsTimeOption>(quantity.get())
    {
    }

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ~Time()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    using Quantity<ValueType, Units::Time, UnitsTimeOption>::from;

    using Quantity<ValueType, Units::Time, UnitsTimeOption>::to;

    //--------------------------------------------------------------------------
    void fromTimeMs(const TimeMs& timeMs,
                    const std::uint32_t rollOverCount = 0)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                          template from<TimeMs, Units::Time::MS>(timeMs,
                                                                 rollOverCount);
    }

    //--------------------------------------------------------------------------
    void fromTimeUs(const TimeUs& timeUs,
                    const std::uint32_t rollOverCount = 0)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                          template from<TimeUs, Units::Time::US>(timeUs,
                                                                 rollOverCount);
    }

    //--------------------------------------------------------------------------
    void fromTimeNs(const TimeNs& timeNs,
                    const std::uint32_t rollOverCount = 0)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                          template from<TimeNs, Units::Time::NS>(timeNs,
                                                                 rollOverCount);
    }

    //--------------------------------------------------------------------------
    void fromTimeMsSigned(const TimeMsSigned& timeMsSigned,
                          const std::uint32_t rollOverCount = 0)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template from<TimeMsSigned, Units::Time::MS>(timeMsSigned,
                                                                 rollOverCount);
    }

    //--------------------------------------------------------------------------
    void fromTimeUsSigned(const TimeUsSigned& timeUsSigned,
                         const std::uint32_t rollOverCount = 0)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template from<TimeUsSigned, Units::Time::US>(timeUsSigned,
                                                                 rollOverCount);
    }

    //--------------------------------------------------------------------------
    void fromTimeNsSigned(const TimeNsSigned& timeNsSigned,
                          const std::uint32_t rollOverCount = 0)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template from<TimeNsSigned, Units::Time::NS>(timeNsSigned,
                                                                 rollOverCount);
    }

    //--------------------------------------------------------------------------
    void fromTimeSFloat(const TimeSFloat& timeSFloat)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                          template from<TimeSFloat, Units::Time::S>(timeSFloat);
    }

    //--------------------------------------------------------------------------
    void fromTimeSDouble(const TimeSDouble& timeSDouble)
    {
        Quantity<ValueType, Units::Time, UnitsTimeOption>::
                        template from<TimeSDouble, Units::Time::S>(timeSDouble);
    }

    //--------------------------------------------------------------------------
    template <typename ValueType2>
    void fromTimeSValueType(const ValueType2& timeS,
                            const std::uint32_t roundingPrecision)
    {
        if (roundingPrecision > 0)
        {
            const std::int32_t multiplier =
                       Units::Time::multiplier(Units::Time::S, UnitsTimeOption);

            const ValueType newValue =
                                 static_cast<std::uint64_t>(timeS) * multiplier;

            const ValueType2 remainder =
                         timeS * multiplier - static_cast<ValueType2>(newValue);

            const ValueType remainderRounded =
                static_cast<ValueType>(
                    remainder / (multiplier / roundingPrecision) + 0.5) *
                                               (multiplier / roundingPrecision);

            QuantityInterface<ValueType>::set(newValue + remainderRounded);
        }
        else
        {
            Quantity<ValueType, Units::Time, UnitsTimeOption>::
                                template from<ValueType, Units::Time::S>(timeS);
        }
    }

    //--------------------------------------------------------------------------
    void fromTimeSFloat(const TimeSFloat& timeSFloat,
                        const std::uint32_t roundingPrecision)
    {
        fromTimeSValueType(timeSFloat, roundingPrecision);
    }

    //--------------------------------------------------------------------------
    void fromTimeSDouble(const TimeSDouble& timeSDouble,
                         const std::uint32_t roundingPrecision)
    {
        fromTimeSValueType(timeSDouble, roundingPrecision);
    }

    //--------------------------------------------------------------------------
    TimeMsSigned toTimeMs() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                                  template to<TimeMs, Units::Time::MS>().get());
    }

    //--------------------------------------------------------------------------
    TimeMsSigned toTimeMs(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                     template to<TimeMs, Units::Time::MS>(rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeMsLong toTimeMsLong() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                              template to<TimeMsLong, Units::Time::MS>().get());
    }

    //--------------------------------------------------------------------------
    TimeMsLong toTimeMsLong(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                   template to<TimeMsLong, Units::Time::MS>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeMsSigned toTimeMsSigned() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                            template to<TimeMsSigned, Units::Time::MS>().get());
    }

    //--------------------------------------------------------------------------
    TimeMsSigned toTimeMsSigned(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template to<TimeMsSigned, Units::Time::MS>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeMsSignedLong toTimeMsSignedLong() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                        template to<TimeMsSignedLong, Units::Time::MS>().get());
    }

    //--------------------------------------------------------------------------
    TimeMsSignedLong toTimeMsSignedLong(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template to<TimeMsSignedLong, Units::Time::MS>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeUsSigned toTimeUs() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                                  template to<TimeUs, Units::Time::US>().get());
    }

    //--------------------------------------------------------------------------
    TimeUsSigned toTimeUs(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                     template to<TimeUs, Units::Time::US>(rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeUsLong toTimeUsLong() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                              template to<TimeUsLong, Units::Time::US>().get());
    }

    //--------------------------------------------------------------------------
    TimeUsLong toTimeUsLong(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                   template to<TimeUsLong, Units::Time::US>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeUsSigned toTimeUsSigned() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                            template to<TimeUsSigned, Units::Time::US>().get());
    }

    //--------------------------------------------------------------------------
    TimeUsSigned toTimeUsSigned(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                 template to<TimeUsSigned, Units::Time::US>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeUsSignedLong toTimeUsSignedLong() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                        template to<TimeUsSignedLong, Units::Time::US>().get());
    }

    //--------------------------------------------------------------------------
    TimeUsSignedLong toTimeUsSignedLong(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template to<TimeUsSignedLong, Units::Time::US>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeNsSigned toTimeNs() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                                  template to<TimeNs, Units::Time::NS>().get());
    }

    //--------------------------------------------------------------------------
    TimeNsSigned toTimeNs(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                     template to<TimeNs, Units::Time::NS>(rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeNsLong toTimeNsLong() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                              template to<TimeNsLong, Units::Time::NS>().get());
    }

    //--------------------------------------------------------------------------
    TimeNsLong toTimeNsLong(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template to<TimeNsLong, Units::Time::NS>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeNsSigned toTimeNsSigned() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                            template to<TimeNsSigned, Units::Time::NS>().get());
    }

    //--------------------------------------------------------------------------
    TimeNsSigned toTimeNsSigned(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template to<TimeNsSigned, Units::Time::NS>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeNsSignedLong toTimeNsSignedLong() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                        template to<TimeNsSignedLong, Units::Time::NS>().get());
    }

    //--------------------------------------------------------------------------
    TimeNsSignedLong toTimeNsSignedLong(std::uint32_t& rollOverCount) const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                    template to<TimeNsSignedLong, Units::Time::NS>(
                                                          rollOverCount).get());
    }

    //--------------------------------------------------------------------------
    TimeSFloat toTimeSFloat() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                               template to<TimeSFloat, Units::Time::S>().get());
    }

    //--------------------------------------------------------------------------
    TimeSDouble toTimeSDouble() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                              template to<TimeSDouble, Units::Time::S>().get());
    }

    //--------------------------------------------------------------------------
    template <typename ValueType2>
    ValueType2 toTimeSValueType() const
    {
        return (Quantity<ValueType, Units::Time, UnitsTimeOption>::
                               template to<ValueType2, Units::Time::S>().get());
    }
};

}; // namespace Plat4m

#endif // PLAT4M_TIME_H
