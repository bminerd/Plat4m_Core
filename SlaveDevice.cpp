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
/// @file SlaveDevice.cpp
/// @author Ben Minerd
/// @date 4/24/14
/// @brief SlaveDevice class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <SlaveDevice.h>

using Plat4m::SlaveDevice;
using Plat4m::Array;
using Plat4m::MasterSlaveBus;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void SlaveDevice::setMailboxArray(Array<MasterSlaveBus::Mailbox>& mailboxArray)
{
    myMailboxArray = &mailboxArray;
}

//------------------------------------------------------------------------------
Array<MasterSlaveBus::Mailbox>* SlaveDevice::getMailboxArray()
{
    return myMailboxArray;
}

//------------------------------------------------------------------------------
SlaveDevice::Error SlaveDevice::transmit(const ByteArray& byteArray,
                                         const bool waitUntilDone,
                                         const int32_t mailboxIndex)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }
    
    Error error = driverTransmit(byteArray, waitUntilDone, mailboxIndex);
    
    return error;
}

//------------------------------------------------------------------------------
SlaveDevice::Error SlaveDevice::receive(ByteArray& byteArray,
                                        const int32_t mailboxIndex)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    Error error = driverReceive(byteArray, mailboxIndex);
    
    return error;
}

//------------------------------------------------------------------------------
SlaveDevice::Error SlaveDevice::transmitReceive(
                                             const ByteArray& transmitByteArray,
                                             ByteArray& receiveByteArray,
                                             const int32_t mailboxIndex)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }
    
    Error error = driverTransmitReceive(transmitByteArray,
                                        receiveByteArray,
                                        mailboxIndex);
    
    return error;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SlaveDevice::SlaveDevice() :
    myMailboxArray(0)
{
}

//------------------------------------------------------------------------------
SlaveDevice::SlaveDevice(Array<MasterSlaveBus::Mailbox>& mailboxArray) :
    myMailboxArray(&mailboxArray)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SlaveDevice::~SlaveDevice()
{
}
