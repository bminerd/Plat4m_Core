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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file Memory.h
/// @author Ben Minerd
/// @date 4/18/2013
/// @brief Memory class header file.
///

#ifndef PLAT4M_MEMORY_H
#define PLAT4M_MEMORY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <ByteArray.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename TAddress>
class Memory : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_COMMUNICATION
    };

    typedef ErrorTemplate<ErrorCode> Error;
    
    struct Config
    {
        int a; // Placeholder
    };
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual Error configure(const Config& config)
    {
        Error error = driverConfigure(config);

        return error;
    }

    //--------------------------------------------------------------------------
    virtual Error clear(const TAddress address, const uint32_t nBytes)
    {
        if (!isEnabled())
        {
            return Error(ERROR_CODE_NOT_ENABLED);
        }

        Error error = driverClear(address, nBytes);

        return error;
    }

    //--------------------------------------------------------------------------
    virtual Error read(const TAddress address, ByteArray& dataArray)
    {
        if (!isEnabled())
        {
            return Error(ERROR_CODE_NOT_ENABLED);
        }

        Error error = driverRead(address, dataArray);

        return error;
    }

    //--------------------------------------------------------------------------
    virtual Error write(const TAddress address, const ByteArray& dataArray)
    {
        if (!isEnabled())
        {
            return Error(ERROR_CODE_NOT_ENABLED);
        }

        Error error = driverWrite(address, dataArray);

        return error;
    }
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
   
    //--------------------------------------------------------------------------
    Memory()
    {
    }
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~Memory()
    {
    }

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    Config myConfig;
    
    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverClear(const TAddress address,
                              const uint32_t nBytes) = 0;

    virtual Error driverRead(const TAddress address, ByteArray& byteArray) = 0;
    
    virtual Error driverWrite(const TAddress address,
                              const ByteArray& byteArray) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_MEMORY_H
