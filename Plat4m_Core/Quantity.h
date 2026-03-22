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
/// @file Quantity.h
/// @author Ben Minerd
/// @date 5/5/2023
/// @brief Quantity class header file.
///

#ifndef PLAT4M_QUANTITY_H
#define PLAT4M_QUANTITY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>
#include <limits>
#include <type_traits>

#include <Plat4m_Core/QuantityInterface.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType,
          typename UnitsType,
          typename UnitsType::Type UnitsTypeOption>
class Quantity : public QuantityInterface<ValueType>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Quantity() :
        QuantityInterface<ValueType>()
    {
    }

    //--------------------------------------------------------------------------
    explicit Quantity(const ValueType& value) :
        QuantityInterface<ValueType>(value)
    {
    }

    //--------------------------------------------------------------------------
    // Public copy constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Quantity(const QuantityInterface<ValueType>& quantity) :
        QuantityInterface<ValueType>(quantity.get())
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    typename UnitsType::Type getUnits()
    {
        return UnitsTypeOption;
    }

    //--------------------------------------------------------------------------
    void from(const Quantity<ValueType, UnitsType, UnitsTypeOption>& quantity,
              const std::uint32_t rollOverCount = 0)
    {
        // Roll over count can be ignored because both quantities are of the
        // same value type and units, so they will roll over symmetrically

        QuantityInterface<ValueType>::set(quantity.get());
    }

    ///
    /// @brief Same units type, but different value type and units.
    ///
    //--------------------------------------------------------------------------
    template <typename ValueType2, typename UnitsType::Type UnitsTypeOption2>
    void from(const Quantity<ValueType2, UnitsType, UnitsTypeOption2>& quantity,
              const std::uint32_t rollOverCount = 0)
    {
        const ValueType2 value = quantity.get();
        const ValueType2 max = std::numeric_limits<ValueType2>::max();
        // (max + 1) * rollOverCount
        const ValueType2 rollOverValue = (max * rollOverCount + rollOverCount);
        const std::int32_t multiplier =
                       UnitsType::multiplier(UnitsTypeOption2, UnitsTypeOption);

        // A positive value indicates a multiplication, negative is division
        if (multiplier > 0)
        {
            if (sizeof(ValueType) > sizeof(ValueType2))
            {
                QuantityInterface<ValueType>::set(
                    static_cast<ValueType>(
                        (value + rollOverValue) *
                                           static_cast<ValueType>(multiplier)));
            }
            else
            {
                QuantityInterface<ValueType>::set(
                    static_cast<ValueType>(
                        (value + rollOverValue) *
                                          static_cast<ValueType2>(multiplier)));
            }
        }
        else
        {
            if (std::is_same<ValueType, float>::value ||
                                         std::is_same<ValueType, double>::value)
            {
                QuantityInterface<ValueType>::set(
                    static_cast<ValueType>(
                        (value + rollOverValue) /
                                          static_cast<ValueType>(-multiplier)));
            }
            else
            {
                QuantityInterface<ValueType>::set(
                    static_cast<ValueType>(
                        (value + rollOverValue) /
                                         static_cast<ValueType2>(-multiplier)));
            }
        }
    }

    //--------------------------------------------------------------------------
    template <typename ValueType2, typename UnitsType::Type UnitsTypeOption2>
    void from(const ValueType2& value, const std::uint32_t rollOverCount = 0)
    {
        Quantity<ValueType2, UnitsType, UnitsTypeOption2> quantity(value);

        from<ValueType2, UnitsTypeOption2>(quantity, rollOverCount);
    }

    //--------------------------------------------------------------------------
    Quantity<ValueType, UnitsType, UnitsTypeOption> to() const
    {
        return (*this);
    }

    //--------------------------------------------------------------------------
    Quantity<ValueType, UnitsType, UnitsTypeOption> to(
                                             std::uint32_t& rollOverCount) const
    {
        // Roll over count can be ignored because both quantities are of the
        // same value type and units, so they will roll over symmetrically

        rollOverCount = 0;

        return (*this);
    }

    ///
    /// @brief Same units type, but different value type and units.
    ///
    //--------------------------------------------------------------------------
    template <typename ValueType2, typename UnitsType::Type UnitsTypeOption2>
    Quantity<ValueType2, UnitsType, UnitsTypeOption2> to() const
    {
        std::uint32_t rollOverCount = 0;

        return (to<ValueType2, UnitsTypeOption2>(rollOverCount));
    }

    ///
    /// @brief Same units type, but different value type and units.
    ///
    //--------------------------------------------------------------------------
    template <typename ValueType2, typename UnitsType::Type UnitsTypeOption2>
    Quantity<ValueType2, UnitsType, UnitsTypeOption2> to(
                                             std::uint32_t& rollOverCount) const
    {
        const ValueType value = QuantityInterface<ValueType>::get();
        const std::int32_t multiplier =
        // const double multiplier =
                       UnitsType::multiplier(UnitsTypeOption, UnitsTypeOption2);

        Quantity<ValueType2, UnitsType, UnitsTypeOption2> result;

        // A positive value indicates a multiplication, negative is division
        if (multiplier > 0)
        {
            const ValueType2 max = std::numeric_limits<ValueType2>::max();

            result.set(static_cast<ValueType2>(
                                  value * static_cast<ValueType2>(multiplier)));

            const ValueType maxOverMultiplier =
                static_cast<ValueType>(
                                     max / static_cast<ValueType2>(multiplier));

            if (value > maxOverMultiplier)
            {
                rollOverCount = static_cast<std::uint32_t>(
                                                     value / maxOverMultiplier);
            }
            else if ((value < 0) && (-value > maxOverMultiplier))
            {
                rollOverCount = static_cast<std::uint32_t>(
                                                    -value / maxOverMultiplier);
            }
            else
            {
                rollOverCount = 0;
            }
        }
        else
        {
            if (std::is_same<ValueType, float>::value ||
                                         std::is_same<ValueType, double>::value)
            {
                result.set(static_cast<ValueType2>(
                                  value / static_cast<ValueType>(-multiplier)));
            }
            else if (std::is_same<ValueType2, float>::value ||
                                        std::is_same<ValueType2, double>::value)
            {
                result.set(static_cast<ValueType2>(value) /
                                          static_cast<ValueType2>(-multiplier));
            }
            else
            {
                if (sizeof(ValueType) > sizeof(ValueType2))
                {
                    result.set(static_cast<ValueType>(value) /
                                           static_cast<ValueType>(-multiplier));
                }
                else
                {
                    result.set(static_cast<ValueType2>(
                                 value / static_cast<ValueType2>(-multiplier)));
                }
            }

            // Conversion to less precise units can't roll over
            rollOverCount = 0;
        }

        return result;
    }
};

}; // namespace Plat4m

#endif // PLAT4M_QUANTITY_H
