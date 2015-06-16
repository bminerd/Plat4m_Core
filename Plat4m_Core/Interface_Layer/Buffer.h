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
 * @file Buffer.h
 * @author Ben Minerd
 * @date 3/25/13
 * @brief Buffer class.
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

template <typename T, int N>
class Buffer
{
public:
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
    
    Buffer() :
        myWriteIndex(0),
        myReadIndex(0),
        myCount(0)
    {
    };
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    /**
     * @brief Writes the given data to the given buffer.
     * @param buffer Buffer to access.
     * @param data Data to write to buffer.
     * @return True if the function was successful, false if not.
     */
    bool write(const T& element)
    {
        if (isFull())
        {
            return false;
        }
        
        myMemory[myWriteIndex++] = element;
        
        myCount++;

        if (myWriteIndex >= ARRAY_SIZE(myMemory))
        {
            myWriteIndex = 0;
        }
        
        return true;
    }

    /**
     * @brief Reads data from the given buffer.
     * @param buffer Buffer to access.
     * @param data Data read from buffer.
     * @return True if the function was successful, false if not.
     */
    bool read(T& element)
    {
        if (isEmpty())
        {
            return false;
        }
        
        element = myMemory[myReadIndex++];
        
        myCount--;

        if (myReadIndex >= ARRAY_SIZE(myMemory))
        {
            myReadIndex = 0;
        }
        
        return true;
    }

    /**
     * @brief Returns a copy of the first element in the buffer.
     * @param buffer Buffer to access.
     * @param data Pointer to first element.
     * @return True if the function was successful, false if not.
     */
    bool peek(T*& element)
    {
        if (isEmpty())
        {
            element = NULL_POINTER;
            
            return false;
        }

        element = (&myMemory[myReadIndex]);

        return true;
    }

    /**
     * @brief Checks to see if the given queue is empty.
     * @param buffer Buffer to access.
     * @param isEmpty Flag that indicates if the buffer is empty.
     * @return True if the function was successful, false if not.
     */
    bool isEmpty()
    {
        return (myCount == 0);
    }

    /**
     * @brief Checks to see if the given buffer is full.
     * @param buffer Buffer to access.
     * @param isFull Flag that indicates if the buffer is full.
     * @return True if the function was successful, false if not.
     */
    bool isFull()
    {
        return (myCount >= ARRAY_SIZE(myMemory));
    }

    /**
     * @brief Gets the number of elements currently in the given buffer.
     * @param buffer Buffer to access.
     * @param count Number of elements in the buffer.
     * @return True if the function was successful, false if not.
     */
    unsigned int count()
    {
        return myCount;
    }
    
private:
    
    T myMemory[N];
    
    unsigned int myWriteIndex;
    
    unsigned int myReadIndex;
    
    unsigned int myCount;
};

#endif // _BUFFER_H_