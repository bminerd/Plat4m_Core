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
// Copyright (c) 2012-2023 Benjamin Minerd
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
/// @file Plat4m.h
/// @author Ben Minerd
/// @date 7/30/2012
/// @brief Plat4m namespace file.
///

#ifndef PLAT4M_H
#define PLAT4M_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>
#include <float.h>
#include <math.h>

#ifdef PLAT4M_CORE_CONFIG
#include <Plat4mCoreConfig.h>
#endif

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define PLAT4M_VERSION "0.0.24"

///
/// @brief Returns the number of elements of any given array.
/// @param array Array to get number of elements.
///
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

#define ARRAY_SIZE_2D(array) (sizeof(array) / sizeof(array[0][0]))

#define FOREVER while (true)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{
    //--------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------

    enum BitValue
    {
        BIT_VALUE_0 = 0,
        BIT_VALUE_1 = 1
    };
    
    enum LogicLevel
    {
        LOGIC_LEVEL_LOW     = 0,
        LOGIC_LEVEL_HIGH    = 1
    };

    enum Endian
    {
        ENDIAN_LITTLE,
        ENDIAN_BIG
    };

    typedef uint32_t Id;

    typedef uint32_t TimeS;
    typedef uint32_t TimeMs;
    typedef uint32_t TimeUs;
    typedef uint32_t TimeNs;

    typedef TimeS TimeSeconds;
    typedef TimeMs TimeMilliseconds;
    typedef TimeUs TimeMicroseconds;
    typedef TimeNs TimeNanoSeconds;

    typedef int32_t TimeSSigned;
    typedef int32_t TimeMsSigned;
    typedef int32_t TimeUsSigned;
    typedef int32_t TimeNsSigned;

    typedef TimeSSigned TimeSecondsSigned;
    typedef TimeMsSigned TimeMillisecondsSigned;
    typedef TimeUsSigned TimeMicrosecondsSigned;
    typedef TimeNsSigned TimeNanosecondsSigned;

    typedef float TimeSFloat;
    typedef TimeSFloat TimeSecondsFloat;

    typedef double TimeSDouble;
    typedef TimeSDouble TimeSecondsDouble;

#ifndef PLAT4M_CORE_CONFIG
    typedef float RealNumber;
#endif

    typedef RealNumber VoltageV;
    typedef RealNumber VoltageMv;

    typedef RealNumber CurrentA;
    typedef RealNumber CurrentMa;

    typedef RealNumber PositionM;

    typedef RealNumber VelocityMpS;
    typedef RealNumber VelocityMpm;
    typedef RealNumber VelocityKph;
    typedef RealNumber VelocityFps;
    typedef RealNumber VelocityFpm;
    typedef RealNumber VelocityMph;

    typedef RealNumber AccelerationG;
    typedef RealNumber AccelerationMg;

    typedef RealNumber FrequencyHz;

    typedef RealNumber AngleDegrees;
    typedef RealNumber AngleRadians;

    typedef RealNumber AngularSpeedDps;
    typedef RealNumber AngularSpeedRadPs;
    typedef RealNumber AngularSpeedRpm;

    typedef RealNumber AngularVelocityDps;
    typedef RealNumber AngularVelocityRadPs;
    typedef RealNumber AngularVelocityRpm;

    typedef RealNumber AngularAccelerationDps2;
    typedef RealNumber AngularAccelerationRadPs2;

    //--------------------------------------------------------------------------
    // Inline functions
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    template <typename T>
    inline bool isNullPointer(const T* pointer)
    {
        return (pointer == 0);
    }

    //--------------------------------------------------------------------------
    template <typename T>
    inline bool isValidPointer(const T* pointer)
    {
        return (pointer != 0);
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TMask>
    inline void setBits(TValue& value, const TMask mask)
    {
    	value |= static_cast<TValue>(mask);
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TMask>
    inline void clearBits(TValue& value, const TMask mask)
    {
        value &= ~static_cast<TValue>(mask);
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline void setBit(TValue& value, const int bitPosition)
    {
        setBits(value, static_cast<TValue>(1) << bitPosition);
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline void clearBit(TValue& value, const int bitPosition)
    {
        clearBits(value, static_cast<TValue>(1) << bitPosition);
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TClearMask, typename TSetMask>
    inline void clearAndSetBits(TValue& value,
                                const TClearMask clearMask,
                                const TSetMask setMask)
    {
        TValue tempValue = value;

        clearBits(tempValue, clearMask);
        setBits(tempValue, setMask);

        value = tempValue;
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TMask>
    inline void setBitsSet(TValue& value,
                           const TMask mask,
                           const bool set)
    {
        if (set)
        {
            setBits(value, mask);
        }
        else
        {
            clearBits(value, mask);
        }
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline void setBitSet(TValue& value,
                          const uint8_t bitPosition,
                          const bool set)
    {
        if (set)
        {
            setBit(value, bitPosition);
        }
        else
        {
            clearBit(value, bitPosition);
        }
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TMask>
    inline bool areBitsSet(const TValue value, const TMask mask)
    {
    	return ((value & mask) == mask);
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline bool isBitSet(const TValue value, const int bitIndex)
    {
        return areBitsSet(value, static_cast<TValue>(1) << bitIndex);
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TMask>
    inline bool areBitsClear(const TValue value, const TMask mask)
    {
    	return (!areBitsSet(value, mask));
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline bool isBitClear(const TValue value, const int bitIndex)
    {
        return areBitsClear(value, static_cast<TValue>(1) << bitIndex);
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TMask>
    inline void toggleBits(TValue& value, const TMask mask)
    {
        value ^= static_cast<TValue>(mask);
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline void toggleBit(TValue& value, const int bitIndex)
    {
        toggleBits(value, static_cast<TValue>(1) << bitIndex);
    }

    //--------------------------------------------------------------------------
    template <typename TValue, typename TMask>
    inline TValue maskBits(const TValue& value, const TMask& bitMask)
    {
        return (value & static_cast<TValue>(bitMask));
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    constexpr TValue power(const TValue value, const int exponent)
    {
        if (exponent != 1)
        {
            return power(value * value, exponent - 1);
        }

        return value;
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    constexpr TValue bitMask(const int nBits, const int bitIndex)
    {
        return ((power(static_cast<TValue>(2), nBits) - 1) << bitIndex);
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline bool equals(const TValue value1, const TValue value2)
    {
        return (value1 == value2);
    }

    //--------------------------------------------------------------------------
    template <>
    inline bool equals(const float value1, const float value2)
    {
        return (fabs(value1 - value2) <= (FLT_EPSILON + FLT_EPSILON));
    }

    //--------------------------------------------------------------------------
    template <>
    inline bool equals(const double value1, const double value2)
    {
        return (fabs(value1 - value2) <= (DBL_EPSILON + DBL_EPSILON));
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline bool findValueIndex(const TValue value,
                               const TValue valueTable[],
                               const uint32_t valueTableSize,
                               uint32_t& index)
    {
        uint32_t i;

        for (i = 0; i < valueTableSize; i++)
        {
            if (value == valueTable[i])
            {
                break;
            }
        }

        if (i == valueTableSize)
        {
            return false;
        }

        index = i;

        return true;
    }

    //--------------------------------------------------------------------------
    template <>
    inline bool findValueIndex(const float value,
                               const float valueTable[],
                               const uint32_t valueTableSize,
                               uint32_t& index)
    {
        uint32_t i;

        for (i = 0; i < valueTableSize; i++)
        {
            if (equals(value, valueTable[i]))
            {
                break;
            }
        }

        if (i == valueTableSize)
        {
            return false;
        }

        index = i;

        return true;
    }

    //--------------------------------------------------------------------------
    template <>
    inline bool findValueIndex(const double value,
                               const double valueTable[],
                               const uint32_t valueTableSize,
                               uint32_t& index)
    {
        uint32_t i;

        for (i = 0; i < valueTableSize; i++)
        {
            if (equals(value, valueTable[i]))
            {
                break;
            }
        }

        if (i == valueTableSize)
        {
            return false;
        }

        index = i;

        return true;
    }

    //--------------------------------------------------------------------------
    inline char uint8ToAscii(const uint8_t value)
    {
        return (0x30 + value);
    }

    //--------------------------------------------------------------------------
    inline uint8_t asciiToUint8(const char character)
    {
        return (character - 0x30);
    }

    //--------------------------------------------------------------------------
    template<typename TValue>
    inline void limitValue(TValue& value,
                           const TValue lowerLimit,
                           const TValue upperLimit)
    {
        if (value < lowerLimit)
        {
            value = lowerLimit;
        }
        else if (value > upperLimit)
        {
            value = upperLimit;
        }
    }

    //--------------------------------------------------------------------------
    inline AngleDegrees radiansToDegrees(const AngleRadians angleRadians)
    {
        return static_cast<AngleDegrees>(angleRadians * (180.0 / M_PI));
    }

    //--------------------------------------------------------------------------
    inline AngleRadians degreesToRadians(const AngleDegrees angleDegrees)
    {
        return static_cast<AngleRadians>(angleDegrees * (M_PI / 180.0));
    }

    //--------------------------------------------------------------------------
    inline AngularVelocityDps radPsToDps(const AngularVelocityRadPs radPs)
    {
        return static_cast<AngularVelocityDps>(radPs * (180.0 / M_PI));
    }

    //--------------------------------------------------------------------------
    inline AngularVelocityRadPs dpsToRadPs(const AngularVelocityDps dps)
    {
        return static_cast<AngularVelocityRadPs>(dps * (M_PI / 180.0));
    }

    //--------------------------------------------------------------------------
    inline AngularAccelerationDps2 radPs2ToDps2(
                                         const AngularAccelerationRadPs2 radPs2)
    {
        return static_cast<AngularAccelerationDps2>(radPs2 * (180.0 / M_PI));
    }

    //--------------------------------------------------------------------------
    inline AngularAccelerationRadPs2 dps2ToRadPs2(
                                             const AngularAccelerationDps2 dps2)
    {
        return static_cast<AngularAccelerationRadPs2>(dps2 * (M_PI / 180.0));
    }

    //--------------------------------------------------------------------------
    template <typename TValue, uint32_t NValues>
    inline uint32_t arraySize(const TValue (&valueArray)[NValues])
    {
        return NValues;
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline void findMinMax(const TValue value,
                           TValue& minValue,
                           TValue& maxValue)
    {
        if (value < minValue)
        {
            minValue = value;
        }
        else if (value > maxValue)
        {
            maxValue = value;
        }
    }

    //--------------------------------------------------------------------------
    template <typename TValue>
    inline TValue integerDivideRound(const TValue& dividend,
                                     const TValue& divisor)
    {
        if (dividend < 0)
        {
            return ((dividend - (divisor / 2)) / divisor);
        }

        return ((dividend + (divisor / 2)) / divisor);
    }
}; // namespace Plat4m

#endif // PLAT4M_H
