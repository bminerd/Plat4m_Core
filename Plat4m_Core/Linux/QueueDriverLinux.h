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
/// @file QueueDriverLinux.h
/// @author Ben Minerd
/// @date 5/28/2019
/// @brief QueueDriverLinux class header file.
///

#ifndef PLAT4M_QUEUE_DRIVER_LINUX_H
#define PLAT4M_QUEUE_DRIVER_LINUX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>

#include <Plat4m_Core/QueueDriver.h>
#include <Plat4m_Core/Thread.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class QueueDriverLinux : public QueueDriver
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    QueueDriverLinux(const uint32_t valueSizeBytes);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~QueueDriverLinux();

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for QueueDriver
    //--------------------------------------------------------------------------

    uint32_t driverGetSize();

    uint32_t driverGetSizeFast();

    bool driverEnqueue(const void* value);

    bool driverEnqueueFast(const void* value);

    bool driverDequeue(void* value);

    bool driverDequeueFast(void* value);

    void driverClear();

private:

    //--------------------------------------------------------------------------
    // Private types
    //--------------------------------------------------------------------------

    enum MessageType : long
    {
        MESSAGE_TYPE_DATA = 1,
        MESSAGE_TYPE_FLUSH
    };

    struct Message
    {
        MessageType messageType;
        void* value;
    };

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const uint32_t myValueSizeBytes;

    int myMessageQueueId;

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    bool messageSend(const Message& message);

    bool messageReceive(Message& message);
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_DRIVER_LINUX_H
