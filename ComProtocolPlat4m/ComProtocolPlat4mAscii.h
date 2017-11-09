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
/// @file ComProtocolPlat4mAscii.h
/// @author Ben Minerd
/// @date 3/1/16
/// @brief ComProtocolPlat4mAscii class header file.
///

// TODO: Consider renaming to ComProtocolFrame (more generic)

#ifndef PLAT4M_COM_PROTOCOL_PLAT4M_ASCII_H
#define PLAT4M_COM_PROTOCOL_PLAT4M_ASCII_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ComProtocol.h>
#include <ErrorTemplate.h>
#include <Callback.h>
#include <ByteArray.h>
#include <ByteArrayN.h>
#include <Message.h>
#include <AsciiMessageHandler.h>
#include <List.h>
#include <ArrayN.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ComProtocolPlat4mAscii : public ComProtocol
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
    
    ComProtocolPlat4mAscii(ComLink& comLink, const bool addNewLine = true);
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    virtual ~ComProtocolPlat4mAscii();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    void addMessageHandler(AsciiMessageHandler& messageHandler);

    void getMessageStorage(AsciiMessage& message);

    ByteArray& messageToByteArray(AsciiMessage& message);

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    bool myAddNewLine;

    ByteArrayN<256> myRawMessageByteArray;

    ByteArrayN<40> myMessageNameStorage;

    uint8_t myParameterNameStorage[10][40];

    ArrayN<ByteArray, 10> myParameterNameStorageArray;

    uint8_t myParameterValueStorage[10][40];

    ArrayN<ByteArray, 10> myParameterValueStorageArray;

    AsciiMessage myAsciiMessageTemplate;

    List<AsciiMessageHandler*> myMessageHandlerList;

    //--------------------------------------------------------------------------
    // Private methods implemented from ComProtocol
    //--------------------------------------------------------------------------

	ParseStatus driverParseData(const ByteArray& rxByteArray,
	                            ByteArray& txByteArray,
	                            Callback<>*& followUpCallback);

};

}; // namespace Plat4m

#endif // PLAT4M_COM_PROTOCOL_PLAT4M_ASCII_H
