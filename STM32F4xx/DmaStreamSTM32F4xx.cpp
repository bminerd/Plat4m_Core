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
/// @file DmaStreamSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 9/15/2016
/// @brief DmaStreamSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <DmaStreamSTM32F4xx.h>
#include <CallbackMethod.h>

using Plat4m::DmaStreamSTM32F4xx;
using Plat4m::Module;
using Plat4m::InterruptSTM32F4xx;
using Plat4m::DmaSTM32F4xx;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F4xx* interruptObjectMap[16];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const InterruptSTM32F4xx::Id DmaStreamSTM32F4xx::myInterruptIdMap[][7] =
{
    {
        InterruptSTM32F4xx::ID_DMA_1_STREAM_1,
        InterruptSTM32F4xx::ID_DMA_1_STREAM_2,
        InterruptSTM32F4xx::ID_DMA_1_STREAM_3,
        InterruptSTM32F4xx::ID_DMA_1_STREAM_4,
        InterruptSTM32F4xx::ID_DMA_1_STREAM_5,
        InterruptSTM32F4xx::ID_DMA_1_STREAM_6,
        InterruptSTM32F4xx::ID_DMA_1_STREAM_7
    },
    {
        InterruptSTM32F4xx::ID_DMA_2_STREAM_1,
        InterruptSTM32F4xx::ID_DMA_2_STREAM_2,
        InterruptSTM32F4xx::ID_DMA_2_STREAM_3,
        InterruptSTM32F4xx::ID_DMA_2_STREAM_4,
        InterruptSTM32F4xx::ID_DMA_2_STREAM_5,
        (InterruptSTM32F4xx::Id) 0,
        (InterruptSTM32F4xx::Id) 0,
    }
};

// Variables

DMA_Stream_TypeDef* DmaStreamSTM32F4xx::myDmaStreamMap[][8] =
{
    // DmaSTM32F4xx::ID_1
    {
        DMA1_Stream0,
        DMA1_Stream1,
        DMA1_Stream2,
        DMA1_Stream3,
        DMA1_Stream4,
        DMA1_Stream5,
        DMA1_Stream6,
        DMA1_Stream7
    },
    // DmaSTM32F4xx::ID_2
    {
        DMA2_Stream0,
        DMA2_Stream1,
        DMA2_Stream2,
        DMA2_Stream3,
        DMA2_Stream4,
        DMA2_Stream5,
        DMA2_Stream6,
        DMA2_Stream7
    }
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaStreamSTM32F4xx::DmaStreamSTM32F4xx(
                                      DmaSTM32F4xx& dma,
                                      const DmaSTM32F4xx::StreamId channelId) :
    Module(),
    myDma(dma),
    myStreamId(channelId),
    myDmaStream(0),
    myInterrupt(myInterruptIdMap[(myDma.getId())][myStreamId],
                createCallback(this, &DmaStreamSTM32F4xx::interruptHandler)),
    myConfig()
{
    if (myDma.getId() == DmaSTM32F4xx::ID_1)
    {
        myDmaStream = myDmaStreamMap[(myDma.getId())][myStreamId];

        interruptObjectMap[myStreamId] = &myInterrupt;
    }
    else if (myDma.getId() == DmaSTM32F4xx::ID_2)
    {
        if (myStreamId > DmaSTM32F4xx::STREAM_ID_5)
        {
            // DMA 2 only has 5 channels, lockup
            while (true)
            {
            }
        }

        myDmaStream = myDmaStreamMap[(myDma.getId())][(myStreamId + 7)];

        interruptObjectMap[(myStreamId + 7)] = &myInterrupt;
    }
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaSTM32F4xx& DmaStreamSTM32F4xx::getDma()
{
    return myDma;
}

//------------------------------------------------------------------------------
DmaSTM32F4xx::StreamId DmaStreamSTM32F4xx::getStreamId() const
{
    return myStreamId;
}

//------------------------------------------------------------------------------
DmaStreamSTM32F4xx::Error DmaStreamSTM32F4xx::setConfig(const Config& config)
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
    setMemory0Address(config.memory0Address);
    setMemory1Address(config.memory1Address);

    myConfig = config;

    setEnabledPrivate(true);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setTransferCompleteCallback(
                                             TransferCompleteCallback& callback)
{
    myTransferCompleteCallback = &callback;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error DmaStreamSTM32F4xx::driverSetEnabled(const bool enabled)
{
    setEnabledPrivate(enabled);
    setInterruptEventEnabled(DmaSTM32F4xx::INTERRUPT_EVENT_TRANSFER_COMPLETE,
                             enabled);
    setInterruptEventEnabled(DmaSTM32F4xx::INTERRUPT_EVENT_TRANSFER_ERROR,
                             enabled);

    myInterrupt.setEnabled(enabled);

    Interrupt::Config interruptConfig;
    interruptConfig.priority = 3;
    myInterrupt.configure(interruptConfig);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setEnabledPrivate(const bool enabled)
{
    setBitsSet(myDmaStream->CR, DMA_SxCR_EN, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setDataTransferDirection(
                              const DataTransferDirection dataTransferDirection)
{
    clearAndSetBits(myDmaStream->CR,
                    DMA_SxCR_DIR,
                    ((uint32_t) dataTransferDirection) << 4);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setCircularModeEnabled(const bool enabled)
{
    setBitsSet(myDmaStream->CR, DMA_SxCR_CIRC, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setPeripheralIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaStream->CR, DMA_SxCR_PINC, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setMemoryIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaStream->CR, DMA_SxCR_MINC, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setPeripheralSize(const PeripheralSize peripheralSize)
{
    clearAndSetBits(myDmaStream->CR,
    				DMA_SxCR_PSIZE,
                    ((uint32_t) peripheralSize) << 11);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setMemorySize(const MemorySize memorySize)
{
    clearAndSetBits(myDmaStream->CR,
    				DMA_SxCR_MSIZE,
                    ((uint32_t) memorySize) << 13);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setPriorityLevel(const PriorityLevel priorityLevel)
{
    clearAndSetBits(myDmaStream->CR,
    				DMA_SxCR_PL,
                    ((uint32_t) priorityLevel) << 16);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setMemoryToMemoryModeEnabled(const bool enabled)
{
//    setBitsSet(myDmaStream->CCR, DMA_CCR_MEM2MEM, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setNDataToTransfer(const uint16_t nData)
{
    myDmaStream->NDTR = nData;
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setPeripheralAddress(const uint32_t address)
{
    myDmaStream->PAR = address;
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setMemory0Address(const uint32_t address)
{
    myDmaStream->M0AR = address;
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setMemory1Address(const uint32_t address)
{
    myDmaStream->M1AR = address;
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::setInterruptEventEnabled(
                              const DmaSTM32F4xx::InterruptEvent interruptEvent,
                              const bool enabled)
{
	if (interruptEvent == DmaSTM32F4xx::INTERRUPT_EVENT_FIFO_OVERRUN_UNDERRUN)
	{
		setBitsSet(myDmaStream->FCR, DMA_SxFCR_FEIE, enabled);
	}
	else
	{
		setBitsSet(myDmaStream->CR,
				   ((uint32_t) 1) << (interruptEvent + 1),
				   enabled);
	}
}

//------------------------------------------------------------------------------
bool DmaStreamSTM32F4xx::isInterruptEventEnabled(
                              const DmaSTM32F4xx::InterruptEvent interruptEvent)
{
	if (interruptEvent == DmaSTM32F4xx::INTERRUPT_EVENT_FIFO_OVERRUN_UNDERRUN)
	{
		return areBitsSet(myDmaStream->FCR, DMA_SxFCR_FEIE);
	}

	return areBitsSet(myDmaStream->CR, ((uint32_t) 1) << (interruptEvent + 1));
}

//------------------------------------------------------------------------------
bool DmaStreamSTM32F4xx::isInterruptEventPending(
                              const DmaSTM32F4xx::InterruptEvent interruptEvent)
{
    return (isInterruptEventEnabled(interruptEvent) &&
            myDma.isStreamInterruptEventPending(myStreamId, interruptEvent));
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::transferCompleteInterruptEventHandler()
{
    if (isValidPointer(myTransferCompleteCallback))
    {
        myTransferCompleteCallback->call();
    }

    myDma.clearStreamInterruptEvent(
                               myStreamId,
                               DmaSTM32F4xx::INTERRUPT_EVENT_TRANSFER_COMPLETE);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::halfTransferInterruptEventHandler()
{
    myDma.clearStreamInterruptEvent(
                                   myStreamId,
                                   DmaSTM32F4xx::INTERRUPT_EVENT_HALF_TRANSFER);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::transferErrorInterruptEventHandler()
{
    myDma.clearStreamInterruptEvent(
                                  myStreamId,
                                  DmaSTM32F4xx::INTERRUPT_EVENT_TRANSFER_ERROR);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DmaStreamSTM32F4xx::interruptHandler()
{
    if (isInterruptEventPending(
                               DmaSTM32F4xx::INTERRUPT_EVENT_TRANSFER_COMPLETE))
    {
        transferCompleteInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                   DmaSTM32F4xx::INTERRUPT_EVENT_HALF_TRANSFER))
    {
        halfTransferInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                  DmaSTM32F4xx::INTERRUPT_EVENT_TRANSFER_ERROR))
    {
        transferErrorInterruptEventHandler();
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream0_IRQHandler(void)
{
    interruptObjectMap[0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream1_IRQHandler(void)
{
    interruptObjectMap[1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream2_IRQHandler(void)
{
    interruptObjectMap[2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream3_IRQHandler(void)
{
    interruptObjectMap[3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream4_IRQHandler(void)
{
    interruptObjectMap[4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream5_IRQHandler(void)
{
    interruptObjectMap[5]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream6_IRQHandler(void)
{
    interruptObjectMap[6]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream7_IRQHandler(void)
{
    interruptObjectMap[7]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream0_IRQHandler(void)
{
    interruptObjectMap[8]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream1_IRQHandler(void)
{
    interruptObjectMap[9]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream2_IRQHandler(void)
{
    interruptObjectMap[10]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream3_IRQHandler(void)
{
    interruptObjectMap[11]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream4_IRQHandler(void)
{
    interruptObjectMap[12]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream5_IRQHandler(void)
{
    interruptObjectMap[13]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream6_IRQHandler(void)
{
    interruptObjectMap[14]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream7_IRQHandler(void)
{
    interruptObjectMap[15]->handler();
}
