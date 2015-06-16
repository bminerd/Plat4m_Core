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
 * @file Uart.h
 * @author Ben Minerd
 * @date 3/22/13
 * @brief Uart class.
 */

#ifndef _UART_H_
#define _UART_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <ComInterface.h>
#include <Buffer.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/
     
#define UART_TX_BUFFER_SIZE 256
#define UART_RX_BUFFER_SIZE 256

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class Uart : public ComInterface
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Enumeration of UART errors.
     */
    enum Error
    {
        ERROR_NONE,                 /// No error.
        ERROR_PARAMETER_INVALID,    /// Parameter is invalid.
        ERROR_NOT_ENABLED,          /// UART is not enabled.
        ERROR_TX_BUFFER_FULL        /// UART transmit buffer is full.
    };
    
    /**
     * @brief Enumeration of UART word lengths.
     */
    enum WordBits
    {
        WORD_BITS_8 = 0,
        WORD_BITS_9
    };

    /**
     * @brief Enumeration of UART stop bits.
     */
    enum StopBits
    {
        STOP_BITS_1 = 0,
        STOP_BITS_2
    };

    /**
     * @brief Enumeration of UART parities.
     */
    enum Parity
    {
        PARITY_NONE = 0,
        PARITY_EVEN,
        PARITY_ODD
    };

    /**
     * @brief Enumeration of UART hardware flow controls.
     */
    enum HardwareFlowControl
    {
        HARDWARE_FLOW_CONTROL_NONE = 0
    };

    /**
     * @brief Enumeration of UART interrupts.
     */
    enum Interrupt
    {
        INTERRUPT_TX = 0,   /// Transmit interrupt.
        INTERRUPT_RX        /// Receive interrupt.
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/
    
    /**
     * @brief UART configuration type.
     */
    struct Config
    {
        uint32_t baudRate;
        WordBits wordBits;
        StopBits stopBits;
        Parity parity;
        HardwareFlowControl hardwareFlowControl;
    };
    
    /*--------------------------------------------------------------------------
     * Public virtual destructors
     *------------------------------------------------------------------------*/
    
    virtual ~Uart();
    
    /*--------------------------------------------------------------------------
     * Public implemented methods
     *------------------------------------------------------------------------*/
    
    ComInterface::Error tx(const ByteArray& byteArray);
    
    unsigned int rxBytesAvailable();
    
    ComInterface::Error getRxBytes(ByteArray& byteArray,
                                   unsigned int nBytes = 0);
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error enable(const bool enable);
    
    Error isEnabled(bool& isEnabled);
    
    Error configure(const Config& config);
    
    void interruptHandler(const Interrupt interrupt);
    
protected:
    
    /*--------------------------------------------------------------------------
     * Protected constructors
     *------------------------------------------------------------------------*/
    
    /**
     * @brief Constructor for Uart.
     */
    Uart();
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    bool myIsEnabled;
    
    Config myConfig;
    
    Buffer<uint8_t, UART_TX_BUFFER_SIZE> myTxBuffer;
    
    Buffer<uint8_t, UART_RX_BUFFER_SIZE> myRxBuffer;
    
    /*--------------------------------------------------------------------------
     * Private pure virtual methods
     *------------------------------------------------------------------------*/
    
    virtual Error driverEnable(const bool enable) = 0;
    
    virtual Error driverConfigure(const Config& config) = 0;
    
    virtual Error driverTx(const uint8_t byte) = 0;
    
    virtual Error driverRx(uint8_t& byte) = 0;
    
    virtual Error driverInterruptEnable(const Interrupt interrupt,
                                        const bool enable) = 0;
};

#endif // _UART_H_