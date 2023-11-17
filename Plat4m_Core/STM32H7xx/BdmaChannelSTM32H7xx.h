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
/// @file BdmaChannelSTM32H7xx.h
/// @author Ben Minerd
/// @date 8/3/2023
/// @brief BdmaChannelSTM32H7xx class header file.
///

#ifndef PLAT4M_DMA_CHANNEL_STM32H7XX_H
#define PLAT4M_DMA_CHANNEL_STM32H7XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <stm32h7xx.h>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/STM32H7xx/BdmaSTM32H7xx.h>
#include <Plat4m_Core/STM32H7xx/InterruptSTM32H7xx.h>
#include <Plat4m_Core/Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BdmaChannelSTM32H7xx : public Module
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
        std::uint16_t nDataToTransfer;
        std::uint32_t peripheralAddress;
        std::uint32_t memoryAddress;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    BdmaChannelSTM32H7xx(BdmaSTM32H7xx& dma,
                         const BdmaSTM32H7xx::ChannelId ChannelId);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    BdmaSTM32H7xx& getDma();

    BDMA_Channel_TypeDef* getDmaChannel();

    BbdmaSTM32H7xx::ChannelId getChannelId() const;

    Error setConfig(const Config& config);

    void setHalfTransferCallback(HalfTransferCallback& callback);

    void setTransferCompleteCallback(TransferCompleteCallback& callback);

    void setEnabledPrivate(const bool enabled);

    InterruptSTM32H7xx& getInterrupt();

    void setNDataToTransfer(const std::uint16_t nData);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const InterruptSTM32H7xx::Id myInterruptIdMap[][8];

    // Variables

    static BDMA_Channel_TypeDef* myDmaChannelMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    BdmaSTM32H7xx& myDma;

    const BdmaSTM32H7xx::ChannelId myChannelId;

    DMA_Channel_TypeDef* myDmaChannel;
    
    InterruptSTM32H7xx myInterrupt;

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

    inline void setDataTransferDirection(
                             const DataTransferDirection dataTransferDirection);

    inline void setCircularModeEnabled(const bool enabled);

    inline void setPeripheralIncrementModeEnabled(const bool enabled);

    inline void setMemoryIncrementModeEnabled(const bool enabled);

    inline void setPeripheralSize(const PeripheralSize peripheralSize);

    inline void setMemorySize(const MemorySize memorySize);

    inline void setPriorityLevel(const PriorityLevel priorityLevel);

    inline void setMemoryToMemoryModeEnabled(const bool enabled);

    inline void setPeripheralAddress(const std::uint32_t address);

    inline void setMemoryAddress(const std::uint32_t address);

    inline void setInterruptEventEnabled(
                              const BdmaSTM32H7xx::InterruptEvent interruptEvent,
                              const bool enabled);

    inline bool isInterruptEventEnabled(
                             const BdmaSTM32H7xx::InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(
                             const BdmaSTM32H7xx::InterruptEvent interruptEvent);

    inline void transferCompleteInterruptEventHandler();

    inline void halfTransferInterruptEventHandler();

    inline void transferErrorInterruptEventHandler();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void interruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_DMA_Channel_STM32H7XX_H
