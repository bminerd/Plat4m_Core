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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file Vector.h
/// @author Ben Minerd
/// @date 2/18/2016
/// @brief Vector class header file.
///

#ifndef VECTOR_H
#define VECTOR_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Matrix.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

namespace Math
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType, uint32_t N>
class Vector : public MatrixBase<ValueType, N, 1>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    Vector() :
        MatrixBase<ValueType, N, 1>()
    {
    }

    //--------------------------------------------------------------------------
    Vector(const ValueType values[N]) :
        MatrixBase<ValueType, N, 1>()
    {
        MatrixBase<ValueType, N, 1>::setValuesProtected(values);
    }

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ~Vector()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    // Assignment operator
    //--------------------------------------------------------------------------
    Vector<ValueType, N>& operator=(const MatrixBase<ValueType, N, 1>& matrix)
    {
        MatrixBase<ValueType, N, 1>::operator=(matrix);

        return (*this);
    }

    //--------------------------------------------------------------------------
    ValueType& operator()(const uint32_t row)
    {
        return (MatrixBase<ValueType, N, 1>::getValue(row, 0));
    }

    //--------------------------------------------------------------------------
    const ValueType& operator()(const uint32_t row) const
    {
        return (MatrixBase<ValueType, N, 1>::getValue(row, 0));
    }
};

}; // namespace Math

}; // namespace Plat4m

#endif // VECTOR_H
