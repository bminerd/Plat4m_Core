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
/// @file UnitsFrequency.h
/// @author Ben Minerd
/// @date 11/7/2023
/// @brief Units::Frequency class header file.
///

#ifndef PLAT4M_UNITS_FREQUENCY_H
#define PLAT4M_UNITS_FREQUENCY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

namespace Units
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Frequency
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Type
    {
        HERTZ = 0,
        HZ = HERTZ,
        KILOHERTZ,
        KHZ = KILOHERTZ,
        MEGAHERTZ,
        MHZ = MEGAHERTZ,
        GIGAHERTZ,
        GHZ = GIGAHERTZ
    };

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static constexpr std::int32_t multiplier(const Type fromType,
                                             const Type toType)
    {
        return (myMultiplierTable[fromType][toType]);
    }

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Rows are from, columns are to (i.e. table[from][to])
    static constexpr std::int32_t myMultiplierTable[4][4] =
    {
        // To:    HZ      KHZ       MHZ          GHZ |  From:
        {          1,   -1000, -1000000, -1000000000}, // HZ
        {       1000,       1,    -1000,    -1000000}, // KHZ
        {    1000000,    1000,        1,       -1000}, // MHZ
        { 1000000000, 1000000,     1000,           1}  // GHZ
    };
};

}; // namespace Units

}; // namespace Plat4m

#endif // PLAT4M_UNITS_FREQUENCY_H
