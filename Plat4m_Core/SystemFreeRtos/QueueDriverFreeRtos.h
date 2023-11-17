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
// Copyright (c) 2018-2023 Benjamin Minerd
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
/// @file QueueDriverFreeRtos.h
/// @author Ben Minerd
/// @date 1/17/2018
/// @brief QueueDriverFreeRtos class header file.
///

#ifndef PLAT4M_QUEUE_DRIVER_FREE_RTOS_H
#define PLAT4M_QUEUE_DRIVER_FREE_RTOS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <FreeRTOS-Kernel/include/FreeRTOS.h>
#include <FreeRTOS-Kernel/include/queue.h>

#include <Plat4m_Core/QueueDriver.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <uint32_t nBytes>
class QueueDriverFreeRtos : public QueueDriver
{
public:

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    QueueDriverFreeRtos(const uint32_t nValues, const uint32_t valueSizeBytes) :
        QueueDriver(),
        myQueueHandle(xQueueCreate(nValues, valueSizeBytes))
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~QueueDriverFreeRtos()
    {
    }

    //--------------------------------------------------------------------------
    // Public virtual methods overriden for QueueDriver
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual uint32_t driverGetSize() override
    {
        return ((uint32_t) uxQueueMessagesWaiting(myQueueHandle));
    }

    //--------------------------------------------------------------------------
    virtual uint32_t driverGetSizeFast() override
    {
        return ((uint32_t) uxQueueMessagesWaitingFromISR(myQueueHandle));
    }

    //--------------------------------------------------------------------------
    virtual bool driverEnqueue(const void* value) override
    {
        return ((bool) xQueueSendToBack(myQueueHandle, value, 0));
    }

    //--------------------------------------------------------------------------
    virtual bool driverEnqueueFast(const void* value) override
    {
        return ((bool) xQueueSendToBackFromISR(myQueueHandle, value, 0));
    }

    //--------------------------------------------------------------------------
    virtual bool driverDequeue(void* value) override
    {
        return ((bool) xQueueReceive(myQueueHandle, value, 0xFFFFFFFF));
    }

    //--------------------------------------------------------------------------
    virtual bool driverDequeueFast(void* value) override
    {
        BaseType_t xTaskWokenByReceive = pdFALSE;

        return ((bool) xQueueReceiveFromISR(myQueueHandle,
                                            value,
                                            &xTaskWokenByReceive));
    }

    //--------------------------------------------------------------------------
    virtual void driverClear() override
    {
        xQueueReset(myQueueHandle);
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    QueueHandle_t myQueueHandle;
};

}; // namespace Plat4m

#endif // PLAT4M_QUEUE_DRIVER_FREE_RTOS_H
