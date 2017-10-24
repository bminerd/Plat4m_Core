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
/// @file SpiSTM32F10x.h
/// @author Ben Minerd
/// @date 1/6/2016
/// @brief SpiSTM32F10x class header file.
///

#ifndef PLAT4M_SPI_STM32F10X_H
#define PLAT4M_SPI_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Spi.h>
#include <GpioPinSTM32F10x.h>
#include <Module.h>
#include <ProcessorSTM32F10x.h>
#include <InterruptSTM32F10x.h>
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

class SpiSTM32F10x : public Spi
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
    
    SpiSTM32F10x(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinSTM32F10x& sckGpioPin,
                 GpioPinSTM32F10x& misoGpioPin,
                 GpioPinSTM32F10x& mosiGpioPin,
                 const GpioRemap gpioRemap);

    SpiSTM32F10x(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinSTM32F10x& sckGpioPin,
                 GpioPinSTM32F10x& dataGpioPin,
                 const GpioRemap gpioRemap);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SpiSTM32F10x();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setTransmitDmaChannel(DmaChannelSTM32F10x& transmitDmaChannel);

    void setReceiveDmaChannel(DmaChannelSTM32F10x& receiveDmaChannel);

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

    static const ProcessorSTM32F10x::Peripheral myPeripheralMap[];

    static const InterruptSTM32F10x::Id myInterruptIdMap[];

    static const uint16_t myModeMap[];

    static const uint16_t myDataBitsMap[];

    static const uint16_t myClockPolarityMap[];

    static const uint16_t myClockPhaseMap[];

    static const Prescaler myClockPrescalerMap[];

    static const uint16_t myBitOrderMap[];

    static const uint8_t myInterruptEnableMap[];

    static const uint8_t myInterruptEventPendingMap[];

    static const GpioPinSTM32F10x::OutputSpeed myDefaultOutputSpeed;

    // Variables

    static SPI_TypeDef* mySpiMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    SPI_TypeDef* mySpi;
    
    GpioPinSTM32F10x& mySckGpioPin;
    
    GpioPinSTM32F10x* myMisoGpioPin;
    
    GpioPinSTM32F10x* myMosiGpioPin;
    
    const GpioRemap myGpioRemap;

    ClockPrescaler myClockPrescaler;

    volatile State myState;

	Transfer* myTransfer;

	InterruptSTM32F10x myInterrupt;

	DmaChannelSTM32F10x* myTransmitDmaChannel;

	DmaChannelSTM32F10x* myReceiveDmaChannel;

	volatile uint32_t myReceivedByteCount;
    
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

#endif // PLAT4M_SPI_STM32F10X_H
