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
/// @file ComLink.cpp
/// @author Ben Minerd
/// @date 4/12/2017
/// @brief ComLink class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <iostream>

#include <Plat4m_Core/ComLink.h>
#include <Plat4m_Core/ComProtocol.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/CallbackMethodParameter.h>

using Plat4m::ComLink;
using Plat4m::Module;
using Plat4m::Thread;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComLink::ComLink(ByteArray& transmitByteArray,
                 ByteArray& receiveByteArray,
                 ComInterfaceDevice& comInterfaceDevice) :
    Module(),
    myTransmitByteArray(transmitByteArray),
    myReceiveByteArray(receiveByteArray),
    myComInterfaceDevice(comInterfaceDevice),
    myComProtocolList(),
    myCurrentComProtocol(0),
    myCurrentComProtocolTimeoutTimeMs(0),
    myDataParsingThread(System::createThread(
                          createCallback(this,
                                         &ComLink::dataParsingThreadCallback))),
    myWaitCondition(System::createWaitCondition(myDataParsingThread)),
    myMutex(System::createMutex(myDataParsingThread)),
	myReceiveByteQueue(System::createQueue<uint8_t>(128, myDataParsingThread))
{
    myDataParsingThread.setPriority(3);
}

//------------------------------------------------------------------------------
ComLink::ComLink(ByteArray& transmitByteArray,
                 ByteArray& receiveByteArray,
                 ComInterfaceDevice& comInterfaceDevice,
                 ComInterface& comInterface) :
    myTransmitByteArray(transmitByteArray),
    myReceiveByteArray(receiveByteArray),
    myComInterfaceDevice(comInterfaceDevice),
    myComProtocolList(),
    myCurrentComProtocol(0),
    myCurrentComProtocolTimeoutTimeMs(0),
    myWaitCondition(System::createWaitCondition(myDataParsingThread)),
    myMutex(System::createMutex(myDataParsingThread)),
	myReceiveByteQueue(System::createQueue<uint8_t>(128, myDataParsingThread)),
    myDataParsingThread(System::createThread(
                           createCallback(this,
                                          &ComLink::dataParsingThreadCallback)))
{
    myDataParsingThread.setPriority(3);

    comInterface.setByteReceivedCallback(
							    createCallback(this,
											   &ComLink::byteReceivedCallback));
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ComLink::~ComLink()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thread& ComLink::getDataParsingThread()
{
	return myDataParsingThread;
}

//------------------------------------------------------------------------------
void ComLink::setComInterface(ComInterface& comInterface)
{
    myComInterfaceDevice.setComInterface(comInterface);
}

//------------------------------------------------------------------------------
void ComLink::addComProtocol(ComProtocol& comProtocol)
{
    ComProtocol* pointer = &comProtocol;

    myComProtocolList.append(pointer);
}

//------------------------------------------------------------------------------
ComLink::Error ComLink::transmitBytes(const ByteArray& byteArray,
                                      const bool waitUntilDone)
{
    MutexLock mutexLock(myMutex);

    myComInterfaceDevice.transmitBytes(byteArray, waitUntilDone);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ComLink::driverEnable(const bool enable)
{
    myComInterfaceDevice.setEnabled(enable);
    myDataParsingThread.enable(enable);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ComLink::byteReceivedCallback(const uint8_t byte)
{
	if (myDataParsingThread.isEnabled())
	{
		myReceiveByteQueue.enqueueFast(byte);
	}
}

//------------------------------------------------------------------------------
void ComLink::dataParsingThreadCallback()
{
	uint8_t byte;
	myReceiveByteQueue.dequeue(byte);
	myReceiveByteArray.append(byte);

	uint32_t nBytes = myReceiveByteQueue.getSize();

	for (uint32_t i = 0; i < nBytes; i++)
	{
		myReceiveByteQueue.dequeue(byte);
		myReceiveByteArray.append(byte);
	}

    // Timeout
    if (isValidPointer(myCurrentComProtocol) &&
        System::checkTimeMs(myCurrentComProtocolTimeoutTimeMs))
    {
        myCurrentComProtocol = 0;
        myReceiveByteQueue.clear();
        myReceiveByteArray.clear();
    }

    if (myReceiveByteArray.getSize() == 0)
    {
        return;
    }

    if (isValidPointer(myCurrentComProtocol))
    {
        tryProtocol(myCurrentComProtocol);
    }
    else
    {
        List<ComProtocol*>::Iterator iterator = myComProtocolList.iterator();

        while (iterator.hasCurrent())
        {
            ComProtocol* comProtocol = iterator.current();
            tryProtocol(comProtocol);

            iterator.next();
        }

    }
}

//------------------------------------------------------------------------------
void ComLink::tryProtocol(ComProtocol* comProtocol)
{
    Callback<>* followUpCallback = 0;

    ComProtocol::ParseStatus parseStatus =
                                     comProtocol->parseData(myReceiveByteArray,
                                                            myTransmitByteArray,
                                                            followUpCallback);

    switch (parseStatus)
    {
        case ComProtocol::PARSE_STATUS_MID_FRAME:
        {
            if (isNullPointer(myCurrentComProtocol))
            {
                myCurrentComProtocol = comProtocol;
                myCurrentComProtocolTimeoutTimeMs =
                                      System::getTimeMs() +
                                      myCurrentComProtocol->getParseTimeoutMs();
            }

            break;
        }
        case ComProtocol::PARSE_STATUS_FOUND_FRAME:
        {
            if (myTransmitByteArray.getSize() > 0)
            {
                MutexLock mutexLock(myMutex);

                myComInterfaceDevice.transmitBytes(myTransmitByteArray);
                myTransmitByteArray.clear();
            }

            if (isValidPointer(followUpCallback))
            {
                followUpCallback->call();
            }

            myCurrentComProtocolTimeoutTimeMs = 0;
            myCurrentComProtocol = 0;
            myReceiveByteArray.clear();

            break;
        }
        default:
        {
            myCurrentComProtocol = 0;
            myReceiveByteArray.clear();

            break;
        }
    }
}
