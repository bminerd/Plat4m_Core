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
/// @file DmaSTM32F30x.cpp
/// @author Ben Minerd
/// @date 8/1/2016
/// @brief DmaSTM32F30x class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <DmaSTM32F30x.h>

using Plat4m::DmaSTM32F30x;
using Plat4m::Module;
using Plat4m::ProcessorSTM32F30x;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F30x::Peripheral DmaSTM32F30x::myPeripheralMap[] =
{
    ProcessorSTM32F30x::PERIPHERAL_DMA_1, /// ID_1
    ProcessorSTM32F30x::PERIPHERAL_DMA_2  /// ID_2
};

// Variables

DMA_TypeDef* DmaSTM32F30x::myDmaMap[] =
{
    DMA1, /// ID_1
    DMA2  /// ID_2
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaSTM32F30x::DmaSTM32F30x(const Id id) :
    Module(),
    myId(id),
    myDma(myDmaMap[myId])
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaSTM32F30x::Id DmaSTM32F30x::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
DMA_TypeDef* DmaSTM32F30x::getDma()
{
    return myDma;
}

//------------------------------------------------------------------------------
DmaSTM32F30x::Error DmaSTM32F30x::setConfig(const Config& config)
{
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
bool DmaSTM32F30x::isChannelInterruptEventPending(
                                            const ChannelId channelId,
                                            const InterruptEvent interruptEvent)
{
    return areBitsSet(myDma->ISR,
                      ((uint32_t) 1) << ((channelId * 4) + interruptEvent + 1));
}

//------------------------------------------------------------------------------
void DmaSTM32F30x::clearChannelInterruptEvent(
                                            const ChannelId channelId,
                                            const InterruptEvent interruptEvent)
{
    setBits(myDma->IFCR,
            ((uint32_t) 1) << ((channelId * 4) + interruptEvent + 1));
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error DmaSTM32F30x::driverSetEnabled(const bool enabled)
{
    ProcessorSTM32F30x::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

