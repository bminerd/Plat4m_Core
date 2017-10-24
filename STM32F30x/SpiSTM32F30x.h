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
/// @file SpiSTM32F30x.h
/// @author Ben Minerd
/// @date 5/15/2017
/// @brief SpiSTM32F30x class header file.
///

#ifndef PLAT4M_SPI_STM32F30X_H
#define PLAT4M_SPI_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Spi.h>
#include <GpioPinSTM32F30x.h>
#include <Module.h>
#include <ProcessorSTM32F30x.h>
#include <InterruptSTM32F30x.h>
#include <DmaChannelSTM32F30x.h>

#include <stm32f30x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SpiSTM32F30x : public Spi
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };
    
    enum GpioRemap
    {
        GPIO_REMAP_0 = 0,
        GPIO_REMAP_1
    };

    enum ClockPrescaler
    {
        CLOCK_PRESCALER_2 = 0,
        CLOCK_PRESCALER_4,
        CLOCK_PRESCALER_8,
        CLOCK_PRESCALER_16,
        CLOCK_PRESCALER_32,
        CLOCK_PRESCALER_64,
        CLOCK_PRESCALER_128,
        CLOCK_PRESCALER_256
    };

    enum State
    {
        STATE_IDLE,
        STATE_BUSY,
        STATE_ERROR // Split into multiple errors!
    };

    enum Interrupt
	{
    	INTERRUPT_TRANSMIT_BUFFER_EMPTY = 0,
		INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY,
		INTERRUPT_ERROR
	};

    enum InterruptEvent
    {
        INTERRUPT_EVENT_TRANSMIT_BUFFER_EMPTY = 0,
        INTERRUPT_EVENT_RECEIVE_BUFFER_NOT_EMPTY,
        INTERRUPT_EVENT_MASTER_MODE_FAULT,
        INTERRUPT_EVENT_OVERRUN_ERROR,
        INTERRUPT_EVENT_CRC_ERROR
    };

    enum DataMode
    {
        DATA_MODE_2_LINE_UNIDIRECTIONAL = 0,
        DATA_MODE_1_LINE_BIDIRECTIONAL
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    SpiSTM32F30x(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinSTM32F30x& sckGpioPin,
                 GpioPinSTM32F30x& misoGpioPin,
                 GpioPinSTM32F30x& mosiGpioPin,
                 const GpioRemap gpioRemap);

    SpiSTM32F30x(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinSTM32F30x& sckGpioPin,
                 GpioPinSTM32F30x& dataGpioPin,
                 const GpioRemap gpioRemap);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SpiSTM32F30x();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setTransmitDmaChannel(DmaChannelSTM32F30x& transmitDmaChannel);

    void setReceiveDmaChannel(DmaChannelSTM32F30x& receiveDmaChannel);

private:

    //--------------------------------------------------------------------------
    // Private types
    //--------------------------------------------------------------------------

    struct Prescaler
    {
        uint32_t value;
        uint32_t bits;
    };

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------
    
    // Constants

    static const ProcessorSTM32F30x::Peripheral myPeripheralMap[];

    static const InterruptSTM32F30x::Id myInterruptIdMap[];

    static const uint16_t myModeMap[];

    static const uint16_t myDataBitsMap[];

    static const uint16_t myClockPolarityMap[];

    static const uint16_t myClockPhaseMap[];

    static const Prescaler myClockPrescalerMap[];

    static const uint16_t myBitOrderMap[];

    static const uint8_t myInterruptEnableMap[];

    static const uint8_t myInterruptEventPendingMap[];

    // Variables

    static SPI_TypeDef* mySpiMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    SPI_TypeDef* mySpi;
    
    GpioPinSTM32F30x& mySckGpioPin;
    
    GpioPinSTM32F30x* myMisoGpioPin;
    
    GpioPinSTM32F30x* myMosiGpioPin;
    
    const GpioRemap myGpioRemap;

    ClockPrescaler myClockPrescaler;

    volatile State myState;

	Transfer* myTransfer;

	InterruptSTM32F30x myInterrupt;

	DmaChannelSTM32F30x* myTransmitDmaChannel;

	DmaChannelSTM32F30x* myReceiveDmaChannel;

	volatile uint32_t myReceivedByteCount;

	volatile uint32_t myTransmittedByteCount;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from Spi
    //--------------------------------------------------------------------------
    
    Error driverSetConfig(const Config& config);

    Error driverMasterTransfer(Transfer& transfer);
    
    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void setPeripheralEnabled(const bool enabled);

    inline bool setClockFrequencyHz(const uint32_t clockFrequencyHz);

    inline void setMode(const Mode mode);

    inline void setDataBits(const DataBits dataBits);

    inline void setClockPolarity(const ClockPolarity clockPolarity);

    inline void setClockPhase(const ClockPhase clockPhase);

    inline void setBitOrder(const BitOrder bitOrder);

    inline void setDataMode(const DataMode dataMode);

    inline void setBidirectionalOutputEnabled(const bool enabled);

    inline void setReceiveOnlyEnabled(const bool enabled);

    inline void setSoftwareSlaveManagementEnabled(const bool enabled);

    inline void setCrcEnabled(const bool enabled);

    inline void setCrcPolynomial(const uint16_t crcPolynomial);

    inline void writeByte(const uint8_t byte);

    inline uint8_t readByte();

    inline void setInterruptEnabled(const Interrupt interrupt,
                                    const bool enabled);
    
    inline bool isInterruptEnabled(const Interrupt interrupt);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void setTransmitDmaEnabled(const bool enabled);

    inline bool isTransmitDmaEnabled();

    inline void setReceiveDmaEnabled(const bool enabled);

    inline bool isReceiveDmaEnabled();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------
    
    void interruptHandler();

    void transmitDmaTransferCompleteCallback();

    void receiveDmaTransferCompleteCallback();
};

}; // namespace Plat4m

#endif // PLAT4M_SPI_STM32F30X_H
