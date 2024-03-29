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
// Copyright (c) 2013 Benjamin Minerd
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
/// @file ArrayN.h
/// @author Ben Minerd
/// @date 6/25/2013
/// @brief ArrayN template class header file.
///

#ifndef PLAT4M_ARRAY_N_H
#define PLAT4M_ARRAY_N_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <string.h>

#include <Plat4m_Core/Array.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename T, uint32_t N>
class ArrayN : public Array<T>
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors and destructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    ArrayN(const bool constructFull = false) :
        Array<T>(myItems, N, (constructFull ? N : 0)),
        myItems()
    {
    }
    
    //--------------------------------------------------------------------------
    ArrayN(const ArrayN<T, N>& arrayN) :
        Array<T>(myItems, N, arrayN.getSize()),
        myItems()
    {
        memcpy(myItems, arrayN.myItems, arrayN.getSize());
    }

    //--------------------------------------------------------------------------
    // Public operator overloads
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ArrayN<T, N>& operator=(const ArrayN<T, N>& arrayN)
    {
        Array<T>::setSize(arrayN.getSize());

        memcpy(myItems, arrayN.myItems, (arrayN.getSize() * sizeof(T)));

        return (*this);
    }

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    T myItems[N];
};

}; // namespace Plat4m

#endif // PLAT4M_ARRAY_N_H
