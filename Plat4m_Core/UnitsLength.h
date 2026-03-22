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
/// @file UnitsLength.h
/// @author Ben Minerd
/// @date 11/7/2023
/// @brief UnitsLength class header file.
///

#ifndef PLAT4M_UNITS_LENGTH_H
#define PLAT4M_UNITS_LENGTH_H

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

class Length
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Type
    {
        MILLIMETERS = 0,
        MM = MILLIMETERS,
        CENTIMETERS,
        CM = CENTIMETERS,
        METERS,
        M = METERS,
        KILOMETERS,
        KM = KILOMETERS
    };

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static constexpr std::int32_t multiplier(const Type fromType,
                                              const Type toType)
    {
        return (myMultiplierTable2[fromType][toType]);
    }

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Rows are from, columns are to (i.e. table[from][to])
    static constexpr std::int32_t myMultiplierTable2[4][4] =
    {
        //To: MM    CM      M       KM |  From:
        {      1,  -10, -1000, -100000}, // MM
        {     10,    1,   -10,   -1000}, // CM
        {   1000,   10,     1,     -10}, // M
        { 100000, 1000,   100,       1}  // KM
    };
};

}; // namespace Units

}; // namespace Plat4m

#endif // PLAT4M_UNITS_LENGTH_H
