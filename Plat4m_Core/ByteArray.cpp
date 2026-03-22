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
/// @file ByteArray.cpp
/// @author Ben Minerd
/// @date 6/17/2013
/// @brief ByteArray class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstring>

#include <Plat4m_Core/ByteArray.h>

using Plat4m::ByteArray;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteArray::ByteArray() :
    Array<std::uint8_t>()
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(std::uint8_t bytes[],
                     const std::uint32_t nBytes,
                     const std::int32_t nUsedBytes) :
    Array<std::uint8_t>(bytes, nBytes, nUsedBytes)
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(const ByteArray& byteArray) :
    Array<std::uint8_t>(byteArray.getData(),
                        byteArray.getMaxSize(),
                        byteArray.getSize())
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(const Array<std::uint8_t>& array) :
    Array<std::uint8_t>(array)
{
}

//------------------------------------------------------------------------------
ByteArray::ByteArray(const char* string) :
    Array<std::uint8_t>(
                     reinterpret_cast<std::uint8_t*>(const_cast<char*>(string)),
                     std::strlen(string),
                     std::strlen(string))
{
}

//------------------------------------------------------------------------------
// Public operator overloads
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteArray& ByteArray::operator=(const ByteArray& byteArray)
{
    Array<std::uint8_t>::operator=(byteArray);

    return (*this);
}

//------------------------------------------------------------------------------
bool ByteArray::operator==(const ByteArray& byteArray) const
{
    return ((getSize()    == byteArray.getSize())    &&
            (getMaxSize() == byteArray.getMaxSize()) &&
            (std::memcmp(getData(), byteArray.getData(), getSize()) == 0));
}

//------------------------------------------------------------------------------
bool ByteArray::operator!=(const ByteArray& byteArray) const
{
    return (!(operator==(byteArray)));
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ByteArray::append(const char* string, const bool greedy)
{
    return Array<std::uint8_t>::append(
                                  reinterpret_cast<const std::uint8_t*>(string),
                                  static_cast<std::uint32_t>(strlen(string)),
                                  greedy);
}

//------------------------------------------------------------------------------
bool ByteArray::append(const ByteArray& byteArray, const bool greedy)
{
    return Array<std::uint8_t>::append(byteArray, greedy);
}

//------------------------------------------------------------------------------
bool ByteArray::prepend(const char string[])
{
    return Array<std::uint8_t>::prepend(
                                  reinterpret_cast<const std::uint8_t*>(string),
                                  static_cast<std::uint32_t>(strlen(string)));
}

//------------------------------------------------------------------------------
bool ByteArray::insert(const char string[], const std::uint32_t index)
{
    return Array<std::uint8_t>::insert(
                                  reinterpret_cast<const std::uint8_t*>(string),
                                  static_cast<std::uint32_t>(strlen(string)),
                                  index);
}

//------------------------------------------------------------------------------
void ByteArray::clear(const bool clearMemory)
{
    Array<std::uint8_t>::clear();

    if (clearMemory)
    {
        memset(getItems(), 0x00, getMaxSize());
    }
}

//------------------------------------------------------------------------------
void ByteArray::setValue(const std::uint8_t value)
{
    memset(getItems(), value, getMaxSize());
    setSize(getMaxSize());
}
