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
// Copyright (c) 2019-2023 Benjamin Minerd
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
/// @file QueueDriverLinux.cpp
/// @author Ben Minerd
/// @date 5/28/2019
/// @brief QueueDriverLinux class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstring>

#include <sys/ipc.h>
#include <sys/msg.h>

#include <Plat4m_Core/Linux/QueueDriverLinux.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>

using Plat4m::QueueDriverLinux;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverLinux::QueueDriverLinux(const uint32_t valueSizeBytes) :
    QueueDriver(),
    myValueSizeBytes(valueSizeBytes),
    myMessageQueueId(msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL))
{
    if (myMessageQueueId < 0)
    {
        while (true)
        {
            // Lock up
        }
    }
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverLinux::~QueueDriverLinux()
{
}

//------------------------------------------------------------------------------
// Public virtual methods overridden for QueueDriver
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t QueueDriverLinux::driverGetSize()
{
    struct msqid_ds messageQueueInfo;

    msgctl(myMessageQueueId, IPC_STAT, &messageQueueInfo);

    return (messageQueueInfo.msg_qnum);
}

//------------------------------------------------------------------------------
uint32_t QueueDriverLinux::driverGetSizeFast()
{
    return driverGetSize();
}

//------------------------------------------------------------------------------
bool QueueDriverLinux::driverEnqueue(const void* value)
{
    Message message;
    message.messageType = MESSAGE_TYPE_DATA;
    message.value       = (void*) value;

    return messageSend(message);
}

//------------------------------------------------------------------------------
bool QueueDriverLinux::driverEnqueueFast(const void* value)
{
    return (driverEnqueue(value));
}

//-----------------------------------------------------------------------------
bool QueueDriverLinux::driverDequeue(void* value)
{
    Message message;
    message.value = value;

    return messageReceive(message);
}

//------------------------------------------------------------------------------
bool QueueDriverLinux::driverDequeueFast(void* value)
{
    return (driverDequeue(value));
}

//------------------------------------------------------------------------------
void QueueDriverLinux::driverClear()
{
    Message message;
    message.messageType = MESSAGE_TYPE_FLUSH;
    message.value       = 0;

    messageSend(message);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool QueueDriverLinux::messageSend(const Message& message)
{
    uint8_t messageBytes[sizeof(message.messageType) + myValueSizeBytes];
    memset(messageBytes, 0, sizeof(messageBytes));

    memcpy(&(messageBytes[0]),
           &(message.messageType),
           sizeof(message.messageType));

    if (isValidPointer(message.value))
    {
        memcpy(&(messageBytes[sizeof(message.messageType)]),
               message.value,
               myValueSizeBytes);
    }

    return msgsnd(myMessageQueueId, messageBytes, myValueSizeBytes, 1);
}

//------------------------------------------------------------------------------
bool QueueDriverLinux::messageReceive(Message& message)
{
    uint8_t messageBytes[sizeof(message.messageType) + myValueSizeBytes];
    memset(messageBytes, 0, sizeof(messageBytes));

    msgrcv(myMessageQueueId, messageBytes, myValueSizeBytes, 0, 0);

    memcpy(&(message.messageType),
           &(messageBytes[0]),
           sizeof(message.messageType));

    if (message.messageType == MESSAGE_TYPE_FLUSH)
    {
        return false;
    }

    memcpy(message.value,
           &(messageBytes[sizeof(message.messageType)]),
           myValueSizeBytes);

    return true;
}
