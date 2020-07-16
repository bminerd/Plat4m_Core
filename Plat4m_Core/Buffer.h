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
/// @file Buffer.h
/// @author Ben Minerd
/// @date 3/25/2013
/// @brief Buffer class header file.
///

#ifndef PLAT4M_BUFFER_H
#define PLAT4M_BUFFER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>
#include <Plat4m_Core/Array.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename T>
class Buffer
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Buffer() :
        myItems(0),
        myNItems(0),
        myWriteIndex(0),
        myReadIndex(0),
        myCount(0)
    {
    }
    
    //--------------------------------------------------------------------------
    Buffer(T* items, const uint32_t nItems) :
        myItems(items),
        myNItems(nItems),
        myWriteIndex(0),
        myReadIndex(0),
        myCount(0)
    {
    }
    
    //--------------------------------------------------------------------------
    Buffer(const Buffer& buffer) :
        myItems(buffer.myItems),
        myNItems(buffer.myNItems),
        myWriteIndex(buffer.myWriteIndex),
        myReadIndex(buffer.myReadIndex),
        myCount(buffer.myCount)
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    T* getItems() const
    {
        return myItems;
    }

    //--------------------------------------------------------------------------
    uint32_t getSize() const
    {
        return myNItems;
    }

    //--------------------------------------------------------------------------
    uint32_t getWriteIndex() const
    {
        return myWriteIndex;
    }

    //--------------------------------------------------------------------------
    uint32_t getReadIndex() const
    {
        return myReadIndex;
    }

    //--------------------------------------------------------------------------
    uint32_t count()
    {
        return myCount;
    }

    //--------------------------------------------------------------------------
    bool isEmpty()
    {
        return (myCount == 0);
    }

    //--------------------------------------------------------------------------
    bool isFull()
    {
        return (myCount >= myNItems);
    }

    //--------------------------------------------------------------------------
    bool write(const T& element)
    {
        if (isFull())
        {
            return false;
        }
        
        writePrivate(element);
        
        return true;
    }

    //--------------------------------------------------------------------------
    bool write(const Array<T>& array)
    {
    	uint32_t size = array.getSize();

        if ((myCount + size) > myNItems)
        {
            return false;
        }

        for (uint32_t i = 0; i < size; i++)
        {
            writePrivate(array[i]);
        }

        return true;
    }

    //--------------------------------------------------------------------------
    bool read(T& element)
    {
        if (isEmpty())
        {
            return false;
        }
        
        element = readPrivate();
        
        return true;
    }

    //--------------------------------------------------------------------------
    bool read(Array<T>& array)
    {
        uint32_t maxSize = array.getMaxSize();
        uint32_t size = array.getSize();
        uint32_t nItemsToRead = maxSize - size;

        if (nItemsToRead > myCount)
        {
            nItemsToRead = myCount;
        }

        for (uint32_t i = 0; i < nItemsToRead; i++)
        {
            array.append(readPrivate());
        }

        return true;
    }

    //--------------------------------------------------------------------------
    bool peek(T*& element)
    {
        if (isEmpty())
        {
            element = 0;
            
            return false;
        }

        element = &(myItems[myReadIndex]);

        return true;
    }
    
    //--------------------------------------------------------------------------
    bool emptyRead()
    {
        if (isEmpty())
        {
            return false;
        }

        readPrivate();

        return true;
    }

    //--------------------------------------------------------------------------
    void clear()
    {
        myWriteIndex = 0;
        myReadIndex  = 0;
        myCount      = 0;
    }

    //--------------------------------------------------------------------------
    void setItems(T* items,
                  const uint32_t nItems,
                  const bool bufferItems = false)
    {
        myItems      = items;
        myNItems     = nItems;
        myWriteIndex = 0;
        myReadIndex  = 0;

        if (bufferItems)
        {
            myCount = nItems;
        }
        else
        {
            myCount = 0;
        }
    }

    //--------------------------------------------------------------------------
    void setCount(const uint32_t count)
    {
    	myCount = count;
    }

    //--------------------------------------------------------------------------
    void setWriteIndex(const uint32_t writeIndex)
    {
    	myWriteIndex = writeIndex;
    }

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    T* myItems;

    uint32_t myNItems;
    
    uint32_t myWriteIndex;
    
    uint32_t myReadIndex;

    uint32_t myCount;

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    inline void writePrivate(const T& element)
    {
        myItems[myWriteIndex++] = element;

        myCount++;

        if (myWriteIndex >= myNItems)
        {
            myWriteIndex = 0;
        }
    }

    //--------------------------------------------------------------------------
    inline T& readPrivate()
    {
        T& element = myItems[myReadIndex++];

        myCount--;

        if (myReadIndex >= myNItems)
        {
            myReadIndex = 0;
        }

        return element;
    }
};

}; // namespace Plat4m

#endif // PLAT4M_BUFFER_H
