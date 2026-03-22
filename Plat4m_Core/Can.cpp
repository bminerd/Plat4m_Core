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
// Copyright (c) 2016-2024 Benjamin Minerd
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
/// @file Can.cpp
/// @author Ben Minerd
/// @date 1/4/2016
/// @brief Can class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Can.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/ByteArrayParser.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public virtual methods overridden for ComInterface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComInterface::Error Can::transmitBytes(const ByteArray& byteArray,
                                       const bool waitUntilDone)
{
    ByteArrayParser parser(byteArray,
                           ENDIAN_LITTLE,
                           ByteArrayParser::PARSE_DIRECTION_FORWARD);

    std::uint32_t canId = 0;

    parser.parse(canId);

    Message message;
    message.id = canId;
    message.idType = ID_TYPE_BASE;
    message.frameType = FRAME_TYPE_DATA;
    message.data.append(byteArray.subArray(4));

    Error error = sendMessage(message);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return (ComInterface::Error(ComInterface::ERROR_CODE_TRANSMIT_FAILED));
    }

    return (ComInterface::Error(ComInterface::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
std::uint32_t Can::getReceivedBytesCount()
{
    return 0;
}

//------------------------------------------------------------------------------
ComInterface::Error Can::getReceivedBytes(ByteArray& byteArray,
                                          const std::uint32_t nBytes)
{
    return (ComInterface::Error(ComInterface::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Error Can::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return (PLAT4M_REPORT_ERROR(Can::Error,
                                    Can::ERROR_CODE_NOT_ENABLED,
                                    ErrorBase::SEVERITY_LOW,
                                    this));
    }

    Error error = subclassSetConfig(config);
    
    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
Can::Config Can::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
Can::Error Can::addReceivedMessageHandler(
                               const IdType idType,
                               const std::uint32_t id,
                               const std::uint32_t mask,
                               ReceivedMessageCallback& receivedMessageCallback)
{
    ReceivedMessageHandler receivedMessageHandler;
    receivedMessageHandler.idType                  = idType;
    receivedMessageHandler.id                      = id;
    receivedMessageHandler.mask                    = mask;
    receivedMessageHandler.receivedMessageCallback = &receivedMessageCallback;

    return (addReceivedMessageHandler(receivedMessageHandler));
}

//------------------------------------------------------------------------------
Can::Error Can::addReceivedMessageHandler(
                                 ReceivedMessageHandler& receivedMessageHandler)
{
    Error error = driverAddAcceptanceFilter(receivedMessageHandler.idType,
                                            receivedMessageHandler.id,
                                            receivedMessageHandler.mask);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myReceivedMessageHandlerList.append(receivedMessageHandler);
    }

    return error;
}

//------------------------------------------------------------------------------
Can::Error Can::sendMessage(const IdType idType,
                            const std::uint32_t id,
                            const FrameType frameType,
                            ByteArray& data)
{
    Message message;
    message.idType    = idType;
    message.id        = id;
    message.frameType = frameType;
    message.data.append(data);

    return (sendMessage(message));
}

//------------------------------------------------------------------------------
Can::Error Can::sendMessage(const Message& message)
{
    if (!isEnabled())
    {
        return (PLAT4M_REPORT_ERROR(Can::Error,
                                    Can::ERROR_CODE_NOT_ENABLED,
                                    ErrorBase::SEVERITY_LOW,
                                    this));
    }

    return (subclassSendMessage(message));
}

//------------------------------------------------------------------------------
Can::Error Can::clearReceivedMessages()
{
    if (!isEnabled())
    {
        return (PLAT4M_REPORT_ERROR(Can::Error,
                                    Can::ERROR_CODE_NOT_ENABLED,
                                    ErrorBase::SEVERITY_LOW,
                                    this));
    }

    return (subclassClearReceivedMessages());
}

//------------------------------------------------------------------------------
void Can::handleReceivedMessages()
{
    while (!(myReceivedMessageBuffer.isEmpty()))
    {
        Message message;
        myReceivedMessageBuffer.read(message);

        bool wasHandlerFound = false;
        List<ReceivedMessageHandler>::Iterator iterator =
                                        myReceivedMessageHandlerList.iterator();

        while (iterator.hasCurrent() && !wasHandlerFound)
        {
            ReceivedMessageHandler& handler = iterator.current();

            if ((message.id & handler.mask) == (handler.id) ||
                (handler.id == 0))
            {
                handler.receivedMessageCallback->call(message);
                wasHandlerFound = true;
            }

            iterator.next();
        }

        if (!wasHandlerFound)
        {
            // Shouldn't have received this message

            break;
        }
    }
}

//------------------------------------------------------------------------------
void Can::enableAllMessagesHandler()
{
    Plat4m::Can::ReceivedMessageHandler handler;
    handler.id = 0;
    handler.idType = Plat4m::Can::ID_TYPE_BASE;
    handler.mask = 0xFFFFFFFF;
    handler.receivedMessageCallback =
                             &(createCallback(this, &Can::allMessagesCallback));

    addReceivedMessageHandler(handler);
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Can() :
    ComInterface(),
    myConfig(),
    myReceivedMessageHandlerList(),
    myTransmitMessageBuffer(),
    myReceivedMessageBuffer()
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::~Can()
{
}

//------------------------------------------------------------------------------
// Protected methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Can::messageReceived(Message& message)
{
    myReceivedMessageBuffer.write(message);
}

//------------------------------------------------------------------------------
// Protected methods (deprecated)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Can::interfaceMessageReceived(Message& message)
{
    messageReceived(message);
}

//------------------------------------------------------------------------------
// Private virtual methods (deprecated)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Error Can::driverSetConfig(const Config& config)
{
    return (subclassSetConfig(config));
}

//------------------------------------------------------------------------------
Can::Error Can::driverSendMessage(const Message& message)
{
    return (subclassSendMessage(message));
}

//------------------------------------------------------------------------------
Can::Error Can::driverAddAcceptanceFilter(const IdType idType,
                                          const std::uint32_t filter,
                                          const std::uint32_t mask)
{
    return (subclassAddAcceptanceFilter(idType, filter, mask));
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Can::allMessagesCallback(const Message& message)
{
    ByteArrayN<128> bytes;
    bytes.append(message.id);
    bytes.append(message.data);

    const std::uint32_t size = bytes.getSize();

    for (std::uint32_t i = 0; i < size; i++)
    {
        byteReceived(bytes[i]);
    }
}
