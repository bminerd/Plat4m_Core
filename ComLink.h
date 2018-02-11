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
/// @file ComLink.h
/// @author Ben Minerd
/// @date 4/22/2013
/// @brief ComLink class header file.
///

#ifndef PLAT4M_COM_LINK_H
#define PLAT4M_COM_LINK_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ComInterfaceDevice.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/ComInterface.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/WaitCondition.h>
#include <Plat4m_Core/Mutex.h>
#include <Plat4m_Core/MutexLock.h>
#include <Plat4m_Core/Queue.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Forward class declarations
//------------------------------------------------------------------------------

class ComProtocol;

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ComLink : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED
    };

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ComLink(ByteArray& transmitByteArray,
            ByteArray& receiveByteArray,
            ComInterfaceDevice& comInterfaceDevice);

    ComLink(ByteArray& transmitByteArray,
            ByteArray& receiveByteArray,
            ComInterfaceDevice& comInterfaceDevice,
            ComInterface& comInterface);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ComLink();

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    Thread& getDataParsingThread();

    void setComInterface(ComInterface& comInterface);

    void addComProtocol(ComProtocol& comProtocol);

    Error transmitBytes(const ByteArray& byteArray,
                        const bool waitUntilDone = false);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ByteArray& myTransmitByteArray;

    ByteArray& myReceiveByteArray;

    ComInterfaceDevice& myComInterfaceDevice;

    List<ComProtocol*> myComProtocolList;

    ComProtocol* myCurrentComProtocol;

    uint32_t myCurrentComProtocolTimeoutTimeMs;

    Thread& myDataParsingThread;

    WaitCondition& myWaitCondition;

    Mutex& myMutex;

    Queue<uint8_t>& myReceiveByteQueue;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverEnable(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void byteReceivedCallback(const uint8_t byte);

    void dataParsingThreadCallback();

    void tryProtocol(ComProtocol* comProtocol);
};

}; // namespace Plat4m

#endif // PLAT4M_COM_LINK
