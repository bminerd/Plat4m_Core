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
/// @file ComInterfaceDevice.h
/// @author Ben Minerd
/// @date 3/15/16
/// @brief ComInterfaceDevice class header file.
///

#ifndef PLAT4M_COM_INTERFACE_DEVICE_H
#define PLAT4M_COM_INTERFACE_DEVICE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <ComInterface.h>
#include <Buffer.h>
#include <ByteArray.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ComInterfaceDevice : public Module
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

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ComInterfaceDevice(Buffer<uint8_t>& transmitBuffer,
                       Buffer<uint8_t>& receiveBuffer);

    ComInterfaceDevice(Buffer<uint8_t>& transmitBuffer,
                       Buffer<uint8_t>& receiveBuffer,
                       ComInterface& comInterface);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ComInterfaceDevice();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    void setComInterface(ComInterface& comInterface);

    Error transmitBytes(const ByteArray& byteArray,
                        const bool waitUntilDone = false);

    Error receiveBytes(ByteArray& byteArray, const uint32_t timeoutMs = 100);

    uint32_t getReceivedBytesCount();

    Error getReceivedBytes(ByteArray& byteArray, const uint32_t nBytes = 0);

    Error transmitReceiveBytes(const ByteArray& txByteArray,
                               ByteArray& rxByteArray,
                               const uint32_t timeoutMs = 100);

protected:

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ComInterface* getComInterface();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ComInterface* myComInterface;

    Buffer<uint8_t>& myTransmitBuffer;

    Buffer<uint8_t>& myReceiveBuffer;

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverTransmitBytes(const ByteArray& byteArray,
                                      const bool waitUntilDone);

    virtual Error driverReceiveBytes(ByteArray& byteArray,
                                     const TimeMs timeoutMs);
    
    virtual Error driverTransmitReceiveBytes(const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const TimeMs timeoutMs);
};

}; // namespace Plat4m

#endif // PLAT4M_COM_INTERFACE_DEVICE_H
