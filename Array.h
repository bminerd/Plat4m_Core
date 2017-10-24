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
/// @file Array.h
/// @author Ben Minerd
/// @date 6/25/13
/// @brief Array template class header file.
///

#ifndef PLAT4M_ARRAY_H
#define PLAT4M_ARRAY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename T>
class Array
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    Array() :
        myItems(0),
        myNMaxItems(0),
        myNUsedItems(0)
    {
    }

    //--------------------------------------------------------------------------
    Array(T* items, const uint32_t nMaxItems, const int32_t nUsedItems = -1) :
        myItems(items),
        myNMaxItems(nMaxItems),
        myNUsedItems(0)
    {
        if (nUsedItems == -1)
        {
            myNUsedItems = myNMaxItems;
        }
        else
        {
            myNUsedItems = nUsedItems;
        }
    }
    
    //--------------------------------------------------------------------------
    template <uint32_t nMaxItems>
    Array(T (&items)[nMaxItems], const int32_t nUsedItems = -1) :
        myItems(items),
        myNMaxItems(nMaxItems),
        myNUsedItems(0)
    {
        if (nUsedItems == -1)
        {
            myNUsedItems = myNMaxItems;
        }
        else
        {
            myNUsedItems = nUsedItems;
        }
    }
    
    //--------------------------------------------------------------------------
    // Public copy constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Array(const Array<T>& array) :
        myItems(array.myItems),
        myNMaxItems(array.myNMaxItems),
        myNUsedItems(array.myNUsedItems)
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
    T* getData()
    {
        return myItems;
    }
    
    //--------------------------------------------------------------------------
    T& getItem(const uint32_t index) const
    {
        return (myItems[index]);
    }

    //--------------------------------------------------------------------------
    void setItems(T* items, uint32_t nMaxItems, const int32_t nUsedItems = -1)
    {
        myItems      = items;
        myNMaxItems  = nMaxItems;

        if (nUsedItems == -1)
        {
            myNUsedItems = nMaxItems;
        }
        else
        {
            myNUsedItems = nUsedItems;
        }
    }

    //--------------------------------------------------------------------------
    template <uint32_t nMaxItems>
    void setItems(T (&items)[nMaxItems], const int32_t nUsedItems = -1)
    {
        myItems      = items;
        myNMaxItems  = nMaxItems;

        if (nUsedItems == -1)
        {
            myNUsedItems = nMaxItems;
        }
        else
        {
            myNUsedItems = nUsedItems;
        }
    }

    //--------------------------------------------------------------------------
    uint32_t getSize() const
    {
        return myNUsedItems;
    }
    
    //--------------------------------------------------------------------------
    bool setSize(const uint32_t size)
    {
        if (size > myNMaxItems)
        {
            return false;
        }

        myNUsedItems = size;

        return true;
    }

    //--------------------------------------------------------------------------
    uint32_t getMaxSize() const
    {
        return myNMaxItems;
    }
    
    //--------------------------------------------------------------------------
    bool append(const T item)
    {
        if (myNUsedItems == myNMaxItems)
        {
            return false;
        }
        
        myItems[myNUsedItems++] = item;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool append(const T* items,
                const uint32_t nItems,
                const bool greedy = false)
    {
        uint32_t nRemainingItems = myNMaxItems - myNUsedItems;
        uint32_t nItemsToAppend = nItems;

        bool returnValue;

        if (nItemsToAppend > nRemainingItems)
        {
            returnValue = false;

            if (greedy)
            {
                nItemsToAppend = nRemainingItems;
            }
            else
            {
                return returnValue;
            }
        }
        else
        {
            returnValue = true;
        }

        for (uint32_t i = 0; i < nItemsToAppend; i++)
        {
            myItems[myNUsedItems++] = items[i];
        }

        return returnValue;
    }

    //--------------------------------------------------------------------------
    template <uint32_t nItems>
    bool append(const T (&items)[nItems],
                const bool greedy = false)
    {
        return append(items, nItems, greedy);
    }
    
    //--------------------------------------------------------------------------
    bool append(const Array<T>& array, const bool greedy = false)
    {
        return append(array.getItems(), array.getSize(), greedy);
    }
    
    //--------------------------------------------------------------------------
    bool prepend(T item, const bool greedy = false)
    {
        if (myNUsedItems == myNMaxItems)
        {
        	if (greedy)
        	{
        		myNUsedItems = myNMaxItems - 1;
        	}
        	else
        	{
        		return false;
        	}
        }
        
        for (uint32_t i = myNUsedItems; i > 0; i--)
        {
            myItems[i] = myItems[(i - 1)];
        }
        
        myItems[0] = item;
        myNUsedItems++;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool prepend(T items[], const uint32_t nItems, const bool greedy = false)
    {
        uint32_t nRemainingItems = myNMaxItems - myNUsedItems;
        uint32_t nItemsToPrepend = nItems;

        bool returnValue;

        if (nItemsToPrepend > nRemainingItems)
        {
            returnValue = false;

            if (greedy)
            {
                nItemsToPrepend = nRemainingItems;
            }
            else
            {
                return returnValue;
            }
        }
        else
        {
            returnValue = true;
        }
        
        for (int32_t i = (myNUsedItems - 1); i >= 0; i--)
        {
            myItems[(i + nItemsToPrepend)] = myItems[i];
        }
        
        for (uint32_t i = 0; i < nItemsToPrepend; i++)
        {
            myItems[i] = items[i];
        }
        
        myNUsedItems += nItemsToPrepend;
        
        return returnValue;
    }
    
    //--------------------------------------------------------------------------
    template <uint32_t nItems>
    bool prepend(T (&items)[nItems], const bool greedy = false)
    {
        return prepend(items, nItems, greedy);
    }

    //--------------------------------------------------------------------------
    bool prepend(const Array<T>& array, const bool greedy = false)
    {
        return prepend(array.getItems(), array.getSize(), greedy);
    }
    
    //--------------------------------------------------------------------------
    bool insert(T item, const uint32_t index)
    {
        if (myNUsedItems == myNMaxItems)
        {
            return false;
        }
        
        for (uint32_t i = myNUsedItems; i > index; i--)
        {
            myItems[i] = myItems[i - 1];
        }
        
        myItems[index] = item;
        myNUsedItems++;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool insert(T items[], const uint32_t nItems, const uint32_t index)
    {
        if ((myNUsedItems + nItems) >= myNMaxItems)
        {
            return false;
        }
        
        for (int32_t i = (myNUsedItems - 1); i >= 0; i--)
        {
            myItems[(i + nItems)] = myItems[i];
        }
        
        for (uint32_t i = 0; i < nItems; i++)
        {
            myItems[(index + i)] = items[i];
        }
        
        myNUsedItems += nItems;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    template <uint32_t nItems>
    bool insert(T (&items)[nItems], const uint32_t index)
    {
        return insert(items, nItems, index);
    }

    //--------------------------------------------------------------------------
    bool replace(T item, const uint32_t index)
    {
        if (index >= myNMaxItems)
        {
            return false;
        }

        myItems[index] = item;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    Array<T> subArray(const uint32_t index,
                      const uint32_t nItems = 0) const
    {
        // If starting index is greater than number of items, return empty Array
        if (index >= myNMaxItems)
        {
            return Array<T>();
        }

        int subNMaxItems  = 0;

        if ((nItems == 0) || (nItems > (myNMaxItems - index)))
        {
            subNMaxItems = myNMaxItems - index;
        }
        else
        {
            subNMaxItems = nItems;
        }

        int subNUsedItems = myNUsedItems - index;

        if (subNUsedItems < 0)
        {
            subNUsedItems = 0;
        }
        else if ((subNUsedItems > nItems) && (nItems != 0))
        {
            subNUsedItems = nItems;
        }
        
        return Array<T>(&(myItems[index]), subNMaxItems, subNUsedItems);
    }
    
    //--------------------------------------------------------------------------
    void clear()
    {
        myNUsedItems = 0;
    }
    
    //--------------------------------------------------------------------------
    T& operator[](const uint32_t index) const
    {
        return myItems[index];
    }
    
    //--------------------------------------------------------------------------
    Array<T>& operator=(const Array<T>& array)
    {
        myItems      = array.myItems;
        myNMaxItems  = array.myNMaxItems;
        myNUsedItems = array.myNUsedItems;

        return (*this);
    }

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    T* myItems;
    uint32_t myNMaxItems;
    uint32_t myNUsedItems;
};

}; // namespace Plat4m

#endif // PLAT4M_ARRAY_H
