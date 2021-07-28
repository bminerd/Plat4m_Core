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
// Copyright (c) 2021 Benjamin Minerd
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
/// @file AsciiMessageHandlerTemplate.h
/// @author Ben Minerd
/// @date 5/3/16
/// @brief AsciiMessageHandlerTemplate class header file.
///

#ifndef ASCII_MESSAGE_HANDLER_TEMPLATE_H
#define ASCII_MESSAGE_HANDLER_TEMPLATE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/ComProtocolPlat4m/AsciiMessage.h>
#include <Plat4m_Core/ComProtocolPlat4m/AsciiMessageHandler.h>
#include <Plat4m_Core/CallbackMethod2Parameters.h>
#include <Plat4m_Core/MemoryAllocator.h>

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
          class TAsciiMessageRequest,
          class TAsciiMessageResponse>
class AsciiMessageHandlerTemplate : public AsciiMessageHandler
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
    AsciiMessageHandlerTemplate(HandlerCallback& handlerCallback) :
        AsciiMessageHandler(),
        myHandlerCallback(handlerCallback),
        myRequest(),
        myResponse(),
        myAsciiRequest(myRequest),
        myAsciiResponse(myResponse)
    {
    }

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~AsciiMessageHandlerTemplate()
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual methods implemented from AsciiMessageHandler
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual bool handleMessage(const AsciiMessage& request,
                               AsciiMessage*& response)
    {
        if (!(myAsciiRequest.match(request)))
        {
            return false;
        }

        myAsciiRequest.setParameterValues(request.getParameterValues());
        myAsciiRequest.stringParametersUpdated();

        myHandlerCallback.call(myRequest, myResponse);

        request.getParameterValues()->clear();
        myAsciiResponse.setParameterValues(request.getParameterValues());
        myAsciiResponse.messageParametersUpdated();

        response = &myAsciiResponse;

        return true;
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    HandlerCallback& myHandlerCallback;

    TMessageRequest myRequest;

    TMessageResponse myResponse;

    TAsciiMessageRequest myAsciiRequest;

    TAsciiMessageResponse myAsciiResponse;
};

//------------------------------------------------------------------------------
// Namespace functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template <class TClass,
          class TRequestMessage,
          class TResponseMessage,
          class TRequestAsciiMessage,
          class TResponseAsciiMessage>
static AsciiMessageHandler& createMessageHandler(
        TClass* object,
        void (TClass::*callback)(const TRequestMessage&, TResponseMessage&))
{
    return *(MemoryAllocator::allocate<
        AsciiMessageHandlerTemplate<TRequestMessage,
                                    TResponseMessage,
                                    TRequestAsciiMessage,
                                    TResponseAsciiMessage>>(
                                            createCallback(object, callback)));
}

}; // namespace Plat4m

#endif // ASCII_MESSAGE_HANDLER_TEMPLATE_H
