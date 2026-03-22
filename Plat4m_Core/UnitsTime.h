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
/// @file UnitsTime.h
/// @author Ben Minerd
/// @date 11/7/2023
/// @brief UnitsTime class header file.
///

#ifndef PLAT4M_UNITS_TIME_H
#define PLAT4M_UNITS_TIME_H

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

class Time
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Type
    {
        NANOSECONDS = 0,
        NS = NANOSECONDS,
        MICROSECONDS,
        US = MICROSECONDS,
        MILLISECONDS,
        MS = MILLISECONDS,
        SECONDS,
        S = SECONDS
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
        // To:    NS       US        MS            S |  From:
        {          1,   -1000, -1000000, -1000000000}, // NS
        {       1000,       1,    -1000,    -1000000}, // US
        {    1000000,    1000,        1,       -1000}, // MS
        { 1000000000, 1000000,     1000,           1}  // S
    };
};

}; // namespace Units

}; // namespace Plat4m

#endif // PLAT4M_UNITS_TIME_H
