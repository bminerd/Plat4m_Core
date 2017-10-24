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
/// @file DmaChannelSTM32F10x.h
/// @author Ben Minerd
/// @date 12/16/2016
/// @brief DmaChannelSTM32F10x class header file.
///

#ifndef PLAT4M_DMA_CHANNEL_STM32F10X_H
#define PLAT4M_DMA_CHANNEL_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>
#include <ErrorTemplate.h>
#include <DmaSTM32F10x.h>
#include <InterruptSTM32F10x.h>
#include <Callback.h>

#include <stm32f10x.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class DmaChannelSTM32F10x : public Module
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

    typedef Callback<> HalfTransferCallback;

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

    DmaChannelSTM32F10x(DmaSTM32F10x& dma,
                        const DmaSTM32F10x::ChannelId channelId);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    DmaSTM32F10x& getDma();

    DmaSTM32F10x::ChannelId getChannelId() const;

    Error setConfig(const Config& config);

    void setHalfTransferCallback(HalfTransferCallback& halfTransferCallback);

    void setTransferCompleteCallback(
                            TransferCompleteCallback& transferCompleteCallback);

    void setNDataToTransfer(const uint16_t nData);

    void setEnabledPrivate(const bool enabled);

    InterruptSTM32F10x& getInterrupt();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const InterruptSTM32F10x::Id myInterruptIdMap[][7];

    // Variables

    static DMA_Channel_TypeDef* myDmaChannelMap[];
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    DmaSTM32F10x& myDma;

    const DmaSTM32F10x::ChannelId myChannelId;

    DMA_Channel_TypeDef* myDmaChannel;
    
    InterruptSTM32F10x myInterrupt;

    Config myConfig;

    HalfTransferCallback* myHalfTransferCallback;

    TransferCompleteCallback* myTransferCompleteCallback;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

//    inline void setEnabledPrivate(const bool enabled);

    inline void setDataTransferDirection(
                             const DataTransferDirection dataTransferDirection);

    inline void setCircularModeEnabled(const bool enabled);

    inline void setPeripheralIncrementModeEnabled(const bool enabled);

    inline void setMemoryIncrementModeEnabled(const bool enabled);

    inline void setPeripheralSize(const PeripheralSize peripheralSize);

    inline void setMemorySize(const MemorySize memorySize);

    inline void setPriorityLevel(const PriorityLevel priorityLevel);

    inline void setMemoryToMemoryModeEnabled(const bool enabled);

    inline void setPeripheralAddress(const uint32_t address);

    inline void setMemoryAddress(const uint32_t address);

    inline void setInterruptEventEnabled(
                              const DmaSTM32F10x::InterruptEvent interruptEvent,
                              const bool enabled);

    inline bool isInterruptEventEnabled(
                             const DmaSTM32F10x::InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(
                             const DmaSTM32F10x::InterruptEvent interruptEvent);

    inline void transferCompleteInterruptEventHandler();

    inline void halfTransferInterruptEventHandler();

    inline void transferErrorInterruptEventHandler();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void interruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_DMA_CHANNEL_STM32F10X_H
