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
// Copyright (c) 2019-2024 Benjamin Minerd
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
/// @file QueueDriverPosix.cpp
/// @author Ben Minerd
/// @date 5/28/2019
/// @brief QueueDriverPosix class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <Plat4m_Core/Posix/QueueDriverPosix.h>
#include <Plat4m_Core/Posix/ThreadPosix.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverPosix::QueueDriverPosix(const std::uint32_t valueSizeBytes) :
    QueueDriver(),
    myValueSizeBytes(valueSizeBytes),
    myMessageQueue(),
    myName()
{
    static std::uint32_t messageQueueIndex = 0;

    std::snprintf(myName, sizeof(myName), "/PLAT4M_MQ_%u", messageQueueIndex);

    mq_attr messageQueueAttributes;
    messageQueueAttributes.mq_flags = 0;
    messageQueueAttributes.mq_msgsize = valueSizeBytes;
    messageQueueAttributes.mq_maxmsg = 10;
    messageQueueAttributes.mq_curmsgs = 0;

    // Unlink any stale message queues with the same name
    mq_unlink(myName);
    myMessageQueue = mq_open(myName,
                             O_CREAT | O_RDWR | O_EXCL,
                             0744,
                             &messageQueueAttributes);

    if (myMessageQueue == -1)
    {
        PLAT4M_REPORT_ERROR(QueueDriver::Error,
                            QueueDriver::ERROR_CODE_INSTANTIATION_FAILED,
                            ErrorBase::SEVERITY_CRITICAL,
                            this);
    }

    mq_getattr(myMessageQueue, &messageQueueAttributes);

    messageQueueIndex++;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
QueueDriverPosix::~QueueDriverPosix()
{
    mq_close(myMessageQueue);
    mq_unlink(myName);
}

//------------------------------------------------------------------------------
// Public virtual methods overridden for QueueDriver
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::uint32_t QueueDriverPosix::driverGetSize()
{
    mq_attr messageQueueAttributes;

    mq_getattr(myMessageQueue, &messageQueueAttributes);

    return (messageQueueAttributes.mq_curmsgs);
}

//------------------------------------------------------------------------------
std::uint32_t QueueDriverPosix::driverGetSizeFast()
{
    return driverGetSize();
}

//------------------------------------------------------------------------------
bool QueueDriverPosix::driverEnqueue(const void* value)
{
    mq_attr messageQueueAttributes;

    mq_getattr(myMessageQueue, &messageQueueAttributes);

    int returnValue = mq_send(myMessageQueue,
                              static_cast<const char*>(value),
                              myValueSizeBytes,
                              10);

    return (returnValue == 0);
}

//------------------------------------------------------------------------------
bool QueueDriverPosix::driverEnqueueFast(const void* value)
{
    return (driverEnqueue(value));
}

//-----------------------------------------------------------------------------
bool QueueDriverPosix::driverDequeue(void* value)
{
    mq_attr messageQueueAttributes;

    mq_getattr(myMessageQueue, &messageQueueAttributes);

    unsigned int priority = 0;

    int returnValue = mq_receive(myMessageQueue,
                                 static_cast<char*>(value),
                                 myValueSizeBytes,
                                 &priority);

    return (returnValue != -1);
}

//------------------------------------------------------------------------------
bool QueueDriverPosix::driverDequeueFast(void* value)
{
    return (driverDequeue(value));
}

//------------------------------------------------------------------------------
void QueueDriverPosix::driverClear()
{
    mq_attr messageQueueAttributes;

    mq_getattr(myMessageQueue, &messageQueueAttributes);

    while (messageQueueAttributes.mq_curmsgs > 0)
    {
        char dummy[myValueSizeBytes];

        unsigned int priority = 0;

        mq_receive(myMessageQueue, dummy, myValueSizeBytes, &priority);

        mq_getattr(myMessageQueue, &messageQueueAttributes);
    }
}
