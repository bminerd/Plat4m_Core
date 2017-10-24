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
/// @file SpiSTM32F4xx.h
/// @author Ben Minerd
/// @date 4/4/13
/// @brief SpiSTM32F4xx class header file.
///

#ifndef SPI_STM32F4XX_H
#define SPI_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Spi.h>
#include <GpioPinSTM32F4xx.h>
#include <Module.h>
#include <InterruptSTM32F4xx.h>

#include <stm32f4xx.h>

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
    
    /**
     * @brief Enumeration of SPIs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };
    
    /**
     * @brief Enumeration of SPI clock prescalers.
     */
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

    /**
     * @brief Enumeration of SPI states.
     */
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
    
    //--------------------------------------------------------------------------
    // Public methods
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
    // Public methods
    //--------------------------------------------------------------------------

    void interruptHandler();

private:

    //--------------------------------------------------------------------------
    // Private types
    //--------------------------------------------------------------------------

    struct Prescaler
    {
        unsigned int value;
        uint32_t bits;
    };

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const uint32_t myClockMap[];

    static const IRQn_Type myIrqMap[];

    static const GpioSTM32F4xx::AlternateFunction myAlternateFunctionMap[];

    static const uint16_t myModeMap[];

    static const uint16_t myDataBitsMap[];

    static const uint16_t myClockPolarityMap[];

    static const uint16_t myClockPhaseMap[];

    static const Prescaler myClockPrescalerMap[];

    static const uint16_t myBitOrderMap[];

    static const uint8_t myInterruptMap[];

    static const GpioSTM32F4xx::OutputSpeed myDefaultOutputSpeed;

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
    
    ClockPrescaler myClockPrescaler;

    volatile State myState;

    Transfer* myTransfer;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

	Module::Error driverEnable(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from Spi
    //--------------------------------------------------------------------------
    
    Error driverSetConfig(const Config& config);

    Error driverMasterTransfer(Transfer& transfer);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------
    
    inline void writeByte(const uint8_t byte);
    
    inline uint8_t readByte();
    
    inline void setInterruptEnabled(const Interrupt interrupt,
                                    const bool enable);
};

}; // namespace Plat4m

#endif // SPI_STM32F4XX_H
