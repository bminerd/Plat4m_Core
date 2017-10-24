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
/// @file ByteArray.cpp
/// @author Ben Minerd
/// @date 6/17/13
/// @brief ByteArray class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ByteArray.h>
#include <string.h>

using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteArray::ByteArray() :
    Array<uint8_t>()
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(uint8_t bytes[],
                     const uint32_t nBytes,
                     const int32_t nUsedBytes) :
    Array<uint8_t>(bytes, nBytes, nUsedBytes)
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(const char string[]) :
    Array<uint8_t>((uint8_t*) string, strlen(string), strlen(string))
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ByteArray::append(const char* string, const bool greedy)
{
    return Array<uint8_t>::append((uint8_t*) string, strlen(string), greedy);
}

//------------------------------------------------------------------------------
bool ByteArray::append(const ByteArray& byteArray, const bool greedy)
{
    return Array<uint8_t>::append(byteArray, greedy);
}

//------------------------------------------------------------------------------
bool ByteArray::prepend(const char string[])
{
    return Array<uint8_t>::prepend((uint8_t*) string, strlen(string));
}

//------------------------------------------------------------------------------
bool ByteArray::insert(const char string[], const unsigned int index)
{
    return Array<uint8_t>::insert((uint8_t*) string, strlen(string), index);
}

//------------------------------------------------------------------------------
void ByteArray::clear(const bool clearMemory)
{
    Array<uint8_t>::clear();

    if (clearMemory)
    {
        memset(getItems(), 0x00, getMaxSize());
    }
}

//------------------------------------------------------------------------------
void ByteArray::setValue(const uint8_t value)
{
    memset(getItems(), value, getMaxSize());
    setSize(getMaxSize());
}
