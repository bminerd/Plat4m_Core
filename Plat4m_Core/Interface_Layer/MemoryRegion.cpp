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
 * @file MemoryRegion.cpp
 * @author Ben Minerd
 * @date 4/18/2013
 * @brief MemoryRegion class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <MemoryRegion.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
MemoryRegion::MemoryRegion(Memory* memory,
                           const unsigned int address,
                           const unsigned int size,
                           const bool lock) :
    myIsLocked(lock && (size != 0)),
    myAddress(address),
    mySize(size),
    myMemory(memory)
{
}

//------------------------------------------------------------------------------
MemoryRegion::~MemoryRegion()
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::getMemory(Memory*& memory) const
{
    if (IS_NULL_POINTER(myMemory))
    {
        return ERROR_MEMORY_NOT_SET;
    }
    
    memory = myMemory;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::getAddress(unsigned int& address) const
{
    address = myAddress;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::getSize(unsigned int& size) const
{
    size = mySize;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::setMemory(Memory* memory)
{
    if (myIsLocked)
    {
        return ERROR_LOCKED;
    }
    
    myMemory = memory;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::setAddress(const unsigned int address)
{
    if (myIsLocked)
    {
        return ERROR_LOCKED;
    }
    
    myAddress = address;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::setSize(const unsigned int size)
{
    if (myIsLocked)
    {
        return ERROR_LOCKED;
    }
    
    mySize = size;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::lock(const bool lock)
{
    myIsLocked = lock;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::read(data_array_t& data,
                                       const unsigned int offset)
{
    if (IS_NULL_POINTER(myMemory))
    {
        return ERROR_MEMORY_NOT_SET;
    }
    
    if (myMemory->read(data, myAddress + offset) != Memory::ERROR_NONE)
    {
        return ERROR_READ;
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
MemoryRegion::Error MemoryRegion::write(const data_array_t& data,
                                        const unsigned int offset)
{
    if (IS_NULL_POINTER(myMemory))
    {
        return ERROR_MEMORY_NOT_SET;
    }
    
    if (myMemory->write(data, myAddress + offset) != Memory::ERROR_NONE)
    {
        return ERROR_WRITE;
    }

    return ERROR_NONE;
}