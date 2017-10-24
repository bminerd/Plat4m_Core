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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file AllocationMemoryLite.h
/// @author Ben Minerd
/// @date 4/9/14
/// @brief AllocationMemoryLite class header file.
///

#ifndef PLAT4M_ALLOCATION_MEMORY_LITE_H
#define PLAT4M_ALLOCATION_MEMORY_LITE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AllocationMemory.h>

#include <stdint.h>
#include <string.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <unsigned int N>
class AllocationMemoryLite : public AllocationMemory
{
public:
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    AllocationMemoryLite() :
        AllocationMemory(),
        myMemoryIndex(0),
        myIsLocked(false)
    {
    }
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~AllocationMemoryLite()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    void setLocked(const bool locked)
    {
        myIsLocked = locked;
    }
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    uint8_t myMemory[N];

    size_t myMemoryIndex;

    bool myIsLocked;

    //--------------------------------------------------------------------------
    // Private methods implemented from AllocationMemory
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    void* driverAllocate(size_t count)
    {
        void* memory = 0;
    
        if ((myMemoryIndex + count) > N)
        {
            // Overflow error

            // Attempting to allocate more memory than available, lock up
            while (true)
            {

            }
        }
        
        if (!myIsLocked)
        {
            memory = &(myMemory[myMemoryIndex]);
            myMemoryIndex += count;
        }
        
        return memory;
    }
    
    //--------------------------------------------------------------------------
    void* driverAllocateArray(size_t count)
    {
        return driverAllocate(count); // Does this work?
    }
    
    //--------------------------------------------------------------------------
    void driverDeallocate(void* pointer)
    {
        // Intentionally blank
    }
    
    //--------------------------------------------------------------------------
    void driverDeallocateArray(void* pointer)
    {
        // Intentionally blank
    }

    //--------------------------------------------------------------------------
    size_t driverGetFreeMemorySize()
    {
        return (N - myMemoryIndex);
    }
};

}; // namespace Plat4m

#endif // PLAT4M_ALLOCATION_MEMORY_LITE_H
