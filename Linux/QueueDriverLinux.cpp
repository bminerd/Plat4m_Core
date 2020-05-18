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
// Copyright (c) 2019 Benjamin Minerd
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

#include <sys/ipc.h>
#include <sys/msg.h>

#include <Plat4m_Core/Linux/QueueDriverLinux.h>
#include <Plat4m_Core/Linux/ThreadLinux.h>

using Plat4m::QueueDriverLinux;

struct Message
{
    long mtype;     /* Message type. */
    uint8_t value;  /* Message text. */
};

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
// Public methods implemented from QueueDriver
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t QueueDriverLinux::driverGetSize()
{
    struct msqid_ds messageQueueInfo;

    int returnValue = msgctl(myMessageQueueId, IPC_STAT, &messageQueueInfo);

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
    message.mtype = 1;
    message.value = *(uint8_t*) value;

    return msgsnd(myMessageQueueId, &message, sizeof(message), 1);
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

    msgrcv(myMessageQueueId, &message, sizeof(message), 1, 0);

    *(uint8_t*) value = message.value;

    return true;
}

//------------------------------------------------------------------------------
bool QueueDriverLinux::driverDequeueFast(void* value)
{
    return (driverDequeue(value));
}

//------------------------------------------------------------------------------
void QueueDriverLinux::driverClear()
{
    // Read out all messages and dump
}
