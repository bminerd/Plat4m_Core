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
/// @file ComProtocol.h
/// @author Ben Minerd
/// @date 4/22/2013
/// @brief ComProtocol class.
///

#ifndef PLAT4M_COM_PROTOCOL_H
#define PLAT4M_COM_PROTOCOL_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/ComLink.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ComProtocol : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID
    };

    enum ParseStatus
    {
        PARSE_STATUS_NOT_A_FRAME,
        PARSE_STATUS_MID_FRAME,
        PARSE_STATUS_INVALID_FRAME,
        PARSE_STATUS_UNSUPPORTED_FRAME,
        PARSE_STATUS_FOUND_FRAME
    };

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    uint32_t getParseTimeoutMs();
    
    ParseStatus parseData(const ByteArray& receiveByteArray,
                          ByteArray& transmitByteArray,
                          Callback<>*& followUpCallback);
    
protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

	ComProtocol(const uint32_t parseTimeoutMs, ComLink& comLink);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ComProtocol();
    
    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    ComLink& getComLink();

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const uint32_t myParseTimeoutMs;

    ComLink& myComLink;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual ParseStatus driverParseData(const ByteArray& receiveByteArray,
    									ByteArray& transmitByteArray,
    									Callback<>*& followUpCallback) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_COM_PROTOCOL_H
