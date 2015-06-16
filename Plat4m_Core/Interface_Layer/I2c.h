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
 * @file I2c.h
 * @author Ben Minerd
 * @date 3/25/13
 * @brief I2c class.
 * @todo Add slave implementation.
 */

#ifndef _I2C_H_
#define _I2C_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ByteArray.h>
#include <Buffer.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class I2c
{
public:

    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of I2C errors.
     */
    enum Error
    {
        ERROR_NONE,
        ERROR_PARAMETER_INVALID,
        ERROR_ADDRESS_INVALID,
        ERROR_NOT_ENABLED,
        ERROR_TX_BUFFER_FULL,
        ERROR_TIMEOUT,
        ERROR_BUS,
        ERROR_BUS_BUSY
    };

    /**
     * @brief Enumeration of I2C master modes.
     */
    enum MasterMode
    {
        MASTER_MODE_TX = 0,
        MASTER_MODE_RX = 1
    };

    enum State
    {
        STATE_IDLE,
        STATE_BUSY,
        STATE_ERROR
    };

    /**
     * @brief Enumeration of I2C address bits.
     */
    enum AddressBits
    {
        ADDRESS_BITS_7 = 0,
        ADDRESS_BITS_10
    };

    /**
     * @brief Enumeration of I2C interrupts.
     */
    enum Interrupt
    {
        // I2C master interrupts
        INTERRUPT_MASTER_START_TRANSMITTED = 0,
        INTERRUPT_MASTER_TRANSMITTER_SELECTED,
        INTERRUPT_MASTER_RECEIVER_SELECTED,
        INTERRUPT_BYTE_TRANSMITTING,
        INTERRUPT_BYTE_TRANSMITTED,
        INTERRUPT_BYTE_RECEIVED,

        // I2C slave interrupts
        INTERRUPT_SLAVE_STOP_RECEIVED,

        // I2C error interrupts
        INTERRUPT_NACK_RECEIVED
    };

    enum TransferMode
    {
        TRANSFER_MODE_TX,
        TRANSFER_MODE_TX_WAIT,
        TRANSFER_MODE_RX,
        TRANSFER_MODE_RX_MAILBOX,
        TRANSFER_MODE_TX_RX,
        TRANSFER_MODE_TX_RX_MAILBOX
    };

    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/

    /**
     * @brief I2C configuration type.
     */
    struct Config
    {
        uint32_t clockSpeedHz;
        AddressBits addressBits;
    };

    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/

    virtual ~I2c();

    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    /**
     * @brief Sets this I2C enabled or disabled.
     * @param enable Flag that indicates if the I2C should be enabled or disabled.
     * @return I2C error.
     */
    Error enable(const bool enable);

    /**
     * @brief Checks to see if this I2C is enabled or disabled.
     * @param isEnabled Flag that indicates if the I2C is enabled or disabled.
     * @return I2C error.
     */
    Error isEnabled(bool& isEnabled);

    /**
     * @brief Configures this I2C.
     * @param config I2C configuration.
     * @return I2C error.
     */
    Error configure(const Config& config);

    Error masterTransfer(const TransferMode transferMode,
                         const uint8_t address,
                         const ByteArray& txByteArray,
                         ByteArray& rxByteArray,
                         const uint32_t timeoutMs = 100);

    Error reset();

    void interruptHandler(const Interrupt interrupt);

protected:

    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/

    I2c(const char* idString);

private:

    /*--------------------------------------------------------------------------
     * Private structs
     *------------------------------------------------------------------------*/

    /**
     * @brief I2C transfer structure.
     */
    struct Transfer
    {
        TransferMode transferMode;
        uint8_t address;
        unsigned int txCount;
        unsigned int rxCount;
        Buffer<uint8_t, 512> txBuffer;
        Buffer<uint8_t, 256> rxBuffer;
        Error error;
    };

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    const char* myIdString;

    bool myIsEnabled;

    Config myConfig;

    volatile State myState;

    Buffer<Transfer, 1> myTransferBuffer;

    Transfer myLastTransfer;

    Interrupt myLastInterrupt;

    /*--------------------------------------------------------------------------
     * Private virtual methods
     *------------------------------------------------------------------------*/

    virtual Error driverEnable(const bool enable) = 0;

    virtual Error driverConfigure(const Config& config) = 0;

    virtual Error driverMasterStart() = 0;

    virtual Error driverMasterStop() = 0;

    virtual Error driverMasterTxAddress(const uint8_t address,
                                        const MasterMode masterMode) = 0;

    virtual Error driverTx(const uint8_t byte) = 0;

    virtual Error driverRx(bool isLastByte) = 0;

    virtual Error driverRead(uint8_t& byte) = 0;

    virtual Error driverInterruptEnable(const Interrupt interrupt,
                                        const bool enable) = 0;

    virtual Error driverReset() = 0;
};

#endif // _I2C_H_