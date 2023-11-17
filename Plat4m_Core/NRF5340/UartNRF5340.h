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
// Copyright (c) 2021-2023 Benjamin Minerd
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
/// @file UartNRF5340.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief UartNRF5340 class header file.
///

#ifndef PLAT4M_UART_NRF5340_H
#define PLAT4M_UART_NRF5340_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <nrf5340_application.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Uart.h>
#include <Plat4m_Core/NRF5340/GpioPinNRF5340.h>
#include <Plat4m_Core/NRF5340/InterruptNRF5340.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class UartNRF5340 : public Uart
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_0 = 0,
        ID_1,
        ID_2,
        ID_3
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_CLEAR_TO_SEND = 0,
        INTERRUPT_EVENT_NOT_CLEAR_TO_SEND,
        INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD,
        INTERRUPT_EVENT_RECEIVER_BUFFER_FULL,
        INTERRUPT_EVENT_DATA_SENT_FROM_TXD,
        INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED,
        INTERRUPT_EVENT_ERROR,
        INTERRUPT_EVENT_RECEIVER_TIMEOUT,
        INTERRUPT_EVENT_RECEIVER_STARTED,
        INTERRUPT_EVENT_TRANSMITTER_STARTED,
        INTERRUPT_EVENT_TRANSMITTER_STOPPED
    };

    enum Task
    {
        TASK_START_RX = 0,
        TASK_STOP_RX,
        TASK_START_TX,
        TASK_STOP_TX,
        TASK_FLUSH_RX
    };

    enum State
    {
        STATE_IDLE = 0,
        STATE_BUSY
    };

    enum BaudRate
    {
        BAUD_RATE_1200 = 0,
        BAUD_RATE_2400,
        BAUD_RATE_4800,
        BAUD_RATE_9600,
        BAUD_RATE_14400,
        BAUD_RATE_19200,
        BAUD_RATE_28800,
        BAUD_RATE_31250,
        BAUD_RATE_38400,
        BAUD_RATE_56000,
        BAUD_RATE_57600,
        BAUD_RATE_76800,
        BAUD_RATE_115200,
        BAUD_RATE_230400,
        BAUD_RATE_250000,
        BAUD_RATE_460800,
        BAUD_RATE_921600,
        BAUD_RATE_1000000
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    UartNRF5340(const Id id,
                GpioPinNRF5340& transmitGpioPin,
                GpioPinNRF5340& receiveGpioPin);

    UartNRF5340(const Id id,
                GpioPinNRF5340& transmitGpioPin,
                GpioPinNRF5340& receiveGpioPin,
                GpioPinNRF5340& clearToSendGpioPin,
                GpioPinNRF5340& readyToSendGpioPin);
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    GpioPinNRF5340& getTransmitGpioPin();

    GpioPinNRF5340& getReceiveGpioPin();

    InterruptNRF5340& getInterrupt();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const InterruptNRF5340::Id myInterruptMap[];

    static const std::uint16_t myParityMap[];

    static const std::uint16_t myFlowControlMap[];

    static const std::uint32_t myInterruptEventEnableMap[];

    static const std::uint32_t myInterruptEventPendingMap[];

    static const std::uint32_t myBaudRateMap[];

    static const std::uint32_t myUartInterruptEventRegisterMap[];

    static const std::uint32_t myUartTaskRegisterMap[];

    // Variables

    static NRF_UARTE_Type* myUartMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    NRF_UARTE_Type* myUart;

    GpioPinNRF5340& myTransmitGpioPin;

    GpioPinNRF5340& myReceiveGpioPin;

    GpioPinNRF5340* myClearToSendGpioPin;

    GpioPinNRF5340* myReadyToSendGpioPin;

    InterruptNRF5340 myInterrupt;

    volatile State myState;

    volatile bool myHasReceiveDmaRolledOver;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Private methods implemented from Uart
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) override;

    virtual ComInterface::Error driverTransmitBytes(
                                             const ByteArray& byteArray,
                                             const bool waitUntilDone) override;

    virtual std::uint32_t driverGetReceivedBytesCount() override;

    virtual ComInterface::Error driverGetReceivedBytes(
                                           ByteArray& byteArray,
                                           const std::uint32_t nBytes) override;

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void initialize();

    inline void setPeripheralEnabled(const bool enabled);

    inline bool setBaudRate(const std::uint32_t baudRate);

    inline bool setWordBits(const WordBits wordBits);

    inline void setStopBits(const StopBits stopBits);

    inline bool setParity(const Parity parity);

    inline void setHardwareFlowControl(
                                 const HardwareFlowControl hardwareFlowControl);

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void setInterruptEvent(const InterruptEvent interruptEvent);

    inline void clearInterruptEvent(const InterruptEvent interruptEvent);

    inline void setTransmitDmaBuffer(std::uint8_t* buffer);

    inline void setTransmitDmaBufferByteCount(const std::uint32_t byteCount);

    inline void setReceiveDmaBuffer(std::uint8_t* buffer);

    inline void setReceiveDmaBufferByteCount(const std::uint32_t byteCount);

    inline void triggerTask(const Task task);

    inline void setTransmitPin(const GpioPortNRF5340::Id gpioPortId,
                               const GpioPinNRF5340::Id gpioPinId);

    inline void setReceivePin(const GpioPortNRF5340::Id gpioPortId,
                               const GpioPinNRF5340::Id gpioPinId);

    inline void setClearToSendPin(const GpioPortNRF5340::Id gpioPortId,
                                  const GpioPinNRF5340::Id gpioPinId);

    inline void setReadyToSendPin(const GpioPortNRF5340::Id gpioPortId,
                                  const GpioPinNRF5340::Id gpioPinId);

    inline void interruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_UART_NRF5340_H
