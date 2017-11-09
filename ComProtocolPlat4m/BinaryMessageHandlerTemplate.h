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
/// @file BinaryMessageHandlerTemplate.h
/// @author Ben Minerd
/// @date 5/24/2016
/// @brief BinaryMessageHandlerTemplate class header file.
///

#ifndef PLAT4M_BINARY_MESSAGE_HANDLER_TEMPLATE_H
#define PLAT4M_BINARY_MESSAGE_HANDLER_TEMPLATE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <ByteArray.h>
#include <Callback.h>
#include <CallbackMethodParameter.h>
#include <CallbackMethod2Parameters.h>
#include <BinaryMessage.h>
#include <ComProtocol.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <class TMessageRequest,
          class TMessageResponse,
          class TBinaryMessageRequest,
          class TBinaryMessageResponse>
class BinaryMessageHandlerTemplate : public BinaryMessageHandler
{
public:

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef Callback<void, const TMessageRequest&, TMessageResponse&>
                                                                HandlerCallback;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    BinaryMessageHandlerTemplate(HandlerCallback& handlerCallback) :
        BinaryMessageHandler(),
        myHandlerCallback(handlerCallback),
        myRequest(),
        myResponse(),
        myBinaryRequest(myRequest),
        myBinaryResponse(myResponse)
    {
    }

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~BinaryMessageHandlerTemplate()
    {
    }

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ComProtocol::ParseStatus handleMessage(
                                      const BinaryMessage& requestBinaryMessage,
                                      BinaryMessage*& responseBinaryMessage)
    {
        if (requestBinaryMessage.getMessageId() !=
                myBinaryRequest.getMessageId())
        {
            return ComProtocol::PARSE_STATUS_UNSUPPORTED_FRAME;
        }

        if (!(myBinaryRequest.parseMessageData(requestBinaryMessage.getData())))
        {
            return ComProtocol::PARSE_STATUS_MID_FRAME;
        }

        myHandlerCallback.call(myRequest, myResponse);

        responseBinaryMessage = &myBinaryResponse;

        return ComProtocol::PARSE_STATUS_FOUND_FRAME;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    HandlerCallback& myHandlerCallback;

    TMessageRequest myRequest;
    TMessageResponse myResponse;
    TBinaryMessageRequest myBinaryRequest;
    TBinaryMessageResponse myBinaryResponse;
};

template <class TMessage,
          class TBinaryMessage>
class BinaryMessageHandlerTemplate<TMessage,
                                   void*,
                                   TBinaryMessage,
                                   void*> : public BinaryMessageHandler
{
public:

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    typedef Callback<void, const TMessage&> HandlerCallback;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    BinaryMessageHandlerTemplate(HandlerCallback& handlerCallback) :
        BinaryMessageHandler(),
        myHandlerCallback(handlerCallback),
        myMessage(),
        myBinaryMessage(myMessage)
    {
    }

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~BinaryMessageHandlerTemplate()
    {
    }

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ComProtocol::ParseStatus handleMessage(
                                             const BinaryMessage& binaryMessage,
                                             BinaryMessage*& dummyBinaryMessage)
    {
        if (binaryMessage.getMessageId() != myBinaryMessage.getMessageId())
        {
            return ComProtocol::PARSE_STATUS_UNSUPPORTED_FRAME;;
        }

        if (!(myBinaryMessage.parseMessageData(binaryMessage.getData())))
        {
            return ComProtocol::PARSE_STATUS_MID_FRAME;
        }

        myHandlerCallback.call(myMessage);

        return ComProtocol::PARSE_STATUS_FOUND_FRAME;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    HandlerCallback& myHandlerCallback;

    TMessage myMessage;
    TBinaryMessage myBinaryMessage;
};


//------------------------------------------------------------------------------
// Namespace functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template <class TClass,
          class TMessageRequest,
          class TMessageResponse,
          class TBinaryMessageRequest,
          class TBinaryMessageResponse>
static BinaryMessageHandler& createMessageHandler(
        TClass* object,
        void (TClass::*callback)(const TMessageRequest&, TMessageResponse&))
{
    return *(new BinaryMessageHandlerTemplate<TMessageRequest,
                                              TMessageResponse,
                                              TBinaryMessageRequest,
                                              TBinaryMessageResponse>(
                                             createCallback(object, callback)));
}

//------------------------------------------------------------------------------
template <class TClass,
          class TMessage,
          class TBinaryMessage>
static BinaryMessageHandler& createMessageHandler(
        TClass* object,
        void (TClass::*callback)(const TMessage&))
{
    return *(new BinaryMessageHandlerTemplate<TMessage,
                                              void*,
                                              TBinaryMessage,
                                              void*>(
                                             createCallback(object, callback)));
}

}; // namespace Plat4m

#endif // PLAT4M_BINARY_MESSAGE_HANDLER_TEMPLATE_H
