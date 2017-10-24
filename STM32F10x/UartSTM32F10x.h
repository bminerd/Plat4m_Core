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
/// @file UartSTM32F10x.h
/// @author Ben Minerd
/// @date 12/16/2016
/// @brief UartSTM32F10x class header file.
///

#ifndef PLAT4M_UART_STM32F10X_H
#define PLAT4M_UART_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Uart.h>
#include <GpioPinSTM32F10x.h>
#include <InterruptSTM32F10x.h>
#include <ProcessorSTM32F10x.h>
#include <DmaChannelSTM32F10x.h>

#include <stm32f10x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class UartSTM32F10x : public Uart
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3,
        ID_4,
        ID_5
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY = 0,
        INTERRUPT_EVENT_TRANSMISSION_COMPLETE,
        INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY
    };

    enum State
    {
        STATE_IDLE,
        STATE_BUSY,
        STATE_ERROR_OVERRUN
    };

    enum TransferMode
    {
        TRANSFER_MODE_TRANSMIT_ONLY = 0,
        TRANSFER_MODE_RECEIVE_ONLY,
        TRANSFER_MODE_HALF_DUPLEX,
        TRANSFER_MODE_FULL_DUPLEX
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    UartSTM32F10x(const Id id,
                  const TransferMode transferMode,
                  GpioPinSTM32F10x& gpioPin);
    
    UartSTM32F10x(const Id id,
                  GpioPinSTM32F10x& transmitGpioPin,
                  GpioPinSTM32F10x& receiveGpioPin);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~UartSTM32F10x();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setTransmitDmaChannel(DmaChannelSTM32F10x& transmitDmaChannel);

    void setReceiveDmaChannel(DmaChannelSTM32F10x& receiveDmaChannel);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    // Constants

    static const ProcessorSTM32F10x::Peripheral myPeripheralMap[];

    static const InterruptSTM32F10x::Id myInterruptMap[];

    static const uint32_t myInterruptEventMap[];

    static const GpioPinSTM32F10x::OutputSpeed myDefaultOutputSpeed;

    // Variables

    static USART_TypeDef* myUartMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    const TransferMode myTransferMode;

    USART_TypeDef* myUart;
    
    GpioPinSTM32F10x& myTransmitGpioPin;
    
    GpioPinSTM32F10x* myReceiveGpioPin;
    
    InterruptSTM32F10x myInterrupt;

    volatile State myState;

    DmaChannelSTM32F10x* myTransmitDmaChannel;

    DmaChannelSTM32F10x* myReceiveDmaChannel;

    uint8_t myDmaBuffer[2];

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from Uart
    //--------------------------------------------------------------------------
    
    Error driverSetConfig(const Config& config);
    
    ComInterface::Error driverTransmitBytes(const ByteArray& byteArray,
                                            const bool waitUntilDone);

    uint32_t driverGetReceivedBytesCount();

    ComInterface::Error driverGetReceivedBytes(ByteArray& byteArray,
                                               const uint32_t nBytes);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void initialize();

    inline void setUsartEnabled(const bool enabled);

    inline void setBaudRate(const uint32_t baudRate);

    inline void setWordBits(const WordBits wordBits);

    inline void setStopBits(const StopBits stopBits);

    inline void setParity(const Parity parity);

    inline void setHardwareFlowControl(
                                 const HardwareFlowControl hardwareFlowControl);

    inline void setTransferMode(const TransferMode transferMode);

    inline void writeByte(const uint8_t byte);

    inline uint8_t readByte();

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void setTransmitDmaEnabled(const bool enabled);

    inline bool isTransmitDmaEnabled();

    inline void setReceiveDmaEnabled(const bool enabled);

    inline bool isReceiveDmaEnabled();

    inline void interruptHandler();

    inline void dmaByte1Received();

    inline void dmaByte2Received();
};

}; // namespace Plat4m

#endif // PLAT4M_UART_STM32F10X_H
