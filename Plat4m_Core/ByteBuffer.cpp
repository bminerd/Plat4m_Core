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
// Copyright (c) 2024 Benjamin Minerd
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
/// @file ByteBuffer.cpp
/// @author Ben Minerd
/// @date 12/27/2023
/// @brief ByteBuffer class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <string.h>

#include <Plat4m_Core/ByteBuffer.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer() :
    myBytes(0),
    myNItems(0),
    myWriteIndex(0),
    myReadIndex(0),
    myCount(0)
{
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(std::uint8_t* bytes,
                       const std::uint32_t nItems,
                       const std::uint32_t itemSizeBytes) :
    myBytes(bytes),
    myNItems(nItems),
    myItemSizeBytes(itemSizeBytes),
    myWriteIndex(0),
    myReadIndex(0),
    myCount(0)
{
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const ByteBuffer& buffer) :
    myBytes(buffer.myBytes),
    myNItems(buffer.myNItems),
    myItemSizeBytes(buffer.myItemSizeBytes),
    myWriteIndex(buffer.myWriteIndex),
    myReadIndex(buffer.myReadIndex),
    myCount(buffer.myCount)
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::uint8_t* ByteBuffer::getBytes() const
{
    return myBytes;
}

//------------------------------------------------------------------------------
std::uint32_t ByteBuffer::getSize() const
{
    return myNItems;
}

//------------------------------------------------------------------------------
std::uint32_t ByteBuffer::getWriteIndex() const
{
    return myWriteIndex;
}

//------------------------------------------------------------------------------
std::uint32_t ByteBuffer::getReadIndex() const
{
    return myReadIndex;
}

//------------------------------------------------------------------------------
std::uint32_t ByteBuffer::count() const
{
    return myCount;
}

//------------------------------------------------------------------------------
bool ByteBuffer::isEmpty() const
{
    return (myCount == 0);
}

//------------------------------------------------------------------------------
bool ByteBuffer::isFull() const
{
    return (myCount >= myNItems);
}

//------------------------------------------------------------------------------
bool ByteBuffer::write(const void* item)
{
    if (isFull())
    {
        return false;
    }

    writePrivate(item);

    return true;
}

//------------------------------------------------------------------------------
bool ByteBuffer::read(void* item)
{
    if (isEmpty())
    {
        return false;
    }

    readPrivate(item);

    return true;
}

//------------------------------------------------------------------------------
bool ByteBuffer::emptyRead()
{
    updateReadIndex();

    return true;
}

//------------------------------------------------------------------------------
bool ByteBuffer::peek(void*& item)
{
    if (isEmpty())
    {
        item = 0;

        return false;
    }

    item = &(myBytes[(myReadIndex * myItemSizeBytes)]);

    return true;
}

//------------------------------------------------------------------------------
void ByteBuffer::clear()
{
    myWriteIndex = 0;
    myReadIndex  = 0;
    myCount      = 0;
}

//------------------------------------------------------------------------------
void ByteBuffer::setBytes(std::uint8_t* bytes,
                          const std::uint32_t nItems,
                          const std::uint32_t itemSizeBytes,
                          const bool bufferItems)
{
    myBytes         = bytes;
    myNItems        = nItems;
    myItemSizeBytes = itemSizeBytes;
    myWriteIndex    = 0;
    myReadIndex     = 0;

    if (bufferItems)
    {
        myCount = nItems;
    }
    else
    {
        myCount = 0;
    }
}

//------------------------------------------------------------------------------
void ByteBuffer::setCount(const std::uint32_t count)
{
    myCount = count;
}

//------------------------------------------------------------------------------
void ByteBuffer::setWriteIndex(const std::uint32_t writeIndex)
{
    myWriteIndex = writeIndex;
}
