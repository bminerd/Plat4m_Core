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
/// @file DmaChannelSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/16/2016
/// @brief DmaChannelSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <DmaChannelSTM32F10x.h>
#include <CallbackMethod.h>

using Plat4m::DmaChannelSTM32F10x;
using Plat4m::Module;
using Plat4m::InterruptSTM32F10x;
using Plat4m::DmaSTM32F10x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F10x* interruptObjectMap[12];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const InterruptSTM32F10x::Id DmaChannelSTM32F10x::myInterruptIdMap[][7] =
{
    {
        InterruptSTM32F10x::ID_DMA_1_CHANNEL_1,
        InterruptSTM32F10x::ID_DMA_1_CHANNEL_2,
        InterruptSTM32F10x::ID_DMA_1_CHANNEL_3,
        InterruptSTM32F10x::ID_DMA_1_CHANNEL_4,
        InterruptSTM32F10x::ID_DMA_1_CHANNEL_5,
        InterruptSTM32F10x::ID_DMA_1_CHANNEL_6,
        InterruptSTM32F10x::ID_DMA_1_CHANNEL_7
    },
    {
        InterruptSTM32F10x::ID_DMA_2_CHANNEL_1,
        InterruptSTM32F10x::ID_DMA_2_CHANNEL_2,
        InterruptSTM32F10x::ID_DMA_2_CHANNEL_3,
#ifdef STM32F10X_HD
        InterruptSTM32F10x::ID_DMA_2_CHANNEL_4_5,
        InterruptSTM32F10x::ID_DMA_2_CHANNEL_4_5,
#else
        InterruptSTM32F10x::ID_DMA_2_CHANNEL_4,
        InterruptSTM32F10x::ID_DMA_2_CHANNEL_5,
#endif
        (InterruptSTM32F10x::Id) 0,
        (InterruptSTM32F10x::Id) 0,
    }
};

// Variables

DMA_Channel_TypeDef* DmaChannelSTM32F10x::myDmaChannelMap[] =
{
    DMA1_Channel1,
    DMA1_Channel2,
    DMA1_Channel3,
    DMA1_Channel4,
    DMA1_Channel5,
    DMA1_Channel6,
    DMA1_Channel7,
    DMA2_Channel1,
    DMA2_Channel2,
    DMA2_Channel3,
    DMA2_Channel4,
    DMA2_Channel5
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaChannelSTM32F10x::DmaChannelSTM32F10x(
                                      DmaSTM32F10x& dma,
                                      const DmaSTM32F10x::ChannelId channelId) :
    Module(),
    myDma(dma),
    myChannelId(channelId),
    myDmaChannel(0),
    myInterrupt(myInterruptIdMap[(myDma.getId())][myChannelId],
                createCallback(this, &DmaChannelSTM32F10x::interruptHandler)),
    myConfig(),
    myHalfTransferCallback(0),
    myTransferCompleteCallback(0)
{
    if (myDma.getId() == DmaSTM32F10x::ID_1)
    {
        myDmaChannel = myDmaChannelMap[myChannelId];

        interruptObjectMap[myChannelId] = &myInterrupt;
    }
    else if (myDma.getId() == DmaSTM32F10x::ID_2)
    {
        if (myChannelId > DmaSTM32F10x::CHANNEL_ID_5)
        {
            // DMA 2 only has 5 channels, lockup
            while (true)
            {
            }
        }

        myDmaChannel = myDmaChannelMap[(myChannelId + 7)];

        interruptObjectMap[(myChannelId + 7)] = &myInterrupt;
    }
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaSTM32F10x& DmaChannelSTM32F10x::getDma()
{
    return myDma;
}

//------------------------------------------------------------------------------
DmaSTM32F10x::ChannelId DmaChannelSTM32F10x::getChannelId() const
{
    return myChannelId;
}

//------------------------------------------------------------------------------
DmaChannelSTM32F10x::Error DmaChannelSTM32F10x::setConfig(const Config& config)
{
//    setEnabledPrivate(false);

    setDataTransferDirection(config.dataTransferDirection);
    setCircularModeEnabled(config.circularModeEnabled);
    setPeripheralIncrementModeEnabled(config.peripheralIncrementModeEnabled);
    setMemoryIncrementModeEnabled(config.memoryIncrementModeEnabled);
    setPeripheralSize(config.peripheralSize);
    setMemorySize(config.memorySize);
    setPriorityLevel(config.priorityLevel);
    setMemoryToMemoryModeEnabled(config.memoryToMemoryModeEnabled);
    setNDataToTransfer(config.nDataToTransfer);

    // TODO Address bounds error checking?

    setPeripheralAddress(config.peripheralAddress);
    setMemoryAddress(config.memoryAddress);

    myConfig = config;

//    setEnabledPrivate(true);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setHalfTransferCallback(
                                     HalfTransferCallback& halfTransferCallback)
{
    myHalfTransferCallback = &halfTransferCallback;
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setTransferCompleteCallback(
                             TransferCompleteCallback& transferCompleteCallback)
{
    myTransferCompleteCallback = &transferCompleteCallback;
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setNDataToTransfer(const uint16_t nData)
{
    myDmaChannel->CNDTR = nData;
}

//------------------------------------------------------------------------------
InterruptSTM32F10x& DmaChannelSTM32F10x::getInterrupt()
{
    return myInterrupt;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error DmaChannelSTM32F10x::driverSetEnabled(const bool enabled)
{
//    setEnabledPrivate(enabled);
    setInterruptEventEnabled(DmaSTM32F10x::INTERRUPT_EVENT_TRANSFER_COMPLETE,
                             enabled);
    setInterruptEventEnabled(DmaSTM32F10x::INTERRUPT_EVENT_HALF_TRANSFER,
                             enabled);
    setInterruptEventEnabled(DmaSTM32F10x::INTERRUPT_EVENT_TRANSFER_ERROR,
                             enabled);

    Interrupt::Config interruptConfig;
    interruptConfig.priority = 7;
    myInterrupt.configure(interruptConfig);

    myInterrupt.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setEnabledPrivate(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR1_EN, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setDataTransferDirection(
                              const DataTransferDirection dataTransferDirection)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR1_DIR,
                    ((uint32_t) dataTransferDirection) << 4);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setCircularModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR1_CIRC, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setPeripheralIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR1_PINC, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setMemoryIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR1_MINC, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setPeripheralSize(const PeripheralSize peripheralSize)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR1_PSIZE,
                    ((uint32_t) peripheralSize) << 8);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setMemorySize(const MemorySize memorySize)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR1_MSIZE,
                    ((uint32_t) memorySize) << 10);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setPriorityLevel(const PriorityLevel priorityLevel)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR1_PL,
                    ((uint32_t) priorityLevel) << 12);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setMemoryToMemoryModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR1_MEM2MEM, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setPeripheralAddress(const uint32_t address)
{
    myDmaChannel->CPAR = address;
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setMemoryAddress(const uint32_t address)
{
    myDmaChannel->CMAR = address;
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::setInterruptEventEnabled(
                              const DmaSTM32F10x::InterruptEvent interruptEvent,
                              const bool enabled)
{
    setBitsSet(myDmaChannel->CCR,
               ((uint32_t) 1) << (interruptEvent + 1),
               enabled);
}

//------------------------------------------------------------------------------
bool DmaChannelSTM32F10x::isInterruptEventEnabled(
                              const DmaSTM32F10x::InterruptEvent interruptEvent)
{
    return areBitsSet(myDmaChannel->CCR,
                      ((uint32_t) 1) << (interruptEvent + 1));
}

//------------------------------------------------------------------------------
bool DmaChannelSTM32F10x::isInterruptEventPending(
                              const DmaSTM32F10x::InterruptEvent interruptEvent)
{
    return (isInterruptEventEnabled(interruptEvent) &&
            myDma.isChannelInterruptEventPending(myChannelId, interruptEvent));
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::transferCompleteInterruptEventHandler()
{
    if (isValidPointer(myTransferCompleteCallback))
    {
        myTransferCompleteCallback->call();
    }

    myDma.clearChannelInterruptEvent(
                               myChannelId,
                               DmaSTM32F10x::INTERRUPT_EVENT_TRANSFER_COMPLETE);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::halfTransferInterruptEventHandler()
{
    if (isValidPointer(myHalfTransferCallback))
    {
        myHalfTransferCallback->call();
    }

    myDma.clearChannelInterruptEvent(
                                   myChannelId,
                                   DmaSTM32F10x::INTERRUPT_EVENT_HALF_TRANSFER);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::transferErrorInterruptEventHandler()
{
    myDma.clearChannelInterruptEvent(
                                  myChannelId,
                                  DmaSTM32F10x::INTERRUPT_EVENT_TRANSFER_ERROR);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DmaChannelSTM32F10x::interruptHandler()
{
    if (isInterruptEventPending(
                               DmaSTM32F10x::INTERRUPT_EVENT_TRANSFER_COMPLETE))
    {
        transferCompleteInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                   DmaSTM32F10x::INTERRUPT_EVENT_HALF_TRANSFER))
    {
        halfTransferInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                  DmaSTM32F10x::INTERRUPT_EVENT_TRANSFER_ERROR))
    {
        transferErrorInterruptEventHandler();
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void DMA1_Channel1_IRQHandler(void)
{
    interruptObjectMap[0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Channel2_IRQHandler(void)
{
    interruptObjectMap[1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Channel3_IRQHandler(void)
{
    interruptObjectMap[2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Channel4_IRQHandler(void)
{
    interruptObjectMap[3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Channel5_IRQHandler(void)
{
    interruptObjectMap[4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Channel6_IRQHandler(void)
{
    interruptObjectMap[5]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Channel7_IRQHandler(void)
{
    interruptObjectMap[6]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Channel1_IRQHandler(void)
{
    interruptObjectMap[7]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Channel2_IRQHandler(void)
{
    interruptObjectMap[8]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Channel3_IRQHandler(void)
{
    interruptObjectMap[9]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Channel4_IRQHandler(void)
{
    interruptObjectMap[10]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Channel4_5_IRQHandler(void)
{
    if (Plat4m::areBitsSet(DMA2->ISR, DMA_ISR_GIF4))
    {
        interruptObjectMap[10]->handler();
    }
    else if (Plat4m::areBitsSet(DMA2->ISR, DMA_ISR_GIF5))
    {
        interruptObjectMap[11]->handler();
    }
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Channel_5_IRQHandler(void)
{
    interruptObjectMap[11]->handler();
}
