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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file DmaSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 9/15/2016
/// @brief DmaSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F4xx/DmaSTM32F4xx.h>

using Plat4m::DmaSTM32F4xx;
using Plat4m::Module;
using Plat4m::ProcessorSTM32F4xx;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F4xx::Peripheral DmaSTM32F4xx::myPeripheralMap[] =
{
    ProcessorSTM32F4xx::PERIPHERAL_DMA_1, /// ID_1
    ProcessorSTM32F4xx::PERIPHERAL_DMA_2  /// ID_2
};

// Variables

DMA_TypeDef* DmaSTM32F4xx::myDmaMap[] =
{
    DMA1, /// ID_1
    DMA2  /// ID_2
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaSTM32F4xx::DmaSTM32F4xx(const Id id) :
    Module(),
    myId(id),
    myDma(myDmaMap[myId])
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaSTM32F4xx::Id DmaSTM32F4xx::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
DMA_TypeDef* DmaSTM32F4xx::getDma()
{
    return myDma;
}

//------------------------------------------------------------------------------
DmaSTM32F4xx::Error DmaSTM32F4xx::setConfig(const Config& config)
{
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
bool DmaSTM32F4xx::isStreamInterruptEventPending(
                                            const StreamId streamId,
                                            const InterruptEvent interruptEvent)
{
    return areBitsSet(myDma->LISR,
                      ((uint32_t) 1) << ((streamId * 4) + interruptEvent + 1));
}

//------------------------------------------------------------------------------
void DmaSTM32F4xx::clearStreamInterruptEvent(
                                            const StreamId streamId,
                                            const InterruptEvent interruptEvent)
{
    setBits(myDma->LIFCR,
            ((uint32_t) 1) << ((streamId * 4) + interruptEvent + 1));
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error DmaSTM32F4xx::driverSetEnabled(const bool enabled)
{
    ProcessorSTM32F4xx::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}
