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
/// @file List.h
/// @author Ben Minerd
/// @date 6/13/2013
/// @brief List class header file.
///

#ifndef PLAT4M_LIST_H
#define PLAT4M_LIST_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/MemoryAllocator.h>
#include <Plat4m_Core/ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename T>
class List
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_ITEM_NULL
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Item
    {
        T value;
        Item* previousItem;
        Item* nextItem;
    };
    
    //--------------------------------------------------------------------------
    // Public friend classes
    //--------------------------------------------------------------------------
    
    class Iterator
    {
    public:
        
        //----------------------------------------------------------------------
        Iterator(List<T>& list) :
            myList(list),
            myCurrentItem(myList.myFirstItem)
        {
        }
        //----------------------------------------------------------------------
        bool hasCurrent()
        {
            return (isValidPointer(myCurrentItem));
        }
        
        //----------------------------------------------------------------------
        bool next()
        {
            if (!hasCurrent())
            {
                return false;
            }
            
            myCurrentItem = myCurrentItem->nextItem;
            
            return true;
        }
        
        //----------------------------------------------------------------------
        bool previous()
        {
            if (!hasCurrent())
            {
                return false;
            }
            
            myCurrentItem = myCurrentItem->previousItem;
            
            return true;
        }
        
        //----------------------------------------------------------------------
        void first()
        {
            myCurrentItem = myList.myFirstItem;
        }
        
        //----------------------------------------------------------------------
        void last()
        {
            myCurrentItem = myList.myLastItem;
        }
        
        //----------------------------------------------------------------------
        T& current()
        {
            return (myCurrentItem->value);
        }
        
    private:
        
        List<T>& myList;
        
        List<T>::Item* myCurrentItem;
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    List() :
         mySize(0),
         myFirstItem(0),
         myLastItem(0)
    {
    }
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    uint32_t size() const
    {
        return mySize;
    }
    
    //--------------------------------------------------------------------------
    bool isEmpty()
    {
        return (mySize == 0);
    }
    
    //--------------------------------------------------------------------------
    void append(T& value)
    {
        Item* item = MemoryAllocator::allocate<Item>();
        
        item->value = value;
        item->previousItem = 0;
        item->nextItem = 0;
        
        // Advance myLastItem pointer
        if (isNullPointer(myFirstItem))
        {
            myFirstItem = item;
        }
        else if (isNullPointer(myLastItem))
        {
            myLastItem = item;
            myFirstItem->nextItem = myLastItem;
            myLastItem->previousItem = myFirstItem;
        }
        else
        {
            Item* oldLastItem = myLastItem;
            item->previousItem = oldLastItem;
            oldLastItem->nextItem = item;
            myLastItem = item;
        }
        
        mySize++;
    }
    
    //--------------------------------------------------------------------------
    void prepend(T& value)
    {
        Item* item = MemoryAllocator::allocate<Item>();
        
        item->value = value;
        item->previousItem = 0;
        item->nextItem = 0;
        
        // Advance myLastItem pointer
        if (isNullPointer(myFirstItem))
        {
            myFirstItem = item;
        }
        else if (isNullPointer(myLastItem))
        {
            myLastItem = item;
            myFirstItem->nextItem = myLastItem;
            myLastItem->previousItem = myFirstItem;
        }
        else
        {
            Item* oldFirstItem = myFirstItem;
            item->nextItem = oldFirstItem;
            oldFirstItem->previousItem = item;
            myFirstItem = item;
        }
        
        mySize++;
    }
    
    //--------------------------------------------------------------------------
    T& first()
    {
        T* value = 0;

        if (isNullPointer(myFirstItem))
        {
            Error error(ERROR_CODE_ITEM_NULL);

            return (*value);
        }

        return (myFirstItem->value);
    }
    
    //--------------------------------------------------------------------------
    T& last()
    {
        T* value = 0;

        if (isNullPointer(myFirstItem))
        {
            Error error(ERROR_CODE_ITEM_NULL);

            return (*value);
        }

        return (myLastItem->value);
    }
    
    //--------------------------------------------------------------------------
    Iterator iterator()
    {
        return Iterator(*this);
    }

    //--------------------------------------------------------------------------
    void remove(T& value)
    {
        Item* currentItem = myFirstItem;

        while (isValidPointer(currentItem))
        {
            T& currentValue = currentItem->value;

            if (value == currentValue)
            {
                if (currentItem == myFirstItem)
                {
                    myFirstItem = myFirstItem->nextItem;
                }
                else if (currentItem == myLastItem)
                {
                    myLastItem = myLastItem->previousItem;

                    if (myFirstItem == myLastItem)
                    {
                        myLastItem = 0;
                        myFirstItem->nextItem = 0;
                    }
                }
                else
                {
                    currentItem->previousItem->nextItem = currentItem->nextItem;
                }

                mySize--;

                MemoryAllocator::deallocate(currentItem);

                break;
            }

            currentItem = currentItem->nextItem;
        }
    }
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    uint32_t mySize;
    
    Item* myFirstItem;
    Item* myLastItem;
};

}; // namespace Plat4m

#endif // PLAT4M_LIST_H
