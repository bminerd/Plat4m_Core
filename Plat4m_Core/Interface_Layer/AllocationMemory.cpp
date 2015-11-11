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
 * Copyright (c) 2014 Benjamin Minerd
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
 * @file AllocationMemory.cpp
 * @author Ben Minerd
 * @date 4/8/14
 * @brief AllocationMemory class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <AllocationMemory.h>

using Plat4m::AllocationMemory;

/*------------------------------------------------------------------------------
 * Static data members
 *----------------------------------------------------------------------------*/

AllocationMemory* AllocationMemory::myDriver = nullptr;

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void* AllocationMemory::allocate(size_t count)
{
    return myDriver->driverAllocate(count);
}

//------------------------------------------------------------------------------
void* AllocationMemory::allocateArray(size_t count)
{
    return myDriver->driverAllocateArray(count);
}

//------------------------------------------------------------------------------
void AllocationMemory::deallocate(void* pointer)
{
    myDriver->driverDeallocate(pointer);
}

//------------------------------------------------------------------------------
void AllocationMemory::deallocateArray(void* pointer)
{
    myDriver->driverDeallocateArray(pointer);
}

/*------------------------------------------------------------------------------
 * Protected constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
AllocationMemory::AllocationMemory()
{
    if (isNullPointer(myDriver))
    {
        myDriver = this;
    }
}

/*------------------------------------------------------------------------------
 * Protected destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
AllocationMemory::~AllocationMemory()
{
}

/*------------------------------------------------------------------------------
 * Global functions
 *----------------------------------------------------------------------------*/

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
