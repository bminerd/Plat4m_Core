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
/// @file DmaChannelSTM32F30x.cpp
/// @author Ben Minerd
/// @date 8/12/2016
/// @brief DmaChannelSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <DmaChannelSTM32F30x.h>
#include <CallbackMethod.h>

using Plat4m::DmaChannelSTM32F30x;
using Plat4m::Module;
using Plat4m::InterruptSTM32F30x;
using Plat4m::DmaSTM32F30x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F30x* interruptObjectMap[12];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const InterruptSTM32F30x::Id DmaChannelSTM32F30x::myInterruptIdMap[][7] =
{
    {
        InterruptSTM32F30x::ID_DMA_1_CHANNEL_1,
        InterruptSTM32F30x::ID_DMA_1_CHANNEL_2,
        InterruptSTM32F30x::ID_DMA_1_CHANNEL_3,
        InterruptSTM32F30x::ID_DMA_1_CHANNEL_4,
        InterruptSTM32F30x::ID_DMA_1_CHANNEL_5,
        InterruptSTM32F30x::ID_DMA_1_CHANNEL_6,
        InterruptSTM32F30x::ID_DMA_1_CHANNEL_7
    },
    {
        InterruptSTM32F30x::ID_DMA_2_CHANNEL_1,
        InterruptSTM32F30x::ID_DMA_2_CHANNEL_2,
        InterruptSTM32F30x::ID_DMA_2_CHANNEL_3,
        InterruptSTM32F30x::ID_DMA_2_CHANNEL_4,
        InterruptSTM32F30x::ID_DMA_2_CHANNEL_5,
        (InterruptSTM32F30x::Id) 0,
        (InterruptSTM32F30x::Id) 0,
    }
};

// Variables

DMA_Channel_TypeDef* DmaChannelSTM32F30x::myDmaChannelMap[] =
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
DmaChannelSTM32F30x::DmaChannelSTM32F30x(
                                      DmaSTM32F30x& dma,
                                      const DmaSTM32F30x::ChannelId channelId) :
    Module(),
    myDma(dma),
    myChannelId(channelId),
    myDmaChannel(0),
    myInterrupt(myInterruptIdMap[(myDma.getId())][myChannelId],
                createCallback(this, &DmaChannelSTM32F30x::interruptHandler)),
    myConfig(),
    myHalfTransferCallback(0),
    myTransferCompleteCallback(0)
{
    if (myDma.getId() == DmaSTM32F30x::ID_1)
    {
        myDmaChannel = myDmaChannelMap[myChannelId];

        interruptObjectMap[myChannelId] = &myInterrupt;
    }
    else if (myDma.getId() == DmaSTM32F30x::ID_2)
    {
        if (myChannelId > DmaSTM32F30x::CHANNEL_ID_5)
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
DmaSTM32F30x& DmaChannelSTM32F30x::getDma()
{
    return myDma;
}

//------------------------------------------------------------------------------
DMA_Channel_TypeDef* DmaChannelSTM32F30x::getDmaChannel()
{
	return myDmaChannel;
}

//------------------------------------------------------------------------------
DmaSTM32F30x::ChannelId DmaChannelSTM32F30x::getChannelId() const
{
    return myChannelId;
}

//------------------------------------------------------------------------------
DmaChannelSTM32F30x::Error DmaChannelSTM32F30x::setConfig(const Config& config)
{
    setEnabledPrivate(false);

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

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setHalfTransferCallback(
                                                 HalfTransferCallback& callback)
{
    myHalfTransferCallback = &callback;

	setInterruptEventEnabled(DmaSTM32F30x::INTERRUPT_EVENT_HALF_TRANSFER, true);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setTransferCompleteCallback(
                                             TransferCompleteCallback& callback)
{
    myTransferCompleteCallback = &callback;

	setInterruptEventEnabled(DmaSTM32F30x::INTERRUPT_EVENT_TRANSFER_COMPLETE,
							 true);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setEnabledPrivate(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR_EN, enabled);
}

//------------------------------------------------------------------------------
InterruptSTM32F30x& DmaChannelSTM32F30x::getInterrupt()
{
    return myInterrupt;
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setNDataToTransfer(const uint16_t nData)
{
    myDmaChannel->CNDTR = nData;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error DmaChannelSTM32F30x::driverSetEnabled(const bool enabled)
{
	if (isValidPointer(myHalfTransferCallback))
	{
		setInterruptEventEnabled(DmaSTM32F30x::INTERRUPT_EVENT_HALF_TRANSFER,
                             	 enabled);
	}

	if (isValidPointer(myTransferCompleteCallback))
	{
		setInterruptEventEnabled(DmaSTM32F30x::INTERRUPT_EVENT_TRANSFER_COMPLETE,
								 enabled);
	}

//    setInterruptEventEnabled(DmaSTM32F30x::INTERRUPT_EVENT_TRANSFER_ERROR,
//                             enabled);

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
void DmaChannelSTM32F30x::setDataTransferDirection(
                              const DataTransferDirection dataTransferDirection)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR_DIR,
                    ((uint32_t) dataTransferDirection) << 4);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setCircularModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR_CIRC, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setPeripheralIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR_PINC, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setMemoryIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR_MINC, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setPeripheralSize(const PeripheralSize peripheralSize)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR_PSIZE,
                    ((uint32_t) peripheralSize) << 8);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setMemorySize(const MemorySize memorySize)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR_MSIZE,
                    ((uint32_t) memorySize) << 10);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setPriorityLevel(const PriorityLevel priorityLevel)
{
    clearAndSetBits(myDmaChannel->CCR,
                    DMA_CCR_PL,
                    ((uint32_t) priorityLevel) << 12);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setMemoryToMemoryModeEnabled(const bool enabled)
{
    setBitsSet(myDmaChannel->CCR, DMA_CCR_MEM2MEM, enabled);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setPeripheralAddress(const uint32_t address)
{
    myDmaChannel->CPAR = address;
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setMemoryAddress(const uint32_t address)
{
    myDmaChannel->CMAR = address;
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::setInterruptEventEnabled(
                              const DmaSTM32F30x::InterruptEvent interruptEvent,
                              const bool enabled)
{
    setBitsSet(myDmaChannel->CCR,
               ((uint32_t) 1) << (interruptEvent + 1),
               enabled);
}

//------------------------------------------------------------------------------
bool DmaChannelSTM32F30x::isInterruptEventEnabled(
                              const DmaSTM32F30x::InterruptEvent interruptEvent)
{
    return areBitsSet(myDmaChannel->CCR,
                      ((uint32_t) 1) << (interruptEvent + 1));
}

//------------------------------------------------------------------------------
bool DmaChannelSTM32F30x::isInterruptEventPending(
                              const DmaSTM32F30x::InterruptEvent interruptEvent)
{
    return (isInterruptEventEnabled(interruptEvent) &&
            myDma.isChannelInterruptEventPending(myChannelId, interruptEvent));
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::transferCompleteInterruptEventHandler()
{
    if (isValidPointer(myTransferCompleteCallback))
    {
        myTransferCompleteCallback->call();
    }

    myDma.clearChannelInterruptEvent(
                               myChannelId,
                               DmaSTM32F30x::INTERRUPT_EVENT_TRANSFER_COMPLETE);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::halfTransferInterruptEventHandler()
{
    if (isValidPointer(myHalfTransferCallback))
    {
        myHalfTransferCallback->call();
    }

    myDma.clearChannelInterruptEvent(
                                   myChannelId,
                                   DmaSTM32F30x::INTERRUPT_EVENT_HALF_TRANSFER);
}

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::transferErrorInterruptEventHandler()
{
    myDma.clearChannelInterruptEvent(
                                  myChannelId,
                                  DmaSTM32F30x::INTERRUPT_EVENT_TRANSFER_ERROR);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DmaChannelSTM32F30x::interruptHandler()
{
    if (isInterruptEventPending(
                               DmaSTM32F30x::INTERRUPT_EVENT_TRANSFER_COMPLETE))
    {
        transferCompleteInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                   DmaSTM32F30x::INTERRUPT_EVENT_HALF_TRANSFER))
    {
        halfTransferInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                  DmaSTM32F30x::INTERRUPT_EVENT_TRANSFER_ERROR))
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
extern "C" void DMA2_Channel5_IRQHandler(void)
{
    interruptObjectMap[11]->handler();
}
