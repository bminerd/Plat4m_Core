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
/// @file UartSTM32F30x.h
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief UartSTM32F30x class header file.
///

#ifndef PLAT4M_UART_STM32F30X_H
#define PLAT4M_UART_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f30x.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Uart.h>
#include <Plat4m_Core/STM32F30x/GpioPinSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/InterruptSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/ProcessorSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/DmaChannelSTM32F30x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class UartSTM32F30x : public Uart
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
        INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY = 0,
        INTERRUPT_EVENT_TRANSMISSION_COMPLETE,
        INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY
    };

    enum State
    {
        STATE_IDLE,
        STATE_BUSY
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

    UartSTM32F30x(const Id id,
                  const TransferMode transferMode,
                  GpioPinSTM32F30x& gpioPin);

    UartSTM32F30x(const Id id,
                  GpioPinSTM32F30x& transmitGpioPin,
                  GpioPinSTM32F30x& receiveGpioPin);
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    GpioPinSTM32F30x& getTransmitGpioPin();

    GpioPinSTM32F30x* getReceiveGpioPin();

    InterruptSTM32F30x& getInterrupt();

    void setTransmitDmaChannel(DmaChannelSTM32F30x& transmitDmaChannel);

    void setReceiveDmaChannel(DmaChannelSTM32F30x& receiveDmaChannel);

    inline void interruptHandler();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    // Constants

    static const ProcessorSTM32F30x::Peripheral myPeripheralMap[];

    static const InterruptSTM32F30x::Id myInterruptMap[];

    static const uint16_t myWordLengthMap[];

    static const uint16_t myStopBitsMap[];

    static const uint16_t myParityMap[];

    static const uint16_t myFlowControlMap[];

    static const uint32_t myInterruptEventEnableMap[];

    static const uint32_t myInterruptEventPendingMap[];

    // Variables

    static USART_TypeDef* myUartMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    const TransferMode myTransferMode;

    USART_TypeDef* myUart;
    
    GpioPinSTM32F30x& myTransmitGpioPin;
    
    GpioPinSTM32F30x* myReceiveGpioPin;
    
    InterruptSTM32F30x myInterrupt;

    volatile State myState;

    DmaChannelSTM32F30x* myTransmitDmaChannel;

    DmaChannelSTM32F30x* myReceiveDmaChannel;

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

//    inline void interruptHandler();

    inline void dmaByte1Received();

    inline void dmaByte2Received();

    inline void transmitDmaTransferComplete();
};

}; // namespace Plat4m

#endif // PLAT4M_UART_STM32F30X_H
