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
/// @file MemoryRegion.h
/// @author Ben Minerd
/// @date 4/18/2013
/// @brief MemoryRegion class header file.
///

#ifndef PLAT4M_MEMORY_REGION_H
#define PLAT4M_MEMORY_REGION_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Memory.h>
#include <Plat4m_Core/Module.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename TAddress>
class MemoryRegion : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
		ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_LOCKED,
        ERROR_CODE_INVALID_OFFSET,
        ERROR_CODE_MEMORY_NOT_SET,
        ERROR_CODE_READ_FAILED,
        ERROR_CODE_WRITE_FAILED,
        ERROR_CODE_COMMUNICATION
    };
    
    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------
    
    struct Config
    {
        int a; // Placeholder
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    MemoryRegion(Memory<TAddress>* memory = 0,
                 const TAddress address = 0,
                 const uint32_t size = 0,
                 const bool locked = true) :
        myMemory(memory),
        myAddress(address),
        mySize(size),
        myIsLocked(locked)
    {
    }
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ~MemoryRegion()
    {
    }
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    Memory<TAddress>* getMemory()
    {
        return myMemory;
    }

    //--------------------------------------------------------------------------
    TAddress getAddress() const
    {
        return myAddress;
    }
    
    //--------------------------------------------------------------------------
    uint32_t getSize() const
    {
        return mySize;
    }
    
    //--------------------------------------------------------------------------
    void setMemory(Memory<TAddress>& memory)
    {
        myMemory = &memory;
    }
    
    //--------------------------------------------------------------------------
    void setAddress(const TAddress address)
    {
        myAddress = address;
    }
    
    //--------------------------------------------------------------------------
    void setSize(const uint32_t size)
    {
        mySize = size;
    }
    
    //--------------------------------------------------------------------------
    void setLocked(const bool locked)
    {
        myIsLocked = locked;
    }
    
    //--------------------------------------------------------------------------
    template <typename TData>
    Error read(Array<TData>& dataArray, const uint32_t offset = 0)
    {
        Memory<TAddress>::Error error = myMemory->read(myAddress, dataArray);
    }

    //--------------------------------------------------------------------------
    template <typename TData>
    Error read(TData& data, const uint32_t offset = 0)
    {
    	Memory<TAddress>::Error error = myMemory->read(myAddress, data);
    }


    //--------------------------------------------------------------------------
    template <typename TData>
    Error write(const TData& data, const uint32_t offset = 0)
    {
    	Memory<TAddress>::Error error = myMemory->write(myAddress, data);
    }
    
private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    Memory<TAddress>* myMemory;
    
    TAddress myAddress;
    
    uint32_t mySize;

    bool myIsLocked;
};

}; // namespace Plat4m

#endif // PLAT4M_MEMORY_REGION_H
