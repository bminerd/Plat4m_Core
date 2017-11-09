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
/// @file ComProtocolPlat4m.h
/// @author Ben Minerd
/// @date 3/1/16
/// @brief ComProtocolPlat4m class header file.
///

// TODO: Consider renaming to ComProtocolFrame (more generic)

#ifndef PLAT4M_COM_PROTOCOL_PLAT4M_BINARY_H
#define PLAT4M_COM_PROTOCOL_PLAT4M_BINARY_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <ComProtocol.h>
#include <ErrorTemplate.h>
#include <Callback.h>
#include <ByteArray.h>
#include <ByteArrayN.h>
#include <Message.h>
#include <FrameHandler.h>
#include <List.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ComProtocolPlat4mBinary : public ComProtocol
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE
    };

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    ComProtocolPlat4mBinary(ComLink& comLink);
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~ComProtocolPlat4mBinary();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    void addFrameHandler(FrameHandler& frameHandler);

    void transmitFrame(Frame& frame, const bool waitUntilDone = false);

    void transmitReceiveFrame(Frame& transmitFrame, Frame& receiveFrame);

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    List<FrameHandler*> myFrameHandlerList;

    ByteArrayN<256> myReceiveMessageByteArray;

    //--------------------------------------------------------------------------
    // Private methods implemented from ComProtocol
    //--------------------------------------------------------------------------

	ParseStatus driverParseData(const ByteArray& receiveByteArray,
	                            ByteArray& transmitByteArray,
	                            Callback<>*& followUpCallback);
};

}; // namespace Plat4m

#endif // PLAT4M_COM_PROTOCOL_PLAT4M_BINARY_H
