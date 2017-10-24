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
/// @file DmaStreamSTM32F4xx.h
/// @author Ben Minerd
/// @date 9/15/2016
/// @brief DmaStreamSTM32F4xx class header file.
///

#ifndef DMA_STREAM_STM32F4XX_H
#define DMA_STREAM_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>
#include <ErrorTemplate.h>
#include <DmaSTM32F4xx.h>
#include <InterruptSTM32F4xx.h>
#include <Callback.h>

#include <stm32f4xx.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class DmaStreamSTM32F4xx : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE = 0
    };

    enum DataTransferDirection
    {
        DATA_TRANSFER_DIRECTION_READ_FROM_PERIPHERAL = 0,
        DATA_TRANSFER_DIRECTION_READ_FROM_MEMORY
    };

    enum PeripheralSize
    {
        PERIPHERAL_SIZE_8_BITS = 0,
        PERIPHERAL_SIZE_16_BITS,
        PERIPHERAL_SIZE_32_BITS
    };

    enum MemorySize
    {
        MEMORY_SIZE_8_BITS = 0,
        MEMORY_SIZE_16_BITS,
        MEMORY_SIZE_32_BITS
    };

    enum PriorityLevel
    {
        PRIORITY_LEVEL_LOW = 0,
        PRIORITY_LEVEL_MEDIUM,
        PRIORITY_LEVEL_HIGH,
        PRIORITY_LEVEL_VERY_HIGH
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> TransferCompleteCallback;

    struct Config
    {
        DataTransferDirection dataTransferDirection;
        bool circularModeEnabled;
        bool peripheralIncrementModeEnabled;
        bool memoryIncrementModeEnabled;
        PeripheralSize peripheralSize;
        MemorySize memorySize;
        PriorityLevel priorityLevel;
        bool memoryToMemoryModeEnabled;
        uint16_t nDataToTransfer;
        uint32_t peripheralAddress;
        uint32_t memoryAddress;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    DmaStreamSTM32F4xx(DmaSTM32F4xx& dma,
                       const DmaSTM32F4xx::StreamId streamId);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    DmaSTM32F4xx& getDma();

    DmaSTM32F4xx::StreamId getStreamId() const;

    Error setConfig(const Config& config);

    void setTransferCompleteCallback(TransferCompleteCallback& callback);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const InterruptSTM32F4xx::Id myInterruptIdMap[][7];

    // Variables

    static DMA_Stream_TypeDef* myDmaStreamMap[][8];
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    DmaSTM32F4xx& myDma;

    const DmaSTM32F4xx::StreamId myStreamId;

    DMA_Stream_TypeDef* myDmaStream;
    
    InterruptSTM32F4xx myInterrupt;

    Config myConfig;

    TransferCompleteCallback* myTransferCompleteCallback;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void setEnabledPrivate(const bool enabled);

    inline void setDataTransferDirection(
                             const DataTransferDirection dataTransferDirection);

    inline void setCircularModeEnabled(const bool enabled);

    inline void setPeripheralIncrementModeEnabled(const bool enabled);

    inline void setMemoryIncrementModeEnabled(const bool enabled);

    inline void setPeripheralSize(const PeripheralSize peripheralSize);

    inline void setMemorySize(const MemorySize memorySize);

    inline void setPriorityLevel(const PriorityLevel priorityLevel);

    inline void setMemoryToMemoryModeEnabled(const bool enabled);

    inline void setNDataToTransfer(const uint16_t nData);

    inline void setPeripheralAddress(const uint32_t address);

    inline void setMemoryAddress(const uint32_t address);

    inline void setInterruptEventEnabled(
                              const DmaSTM32F4xx::InterruptEvent interruptEvent,
                              const bool enabled);

    inline bool isInterruptEventEnabled(
                             const DmaSTM32F4xx::InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(
                             const DmaSTM32F4xx::InterruptEvent interruptEvent);

    inline void transferCompleteInterruptEventHandler();

    inline void halfTransferInterruptEventHandler();

    inline void transferErrorInterruptEventHandler();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void interruptHandler();
};

}; // namespace Plat4m

#endif // DMA_STREAM_STM32F4XX_H
