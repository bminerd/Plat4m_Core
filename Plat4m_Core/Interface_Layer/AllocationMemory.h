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
 * @file AllocationMemory.h
 * @author Ben Minerd
 * @date 4/8/14
 * @brief AllocationMemory class.
 */

#ifndef ALLOCATION_MEMORY_H
#define ALLOCATION_MEMORY_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <AllocationMemoryDriver.h>

namespace Plat4m
{

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class AllocationMemory
{
public:

    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static void* allocate(size_t count);
    
    static void* allocateArray(size_t count);
    
    static void deallocate(void* pointer);
    
    static void deallocateArray(void* pointer);

protected:

    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/

    AllocationMemory();

    /*--------------------------------------------------------------------------
     * Protected destructors
     *------------------------------------------------------------------------*/

    virtual ~AllocationMemory();

private:

    /*--------------------------------------------------------------------------
     * Private static data members
     *------------------------------------------------------------------------*/

    static AllocationMemory* myDriver;

    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/

    virtual void* driverAllocate(size_t count) = 0;

    virtual void* driverAllocateArray(size_t count) = 0;

    virtual void driverDeallocate(void* pointer) = 0;

    virtual void driverDeallocateArray(void* pointer) = 0;
};

}; // namespace Plat4m

#endif // ALLOCATION_MEMORY_H
