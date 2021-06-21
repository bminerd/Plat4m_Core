/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file UartNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief UartNRF5340 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <NRF5340/UartNRF5340.h>
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

static InterruptNRF5340* interruptObjectMap[2];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const InterruptNRF5340::Id UartNRF5340::myInterruptMap[] =
{
    InterruptNRF5340::ID_UARTE0_UART0, /// ID_0
    InterruptNRF5340::ID_UARTE1        /// ID_1
};

const uint32_t UartNRF5340::myInterruptEventEnableMap[] =
{
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

const uint32_t UartNRF5340::myBaudRateMap[] =
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
    NRF_UARTE0, /// ID_0
    NRF_UARTE1  /// ID_1
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
// Private methods implemented from Module
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
// Private methods implemented from Uart
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

    setTransmitDmaBuffer(getTransmitBuffer()->getItems());

    setReceiveDmaBuffer(getReceiveBuffer()->getItems());
    setReceiveDmaBufferByteCount(getReceiveBuffer()->getSize());

    triggerTask(TASK_START_RX);

    // Queue DMA settings for next transaction

    setReceiveDmaBuffer(getReceiveBuffer()->getItems());
    setReceiveDmaBufferByteCount(getReceiveBuffer()->getSize());

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ComInterface::Error UartNRF5340::driverTransmitBytes(
                                                     const ByteArray& byteArray,
                                                     const bool waitUntilDone)
{
    ComInterface::Error error;

    const uint32_t nBytes = byteArray.getSize();

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

    uint32_t nBytes = getReceiveBuffer()->count();

    if (wasReceiveInterruptEnabled)
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD, true);
    }

    return nBytes;
}

//------------------------------------------------------------------------------
ComInterface::Error UartNRF5340::driverGetReceivedBytes(ByteArray& byteArray,
                                                         const uint32_t nBytes)
{
    uint8_t byte;
    uint32_t nBytesToRead;

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
bool UartNRF5340::setBaudRate(const uint32_t baudRate)
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
    bool returnValue = false;

    if (parity != PARITY_ODD)
    {
        setBitsSet(myUart->CONFIG, 0x0000000E, (parity == PARITY_EVEN));

        returnValue = true;
    }

    return returnValue;
}

//------------------------------------------------------------------------------
void UartNRF5340::setHardwareFlowControl(
                                  const HardwareFlowControl hardwareFlowControl)
{
    // Hardware flow control isn't currently supported in Plat4m
}

//------------------------------------------------------------------------------
void UartNRF5340::writeByte(const uint8_t byte)
{
    ((NRF_UART_Type*) myUart)->TXD = byte;
}

//------------------------------------------------------------------------------
uint8_t UartNRF5340::readByte()
{
    return (((NRF_UART_Type*) myUart)->RXD);
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
    bool isPending = false;

    switch (interruptEvent)
    {
        case INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD:
        {
            isPending = myUart->EVENTS_RXDRDY;

            break;
        }
        case INTERRUPT_EVENT_RECEIVER_BUFFER_FULL:
        {
            isPending = myUart->EVENTS_ENDRX;

            break;
        }
        case INTERRUPT_EVENT_DATA_SENT_FROM_TXD:
        {
            isPending = myUart->EVENTS_TXDRDY;

            break;
        }
        case INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED:
        {
            isPending = myUart->EVENTS_ENDTX;

            break;
        }
        case INTERRUPT_EVENT_ERROR:
        {
            isPending = myUart->EVENTS_ERROR;

            break;
        }
        case INTERRUPT_EVENT_RECEIVER_TIMEOUT:
        {
            isPending = myUart->EVENTS_RXTO;

            break;
        }
        case INTERRUPT_EVENT_RECEIVER_STARTED:
        {
            isPending = myUart->EVENTS_RXSTARTED;

            break;
        }
        case INTERRUPT_EVENT_TRANSMITTER_STARTED:
        {
            isPending = myUart->EVENTS_TXSTARTED;

            break;
        }
        case INTERRUPT_EVENT_TRANSMITTER_STOPPED:
        {
            isPending = myUart->EVENTS_TXSTOPPED;

            break;
        }
        default:
        {
            break;
        }
    }

    return isPending;
}

//------------------------------------------------------------------------------
void UartNRF5340::clearInterruptEvent(const InterruptEvent interruptEvent)
{
    switch (interruptEvent)
    {
        case INTERRUPT_EVENT_DATA_RECEIVED_IN_RXD:
        {
            myUart->EVENTS_RXDRDY = 0;

            break;
        }
        case INTERRUPT_EVENT_RECEIVER_BUFFER_FULL:
        {
            myUart->EVENTS_ENDRX = 0;

            break;
        }
        case INTERRUPT_EVENT_DATA_SENT_FROM_TXD:
        {
            myUart->EVENTS_TXDRDY = 0;

            break;
        }
        case INTERRUPT_EVENT_LAST_TX_BYTE_TRANSMITTED:
        {
            myUart->EVENTS_ENDTX = 0;

            break;
        }
        case INTERRUPT_EVENT_ERROR:
        {
            myUart->EVENTS_ERROR = 0;

            break;
        }
        case INTERRUPT_EVENT_RECEIVER_TIMEOUT:
        {
            myUart->EVENTS_RXTO = 0;

            break;
        }
        case INTERRUPT_EVENT_RECEIVER_STARTED:
        {
            myUart->EVENTS_RXSTARTED = 0;

            break;
        }
        case INTERRUPT_EVENT_TRANSMITTER_STARTED:
        {
            myUart->EVENTS_TXSTARTED = 0;

            break;
        }
        case INTERRUPT_EVENT_TRANSMITTER_STOPPED:
        {
            myUart->EVENTS_TXSTOPPED = 0;

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void UartNRF5340::setTransmitDmaBuffer(uint8_t* buffer)
{
    myUart->TXD.PTR = (uint32_t) buffer;
}

//------------------------------------------------------------------------------
void UartNRF5340::setTransmitDmaBufferByteCount(const uint32_t byteCount)
{
    myUart->TXD.MAXCNT = byteCount;
}

//------------------------------------------------------------------------------
void UartNRF5340::setReceiveDmaBuffer(uint8_t* buffer)
{
    myUart->RXD.PTR = (uint32_t) buffer;
}

//------------------------------------------------------------------------------
void UartNRF5340::setReceiveDmaBufferByteCount(const uint32_t byteCount)
{
    myUart->RXD.MAXCNT = byteCount;
}

//------------------------------------------------------------------------------
void UartNRF5340::triggerTask(const Task task)
{
    switch (task)
    {
        case TASK_START_RX:
        {
            myUart->TASKS_STARTRX = 1;

            break;
        }
        case TASK_STOP_RX:
        {
            myUart->TASKS_STOPRX = 1;

            break;
        }
        case TASK_START_TX:
        {
            myUart->TASKS_STARTTX = 1;

            break;
        }
        case TASK_STOP_TX:
        {
            myUart->TASKS_STOPTX = 1;
            
            break;
        }
        case TASK_SUSPEND:
        {
            ((NRF_UART_Type*) myUart)->TASKS_SUSPEND = 1;
            
            break;
        }
        case TASK_FLUSH_RX:
        {
            myUart->TASKS_FLUSHRX = 1;

            break;
        }
        default:
        {
            break;
        }
    }
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

        uint32_t writeIndex = getReceiveBuffer()->getWriteIndex();
        writeIndex++;
        getReceiveBuffer()->setWriteIndex(writeIndex);
        uint32_t count = getReceiveBuffer()->count();
        count++;
        getReceiveBuffer()->setCount(count);

        // If the receiver DMA has rolled over the end of the buffer (and has
        // been reset above), update the Buffer read and write indices
        if (myHasReceiveDmaRolledOver)
        {
            uint32_t size = getReceiveBuffer()->getSize();
            uint32_t writeIndex = getReceiveBuffer()->getWriteIndex();
            uint32_t readIndex = getReceiveBuffer()->getReadIndex();

            uint32_t count = 0;

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

        uint8_t byte;

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
extern "C" void UARTE0_UART0_IRQHandler(void)
{
    interruptObjectMap[UartNRF5340::ID_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void UARTE1_IRQHandler(void)
{
    interruptObjectMap[UartNRF5340::ID_1]->handler();
}
