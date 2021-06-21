/**
 * Copyright (c) 2021, SignalQuest LLC
 */

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

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Uart.h>
#include <NRF5340/GpioPinNRF5340.h>
#include <NRF5340/InterruptNRF5340.h>

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
        ID_1
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD = 0,
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
        TASK_SUSPEND,
        TASK_FLUSH_RX
    };

    enum State
    {
        STATE_IDLE,
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

    static const uint16_t myParityMap[];

    static const uint16_t myFlowControlMap[];

    static const uint32_t myInterruptEventEnableMap[];

    static const uint32_t myInterruptEventPendingMap[];

    static const uint32_t myBaudRateMap[];

    // Variables

    static NRF_UARTE_Type* myUartMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;

    NRF_UARTE_Type* myUart;
    
    GpioPinNRF5340& myTransmitGpioPin;
    
    GpioPinNRF5340& myReceiveGpioPin;
    
    InterruptNRF5340 myInterrupt;

    volatile State myState;

    volatile bool myHasReceiveDmaRolledOver;

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

    inline void setPeripheralEnabled(const bool enabled);

    inline bool setBaudRate(const uint32_t baudRate);

    inline bool setWordBits(const WordBits wordBits);

    inline void setStopBits(const StopBits stopBits);

    inline bool setParity(const Parity parity);

    inline void setHardwareFlowControl(
                                 const HardwareFlowControl hardwareFlowControl);

    inline void writeByte(const uint8_t byte);

    inline uint8_t readByte();

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void clearInterruptEvent(const InterruptEvent interruptEvent);

    inline void setTransmitDmaBuffer(uint8_t* buffer);

    inline void setTransmitDmaBufferByteCount(const uint32_t byteCount);

    inline void setReceiveDmaBuffer(uint8_t* buffer);

    inline void setReceiveDmaBufferByteCount(const uint32_t byteCount);

    inline void triggerTask(const Task task);

    inline void setTransmitPin(const GpioPortNRF5340::Id gpioPortId,
                               const GpioPinNRF5340::Id gpioPinId);

    inline void setReceivePin(const GpioPortNRF5340::Id gpioPortId,
                               const GpioPinNRF5340::Id gpioPinId);

    inline void interruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_UART_NRF5340_H
