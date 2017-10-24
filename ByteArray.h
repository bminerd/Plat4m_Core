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
/// @file ByteArray.h
/// @author Ben Minerd
/// @date 6/17/13
/// @brief ByteArray class header file.
///

#ifndef PLAT4M_BYTE_ARRAY_H
#define PLAT4M_BYTE_ARRAY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Array.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ByteArray : public Array<uint8_t>
{
public:

    //--------------------------------------------------------------------------
    // Public constructors and destructors
    //--------------------------------------------------------------------------
    
    ByteArray();
    
    ByteArray(uint8_t bytes[],
              const uint32_t nBytes,
              const int32_t nUsedBytes = -1);
    
    ByteArray(const char string[]);

    template <typename TData>
    ByteArray(const Array<TData>& array) :
        Array<uint8_t>((uint8_t*) array.getItems(),
                       array.getMaxSize() * sizeof(TData),
                       array.getSize() * sizeof(TData))
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    using Array<uint8_t>::append;
    using Array<uint8_t>::prepend;
    using Array<uint8_t>::insert;
    using Array<uint8_t>::clear;

    //--------------------------------------------------------------------------
    template <typename T>
    bool append(T item, const Endian endian, const bool greedy = false)
    {
        if (endian == ENDIAN_LITTLE)
        {
            return append((uint8_t*) &item, sizeof(item), greedy);
        }
        else if (endian == ENDIAN_BIG)
        {
            uint8_t* pointer = (uint8_t*) &item;

            for (uint32_t i = 0; i < sizeof(item); i++)
            {
                if (!append((uint8_t) pointer[sizeof(item) - 1 - i]))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool append(const char* string, const bool greedy = false);

    bool append(const ByteArray& byteArray, const bool greedy = false);

    bool prepend(const char string[]);

    bool insert(const char string[], const uint32_t index);

    void clear(const bool clearMemory);

    void setValue(const uint8_t value);


};

}; // namespace Plat4m

#endif // PLAT4M_BYTE_ARRAY_H
