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
// Copyright (c) 2019 Benjamin Minerd
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
/// @file SerialPortLinux.h
/// @author Ben Minerd
/// @date 5/26/2019
/// @brief SerialPortLinux class header file.
///

#ifndef PLAT4M_SERIAL_PORT_LINUX_H
#define PLAT4M_SERIAL_PORT_LINUX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Linux.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/SerialPort.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Mutex.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SerialPortLinux : public SerialPort
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    SerialPortLinux(const char* comPort);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SerialPortLinux();
    
private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const uint8_t myWordBitsMap[];

    static const uint8_t myStopBitsMap[];

    static const uint8_t myParityMap[];
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    HANDLE mySerialHandle;

    Thread& myReceiveThread;

    Mutex& myMutex;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

	Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from SerialPort
    //--------------------------------------------------------------------------
    
    SerialPort::Error driverSetConfig(const Config& config);
    
    ComInterface::Error driverTransmitBytes(const ByteArray& byteArray,
                                            const bool waitUntilDone);
    
    uint32_t driverGetReceivedBytesCount();

    ComInterface::Error driverGetReceivedBytes(ByteArray& byteArray,
                                               const uint32_t nBytes);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void receiveThreadCallback();
};

}; // namespace Plat4m

#endif // PLAT4M_SERIAL_PORT_LINUX_H
