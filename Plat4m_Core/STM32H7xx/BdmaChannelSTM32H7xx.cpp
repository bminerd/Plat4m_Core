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
/// @file DmaStreamSTM32H7xx.cpp
/// @author Ben Minerd
/// @date 8/3/2023
/// @brief DmaStreamSTM32H7xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32H7xx/DmaStreamSTM32H7xx.h>
#include <Plat4m_Core/CallbackMethod.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32H7xx* interruptObjectMap[12];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const InterruptSTM32H7xx::Id DmaStreamSTM32H7xx::myInterruptIdMap[][7] =
{
    {
        InterruptSTM32H7xx::ID_DMA_1_STREAM_0,
        InterruptSTM32H7xx::ID_DMA_1_STREAM_1,
        InterruptSTM32H7xx::ID_DMA_1_STREAM_2,
        InterruptSTM32H7xx::ID_DMA_1_STREAM_3,
        InterruptSTM32H7xx::ID_DMA_1_STREAM_4,
        InterruptSTM32H7xx::ID_DMA_1_STREAM_5,
        InterruptSTM32H7xx::ID_DMA_1_STREAM_6
    },
    {
        InterruptSTM32H7xx::ID_DMA_2_STREAM_0,
        InterruptSTM32H7xx::ID_DMA_2_STREAM_1,
        InterruptSTM32H7xx::ID_DMA_2_STREAM_2,
        InterruptSTM32H7xx::ID_DMA_2_STREAM_3,
        InterruptSTM32H7xx::ID_DMA_2_STREAM_4,
        (InterruptSTM32H7xx::Id) 0,
        (InterruptSTM32H7xx::Id) 0,
    }
};

// Variables

DMA_Stream_TypeDef* DmaStreamSTM32H7xx::myDmaStreamMap[] =
{
    DMA1_Stream1,
    DMA1_Stream2,
    DMA1_Stream3,
    DMA1_Stream4,
    DMA1_Stream5,
    DMA1_Stream6,
    DMA1_Stream7,
    DMA2_Stream1,
    DMA2_Stream2,
    DMA2_Stream3,
    DMA2_Stream4,
    DMA2_Stream5
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaStreamSTM32H7xx::DmaStreamSTM32H7xx(
                                      DmaSTM32H7xx& dma,
                                      const DmaSTM32H7xx::StreamId StreamId) :
    Module(),
    myDma(dma),
    myStreamId(StreamId),
    myDmaStream(0),
    myInterrupt(myInterruptIdMap[(myDma.getId())][myStreamId],
                createCallback(this, &DmaStreamSTM32H7xx::interruptHandler)),
    myConfig(),
    myHalfTransferCallback(0),
    myTransferCompleteCallback(0)
{
    if (myDma.getId() == DmaSTM32H7xx::ID_1)
    {
        myDmaStream = myDmaStreamMap[myStreamId];

        interruptObjectMap[myStreamId] = &myInterrupt;
    }
    else if (myDma.getId() == DmaSTM32H7xx::ID_2)
    {
        if (myStreamId > DmaSTM32H7xx::Stream_ID_5)
        {
            // DMA 2 only has 5 Streams, lockup
            while (true)
            {
            }
        }

        myDmaStream = myDmaStreamMap[(myStreamId + 7)];

        interruptObjectMap[(myStreamId + 7)] = &myInterrupt;
    }
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
DmaSTM32H7xx& DmaStreamSTM32H7xx::getDma()
{
    return myDma;
}

//------------------------------------------------------------------------------
DMA_Stream_TypeDef* DmaStreamSTM32H7xx::getDmaStream()
{
	return myDmaStream;
}

//------------------------------------------------------------------------------
DmaSTM32H7xx::StreamId DmaStreamSTM32H7xx::getStreamId() const
{
    return myStreamId;
}

//------------------------------------------------------------------------------
DmaStreamSTM32H7xx::Error DmaStreamSTM32H7xx::setConfig(const Config& config)
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
void DmaStreamSTM32H7xx::setHalfTransferCallback(
                                                 HalfTransferCallback& callback)
{
    myHalfTransferCallback = &callback;

	setInterruptEventEnabled(DmaSTM32H7xx::INTERRUPT_EVENT_HALF_TRANSFER, true);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setTransferCompleteCallback(
                                             TransferCompleteCallback& callback)
{
    myTransferCompleteCallback = &callback;

	setInterruptEventEnabled(DmaSTM32H7xx::INTERRUPT_EVENT_TRANSFER_COMPLETE,
							 true);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setEnabledPrivate(const bool enabled)
{
    setBitsSet(myDmaStream->CCR, DMA_CCR_EN, enabled);
}

//------------------------------------------------------------------------------
InterruptSTM32H7xx& DmaStreamSTM32H7xx::getInterrupt()
{
    return myInterrupt;
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setNDataToTransfer(const uint16_t nData)
{
    myDmaStream->CNDTR = nData;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error DmaStreamSTM32H7xx::driverSetEnabled(const bool enabled)
{
	if (isValidPointer(myHalfTransferCallback))
	{
		setInterruptEventEnabled(DmaSTM32H7xx::INTERRUPT_EVENT_HALF_TRANSFER,
                             	 enabled);
	}

	if (isValidPointer(myTransferCompleteCallback))
	{
		setInterruptEventEnabled(DmaSTM32H7xx::INTERRUPT_EVENT_TRANSFER_COMPLETE,
								 enabled);
	}

//    setInterruptEventEnabled(DmaSTM32H7xx::INTERRUPT_EVENT_TRANSFER_ERROR,
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
void DmaStreamSTM32H7xx::setDataTransferDirection(
                              const DataTransferDirection dataTransferDirection)
{
    clearAndSetBits(myDmaStream->CCR,
                    DMA_CCR_DIR,
                    ((uint32_t) dataTransferDirection) << 4);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setCircularModeEnabled(const bool enabled)
{
    setBitsSet(myDmaStream->CCR, DMA_CCR_CIRC, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setPeripheralIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaStream->CCR, DMA_CCR_PINC, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setMemoryIncrementModeEnabled(const bool enabled)
{
    setBitsSet(myDmaStream->CCR, DMA_CCR_MINC, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setPeripheralSize(const PeripheralSize peripheralSize)
{
    clearAndSetBits(myDmaStream->CCR,
                    DMA_CCR_PSIZE,
                    ((uint32_t) peripheralSize) << 8);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setMemorySize(const MemorySize memorySize)
{
    clearAndSetBits(myDmaStream->CCR,
                    DMA_CCR_MSIZE,
                    ((uint32_t) memorySize) << 10);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setPriorityLevel(const PriorityLevel priorityLevel)
{
    clearAndSetBits(myDmaStream->CCR,
                    DMA_CCR_PL,
                    ((uint32_t) priorityLevel) << 12);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setMemoryToMemoryModeEnabled(const bool enabled)
{
    setBitsSet(myDmaStream->CCR, DMA_CCR_MEM2MEM, enabled);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setPeripheralAddress(const uint32_t address)
{
    myDmaStream->CPAR = address;
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setMemoryAddress(const uint32_t address)
{
    myDmaStream->CMAR = address;
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::setInterruptEventEnabled(
                              const DmaSTM32H7xx::InterruptEvent interruptEvent,
                              const bool enabled)
{
    setBitsSet(myDmaStream->CCR,
               ((uint32_t) 1) << (interruptEvent + 1),
               enabled);
}

//------------------------------------------------------------------------------
bool DmaStreamSTM32H7xx::isInterruptEventEnabled(
                              const DmaSTM32H7xx::InterruptEvent interruptEvent)
{
    return areBitsSet(myDmaStream->CCR,
                      ((uint32_t) 1) << (interruptEvent + 1));
}

//------------------------------------------------------------------------------
bool DmaStreamSTM32H7xx::isInterruptEventPending(
                              const DmaSTM32H7xx::InterruptEvent interruptEvent)
{
    return (isInterruptEventEnabled(interruptEvent) &&
            myDma.isStreamInterruptEventPending(myStreamId, interruptEvent));
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::transferCompleteInterruptEventHandler()
{
    if (isValidPointer(myTransferCompleteCallback))
    {
        myTransferCompleteCallback->call();
    }

    myDma.clearStreamInterruptEvent(
                               myStreamId,
                               DmaSTM32H7xx::INTERRUPT_EVENT_TRANSFER_COMPLETE);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::halfTransferInterruptEventHandler()
{
    if (isValidPointer(myHalfTransferCallback))
    {
        myHalfTransferCallback->call();
    }

    myDma.clearStreamInterruptEvent(
                                   myStreamId,
                                   DmaSTM32H7xx::INTERRUPT_EVENT_HALF_TRANSFER);
}

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::transferErrorInterruptEventHandler()
{
    myDma.clearStreamInterruptEvent(
                                  myStreamId,
                                  DmaSTM32H7xx::INTERRUPT_EVENT_TRANSFER_ERROR);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void DmaStreamSTM32H7xx::interruptHandler()
{
    if (isInterruptEventPending(
                               DmaSTM32H7xx::INTERRUPT_EVENT_TRANSFER_COMPLETE))
    {
        transferCompleteInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                   DmaSTM32H7xx::INTERRUPT_EVENT_HALF_TRANSFER))
    {
        halfTransferInterruptEventHandler();
    }
    else if (isInterruptEventPending(
                                  DmaSTM32H7xx::INTERRUPT_EVENT_TRANSFER_ERROR))
    {
        transferErrorInterruptEventHandler();
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream1_IRQHandler(void)
{
    interruptObjectMap[0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream2_IRQHandler(void)
{
    interruptObjectMap[1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream3_IRQHandler(void)
{
    interruptObjectMap[2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream4_IRQHandler(void)
{
    interruptObjectMap[3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream5_IRQHandler(void)
{
    interruptObjectMap[4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream6_IRQHandler(void)
{
    interruptObjectMap[5]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA1_Stream7_IRQHandler(void)
{
    interruptObjectMap[6]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream1_IRQHandler(void)
{
    interruptObjectMap[7]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream2_IRQHandler(void)
{
    interruptObjectMap[8]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream3_IRQHandler(void)
{
    interruptObjectMap[9]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream4_IRQHandler(void)
{
    interruptObjectMap[10]->handler();
}

//------------------------------------------------------------------------------
extern "C" void DMA2_Stream5_IRQHandler(void)
{
    interruptObjectMap[11]->handler();
}
