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
/// @file IoExpander.h
/// @author Ben Minerd
/// @date 3/22/2013
/// @brief IoExpander class header file.
///

#ifndef PLAT4M_IO_EXPANDER_H
#define PLAT4M_IO_EXPANDER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/GpioPin.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class IoExpander : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    /**
     * @brief Enumeration of IO expander errors.
     */
    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_COMMUNICATION
    };

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------
    
    typedef ErrorTemplate<ErrorCode> Error;
    
    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error registerRead(const Id registerId, uint8_t& value);
    
    virtual Error registerWrite(const Id registerId, const uint8_t value);
    
    virtual Error registerReadBit(const Id registerId,
                                  const unsigned int bit,
                                  Plat4m::BitValue& bitValue);
    
    virtual Error registerWriteBit(const Id registerId,
                                   const unsigned int bit,
                                   const Plat4m::BitValue bitValue);
    
    virtual Error pinConfigure(const Id pinId, const GpioPin::Config& config);
    
    virtual Error pinSetLevel(const Id pinId, const GpioPin::Level level);
    
    virtual Error pinGetLevel(const Id pinId, GpioPin::Level& level);
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    IoExpander();
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------
    
    virtual ~IoExpander();

private:
    
    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverRegisterRead(const Id registerId, uint8_t& value) = 0;
    
    virtual Error driverRegisterWrite(const Id registerId,
                                      const uint8_t value) = 0;
    
    virtual Error driverRegisterReadBit(const Id registerId,
                                        const unsigned int bit,
                                        Plat4m::BitValue& bitValue) = 0;
    
    virtual Error driverRegisterWriteBit(const Id registerId,
                                         const unsigned int bit,
                                         const Plat4m::BitValue bitValue) = 0;
    
    virtual Error driverPinConfigure(const Id pinId,
                                     const GpioPin::Config& config) = 0;
    
    virtual Error driverPinSetLevel(const Id pinId,
                                    const GpioPin::Level level) = 0;
    
    virtual Error driverPinGetLevel(const Id pinId,
                                    GpioPin::Level& level) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_IO_EXPANDER_H
