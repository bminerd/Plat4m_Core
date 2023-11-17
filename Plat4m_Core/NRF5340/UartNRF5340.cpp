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
/// @file UartNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief UartNRF5340 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstring>

#include <Plat4m_Core/NRF5340/UartNRF5340.h>
#include <Plat4m_Core/CallbackMethod.h>

using Plat4m::UartNRF5340;
using Plat4m::Uart;
using Plat4m::Module;
using Plat4m::ComInterface;
using Plat4m::InterruptNRF5340;
using Plat4m::GpioPinNRF5340;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptNRF5340* interruptObjectMap[4];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const InterruptNRF5340::Id UartNRF5340::myInterruptMap[] =
{
    InterruptNRF5340::ID_SPIM0_SPIS0_TWIM0_TWIS0_UARTE0, /// ID_0
    InterruptNRF5340::ID_SPIM1_SPIS1_TWIM1_TWIS1_UARTE1, /// ID_1
    InterruptNRF5340::ID_SPIM2_SPIS2_TWIM2_TWIS2_UARTE2, /// ID_2
    InterruptNRF5340::ID_SPIM3_SPIS3_TWIM3_TWIS3_UARTE3  /// ID_3
};

const std::uint32_t UartNRF5340::myInterruptEventEnableMap[] =
{
    0x00000001, /// INTERRUPT_EVENT_CLEAR_TO_SEND
    0x00000002, /// INTERRUPT_EVENT_NOT_CLEAR_TO_SEND
    0x00000004, /// INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD
    0x00000010, /// INTERRUPT_EVENT_RECEIVER_BUFFER_FULL
    0x00000080, /// INTERRUPT_EVENT_DATA_SENT_FROM_TXD
    0x00000100, /// INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED
    0x00000200, /// INTERRUPT_EVENT_ERROR
    0x00010000, /// INTERRUPT_EVENT_RECEIVER_TIMEOUT
    0x00080000, /// INTERRUPT_EVENT_RECEIVER_STARTED
    0x00100000, /// INTERRUPT_EVENT_TRANSMITTER_STARTED
    0x00400000  /// INTERRUPT_EVENT_TRANSMITTER_STOPPED
};

const std::uint32_t UartNRF5340::myBaudRateMap[] =
{
    0x0004F000, /// BAUD_RATE_1200
    0x0009D000, /// BAUD_RATE_2400
    0x0013B000, /// BAUD_RATE_4800
    0x00275000, /// BAUD_RATE_9600
    0x003AF000, /// BAUD_RATE_14400
    0x004EA000, /// BAUD_RATE_19200
    0x0075C000, /// BAUD_RATE_28800
    0x00800000, /// BAUD_RATE_31250
    0x009D0000, /// BAUD_RATE_38400
    0x00E50000, /// BAUD_RATE_56000
    0x00EB0000, /// BAUD_RATE_57600
    0x013A9000, /// BAUD_RATE_76800
    0x01D60000, /// BAUD_RATE_115200
    0x03B00000, /// BAUD_RATE_230400
    0x04000000, /// BAUD_RATE_250000
    0x07400000, /// BAUD_RATE_460800
    0x0F000000, /// BAUD_RATE_921600
    0x10000000  /// BAUD_RATE_1000000
};

// Variables

NRF_UARTE_Type* UartNRF5340::myUartMap[] =
{
    // NRF_UARTE0_NS, /// ID_0
    // NRF_UARTE1_NS, /// ID_1
    // NRF_UARTE2_NS, /// ID_2
    // NRF_UARTE3_NS  /// ID_3
    NRF_UARTE0_S, /// ID_0
    NRF_UARTE1_S, /// ID_1
    NRF_UARTE2_S, /// ID_2
    NRF_UARTE3_S  /// ID_3
};

const std::uint32_t UartNRF5340::myUartInterruptEventRegisterMap[] =
{
    0x00000100, /// INTERRUPT_EVENT_CLEAR_TO_SEND
    0x00000104, /// INTERRUPT_EVENT_NOT_CLEAR_TO_SEND
    0x00000108, /// INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD
    0x00000110, /// INTERRUPT_EVENT_RECEIVER_BUFFER_FULL
    0x0000011C, /// INTERRUPT_EVENT_DATA_SENT_FROM_TXD
    0x00000120, /// INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED
    0x00000124, /// INTERRUPT_EVENT_ERROR
    0x00000144, /// INTERRUPT_EVENT_RECEIVER_TIMEOUT
    0x0000014C, /// INTERRUPT_EVENT_RECEIVER_STARTED
    0x00000150, /// INTERRUPT_EVENT_TRANSMITTER_STARTED
    0x00000158  /// INTERRUPT_EVENT_TRANSMITTER_STOPPED
};

const std::uint32_t UartNRF5340::myUartTaskRegisterMap[] =
{
    0x00000000, /// TASK_START_RX
    0x00000004, /// TASK_STOP_RX
    0x00000008, /// TASK_START_TX
    0x0000000C, /// TASK_STOP_TX
    0x0000002C, /// TASK_FLUSH_RX
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
UartNRF5340::UartNRF5340(const Id id,
                         GpioPinNRF5340& transmitGpioPin,
                         GpioPinNRF5340& receiveGpioPin) :
    Uart(),
    myId(id),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitGpioPin),
    myReceiveGpioPin(receiveGpioPin),
    myClearToSendGpioPin(0),
    myReadyToSendGpioPin(0),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartNRF5340::interruptHandler)),
    myState(STATE_IDLE),
    myHasReceiveDmaRolledOver(false)
{
    initialize();
}

//------------------------------------------------------------------------------
UartNRF5340::UartNRF5340(const Id id,
                         GpioPinNRF5340& transmitGpioPin,
                         GpioPinNRF5340& receiveGpioPin,
                         GpioPinNRF5340& clearToSendGpioPin,
                         GpioPinNRF5340& readyToSendGpioPin) :
    Uart(),
    myId(id),
    myUart(myUartMap[id]),
    myTransmitGpioPin(transmitGpioPin),
    myReceiveGpioPin(receiveGpioPin),
    myClearToSendGpioPin(&clearToSendGpioPin),
    myReadyToSendGpioPin(&readyToSendGpioPin),
    myInterrupt(myInterruptMap[id],
                createCallback(this, &UartNRF5340::interruptHandler)),
    myState(STATE_IDLE),
    myHasReceiveDmaRolledOver(false)
{
    initialize();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinNRF5340& UartNRF5340::getTransmitGpioPin()
{
    return myTransmitGpioPin;
}

//------------------------------------------------------------------------------
GpioPinNRF5340& UartNRF5340::getReceiveGpioPin()
{
    return myReceiveGpioPin;
}

//------------------------------------------------------------------------------
InterruptNRF5340& UartNRF5340::getInterrupt()
{
    return myInterrupt;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error UartNRF5340::driverSetEnabled(const bool enabled)
{
    myTransmitGpioPin.setEnabled(enabled);
    myReceiveGpioPin.setEnabled(enabled);
    
    if (enabled)
    {
        GpioPin::Config gpioPinConfig;
        gpioPinConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL;
        gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;

        myTransmitGpioPin.configure(gpioPinConfig);

        gpioPinConfig.mode     = GpioPin::MODE_DIGITAL_INPUT;
        gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;

        myReceiveGpioPin.configure(gpioPinConfig);
        
        setTransmitPin(myTransmitGpioPin.getGpioPort().getId(),
                       myTransmitGpioPin.getId());
        setReceivePin(myReceiveGpioPin.getGpioPort().getId(),
                      myReceiveGpioPin.getId());

        if (isValidPointer(myClearToSendGpioPin) &&
                                           isValidPointer(myReadyToSendGpioPin))
        {
            gpioPinConfig.mode     = GpioPin::MODE_DIGITAL_INPUT;
            gpioPinConfig.resistor = GpioPin::RESISTOR_NONE;

            myClearToSendGpioPin->configure(gpioPinConfig);

            gpioPinConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL;
            gpioPinConfig.resistor = GpioPin::RESISTOR_PULL_UP;

            myReadyToSendGpioPin->configure(gpioPinConfig);

            setClearToSendPin(myClearToSendGpioPin->getGpioPort().getId(),
                              myClearToSendGpioPin->getId());
            setReadyToSendPin(myReadyToSendGpioPin->getGpioPort().getId(),
                              myReadyToSendGpioPin->getId());
        }
    }

    setInterruptEventEnabled(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD, enabled);
    setInterruptEventEnabled(INTERRUPT_EVENT_RECEIVER_BUFFER_FULL, enabled);

    InterruptNRF5340::Config interruptConfig;
    interruptConfig.priority = 7;
    myInterrupt.configure(interruptConfig);

    myInterrupt.setEnabled(enabled);

    setPeripheralEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Uart
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Uart::Error UartNRF5340::driverSetConfig(const Config& config)
{
    if (!setBaudRate(config.baudRate) ||
        !setWordBits(config.wordBits) ||
        !setParity(config.parity))
    {
        return Error(ERROR_CODE_PARAMETER_INVALID);
    }

    setStopBits(config.stopBits);
    setHardwareFlowControl(config.hardwareFlowControl);

    if (getReceiveBuffer()->getSize() > 0)
    {
        setReceiveDmaBuffer(getReceiveBuffer()->getItems());
        setReceiveDmaBufferByteCount(getReceiveBuffer()->getSize());

        triggerTask(TASK_START_RX);

        // Queue DMA settings for next transaction

        setReceiveDmaBuffer(getReceiveBuffer()->getItems());
        setReceiveDmaBufferByteCount(getReceiveBuffer()->getSize());
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterface::Error UartNRF5340::driverTransmitBytes(const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
    ComInterface::Error error;

    const std::uint32_t nBytes = byteArray.getSize();

    if (nBytes == 0)
    {
    	return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
    }

    while (myState == STATE_BUSY)
    {
        // Wait
    }

    myState = STATE_BUSY;

    getTransmitBuffer()->clear();
    getTransmitBuffer()->write(byteArray);

    setTransmitDmaBuffer(getTransmitBuffer()->getItems());
    setTransmitDmaBufferByteCount(nBytes);

    setInterruptEventEnabled(INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED, true);

    triggerTask(TASK_START_TX);

    if (waitUntilDone)
    {
        while (myState == STATE_BUSY)
        {
        }
    }

    return error;
}

//------------------------------------------------------------------------------
uint32_t UartNRF5340::driverGetReceivedBytesCount()
{
    const bool wasReceiveInterruptEnabled =
                  isInterruptEventEnabled(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD);

    if (wasReceiveInterruptEnabled)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD, false);
    }

    std::uint32_t nBytes = getReceiveBuffer()->count();

    if (wasReceiveInterruptEnabled)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD, true);
    }

    return nBytes;
}

//------------------------------------------------------------------------------
ComInterface::Error UartNRF5340::driverGetReceivedBytes(
                                                     ByteArray& byteArray,
                                                     const std::uint32_t nBytes)
{
    std::uint8_t byte;
    std::uint32_t nBytesToRead;

    if (nBytes == 0)
    {
        nBytesToRead = driverGetReceivedBytesCount();
    }
    else
    {
        nBytesToRead = nBytes;
    }

    while (nBytesToRead--)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD, false);

        if (getReceiveBuffer()->read(byte))
        {
            byteArray.append(byte);
        }

        setInterruptEventEnabled(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD, true);
    }

    return ComInterface::Error(ComInterface::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void UartNRF5340::initialize()
{
    if (isValidPointer(interruptObjectMap[myId]))
    {
        // Instantiating same peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;
}

//------------------------------------------------------------------------------
void UartNRF5340::setPeripheralEnabled(const bool enabled)
{
    setBitsSet(myUart->ENABLE, 0x00000008, enabled);
}

//------------------------------------------------------------------------------
bool UartNRF5340::setBaudRate(const std::uint32_t baudRate)
{
    bool returnValue = true;

    switch (baudRate)
    {
        case 1200:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_1200];

            break;
        }
        case 2400:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_2400];

            break;
        }
        case 4800:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_4800];
            
            break;
        }
        case 9600:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_9600];
            
            break;
        }
        case 14400:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_14400];
            
            break;
        }
        case 19200:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_19200];
            
            break;
        }
        case 28800:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_28800];
            
            break;
        }
        case 31250:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_31250];
            
            break;
        }
        case 38400:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_38400];
            
            break;
        }
        case 56000:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_56000];
            
            break;
        }
        case 57600:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_57600];
            
            break;
        }
        case 76800:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_76800];
            
            break;
        }
        case 115200:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_115200];
            
            break;
        }
        case 230400:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_230400];
            
            break;
        }
        case 250000:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_250000];
            
            break;
        }
        case 460800:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_460800];
            
            break;
        }
        case 912600:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_921600];
            
            break;
        }
        case 1000000:
        {
            myUart->BAUDRATE = myBaudRateMap[BAUD_RATE_1000000];
            
            break;
        }
        default:
        {
            returnValue = false;

            break;
        }
    }

    return returnValue;
}

//------------------------------------------------------------------------------
bool UartNRF5340::setWordBits(const WordBits wordBits)
{
    bool returnValue = false;

    if (wordBits == Uart::WORD_BITS_8)
    {
        returnValue = true;
    }

    return returnValue;
}

//------------------------------------------------------------------------------
void UartNRF5340::setStopBits(const StopBits stopBits)
{
    setBitsSet(myUart->CONFIG, 0x00000010, (stopBits == STOP_BITS_2));
}

//------------------------------------------------------------------------------
bool UartNRF5340::setParity(const Parity parity)
{
    bool returnValue = true;

    setBitsSet(myUart->CONFIG, 0x0000000E, (parity != PARITY_NONE));
    setBitsSet(myUart->CONFIG, 0x00000100, (parity == PARITY_ODD));

    return returnValue;
}

//------------------------------------------------------------------------------
void UartNRF5340::setHardwareFlowControl(
                                  const HardwareFlowControl hardwareFlowControl)
{
    setBitSet(myUart->CONFIG,
              0x00000001,
              (hardwareFlowControl == HARDWARE_FLOW_CONTROL_ENABLED));
}

//------------------------------------------------------------------------------
void UartNRF5340::setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                           const bool enabled)
{
    setBitsSet(myUart->INTEN,
               myInterruptEventEnableMap[interruptEvent],
               enabled);
}

//------------------------------------------------------------------------------
bool UartNRF5340::isInterruptEventEnabled(const InterruptEvent interruptEvent)
{
    return areBitsSet(myUart->INTEN, myInterruptEventEnableMap[interruptEvent]);
}

//------------------------------------------------------------------------------
bool UartNRF5340::isInterruptEventPending(const InterruptEvent interruptEvent)
{
    bool isPending =
        *((std::uint32_t*)(((std::uint32_t) myUart) +
                              myUartInterruptEventRegisterMap[interruptEvent]));

    return isPending;
}

//------------------------------------------------------------------------------
void UartNRF5340::setInterruptEvent(const InterruptEvent interruptEvent)
{
    *((std::uint32_t*)(((std::uint32_t) myUart) +
                          myUartInterruptEventRegisterMap[interruptEvent])) = 1;
}

//------------------------------------------------------------------------------
void UartNRF5340::clearInterruptEvent(const InterruptEvent interruptEvent)
{
    *((std::uint32_t*)(((std::uint32_t) myUart) +
                          myUartInterruptEventRegisterMap[interruptEvent])) = 0;
}

//------------------------------------------------------------------------------
void UartNRF5340::setTransmitDmaBuffer(std::uint8_t* buffer)
{
    myUart->TXD.PTR = (std::uint32_t) buffer;
}

//------------------------------------------------------------------------------
void UartNRF5340::setTransmitDmaBufferByteCount(const std::uint32_t byteCount)
{
    myUart->TXD.MAXCNT = byteCount;
}

//------------------------------------------------------------------------------
void UartNRF5340::setReceiveDmaBuffer(std::uint8_t* buffer)
{
    myUart->RXD.PTR = (std::uint32_t) buffer;
}

//------------------------------------------------------------------------------
void UartNRF5340::setReceiveDmaBufferByteCount(const std::uint32_t byteCount)
{
    myUart->RXD.MAXCNT = byteCount;
}

//------------------------------------------------------------------------------
void UartNRF5340::triggerTask(const Task task)
{
    *((std::uint32_t*)(((std::uint32_t) myUart) +
                                              myUartTaskRegisterMap[task])) = 1;
}

//------------------------------------------------------------------------------
void UartNRF5340::setTransmitPin(const GpioPortNRF5340::Id gpioPortId,
                                 const GpioPinNRF5340::Id gpioPinId)
{
    clearAndSetBits(myUart->PSEL.TXD, 0x00000020, gpioPortId << 5);
    clearAndSetBits(myUart->PSEL.TXD, 0x0000001F, gpioPinId);
    clearBits(myUart->PSEL.TXD, 0x80000000);
}

//------------------------------------------------------------------------------
void UartNRF5340::setReceivePin(const GpioPortNRF5340::Id gpioPortId,
                                const GpioPinNRF5340::Id gpioPinId)
{
    clearAndSetBits(myUart->PSEL.RXD, 0x00000020, gpioPortId << 5);
    clearAndSetBits(myUart->PSEL.RXD, 0x0000001F, gpioPinId);
    clearBits(myUart->PSEL.RXD, 0x80000000);
}

//------------------------------------------------------------------------------
void UartNRF5340::setClearToSendPin(const GpioPortNRF5340::Id gpioPortId,
                                    const GpioPinNRF5340::Id gpioPinId)
{
    clearAndSetBits(myUart->PSEL.CTS, 0x00000020, gpioPortId << 5);
    clearAndSetBits(myUart->PSEL.CTS, 0x0000001F, gpioPinId);
    clearBits(myUart->PSEL.CTS, 0x80000000);
}

//------------------------------------------------------------------------------
void UartNRF5340::setReadyToSendPin(const GpioPortNRF5340::Id gpioPortId,
                                    const GpioPinNRF5340::Id gpioPinId)
{
    clearAndSetBits(myUart->PSEL.RTS, 0x00000020, gpioPortId << 5);
    clearAndSetBits(myUart->PSEL.RTS, 0x0000001F, gpioPinId);
    clearBits(myUart->PSEL.RTS, 0x80000000);
}

//------------------------------------------------------------------------------
void UartNRF5340::interruptHandler()
{
    // The ENDRX event is highest priority to ensure we wrap the DMA back to the
    // start of the receive buffer
    if (isInterruptEventPending(INTERRUPT_EVENT_RECEIVER_BUFFER_FULL))
    {
        clearInterruptEvent(INTERRUPT_EVENT_RECEIVER_BUFFER_FULL);

        triggerTask(TASK_START_RX);

        // Queue DMA settings for next transaction
        setReceiveDmaBuffer(getReceiveBuffer()->getItems());
        setReceiveDmaBufferByteCount(getReceiveBuffer()->getSize());

        myHasReceiveDmaRolledOver = true;
    }

    // Use the RXRDY event to count the number of bytes that have been received
    // via DMA
    if (isInterruptEventPending(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD))
    {
        clearInterruptEvent(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD);

        // Force increment the receive buffer write index and count

        std::uint32_t writeIndex = getReceiveBuffer()->getWriteIndex();
        writeIndex++;
        getReceiveBuffer()->setWriteIndex(writeIndex);
        std::uint32_t count = getReceiveBuffer()->count();
        count++;
        getReceiveBuffer()->setCount(count);

        // If the receiver DMA has rolled over the end of the buffer (and has
        // been reset above), update the Buffer read and write indices
        if (myHasReceiveDmaRolledOver)
        {
            std::uint32_t size = getReceiveBuffer()->getSize();
            std::uint32_t writeIndex = getReceiveBuffer()->getWriteIndex();
            std::uint32_t readIndex = getReceiveBuffer()->getReadIndex();

            std::uint32_t count = 0;

            if (writeIndex >= readIndex)
            {
                count = writeIndex - readIndex;
            }
            else
            {
                count = writeIndex + (size - readIndex);
            }

            getReceiveBuffer()->setCount(count);

            myHasReceiveDmaRolledOver = false;
        }

        std::uint8_t byte;

        if (getReceiveBuffer()->read(byte))
        {
            byteReceived(byte);
        }
    }

    if (isInterruptEventPending(INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED))
    {
        clearInterruptEvent(INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED);
        setInterruptEventEnabled(INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED,
                                 false);

        myState = STATE_IDLE;
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void SPIM0_SPIS0_TWIM0_TWIS0_UARTE0_IRQHandler(void)
{
    interruptObjectMap[UartNRF5340::ID_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void SPIM1_SPIS1_TWIM1_TWIS1_UARTE1_IRQHandler(void)
{
    interruptObjectMap[UartNRF5340::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void SPIM2_SPIS2_TWIM2_TWIS2_UARTE2_IRQHandler(void)
{
    interruptObjectMap[UartNRF5340::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void SPIM3_SPIS3_TWIM3_TWIS3_UARTE3_IRQHandler(void)
{
    interruptObjectMap[UartNRF5340::ID_3]->handler();
}
