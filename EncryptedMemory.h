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
/// @file EncryptedMemory.h
/// @author Ben Minerd
/// @date 3/18/2016
/// @brief EncryptedMemory class header file.
///

#ifndef PLAT4M_ENCRYPTED_MEMORY_H
#define PLAT4M_ENCRYPTED_MEMORY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Memory.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Array.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename TAddress>
class EncryptedMemory : public Memory<TAddress>
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_COMMUNICATION
    };
    
    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    using typename Memory<TAddress>::Error;

    using typename Memory<TAddress>::Config;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    EncryptedMemory() :
        Memory<TAddress>(),
        myMemory()
    {
    }

    //--------------------------------------------------------------------------
    EncryptedMemory(Memory<TAddress>& memory) :
        Memory<TAddress>(),
        myMemory(&memory)
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~EncryptedMemory()
    {
    }

    //--------------------------------------------------------------------------
    // Public methods implemented from Memory
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Error configure(const Config& config)
    {
        return myMemory->configure(config);
    }

    //--------------------------------------------------------------------------
    Error erase(const TAddress address, const unsigned int nBytes)
    {
        return myMemory->erase(address, nBytes);
    }

    //--------------------------------------------------------------------------
    Error read(const TAddress address, ByteArray& dataArray)
    {
        Error error = myMemory->read(address, dataArray);

        // if (error == ERROR_CODE_NONE)
        // {
        //     myEncryptionPolicy->encrypt(dataArray);
        // }

        return error;
    }

    //--------------------------------------------------------------------------
    Error write(const TAddress address, const ByteArray& dataArray)
    {
        // myEncryptionPolicy->decrypt(dataArray);

        Error error = myMemory->write(address, dataArray);

        return error;
    }

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    Memory<TAddress>* myMemory;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    Error driverConfigure(const Config& config)
    {
        // Intentionally not implemented
        return Memory<TAddress>::ERROR_CODE_NONE;
    }
    
    //--------------------------------------------------------------------------
    Error driverErase(const TAddress address, const unsigned int nBytes)
    {
        // Intentionally not implemented
        return Memory<TAddress>::ERROR_CODE_NONE;
    }

    //--------------------------------------------------------------------------
    Error driverRead(const TAddress address, ByteArray& dataArray)
    {
        // Intentionally not implemented
        return Memory<TAddress>::ERROR_CODE_NONE;
    }
    
    //--------------------------------------------------------------------------
    Error driverWrite(const TAddress address, const ByteArray& dataArray)
    {
        // Intentionally not implemented
        return Memory<TAddress>::ERROR_CODE_NONE;
    }
};

}; // namespace Plat4m

#endif // PLAT4M_ENCRYPTED_MEMORY_H
