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
// Copyright (c) 2016-2023 Benjamin Minerd
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
/// @file DmaSTM32F4xx.h
/// @author Ben Minerd
/// @date 9/15/2016
/// @brief DmaSTM32F4xx class header file.
///

#ifndef PLAT4M_DMA_STM32F4XX_H
#define PLAT4M_DMA_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <stm32f4xx.h>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/STM32F4xx/ProcessorSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class DmaSTM32F4xx : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0
    };

    enum Id
    {
        ID_1 = 0,
        ID_2
    };

    enum StreamId
    {
        STREAM_ID_1 = 0,
        STREAM_ID_2,
        STREAM_ID_3,
        STREAM_ID_4,
        STREAM_ID_5,
        STREAM_ID_6,
        STREAM_ID_7
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_DIRECT_MODE_ERROR = 0,
        INTERRUPT_EVENT_TRANSFER_ERROR,
        INTERRUPT_EVENT_HALF_TRANSFER,
        INTERRUPT_EVENT_TRANSFER_COMPLETE,
        INTERRUPT_EVENT_FIFO_OVERRUN_UNDERRUN
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
        int a;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    DmaSTM32F4xx(const Id id);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId() const;

    DMA_TypeDef* getDma();

    Error setConfig(const Config& config);

    bool isStreamInterruptEventPending(const StreamId streamId,
                                       const InterruptEvent interruptEvent);

    void clearStreamInterruptEvent(const StreamId streamId,
                                   const InterruptEvent interruptEvent);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F4xx::Peripheral myPeripheralMap[];

    // Variables

    static DMA_TypeDef* myDmaMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    DMA_TypeDef* myDma;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;
};

}; // namespace Plat4m

#endif // PLAT4M_DMA_STM32F4XX_H
