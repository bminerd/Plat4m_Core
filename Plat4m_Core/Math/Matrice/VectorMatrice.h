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
/// @file VectorMatrice.h
/// @author Ben Minerd
/// @date 12/28/2023
/// @brief VectorMatrice class header file.
///

#ifndef PLAT4M_VECTOR_MATRICE_H
#define PLAT4M_VECTOR_MATRICE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Matrice/Matrice.h>
#include <Matrice/Vector.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Aliases
//------------------------------------------------------------------------------

template <typename ValueType, std::uint32_t Rows>
using Vector = Matrice::Vector<ValueType, Rows>;

template <typename ValueType, std::uint32_t Rows>
using VectorInternal =
                    Matrice::Vector<ValueType, Rows, Matrice::STORAGE_INTERNAL>;

template <typename ValueType, std::uint32_t Rows>
using VectorExternal =
                    Matrice::Vector<ValueType, Rows, Matrice::STORAGE_EXTERNAL>;

template <typename ValueType, std::uint32_t Rows>
using VectorConstant =
                    Matrice::Vector<ValueType, Rows, Matrice::STORAGE_CONSTANT>;

}; // namespace Plat4m

#endif // PLAT4M_VECTOR_MATRICE_H
