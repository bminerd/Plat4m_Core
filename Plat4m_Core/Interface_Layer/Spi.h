/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file Spi.h
 * @author Ben Minerd
 * @date 4/4/13
 * @brief Spi class.
 */

#ifndef _SPI_H_
#define _SPI_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ByteArray.h>
#include <Buffer.h>
#include <GpioPin.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Spi
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of SPI errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_TX_BUFFER_FULL,
        ERROR_TIMEOUT
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

    /**
     * @brief Enumeration of SPI modes.
     */
    enum Mode
    {
        MODE_MASTER = 0,
        MODE_SLAVE
    };

    /**
     * @brief Enumeration of SPI data bits.
     */
    enum DataBits
    {
        DATA_BITS_8 = 0,
        DATA_BITS_16
    };

    /**
     * @brief Enumeration of SPI clock polarities.
     */
    enum ClockPolarity
    {
        CLOCK_POLARITY_LOW = 0,
        CLOCK_POLARITY_HIGH
    };

    /**
     * @brief Enumeration of SPI clock phases.
     */
    enum ClockPhase
    {
        CLOCK_PHASE_LEADING_EDGE = 0,
        CLOCK_PHASE_TRAILING_EDGE
    };

    /**
     * @brief Enumeration of SPI data orders.
     */
    enum BitOrder
    {
        BIT_ORDER_LSB_FIRST = 0,
        BIT_ORDER_MSB_FIRST
    };

    enum TransmissionMode
    {
        TRANSMISSION_MODE_RX_TX,
        TRANSMISSION_MODE_RX,
        TRANSMISSION_MODE_TX
    };
    
    /**
     * @brief Enumeration of SPI transfer modes.
     */
    enum TransferMode
    {
        TRANSFER_MODE_TX,
        TRANSFER_MODE_TX_WAIT,
        TRANSFER_MODE_RX,
        TRANSFER_MODE_RX_MAILBOX,
        TRANSFER_MODE_TX_RX_SEQUENTIAL,
        TRANSFER_MODE_TX_RX_SEQUENTIAL_MAILBOX,
        TRANSFER_MODE_TX_RX_CONCURRENT,
        TRANSFER_MODE_TX_RX_CONCURRENT_MAILBOX
    };

    /**
     * @brief Enumeration of SPI interrupts.
     */
    enum Interrupt
    {
        INTERRUPT_TX = 0,
        INTERRUPT_RX
        // TODO Add error interrupt
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    /**
     * @brief SPI configuration structure.
     */
    struct Config
    {
        Mode mode;
        DataBits dataBits;
        ClockPolarity clockPolarity;
        ClockPhase clockPhase;
        id_t clockPrescaler;
        BitOrder bitOrder;
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~Spi();
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Sets this SPI enabled or disabled.
     * @param enable Flag that indicates if the SPI should be enabled or
     * disabled.
     * @return SPI error.
     */
    Error enable(const bool enable);

    /**
     * @brief Checks to see if this SPI is enabled or disabled.
     * @param isEnabled Flag that indicates if this SPI is enabled or disabled.
     * @return SPI error.
     */
    Error isEnabled(bool& isEnabled);

    /**
     * @brief Configures this SPI.
     * @param config SPI configuration.
     * @return SPI error.
     */
    Error configure(const Config& config);
    
    Error masterTransfer(const TransferMode transferMode,
                         GpioPin* chipSelectGpioPin,
                         const ByteArray& txByteArray,
                         ByteArray& rxByteArray,
                         const uint32_t timeoutMs = 100);
    
    Error reset();
    
    void interruptHandler(const Interrupt interrupt);

protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/

    Spi(const TransmissionMode transmissionMode);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private structs
     *------------------------------------------------------------------------*/
    
    /**
     * @brief SPI transfer structure.
     */
    struct Transfer
    {
        TransferMode transferMode;
        GpioPin* chipSelectGpioPin;
        unsigned int txCount;
        unsigned int rxCount;
        Buffer<uint8_t, 512> txBuffer;
        Buffer<uint8_t, 256> rxBuffer;
        Error error;
    };
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    const TransmissionMode myTransmissionMode;
    
    Config myConfig;
    
    volatile State myState;
    
    Buffer<Transfer, 1> myTransferBuffer;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverTx(const uint8_t byte) = 0;
    
    virtual Error driverRx(uint8_t& byte, const bool isLastByte) = 0;
    
    virtual Error driverInterruptEnable(const Interrupt interrupt,
                                        const bool enable) = 0;
};

#endif // _SPI_INTERFACE_H_
