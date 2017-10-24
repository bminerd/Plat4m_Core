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
/// @file MatrixBase.h
/// @author Ben Minerd
/// @date 2/18/2016
/// @brief MatrixBase class header file.
///

#ifndef PLAT4M_MATH_MATRIX_BASE_H
#define PLAT4M_MATH_MATRIX_BASE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>

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
/// @brief Base class for all matrix-based classes. Specializations (ex. 3x3
/// matrices) can be added by inheriting from this class.
/// @tparam ValueType Type of value to be stored in this matrix (ex. double,
/// float, uint32_t, etc.).
/// @tparam N Number of matrix rows.
/// @tparam M Number of matrix columns.
/// @note Currently supports only row-major matrices.
/// @note The underlying array indexing is done by incrementing pointers.
/// Normally this approach is avoided in Plat4m due to poor readability, but the
/// performance gains are ~10x versus using for-loops and standard indexing.
///
template <typename ValueType, uint32_t N, uint32_t M>
class MatrixBase
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    MatrixBase() :
        myValues()
    {
    }

    //--------------------------------------------------------------------------
    MatrixBase(const ValueType values[N][M]) :
        myValues()
    {
        setValuesProtected(values);
    }

    //--------------------------------------------------------------------------
    MatrixBase(const MatrixBase& matrix) :
        myValues()
    {
        setStaticValuesProtected(matrix.myValues);
    }

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ~MatrixBase()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ValueType& getValue(const uint32_t row, const uint32_t column)
    {
        ValueType* value = 0;

        if ((row <= N) && (column <= M))
        {
            value = &getValueFast(row, column);
        }

        return (*value);
    }

    //
    //--------------------------------------------------------------------------
    const ValueType& getValue(const uint32_t row, const uint32_t column) const
    {
        ValueType* value = 0;

        if ((row <= N) && (column <= M))
        {
            value = &getValueFast(row, column);
        }

        return (*value);
    }

    ///
    /// @brief Gets the value at (row, column) of this matrix.
    /// @param row Row of value to get.
    /// @param row Column of value to get.
    /// @return Reference to the value at (row, column).
    /// @note This method doesn't perform any index error checking and is meant
    /// for internal use only. The method getValue() should be used instead.
    ///
    //--------------------------------------------------------------------------
    ValueType& getValueFast(const uint32_t row, const uint32_t column)
    {
        return (myValues[row][column]);
    }

    ///
    /// @brief Gets the value at (row, column) of this matrix.
    /// @param row Row of value to get.
    /// @param row Column of value to get.
    /// @return Constant reference to the value at (row, column).
    /// @note This method doesn't perform any index error checking and is meant
    /// for internal use only. The method getValue() should be used instead.
    ///
    //--------------------------------------------------------------------------
    const ValueType& getValueFast(const uint32_t row,
                                  const uint32_t column) const
    {
        return (myValues[row][column]);
    }

    //--------------------------------------------------------------------------
    void setValue(const uint32_t row,
                  const uint32_t column,
                  const ValueType value)
    {
        myValues[row][column] = value;
    }

    //--------------------------------------------------------------------------
    void setValues(const ValueType value)
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*myValuePointer++) = value;
        }
    }

    //--------------------------------------------------------------------------
    void setValues(const ValueType values[N][M])
    {
        setValuesProtected(values);
    }

    //--------------------------------------------------------------------------
    void setStaticValues(const ValueType values[N][M])
    {
        setStaticValuesProtected(values);
    }

    //--------------------------------------------------------------------------
    MatrixBase<ValueType, M, N> T()
    {
        return transpose();
    }

    //--------------------------------------------------------------------------
    MatrixBase<ValueType, M, N> transpose()
    {
        MatrixBase<ValueType, M, N> matrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = 0;

        while (i != (N * M))
        {
            matrix.getValueFast((i % M), (i / M)) = (*myValuePointer++);

            i++;
        }

        return matrix;
    }

    //--------------------------------------------------------------------------
    ValueType maxValue() const
    {
        const ValueType* myValuePointer = &(getValueFast(0, 0));
        ValueType maxValue = (*myValuePointer);

        int32_t i = N * M;

        while (i--)
        {
            if ((*myValuePointer) > maxValue)
            {
                maxValue = (*myValuePointer);
            }

            myValuePointer++;
        }

        return maxValue;
    }

    //--------------------------------------------------------------------------
    ValueType minValue() const
    {
        const ValueType* myValuePointer = &(getValueFast(0, 0));
        ValueType minValue = (*myValuePointer);

        int32_t i = N * M;

        while (i--)
        {
            if ((*myValuePointer) < minValue)
            {
                minValue = (*myValuePointer);
            }

            myValuePointer++;
        }

        return minValue;
    }

    //--------------------------------------------------------------------------
    void limitValues(const ValueType lowerLimitValue,
                     const ValueType upperLimitValue)
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            if ((*myValuePointer) < lowerLimitValue)
            {
                (*myValuePointer) = lowerLimitValue;
            }
            else if ((*myValuePointer) > upperLimitValue)
            {
                (*myValuePointer) = upperLimitValue;
            }

            myValuePointer++;
        }
    }

    //--------------------------------------------------------------------------
    void limitUpperValue(const ValueType limitValue)
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            if ((*myValuePointer) > limitValue)
            {
                (*myValuePointer) = limitValue;
            }

            myValuePointer++;
        }
    }

    //--------------------------------------------------------------------------
    void limitLowerValue(const ValueType limitValue)
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            if ((*myValuePointer) < limitValue)
            {
                (*myValuePointer) = limitValue;
            }

            myValuePointer++;
        }
    }

    //--------------------------------------------------------------------------
    // Public overloaded operators
    //--------------------------------------------------------------------------

    // Assignment operator
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M>& operator=(
                                      const MatrixBase<ValueType, N, M>& matrix)
    {
        setStaticValuesProtected(matrix.myValues);

        return (*this);
    }

    //
    //--------------------------------------------------------------------------
    ValueType& operator()(const uint32_t row, const uint32_t column)
    {
        return getValue(row, column);
    }

    //
    //--------------------------------------------------------------------------
    const ValueType& operator()(const uint32_t row, const uint32_t column) const
    {
        return getValue(row, column);
    }

    // Unary plus operator
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator+() const
    {
        MatrixBase<ValueType, N, M> matrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));
        ValueType* valuePointer = &(matrix.getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*valuePointer++) = +(*myValuePointer++);
        }

        return matrix;
    }
    
    // Unary minus operator
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator-() const
    {
        MatrixBase<ValueType, N, M> matrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));
        ValueType* valuePointer = &(matrix.getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*valuePointer++) = -(*myValuePointer++);
        }

        return matrix;
    }

    // Addition operator
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator+(
                                const MatrixBase<ValueType, N, M>& matrix) const
    {
        MatrixBase<ValueType, N, M> resultMatrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));
        const ValueType* valuePointer = &(matrix.getValueFast(0, 0));
        ValueType* resultValuePointer = &(resultMatrix.getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*resultValuePointer++) = (*myValuePointer++) + (*valuePointer++);
        }

        return resultMatrix;
    }

    // Subtraction operator
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator-(
                                const MatrixBase<ValueType, N, M>& matrix) const
    {
        MatrixBase<ValueType, N, M> resultMatrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));
        const ValueType* valuePointer = &(matrix.getValueFast(0, 0));
        ValueType* resultValuePointer = &(resultMatrix.getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*resultValuePointer++) = (*myValuePointer++) - (*valuePointer++);
        }

        return resultMatrix;
    }

    // Multiplication operator
    //
    // Format is:
    //
    //           (left)   (right)
    // result  =  this  *  matrix
    //
    // Algorithm:
    //
    // Loop through every element in the resulting N * M2 matrix. For every
    // element, perform the sum of the current row of the left matrix times the
    // current column of the right matrix.
    //
    //     result      =        this     *    matrix
    //
    //       M2                   M            M2
    //   |C11 C12 C13|      |A11 A12 A13| |B11 B12 B13|
    // N |C21 C22 C23| =  N |A21 A22 A23| |B21 B22 B23| N2 = M
    //   |C31 C32 C33|      |A31 A32 A33| |B31 B32 B33|
    //
    //    result   =       this   *    matrix
    //
    //       M2              M           M2
    //   |C11 C12|      |A11 A12 A13| |B11 B12|
    // N |C21 C22| =  N |A21 A22 A23| |B21 B22| N2 = M
    //                                |B31 B32|
    //
    // result =      this * matrix
    //
    //    M2           M      M2
    //   |C1|      |A11 A12| |B1|
    // N |C2| =  N |A21 A22| |B2| N2 = M
    //
    //--------------------------------------------------------------------------
    template <unsigned int M2>
    MatrixBase<ValueType, N, M2> operator*(
                               const MatrixBase<ValueType, M, M2>& matrix) const
    {
        MatrixBase<ValueType, N, M2> resultMatrix;

        const ValueType* leftPointer  = &(getValueFast(0, 0));
        const ValueType* rightPointer = &(matrix.getValueFast(0, 0));
        ValueType* resultPointer = &(resultMatrix.getValueFast(0, 0));

        int32_t i = 0;

        // Loop through every element (N * M2) in the resulting Matrix starting
        // at (1, 1) moving left-to-right, top-to-bottom until (N, M2)
        while (i++ < (N * M2))
        {
            int32_t rowSize = M;

            // Loop
            while (rowSize--)
            {
                (*resultPointer) += (*leftPointer++) * (*rightPointer);

                rightPointer += M2;
            }

            resultPointer++;

            //
            // Integer divide i / M2 gives the first element in the current row
            // of the left matrix
            //
            leftPointer = &(getValueFast((i / M2), 0));

            //
            // Integer modulo i % M2 gives the first element in the current
            // column of the right matrix
            //
            rightPointer = &(matrix.getValueFast(0, (i % M2)));
        }

        return resultMatrix;
    }

    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, 1> operator*(
                               const MatrixBase<ValueType, M, 1>& matrix) const
    {
        MatrixBase<ValueType, N, 1> resultMatrix;

        const ValueType* leftPointer  = &(getValueFast(0, 0));
        const ValueType* rightPointer = &(matrix.getValueFast(0, 0));
        ValueType* resultPointer = &(resultMatrix.getValueFast(0, 0));

        int32_t i = N;

        // Loop through every element (N) in the resulting Vector starting
        // at (1) moving top-to-bottom until (N)
        while (i--)
        {
            int32_t rowSize = M;

            // Loop
            while (rowSize--)
            {
                (*resultPointer) += (*leftPointer++) * (*rightPointer++);
            }

            resultPointer++;

            //
            // First element in the right matrix
            //
            rightPointer = &(matrix.getValueFast(0, 0));
        }

        return resultMatrix;
    }

    // Addition operator (scalar)
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator+(const ValueType scalar) const
    {
        MatrixBase<ValueType, N, M> matrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));
        ValueType* valuePointer = &(matrix.getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*valuePointer++) = (*myValuePointer++) + scalar;
        }

        return matrix;
    }

    // Addition-equals operator (scalar)
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M>& operator+=(const ValueType scalar)
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*myValuePointer++) += scalar;
        }

        return (*this);
    }

    // Subtraction operator (scalar)
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator-(const ValueType scalar) const
    {
        MatrixBase<ValueType, N, M> matrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));
        ValueType* valuePointer = &(matrix.getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*valuePointer++) = (*myValuePointer++) - scalar;
        }

        return matrix;
    }

    // Subtraction-equals operator (scalar)
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M>& operator-=(const ValueType scalar)
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*myValuePointer++) -= scalar;
        }

        return (*this);
    }

    // Multiplication operator (scalar)
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator*(const ValueType scalar) const
    {
        MatrixBase<ValueType, N, M> matrix;

        const ValueType* myValuePointer = &(getValueFast(0, 0));
        ValueType* valuePointer = &(matrix.getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*valuePointer++) = (*myValuePointer++) * scalar;
        }

        return matrix;
    }

    // Multiplication-equals operator (scalar)
    //--------------------------------------------------------------------------
    MatrixBase<ValueType, N, M> operator*=(const ValueType scalar)
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));

        int32_t i = N * M;

        while (i--)
        {
            (*myValuePointer++) *= scalar;
        }

        return (*this);
    }

protected:

    //--------------------------------------------------------------------------
    inline void setValuesProtected(const ValueType values[][M])
    {
        int32_t i;
        int32_t j;

        for (i = 0; i < N; i++)
        {
            for (j = 0; j < M; j++)
            {
                myValues[i][j] = values[i][j];
            }
        }
    }

    //--------------------------------------------------------------------------
    inline void setValuesProtected(const ValueType values[N])
    {
        int32_t i;

        for (i = 0; i < N; i++)
        {
            myValues[i][0] = values[i];
        }
    }

    //--------------------------------------------------------------------------
    inline void setStaticValuesProtected(const ValueType values[][M])
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));
        const ValueType* valuePointer = &(values[0][0]);

        int32_t i = N * M;

        while (i--)
        {
            (*myValuePointer++) = (*valuePointer++);
        }
    }

    //--------------------------------------------------------------------------
    inline void setStaticValuesProtected(const ValueType values[N])
    {
        ValueType* myValuePointer = &(getValueFast(0, 0));
        const ValueType* valuePointer = &(values[0]);

        int32_t i = N * M;

        while (i--)
        {
            (*myValuePointer++) = (*valuePointer++);
        }
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ValueType myValues[N][M];
};

}; // namespace Math

}; // namespace Plat4m

#endif // PLAT4M_MATH_MATRIX_BASE_H
