/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file Array.h
 * @author Ben Minerd
 * @date 6/25/13
 * @brief Array class.
 */

#ifndef _ARRAY_H_
#define _ARRAY_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

template <typename T>
class Array
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    //--------------------------------------------------------------------------
    Array() :
        myItems(NULL_POINTER),
        myNMaxItems(0),
        myNUsedItems(0)
    {
    }
    
    //--------------------------------------------------------------------------
    Array(const Array<T>& array) :
        myItems(array.myItems),
        myNMaxItems(array.myNMaxItems),
        myNUsedItems(array.myNUsedItems)
    {
    }
    
    //--------------------------------------------------------------------------
    Array(T items[], const unsigned int nMaxItems, const int nUsedItems = -1) :
        myItems(items),
        myNMaxItems(nMaxItems),
        myNUsedItems((nUsedItems == -1) ? nMaxItems : nUsedItems)
    {
    }
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    //--------------------------------------------------------------------------
    T* getItems() const
    {
        return myItems;
    }
    
    //--------------------------------------------------------------------------
    unsigned int getSize() const
    {
        return myNUsedItems;
    }
    
    //--------------------------------------------------------------------------
    unsigned int getMaxSize() const
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
    bool append(const T items[], const unsigned int nItems)
    {
        if ((myNUsedItems + nItems) > myNMaxItems)
        {
            return false;
        }
        
        for (int i = 0; i < nItems; i++)
        {
            myItems[myNUsedItems++] = items[i];
        }
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool append(const Array<T>& array)
    {
        return append(array.getItems(), array.getSize());
    }
    
    //--------------------------------------------------------------------------
    bool prepend(T item)
    {
        if (myNUsedItems == myNMaxItems)
        {
            return false;
        }
        
        for (int i = myNUsedItems; i > 0; i--)
        {
            myItems[i] = myItems[i - 1];
        }
        
        myItems[0] = item;
        myNUsedItems++;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool prepend(T items[], const unsigned int nItems)
    {
        if ((myNUsedItems + nItems) >= myNMaxItems)
        {
            return false;
        }
        
        for (int i = (myNUsedItems + nItems); i > 0; i--)
        {
            myItems[i] = myItems[i - nItems - 1];
        }
        
        for (int i = 0; i < nItems; i++)
        {
            myItems[i] = items[i];
        }
        
        myNUsedItems += nItems;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool prepend(const Array<T>& array)
    {
        return prepend(array.getItems(), array.getSize());
    }
    
    //--------------------------------------------------------------------------
    bool insert(T item, const unsigned int index)
    {
        if (myNUsedItems == myNMaxItems)
        {
            return false;
        }
        
        for (int i = myNUsedItems; i > index; i--)
        {
            myItems[i] = myItems[i - 1];
        }
        
        myItems[index] = item;
        myNUsedItems++;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool insert(T items[], const unsigned int nItems, const unsigned int index)
    {
        if ((myNUsedItems + nItems) >= myNMaxItems)
        {
            return false;
        }
        
        for (int i = (myNUsedItems + nItems); i > index; i--)
        {
            myItems[i] = myItems[i - nItems - 1];
        }
        
        for (int i = index; i < nItems; i++)
        {
            myItems[index + i] = items[i];
        }
        
        myNUsedItems += nItems;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    bool replace(T item, const unsigned int index)
    {
        myItems[index] = item;
        
        return true;
    }
    
    //--------------------------------------------------------------------------
    Array<T> subArray(const unsigned int index,
                      const unsigned int nItems = 0) const
    {
        if (nItems == 0)
        {
            return Array<T>(&(myItems[index]), myNUsedItems - index);
        }
        
        return Array<T>(&(myItems[index]), nItems);
    }
    
    //--------------------------------------------------------------------------
    void clear()
    {
        // Memset all items?
        myNUsedItems = 0;
    }
    
    //--------------------------------------------------------------------------
    T& operator[](const int index) const
    {
        return myItems[index];
    }
    
    //--------------------------------------------------------------------------
    void operator=(const Array<T>& array)
    {
        myItems         = array.myItems;
        myNMaxItems     = array.myNMaxItems;
        myNUsedItems    = array.myNUsedItems;
    }

    //--------------------------------------------------------------------------
    void setSize(const unsigned int size)
    {
        myNUsedItems = size;
    }

protected:
    
//    //--------------------------------------------------------------------------
//    void setSize(const unsigned int size)
//    {
//        myNUsedItems = size;
//    }
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    T* myItems;
    unsigned int myNMaxItems;
    unsigned int myNUsedItems;
};

#endif // _ARRAY_H_
