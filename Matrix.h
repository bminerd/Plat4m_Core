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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file Matrix.h
/// @author Ben Minerd
/// @date 2/17/2016
/// @brief Matrix class header file.
///

#ifndef PLAT4M_MATH_MATRIX_H
#define PLAT4M_MATH_MATRIX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <MatrixBase.h>

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

///
/// @brief Class that inherits from MatrixBase to provide a general N x M matrix
/// implementation.
/// @tparam ValueType Type of value to be stored in this matrix (ex. double,
/// float, uint32_t, etc.).
/// @tparam N Number of matrix rows.
/// @tparam M Number of matrix columns.
///
template <typename ValueType, uint32_t N, uint32_t M>
class Matrix : public MatrixBase<ValueType, N, M>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Matrix() :
        MatrixBase<ValueType, N, M>()
    {
    }

    //--------------------------------------------------------------------------
    Matrix(const ValueType values[N][M]) :
        MatrixBase<ValueType, N, M>(values)
    {
    }

    //--------------------------------------------------------------------------
    // Public overloaded operators
    //--------------------------------------------------------------------------

    // Assignment operator
    //--------------------------------------------------------------------------
    Matrix<ValueType, N, M>& operator=(
                                      const MatrixBase<ValueType, N, M>& matrix)
    {
        MatrixBase<ValueType, N, M>::operator=(matrix);

        return (*this);
    }
};

// Add other Matrix definitions for 2x2, 3x3, etc.

template <typename ValueType, uint32_t N>
class Matrix<ValueType, N, N> : public MatrixBase<ValueType, N, N>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Matrix() :
        MatrixBase<ValueType, N, N>()
    {
    }

    //--------------------------------------------------------------------------
    Matrix(const ValueType values[N][N]) :
        MatrixBase<ValueType, N, N>(values)
    {
    }

    //--------------------------------------------------------------------------
    Matrix(const MatrixBase<ValueType, N, N>& matrix) :
        MatrixBase<ValueType, N, N>(matrix)
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ValueType determinant()
    {
        ValueType result;

        return result;
    }

    //--------------------------------------------------------------------------
    // Public overloaded operators
    //--------------------------------------------------------------------------

    // Assignment operator
    //--------------------------------------------------------------------------
    Matrix<ValueType, N, N>& operator=(
                                      const MatrixBase<ValueType, N, N>& matrix)
    {
        MatrixBase<ValueType, N, N>::operator=(matrix);

        return (*this);
    }

    // Assignment operator
    //--------------------------------------------------------------------------
    Matrix<ValueType, N, N>& operator=(MatrixBase<ValueType, N, N>& matrix)
    {
        MatrixBase<ValueType, N, N>::operator=(matrix);

        return (*this);
    }
};

}; // namespace Math

}; // namespace Plat4m

#endif // PLAT4M_MATH_MATRIX_H
