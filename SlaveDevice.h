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
/// @file SlaveDevice.h
/// @author Ben Minerd
/// @date 4/24/2014
/// @brief SlaveDevice class header file.
///

#ifndef PLAT4M_SLAVE_DEVICE_H
#define PLAT4M_SLAVE_DEVICE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>
#include <ErrorTemplate.h>
#include <ByteArray.h>
#include <BufferN.h>
#include <ArrayN.h>
#include <MasterSlaveBus.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SlaveDevice : public Module
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

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    void setMailboxArray(Array<MasterSlaveBus::Mailbox>& mailboxArray);

    Array<MasterSlaveBus::Mailbox>* getMailboxArray();

    Error transmit(const ByteArray& byteArray,
                   const bool waitUntilDone = false,
                   const int32_t mailboxIndex = -1);

    Error receive(ByteArray& byteArray,
                  const int32_t mailboxIndex = -1);

    Error transmitReceive(const ByteArray& transmitByteArray,
                          ByteArray& receiveByteArray,
                          const int32_t mailboxIndex = -1);
    
protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    SlaveDevice();

    SlaveDevice(Array<MasterSlaveBus::Mailbox>& mailboxArray);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SlaveDevice();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Array<MasterSlaveBus::Mailbox>* myMailboxArray;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverTransmit(const ByteArray& byteArray,
                                 const bool waitUntilDone,
                                 const int32_t mailboxIndex) = 0;

    virtual Error driverReceive(ByteArray& byteArray,
                                const int32_t mailboxIndex) = 0;
    
    virtual Error driverTransmitReceive(const ByteArray& transmitByteArray,
                                        ByteArray& receiveByteArray,
                                        const int32_t mailboxIndex) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_SLAVE_DEVICE_H
