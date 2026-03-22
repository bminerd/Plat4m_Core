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
/// @file Can.h
/// @author Ben Minerd
/// @date 1/4/2016
/// @brief Can class header file.
///

#ifndef PLAT4M_CAN_H
#define PLAT4M_CAN_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/ComInterface.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/ByteArrayN.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/BufferN.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Can : public ComInterface
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_BAUD_RATE_INVALID,
        ERROR_CODE_TX_BUFFER_FULL,
        ERROR_CODE_ACCEPTANCE_FILTER_LIST_FULL
    };

    enum IdType
    {
        ID_TYPE_BASE = 0, /// 11-bit identifier
        ID_TYPE_EXTENDED  /// 29-bit identifier
    };

    enum FrameType
    {
        FRAME_TYPE_DATA = 0,
        FRAME_TYPE_REMOTE
    };

    //--------------------------------------------------------------------------
    // Public typedefs
    //--------------------------------------------------------------------------

    using Error = ErrorTemplate<ErrorCode>;

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------

    struct Config
    {
        std::uint32_t baudRate;
        unsigned int timeQuantaSyncJumpWidth;
        unsigned int timeQuantaBitSegment1;
        unsigned int timeQuantaBitSegment2;
        bool timeTriggeredCommunicationEnabled;
    };

    struct Message
    {
        IdType idType;
        std::uint32_t id;
        FrameType frameType;
        ByteArrayN<8> data;
    };

    using ReceivedMessageCallback = Callback<void, const Message&>;

    struct ReceivedMessageHandler
    {
        IdType idType;
        std::uint32_t id;
        std::uint32_t mask;
        ReceivedMessageCallback* receivedMessageCallback;
    };

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for ComInterface
    //--------------------------------------------------------------------------

    virtual ComInterface::Error transmitBytes(
                                      const ByteArray& byteArray,
                                      const bool waitUntilDone = true) override;

    virtual std::uint32_t getReceivedBytesCount() override;

    virtual ComInterface::Error getReceivedBytes(
                                       ByteArray& byteArray,
                                       const std::uint32_t nBytes = 0) override;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
 
    Error setConfig(const Config& config);

    Config getConfig() const;

    Error addReceivedMessageHandler(
                              const IdType idType,
                              const std::uint32_t mask,
                              const std::uint32_t id,
                              ReceivedMessageCallback& receivedMessageCallback);

    Error addReceivedMessageHandler(
                                ReceivedMessageHandler& receivedMessageHandler);

    Error sendMessage(const IdType idType,
                      const std::uint32_t id,
                      const FrameType frameType,
                      ByteArray& data);

    Error sendMessage(const Message& message);

    Error clearReceivedMessages();

    void handleReceivedMessages();

    void enableAllMessagesHandler();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    Can();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Can();

    //--------------------------------------------------------------------------
    // Protected pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error subclassSetConfig(const Config& config) = 0;

    virtual Error subclassSendMessage(const Message& message) = 0;

    virtual Error subclassAddAcceptanceFilter(const IdType idType,
                                              const std::uint32_t filter,
                                              const std::uint32_t mask) = 0;

    virtual Error subclassClearReceivedMessages() = 0;

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void messageReceived(Message& message);

    //--------------------------------------------------------------------------
    // Protected methods (deprecated)
    //--------------------------------------------------------------------------

    void interfaceMessageReceived(Message& message);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    List<ReceivedMessageHandler> myReceivedMessageHandlerList;

    BufferN<Message, 16> myTransmitMessageBuffer;

    BufferN<Message, 16> myReceivedMessageBuffer;

    //--------------------------------------------------------------------------
    // Private virtual methods (deprecated)
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config);

    virtual Error driverSendMessage(const Message& message);

    virtual Error driverAddAcceptanceFilter(const IdType idType,
                                            const std::uint32_t filter,
                                            const std::uint32_t mask);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void allMessagesCallback(const Message& message);
};

}; // namespace Plat4m

#endif // PLAT4M_CAN_H
