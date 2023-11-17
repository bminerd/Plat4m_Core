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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file SpiSTM32F4xx.h
/// @author Ben Minerd
/// @date 4/4/2013
/// @brief SpiSTM32F4xx class header file.
///

#ifndef PLAT4M_SPI_STM32F4XX_H
#define PLAT4M_SPI_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <stm32f4xx.h>
#include <stm32f4xx_spi.h>

#include <Plat4m_Core/Spi.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/STM32F4xx/GpioPinSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/InterruptSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class SpiSTM32F4xx : public Spi
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
        ID_4
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

    enum Event
    {
        EVENT_TRANSMIT_BUFFER_EMPTY    = SPI_I2S_FLAG_TXE,
        EVENT_RECEIVE_BUFFER_NOT_EMPTY = SPI_I2S_FLAG_RXNE,
        EVENT_MASTER_MODE_FAULT        = SPI_FLAG_MODF,
        EVENT_OVERRUN_ERROR            = SPI_I2S_FLAG_OVR,
        EVENT_CRC_ERROR                = SPI_FLAG_CRCERR
    };

    enum State
    {
        STATE_IDLE,
        STATE_BUSY,
        STATE_ERROR
    };

    enum Interrupt
    {
        INTERRUPT_TRANSMIT_BUFFER_EMPTY = 0,
        INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY,
        INTERRUPT_ERROR
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    SpiSTM32F4xx(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinSTM32F4xx& sckGpioPin,
                 GpioPinSTM32F4xx& misoGpioPin,
                 GpioPinSTM32F4xx& mosiGpioPin);

    SpiSTM32F4xx(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinSTM32F4xx& sckGpioPin,
                 GpioPinSTM32F4xx& dataGpioPin);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~SpiSTM32F4xx();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void interruptHandler();

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

    static const ProcessorSTM32F4xx::Peripheral myPeripheralMap[];

    static const InterruptSTM32F4xx::Id myInterruptIdMap[];

    static const IRQn_Type myIrqMap[];

    static const uint16_t myModeMap[];

    static const uint16_t myDataBitsMap[];

    static const uint16_t myClockPolarityMap[];

    static const uint16_t myClockPhaseMap[];

    static const Prescaler myClockPrescalerMap[];

    static const uint16_t myBitOrderMap[];

    static const uint8_t myInterruptMap[];

    // Variables

    static SPI_TypeDef* mySpiMap[];
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    SPI_TypeDef* mySpi;

    GpioPinSTM32F4xx& mySckGpioPin;

    GpioPinSTM32F4xx* myMisoGpioPin;

    GpioPinSTM32F4xx* myMosiGpioPin;

    InterruptSTM32F4xx myInterrupt;

    ClockPrescaler myClockPrescaler;

    volatile State myState;

    Transfer* myTransfer;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Spi
    //--------------------------------------------------------------------------
    
    virtual Error driverSetConfig(const Config& config) override;

    virtual Error driverMasterTransfer(Transfer& transfer) override;

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void writeByte(const uint8_t byte);

    inline uint8_t readByte();

    inline void setInterruptEnabled(const Interrupt interrupt,
                                    const bool enable);
};

}; // namespace Plat4m

#endif // PLAT4M_SPI_STM32F4XX_H
