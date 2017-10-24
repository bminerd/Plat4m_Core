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
/// @file Can.h
/// @author Ben Minerd
/// @date 1/4/2016
/// @brief Can class header file.
///

#ifndef CAN_H
#define CAN_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <ByteArrayN.h>
#include <Callback.h>
#include <List.h>
#include <BufferN.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Can : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    /**
     * @brief Enumeration of CAN errors.
     */
    enum ErrorCode
    {
        ERROR_CODE_NONE,              		   /// No error.
        ERROR_CODE_PARAMETER_INVALID, 		   /// Parameter is invalid.
        ERROR_CODE_NOT_ENABLED,       		   /// CAN is not enabled.
		ERROR_CODE_BAUD_RATE_INVALID, 		   /// Baud rate is invalid.
        ERROR_CODE_TX_BUFFER_FULL,    		   /// CAN transmit buffer is full.
		ERROR_CODE_ACCEPTANCE_FILTER_LIST_FULL ///
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

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------
    
    ///
    /// @brief CAN configuration structure.
    ///
    struct Config
    {
    	uint32_t baudRate;
    	unsigned int timeQuantaSyncJumpWidth;
    	unsigned int timeQuantaBitSegment1;
    	unsigned int timeQuantaBitSegment2;
    	bool timeTriggeredCommunicationEnabled;
    };
    
    struct Message
    {
    	IdType idType;
    	uint32_t id;
    	FrameType frameType;
    	ByteArrayN<8> data;
    };
    
    typedef Callback<void, const Message&> ReceivedMessageCallback;

    struct ReceivedMessageHandler
    {
    	IdType idType;
		uint32_t id;
		uint32_t mask;
    	ReceivedMessageCallback* receivedMessageCallback;
    };

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    ///
    /// @brief Configures this CAN object with the given configuration
    /// structure.
    /// @return Error with a code: 1) ERROR_CODE_NONE if configuration was
    /// successful, 2) ERROR_CODE_NOT_ENABLED if ::enable(true) method wasn't
    /// previously called...
    ///
    virtual Error setConfig(const Config& config);
    
    virtual Error addReceivedMessageHandler(
    						  const IdType idType,
    				          const uint32_t id,
							  const uint32_t mask,
			                  ReceivedMessageCallback& receivedMessageCallback);

    virtual Error addReceivedMessageHandler(
    					        ReceivedMessageHandler& receivedMessageHandler);

    virtual Error sendMessage(const IdType idType,
                              const uint32_t id,
                              const FrameType frameType,
                              ByteArrayN<8>& data);

    virtual Error sendMessage(const Message& message);

    virtual void handleReceivedMessages();

protected:
    
    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------
    
    /**
     * @brief Constructor for Can.
     */
    Can();
    
    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Can();

    //--------------------------------------------------------------------------
    // Protected methods
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
    // Private pure virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverSendMessage(const Message& message) = 0;

    virtual Error driverAddAcceptanceFilter(const IdType idType,
    										const uint32_t filter,
    										const uint32_t mask) = 0;
};

}; // namespace Plat4m

#endif // CAN_H
