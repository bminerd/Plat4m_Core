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
/// @file BdmaSTM32H7xx.cpp
/// @author Ben Minerd
/// @date 8/3/2023
/// @brief BdmaSTM32H7xx class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32H7xx/BdmaSTM32H7xx.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32H7xx::Peripheral BdmaSTM32H7xx::myPeripheralMap[] =
{
    ProcessorSTM32H7xx::PERIPHERAL_BDMA_AND_DMA_MUX_2, /// ID_1
};

// Variables

BDMA_TypeDef* BdmaSTM32H7xx::myBdmaMap[] =
{
    BDMA, /// ID_1
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BdmaSTM32H7xx::BdmaSTM32H7xx(const Id id) :
    Module(),
    myId(id),
    myBdma(myBdmaMap[myId])
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BdmaSTM32H7xx::Id BdmaSTM32H7xx::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
BDMA_TypeDef* BdmaSTM32H7xx::getDma()
{
    return myBdma;
}

//------------------------------------------------------------------------------
bool BdmaSTM32H7xx::isChannelInterruptEventPending(
                                            const ChannelId channelId,
                                            const InterruptEvent interruptEvent)
{
    return areBitsSet(myBdma->ISR,
        (static_cast<std::uint32_t>(1)) << ((channelId * 4) + interruptEvent + 1));
}

//------------------------------------------------------------------------------
void BdmaSTM32H7xx::clearChannelInterruptEvent(
                                            const ChannelId channelId,
                                            const InterruptEvent interruptEvent)
{
    setBits(myBdma->IFCR,
        (static_cast<std::uint32_t>(1)) << ((channelId * 4) + interruptEvent + 1));
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error BdmaSTM32H7xx::driverSetEnabled(const bool enabled)
{
    ProcessorSTM32H7xx::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

