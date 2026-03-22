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
/// @file ByteByteBuffer.h
/// @author Ben Minerd
/// @date 12/27/2013
/// @brief ByteBuffer class header file.
///

#ifndef PLAT4M_BYTE_BUFFER_H
#define PLAT4M_BYTE_BUFFER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>
#include <cstring>

#include <Plat4m_Core/Array.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ByteBuffer
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ByteBuffer();

    ByteBuffer(std::uint8_t* bytes,
               const std::uint32_t nItems,
               const std::uint32_t itemSizeBytes);

    ByteBuffer(const ByteBuffer& buffer);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    std::uint8_t* getBytes() const;

    std::uint32_t getSize() const;

    std::uint32_t getWriteIndex() const;

    std::uint32_t getReadIndex() const;

    std::uint32_t count() const;

    bool isEmpty() const;

    bool isFull() const;

    bool write(const void* item);

    bool read(void* item);

    bool emptyRead();

    bool peek(void*& item);

    void clear();

    void setBytes(std::uint8_t* bytes,
                  const std::uint32_t nItems,
                  const std::uint32_t itemSizeBytes,
                  const bool bufferItems = false);

    void setCount(const std::uint32_t count);

    void setWriteIndex(const std::uint32_t writeIndex);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    std::uint8_t* myBytes;

    std::uint32_t myNItems;

    std::uint32_t myItemSizeBytes;

    std::uint32_t myWriteIndex;

    std::uint32_t myReadIndex;

    std::uint32_t myCount;

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    inline void writePrivate(const void* item)
    {
        std::memcpy(&(myBytes[(myWriteIndex * myItemSizeBytes)]),
                    item,
                    myItemSizeBytes);

        myWriteIndex++;

        myCount++;

        if (myWriteIndex >= myNItems)
        {
            myWriteIndex = 0;
        }
    }

    //--------------------------------------------------------------------------
    inline void readPrivate(void* item)
    {
        std::memcpy(item,
                    &(myBytes[(myReadIndex * myItemSizeBytes)]),
                    myItemSizeBytes);

        updateReadIndex();
    }

    //--------------------------------------------------------------------------
    inline void updateReadIndex()
    {
        myReadIndex++;

        myCount--;

        if (myReadIndex >= myNItems)
        {
            myReadIndex = 0;
        }
    }
};

}; // namespace Plat4m

#endif // PLAT4M_BYTE_BUFFER_H
