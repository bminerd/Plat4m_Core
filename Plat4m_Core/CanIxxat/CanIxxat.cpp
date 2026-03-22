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
// Copyright (c) 2024 Benjamin Minerd
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
/// @file CanIxxat.cpp
/// @author Ben Minerd
/// @date 7/12/2024
/// @brief CanIxxat class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <vci.h>
#include <vci3.h>
#include <vcierr.h>
#include <vcinpl.h>

#include <Plat4m_Core/CanIxxat/CanIxxat.h>
#include <Plat4m_Core/System.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanIxxat::CanIxxat() :
    Can(),
    myPortHandle(0),
    myChannelHandle(0),
    myReceiveThread(System::createThread(
                         createCallback(this, &CanIxxat::receiveThreadCallback),
                         0,
                         0,
                         false,
                         "CanIxxat Receive Thread")),
    myIsInitialConfigurationSet(false)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanIxxat::~CanIxxat()
{
    myReceiveThread.~Thread();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanIxxat::Error CanIxxat::setCanIxxatConfig(const CanIxxatConfig& config)
{
    HRESULT result;

    result = canChannelInitialize(myChannelHandle,
                                  config.rxFifoSize,
                                  config.rxThreshold,
                                  config.txFifoSize,
                                  config.txThreshold);

    if (result != VCI_OK)
    {
        return (PLAT4M_REPORT_ERROR(
                             CanIxxat::Error,
                             CanIxxat::ERROR_CODE_CHANNEL_INITIALIZATION_FAILED,
                             ErrorBase::SEVERITY_HIGH,
                             this));
    }

    result = canChannelActivate(myChannelHandle, TRUE);

    if (result != VCI_OK)
    {
        // Need new error?
        return (PLAT4M_REPORT_ERROR(
                             CanIxxat::Error,
                             CanIxxat::ERROR_CODE_CHANNEL_INITIALIZATION_FAILED,
                             ErrorBase::SEVERITY_HIGH,
                             this));
    }

    myIsInitialConfigurationSet = true;

    myReceiveThread.enable();

    return (Error(ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
// Protected virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error CanIxxat::subclassSetEnabled(const bool enabled)
{
    if (enabled)
    {
        HRESULT result;

        HWND hMain = GetConsoleWindow();
        result = VCI_OpenDevice(&myPortHandle, hMain);

        if (result != VCI_OK)
        {
            return (PLAT4M_REPORT_ERROR(Module::Error,
                                        Module::ERROR_CODE_ENABLE_FAILED,
                                        ErrorBase::SEVERITY_HIGH,
                                        this));
        }
    }
    else
    {
        if (isValidPointer(myChannelHandle))
        {
            canChannelClose(myChannelHandle);
        }

        if (isValidPointer(myPortHandle))
        {
            // VCI_ResetControl();
            VCI_Close();
        }
    }

    if (myIsInitialConfigurationSet)
    {
        myReceiveThread.setEnabled(enabled);
    }

    return (Module::Error(Module::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
// Protected virtual methods overridden for Can
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Error CanIxxat::subclassSetConfig(const Config& config)
{
    HRESULT result;

    result = VCI_ConfigureBaud(myPortHandle, config.baudRate);

    if ((result != VCI_OK) && (result != VCI_E_ACCESSDENIED))
    {
        return (PLAT4M_REPORT_ERROR(Can::Error,
                                    Can::ERROR_CODE_BAUD_RATE_INVALID,
                                    ErrorBase::SEVERITY_HIGH,
                                    this));
    }

    result = canChannelOpen(myPortHandle, 0, FALSE, &myChannelHandle);

    if (result != VCI_OK)
    {
        // Need new error?
        return (PLAT4M_REPORT_ERROR(Can::Error,
                                    Can::ERROR_CODE_PARAMETER_INVALID,
                                    ErrorBase::SEVERITY_HIGH,
                                    this));
    }

    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
Can::Error CanIxxat::subclassSendMessage(const Message& message)
{
    CANMSG txMsg;
    HRESULT result;

    txMsg.uMsgInfo.Bytes.bType = CAN_MSGTYPE_DATA;
    txMsg.uMsgInfo.Bytes.bAddFlags = 0;
    txMsg.uMsgInfo.Bytes.bFlags = message.data.getSize();

    if ((message.id) & 0x80000000)
    {
        txMsg.uMsgInfo.Bytes.bFlags |= CAN_MSGFLAGS_EXT;
    }

    txMsg.uMsgInfo.Bytes.bFlags |= CAN_MSGFLAGS_SRR; // Makes messages visible in MiniMon
    txMsg.uMsgInfo.Bytes.bAccept = 0;
    txMsg.dwTime = 0;
    txMsg.dwMsgId = (message.id) & 0x1FFFFFFF;

    for (std::uint32_t i = 0; i < message.data.getSize(); i++)
    {
        txMsg.abData[i] = message.data[i];
    }

    result = canChannelSendMessage(myChannelHandle, 0, &txMsg);

    if (result != VCI_OK)
    {
//        tx_failed_count++;
    }

    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
Can::Error CanIxxat::subclassAddAcceptanceFilter(const IdType idType,
                                                 const std::uint32_t filter,
                                                 const std::uint32_t mask)
{
    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
Can::Error CanIxxat::subclassClearReceivedMessages()
{
    HRESULT result;
    CANMSG msg;
    unsigned cnt = 0;

    do
    {
        result = canChannelPeekMessage(myChannelHandle, NULL);
    } while (result == VCI_OK);

    /* For older versions of the VCI. */
    /* Older versions of the VCI would return an invalid argument error (they wouldn't accept the null pointer) */
    /* For those, we check each message, and then remove it from the queue. */
    if (result == VCI_E_INVALIDARG)
    {
        do
        {
            result = canChannelPeekMessage(myChannelHandle, &msg);

            if (result == VCI_OK)
            {
                result = canChannelReadMessage(myChannelHandle, 0, &msg);
            }
        } while (result == VCI_OK);
    }

    if ((result == VCI_E_RXQUEUE_EMPTY) || (result == VCI_E_TIMEOUT))
    {
        result = VCI_OK;
    }

    return (Can::Error(Can::ERROR_CODE_NONE));
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void CanIxxat::receiveThreadCallback()
{
    CANMSG rxMsg;
    HRESULT result;
    const std::uint32_t timeoutMsec = 0xFFFFFFFF; // Infinite

    result = canChannelReadMessage(myChannelHandle, timeoutMsec, &rxMsg);

    if (result == VCI_OK)
    {
        if (rxMsg.uMsgInfo.Bytes.bType == CAN_MSGTYPE_DATA)
        {
            Message message;
            message.frameType = Can::FRAME_TYPE_DATA;

            message.id = rxMsg.dwMsgId & 0x1FFFFFFF;

            if (rxMsg.uMsgInfo.Bytes.bFlags & CAN_MSGFLAGS_EXT)
            {
                message.idType = ID_TYPE_EXTENDED;
                message.id |= 0x80000000;
            }
            else
            {
                message.idType = ID_TYPE_BASE;
            }

            std::uint32_t length = rxMsg.uMsgInfo.Bytes.bFlags & 0xF;

            for (std::uint32_t i = 0; i < length; i++)
            {
                message.data.append(rxMsg.abData[i]);
            }

            messageReceived(message);
            handleReceivedMessages();
        }
    }
}
