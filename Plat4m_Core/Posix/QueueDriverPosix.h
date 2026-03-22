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
/// @file QueueDriverPosix.h
/// @author Ben Minerd
/// @date 5/28/2019
/// @brief QueueDriverPosix class header file.
///

#ifndef PLAT4M_QUEUE_DRIVER_POSIX_H
#define PLAT4M_QUEUE_DRIVER_POSIX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>
#include <mqueue.h>

#include <Plat4m_Core/QueueDriver.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Thread.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class QueueDriverPosix : public QueueDriver
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    QueueDriverPosix(const std::uint32_t valueSizeBytes);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~QueueDriverPosix();

    //--------------------------------------------------------------------------
    // Public virtual methods overridden for QueueDriver
    //--------------------------------------------------------------------------

    std::uint32_t driverGetSize();

    std::uint32_t driverGetSizeFast();

    bool driverEnqueue(const void* value);

    bool driverEnqueueFast(const void* value);

    bool driverDequeue(void* value);

    bool driverDequeueFast(void* value);

    void driverClear();

private:

    //--------------------------------------------------------------------------
    // Private types
    //--------------------------------------------------------------------------

    enum MessageType : std::uint8_t
    {
        MESSAGE_TYPE_DATA = 0,
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

    const std::uint32_t myValueSizeBytes;

    mqd_t myMessageQueue;

    char myName[20];
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_DRIVER_POSIX_H
