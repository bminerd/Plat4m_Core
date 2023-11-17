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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file BdmaSTM32H7xx.h
/// @author Ben Minerd
/// @date 8/3/2023
/// @brief BdmaSTM32H7xx class header file.
///

#ifndef PLAT4M_BDMA_STM32H7XX_H
#define PLAT4M_BDMA_STM32H7XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <stm32h7xx.h>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/STM32H7xx/ProcessorSTM32H7xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BdmaSTM32H7xx : public Module
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
        ID_1 = 0
    };

    enum ChannelId
    {
        CHANNEL_ID_0 = 0,
        CHANNEL_ID_1,
        CHANNEL_ID_2,
        CHANNEL_ID_3,
        CHANNEL_ID_4,
        CHANNEL_ID_5,
        CHANNEL_ID_6,
        CHANNEL_ID_7
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_TRANSFER_COMPLETE = 0,
        INTERRUPT_EVENT_HALF_TRANSFER,
        INTERRUPT_EVENT_TRANSFER_ERROR
    };

    using Error = ErrorTemplate<ErrorCode>;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    BdmaSTM32H7xx(const Id id);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId() const;

    BDMA_TypeDef* getDma();

    bool isChannelInterruptEventPending(const ChannelId channelId,
                                       const InterruptEvent interruptEvent);

    void clearChannelInterruptEvent(const ChannelId channelId,
                                   const InterruptEvent interruptEvent);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32H7xx::Peripheral myPeripheralMap[];

    // Variables

    static BDMA_TypeDef* myDmaMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    BDMA_TypeDef* myBdma;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;
};

}; // namespace Plat4m

#endif // PLAT4M_DMA_STM32H7XX_H
