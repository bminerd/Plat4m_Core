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
/// @file ComInterface.h
/// @author Ben Minerd
/// @date 4/22/2013
/// @brief ComInterface class header file.
///

#ifndef PLAT4M_COM_INTERFACE_H
#define PLAT4M_COM_INTERFACE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/Buffer.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ComInterface : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_TRANSMIT_BUFFER_FULL,
        ERROR_CODE_TRANSMIT_FAILED,
        ERROR_CODE_RECEIVE_FAILED
    };

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<void, uint8_t> ByteReceivedCallback;

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error transmitBytes(const ByteArray& byteArray,
                                const bool waitUntilDone = true) = 0;

    virtual uint32_t getReceivedBytesCount() = 0;

    virtual Error getReceivedBytes(ByteArray& byteArray,
                                   const uint32_t nBytes = 0) = 0;

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual void setTransmitBuffer(Buffer<uint8_t>& transmitBuffer);

    virtual void setReceiveBuffer(Buffer<uint8_t>& receiveBuffer);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setByteReceivedCallback(ByteReceivedCallback& byteReceivedCallback);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    ComInterface();

    ComInterface(Buffer<uint8_t>& transmitBuffer,
                 Buffer<uint8_t>& receiveBuffer);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ComInterface();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    inline Buffer<uint8_t>* getTransmitBuffer()
    {
        return myTransmitBuffer;
    }

    //--------------------------------------------------------------------------
    inline Buffer<uint8_t>* getReceiveBuffer()
    {
        return myReceiveBuffer;
    }

    //--------------------------------------------------------------------------
    inline void byteReceived(const uint8_t byte)
    {
        if (isValidPointer(myByteReceivedCallback))
        {
            myByteReceivedCallback->call(byte);
        }
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Buffer<uint8_t>* myTransmitBuffer;

    Buffer<uint8_t>* myReceiveBuffer;

    ByteReceivedCallback* myByteReceivedCallback;
};

}; // namespace Plat4m

#endif // PLAT4M_COM_INTERFACE_H
