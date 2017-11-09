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
/// @file BinaryMessageServer.h
/// @author Ben Minerd
/// @date 4/13/2017
/// @brief BinaryMessageServer class header file.
///

#ifndef PLAT4M_BINARY_MESSAGE_SERVER_H
#define PLAT4M_BINARY_MESSAGE_SERVER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <BinaryMessageFrameHandler.h>
#include <BinaryMessageHandler.h>
#include <Frame.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BinaryMessageServer : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
        uint32_t outputRateHz;
    };

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    BinaryMessageServer(const uint16_t groupId,
                        ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                        BinaryMessageFrameHandler& binaryMessageFrameHandler);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~BinaryMessageServer();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void addMessageHandler(BinaryMessageHandler& messageHandler);

    void transmitMessage(BinaryMessage& binaryMessage);

    void transmitFrame(Frame& frame);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ComProtocolPlat4mBinary& myComProtocolPlat4mBinary;

    BinaryMessageFrameHandler& myBinaryMessageFrameHandler;

    BinaryMessageHandlerGroup myBinaryMessageHandlerGroup;

    ByteArrayN<128> myMessageByteArray;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    virtual Module::Error interfaceSetEnabled(const bool enabled);
};

}; // namespace Plat4m

#endif // PLAT4M_BINARY_MESSAGE_SERVER_H
