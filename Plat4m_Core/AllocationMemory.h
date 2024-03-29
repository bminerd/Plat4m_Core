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
// Copyright (c) 2014 Benjamin Minerd
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
/// @file AllocationMemory.h
/// @author Ben Minerd
/// @date 4/8/2014
/// @brief AllocationMemory class header file.
///

#ifndef PLAT4M_ALLOCATION_MEMORY_H
#define PLAT4M_ALLOCATION_MEMORY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stddef.h>

#include <Plat4m_Core/Module.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class AllocationMemory : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------
    
    static void* allocate(size_t count);
    
    static void* allocateArray(size_t count);
    
    static void deallocate(void* pointer);
    
    static void deallocateArray(void* pointer);

    static size_t getFreeMemorySize();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    AllocationMemory();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~AllocationMemory();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static AllocationMemory* myBaseDriver;

    static AllocationMemory* myNestedDriver;

    static AllocationMemory* myCurrentDriver;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual void* driverAllocate(size_t count) = 0;

    virtual void* driverAllocateArray(size_t count) = 0;

    virtual void driverDeallocate(void* pointer) = 0;

    virtual void driverDeallocateArray(void* pointer) = 0;

    virtual size_t driverGetFreeMemorySize() = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_ALLOCATION_MEMORY_H
