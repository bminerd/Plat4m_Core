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
/// @file DacIc.h
/// @author Ben Minerd
/// @date 4/3/2013
/// @brief DacIc class header file.
///

#ifndef PLAT4M_DAC_IC_H
#define PLAT4M_DAC_IC_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class DacIc : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_COMMUNICATION
    };
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error registerRead(const Id registerId, uint16_t& value);
    
    virtual Error registerWrite(const Id registerId, const uint16_t value);
    
    virtual Error registerReadBit(const Id registerId,
                                  const unsigned int bit,
                                  Plat4m::BitValue& bitValue);
    
    virtual Error registerWriteBit(const Id registerId,
                                   const unsigned int bit,
                                   const Plat4m::BitValue bitValue);
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    DacIc();
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~DacIc();

private:
    
    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverRegisterRead(const Id registerId,
                                     uint16_t& value) = 0;
    
    virtual Error driverRegisterWrite(const Id registerId,
                                      const uint16_t value) = 0;
    
    virtual Error driverRegisterReadBit(const Id registerId,
                                        const unsigned int bit,
                                        Plat4m::BitValue& bitValue) = 0;
    
    virtual Error driverRegisterWriteBit(const Id registerId,
                                         const unsigned int bit,
                                         const Plat4m::BitValue bitValue) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_DAC_IC_H
