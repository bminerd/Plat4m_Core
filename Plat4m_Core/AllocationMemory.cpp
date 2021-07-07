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
/// @file AllocationMemory.cpp
/// @author Ben Minerd
/// @date 4/8/2014
/// @brief AllocationMemory class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/AllocationMemory.h>
#include <Plat4m_Core/Plat4m.h>

using Plat4m::AllocationMemory;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

AllocationMemory* AllocationMemory::myBaseDriver = 0;

AllocationMemory* AllocationMemory::myNestedDriver = 0;

AllocationMemory* AllocationMemory::myCurrentDriver = 0;

//------------------------------------------------------------------------------
extern "C" void* allocationMemoryAllocate(size_t count)
{
	return AllocationMemory::allocate(count);
}

//------------------------------------------------------------------------------
extern "C" void allocationMemoryDeallocate(void* pointer)
{
	AllocationMemory::deallocate(pointer);
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void* AllocationMemory::allocate(size_t count)
{
    if (isNullPointer(myCurrentDriver))
    {
        return malloc(count);
    }

    if (!(myCurrentDriver->isEnabled()))
    {
        return malloc(count);
    }

    return (myCurrentDriver->driverAllocate(count));
}

//------------------------------------------------------------------------------
void* AllocationMemory::allocateArray(size_t count)
{
    if (isNullPointer(myCurrentDriver))
    {
        return malloc(count);
    }

    if (!(myCurrentDriver->isEnabled()))
    {
        return malloc(count);
    }

    return (myCurrentDriver->driverAllocateArray(count));
}

//------------------------------------------------------------------------------
void AllocationMemory::deallocate(void* pointer)
{
    if (isNullPointer(myCurrentDriver))
    {
        free(pointer);
    }
    else if (!(myCurrentDriver->isEnabled()))
    {
        free(pointer);
    }
    else
    {
        myCurrentDriver->driverDeallocate(pointer);
    }
}

//------------------------------------------------------------------------------
void AllocationMemory::deallocateArray(void* pointer)
{
    if (isNullPointer(myCurrentDriver))
    {
        free(pointer);
    }
    else if (!(myCurrentDriver->isEnabled()))
    {
        free(pointer);
    }
    else
    {
        myCurrentDriver->driverDeallocateArray(pointer);
    }
}

//------------------------------------------------------------------------------
size_t AllocationMemory::getFreeMemorySize()
{
    if (isNullPointer(myCurrentDriver))
    {
        return 0;
    }

    return (myCurrentDriver->driverGetFreeMemorySize());
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AllocationMemory::AllocationMemory()
{
    if (isNullPointer(myBaseDriver))
    {
        myBaseDriver = this;
        myCurrentDriver = myBaseDriver;
    }
    else if (isNullPointer(myNestedDriver))
    {
        myNestedDriver = this;
        myCurrentDriver = myNestedDriver;
    }

    enable();
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AllocationMemory::~AllocationMemory()
{
    if (isValidPointer(myNestedDriver))
    {
        myNestedDriver = 0;
        myCurrentDriver = myBaseDriver;
    }
    else if (isValidPointer(myBaseDriver))
    {
        myBaseDriver = 0;
        myCurrentDriver = 0;
    }

    disable();
}

//------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void* operator new(size_t count)
{
    return AllocationMemory::allocate(count);
}

//------------------------------------------------------------------------------
void* operator new[](size_t count)
{
    return AllocationMemory::allocateArray(count);
}

//------------------------------------------------------------------------------
void operator delete(void* pointer)
{
    return AllocationMemory::deallocate(pointer);
}

//------------------------------------------------------------------------------
void operator delete[](void* pointer)
{
    return AllocationMemory::deallocateArray(pointer);
}
