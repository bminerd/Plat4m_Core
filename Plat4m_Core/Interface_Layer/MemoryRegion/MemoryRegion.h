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
 * @file MemoryRegion.h
 * @author Ben Minerd
 * @date 4/18/2013
 * @brief MemoryRegion class.
 */

#ifndef _MEMORY_REGION_H_
#define _MEMORY_REGION_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Memory.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class MemoryRegion
{
public:

    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of external memory errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_LOCKED,
        ERROR_NOT_ENABLED,
        ERROR_INVALID_OFFSET,
        ERROR_MEMORY_NOT_SET,
        ERROR_READ,
        ERROR_WRITE,
        ERROR_COMMUNICATION
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    struct Config
    {
        int a; // Placeholder
    };
    
    /*--------------------------------------------------------------------------
     * Public constructors and destructors
     *------------------------------------------------------------------------*/
   
    MemoryRegion(Memory* memory = NULL_POINTER,
                 const unsigned int address = 0,
                 const unsigned int size = 0,
                 const bool lock = true);
    
    ~MemoryRegion();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error getMemory(Memory*& memory) const;

    Error getAddress(unsigned int& address) const;
    
    Error getSize(unsigned int& size) const;
    
    Error setMemory(Memory* memory);
    
    Error setAddress(const unsigned int address);
    
    Error setSize(const unsigned int size);
    
    Error lock(const bool lock);
    
    /**
     * @brief Gets a given external memory reading.
     * @param externalMemory External memory to access.
     * @param reading External memory reading.
     * @return External memory error.
     */
    Error read(data_array_t& data, const unsigned int offset = 0);

    /**
     * @brief Gets a given external memory reading.
     * @param externalMemory External memory to access.
     * @param reading External memory reading.
     * @return External memory error.
     */
    Error write(const data_array_t& data, const unsigned int offset = 0);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsLocked;
    
    unsigned int myAddress;
    
    unsigned int mySize;

    Memory* myMemory;
};

#endif // _MEMORY_REGION_H_
