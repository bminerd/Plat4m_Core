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
// Copyright (c) 2022-2023 Benjamin Minerd
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
/// @file UartSTM32H7xx.h
/// @author Ben Minerd
/// @date 6/27/2022
/// @brief UartSTM32H7xx class header file.
///

#ifndef PLAT4M_UART_STM32H7XX_H
#define PLAT4M_UART_STM32H7XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32h7xx.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Uart.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/STM32H7xx/GpioPinSTM32H7xx.h>
#include <Plat4m_Core/STM32H7xx/InterruptSTM32H7xx.h>
#include <Plat4m_Core/STM32H7xx/ProcessorSTM32H7xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class UartSTM32H7xx : public Uart
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_DUPLICATE_PERIPHERAL_INSTANTIATION
    };

    using Error = ErrorTemplate<ErrorCode>;

    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3,
        ID_4,
        ID_5,
        ID_6,
        ID_7,
        ID_8
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_TRANSMIT_FIFO_EMPTY = 0,
        INTERRUPT_EVENT_TRANSMISSION_COMPLETE,
        INTERRUPT_EVENT_RECEIVE_FIFO_NOT_EMPTY
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

    struct Registers
    {
        struct CR1
        {
            enum BitMask : std::uint32_t
            {
                UE      = bitMask<std::uint32_t>(1,  0),
                UESM    = bitMask<std::uint32_t>(1,  1),
                RE      = bitMask<std::uint32_t>(1,  2),
                TE      = bitMask<std::uint32_t>(1,  3),
                IDLEIE  = bitMask<std::uint32_t>(1,  4),
                RXFNEIE = bitMask<std::uint32_t>(1,  5),
                TCIE    = bitMask<std::uint32_t>(1,  6),
                TXFNEIE = bitMask<std::uint32_t>(1,  7),
                PEIE    = bitMask<std::uint32_t>(1,  8),
                PS      = bitMask<std::uint32_t>(1,  9),
                PCE     = bitMask<std::uint32_t>(1, 10),
                WAKE    = bitMask<std::uint32_t>(1, 11),
                M0      = bitMask<std::uint32_t>(1, 12),
                MME     = bitMask<std::uint32_t>(1, 13),
                CMIE    = bitMask<std::uint32_t>(1, 14),
                OVER8   = bitMask<std::uint32_t>(1, 15),
                DEDT    = bitMask<std::uint32_t>(5, 16),
                DEAT    = bitMask<std::uint32_t>(5, 21),
                RTOIE   = bitMask<std::uint32_t>(1, 26),
                EOBIE   = bitMask<std::uint32_t>(1, 27),
                M1      = bitMask<std::uint32_t>(1, 28),
                FIFOEN  = bitMask<std::uint32_t>(1, 29),
                TXFEIE  = bitMask<std::uint32_t>(1, 30),
                RXFFIE  = bitMask<std::uint32_t>(1, 31)
            };
        };
        struct CR2
        {
            enum BitMask : std::uint32_t
            {
                SLVEN    = bitMask<std::uint32_t>(1,  0),
                DIS_NSS  = bitMask<std::uint32_t>(1,  3),
                ADDM7    = bitMask<std::uint32_t>(1,  4),
                LBDL     = bitMask<std::uint32_t>(1,  5),
                LBDIE    = bitMask<std::uint32_t>(1,  6),
                LBCL     = bitMask<std::uint32_t>(1,  8),
                CPHA     = bitMask<std::uint32_t>(1,  9),
                CPOL     = bitMask<std::uint32_t>(1, 10),
                CLKEN    = bitMask<std::uint32_t>(1, 11),
                STOP     = bitMask<std::uint32_t>(2, 12),
                LINEN    = bitMask<std::uint32_t>(1, 14),
                SWAP     = bitMask<std::uint32_t>(1, 15),
                RXINV    = bitMask<std::uint32_t>(1, 16),
                TXINV    = bitMask<std::uint32_t>(1, 17),
                DATAINV  = bitMask<std::uint32_t>(1, 18),
                MSBFIRST = bitMask<std::uint32_t>(1, 19),
                ARBEN    = bitMask<std::uint32_t>(1, 20),
                ABRMOD   = bitMask<std::uint32_t>(2, 21),
                RTOEN    = bitMask<std::uint32_t>(1, 23),
                ADD      = bitMask<std::uint32_t>(8, 24)
            };
        };
        struct CR3
        {
            enum BitMask : std::uint32_t
            {
                EIE     = bitMask<std::uint32_t>(1,  0),
                IREN    = bitMask<std::uint32_t>(1,  1),
                IRLP    = bitMask<std::uint32_t>(1,  2),
                HDSEL   = bitMask<std::uint32_t>(1,  3),
                NACK    = bitMask<std::uint32_t>(1,  4),
                SCEN    = bitMask<std::uint32_t>(1,  5),
                DMAR    = bitMask<std::uint32_t>(1,  6),
                DMAT    = bitMask<std::uint32_t>(1,  7),
                RTSE    = bitMask<std::uint32_t>(1,  8),
                CTSE    = bitMask<std::uint32_t>(1,  9),
                CTSIE   = bitMask<std::uint32_t>(1, 10),
                ONEBIT  = bitMask<std::uint32_t>(1, 11),
                OVRDIS  = bitMask<std::uint32_t>(1, 12),
                DDRE    = bitMask<std::uint32_t>(1, 13),
                DEM     = bitMask<std::uint32_t>(1, 14),
                DEP     = bitMask<std::uint32_t>(1, 15),
                SCARCNT = bitMask<std::uint32_t>(3, 17),
                WUS     = bitMask<std::uint32_t>(2, 20),
                WUFIE   = bitMask<std::uint32_t>(1, 22),
                TXFTIE  = bitMask<std::uint32_t>(1, 23),
                TCBGTIE = bitMask<std::uint32_t>(1, 24),
                RXFTCFG = bitMask<std::uint32_t>(3, 25),
                RXFTIE  = bitMask<std::uint32_t>(1, 28),
                TXFTCFG = bitMask<std::uint32_t>(3, 29)
            };
        };

        enum BrrBitMask : std::uint32_t
        {
            BRR = bitMask<std::uint32_t>(16, 0)
        };

        struct ISR
        {
            enum BitMask : std::uint32_t
            {
                PE    = bitMask<std::uint32_t>(1,  0),
                FE    = bitMask<std::uint32_t>(1,  1),
                NE    = bitMask<std::uint32_t>(1,  2),
                ORE   = bitMask<std::uint32_t>(1,  3),
                IDLE  = bitMask<std::uint32_t>(1,  4),
                RXFNE = bitMask<std::uint32_t>(1,  5),
                TC    = bitMask<std::uint32_t>(1,  6),
                TXFNF = bitMask<std::uint32_t>(1,  7),
                LBDF  = bitMask<std::uint32_t>(1,  8),
                CTSIF = bitMask<std::uint32_t>(1,  9),
                CTS   = bitMask<std::uint32_t>(1, 10),
                RTOF  = bitMask<std::uint32_t>(1, 11),
                EOBF  = bitMask<std::uint32_t>(1, 12),
                UDR   = bitMask<std::uint32_t>(1, 13),
                ABRE  = bitMask<std::uint32_t>(1, 14),
                ABRF  = bitMask<std::uint32_t>(1, 15),
                BUSY  = bitMask<std::uint32_t>(1, 16),
                CMF   = bitMask<std::uint32_t>(1, 17),
                SBKF  = bitMask<std::uint32_t>(1, 18),
                RWU   = bitMask<std::uint32_t>(1, 19),
                WUF   = bitMask<std::uint32_t>(1, 20),
                TEACK = bitMask<std::uint32_t>(1, 21),
                REACK = bitMask<std::uint32_t>(1, 22),
                TXFE  = bitMask<std::uint32_t>(1, 23),
                RXFF  = bitMask<std::uint32_t>(1, 24),
                TCBGT = bitMask<std::uint32_t>(1, 25),
                RXFT  = bitMask<std::uint32_t>(1, 26),
                TXFT  = bitMask<std::uint32_t>(1, 27)
            };
        };
        struct ICR
        {
            enum BitMask : std::uint32_t
            {
                PECF    = bitMask<std::uint32_t>(1,  0),
                FECF    = bitMask<std::uint32_t>(1,  1),
                NECF    = bitMask<std::uint32_t>(1,  2),
                OREFC   = bitMask<std::uint32_t>(1,  3),
                IDLECF  = bitMask<std::uint32_t>(1,  4),
                TXFECF  = bitMask<std::uint32_t>(1,  5),
                TCCF    = bitMask<std::uint32_t>(1,  6),
                TCBGTCF = bitMask<std::uint32_t>(1,  7),
                LBDCF   = bitMask<std::uint32_t>(1,  8),
                CTSCF   = bitMask<std::uint32_t>(1,  9),
                RTOCF   = bitMask<std::uint32_t>(1, 11),
                EOBCF   = bitMask<std::uint32_t>(1, 12),
                UDRFC   = bitMask<std::uint32_t>(1, 13),
                CMCF    = bitMask<std::uint32_t>(1, 17),
                WUCF    = bitMask<std::uint32_t>(1, 20)
            };
        };

        struct PRESC
        {
            enum BitMask : std::uint32_t
            {
                PRESCALER = bitMask<std::uint32_t>(4, 0)
            };
        };
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    UartSTM32H7xx(const Id id,
                  const TransferMode transferMode,
                  GpioPinSTM32H7xx& gpioPin);

    UartSTM32H7xx(const Id id,
                  GpioPinSTM32H7xx& transmitGpioPin,
                  GpioPinSTM32H7xx& receiveGpioPin);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    GpioPinSTM32H7xx& getTransmitGpioPin();

    GpioPinSTM32H7xx* getReceiveGpioPin();

    InterruptSTM32H7xx& getInterrupt();

    inline void interruptHandler();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32H7xx::Peripheral myPeripheralMap[];

    static const InterruptSTM32H7xx::Id myInterruptMap[];

    static const GpioPinSTM32H7xx::AlternateFunction myAlternateFunctionMap[];

    static const std::uint16_t myWordLengthMap[];

    static const std::uint16_t myStopBitsMap[];

    static const std::uint16_t myParityMap[];

    static const std::uint16_t myFlowControlMap[];

    static const std::uint32_t myInterruptEventEnableMap[];

    static const std::uint32_t myInterruptEventPendingMap[];

    // Variables

    static USART_TypeDef* myUartMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    const TransferMode myTransferMode;

    USART_TypeDef* myUart;

    GpioPinSTM32H7xx& myTransmitGpioPin;

    GpioPinSTM32H7xx* myReceiveGpioPin;

    InterruptSTM32H7xx myInterrupt;

    volatile State myState;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Uart
    //--------------------------------------------------------------------------

    virtual Uart::Error driverSetConfig(const Config& config) override;

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

    inline void setUsartEnabled(const bool enabled);

    inline void setBaudRate(const std::uint32_t baudRate);

    inline void setWordBits(const WordBits wordBits);

    inline void setStopBits(const StopBits stopBits);

    inline void setParity(const Parity parity);

    inline void setHardwareFlowControl(
                                 const HardwareFlowControl hardwareFlowControl);

    inline void setTransferMode(const TransferMode transferMode);

    inline void writeByte(const std::uint8_t byte);

    inline std::uint8_t readByte();

    inline void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                         const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void setTransmitDmaEnabled(const bool enabled);

    inline bool isTransmitDmaEnabled();

    inline void setReceiveDmaEnabled(const bool enabled);

    inline bool isReceiveDmaEnabled();
};

}; // namespace Plat4m

#endif // PLAT4M_UART_STM32H7XX_H
