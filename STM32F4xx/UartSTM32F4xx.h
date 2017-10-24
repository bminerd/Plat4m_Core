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
/// @file UartSTM32F4xx.h
/// @author Ben Minerd
/// @date 5/14/13
/// @brief UartSTM32F4xx class.
///

#ifndef UART_STM32F4XX_H
#define UART_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Uart.h>
#include <GpioPinSTM32F4xx.h>
#include <InterruptSTM32F4xx.h>

#include <stm32f4xx_usart.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class UartSTM32F4xx : public Uart
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    /**
     * @brief Enumeration of UART driver IDs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3,
        ID_4,
        ID_5,
        ID_6
    };
    
    enum InterruptFlag
    {
        INTERRUPT_FLAG_TRANSMIT_BUFFER_EMPTY = 0,
        INTERRUPT_FLAG_RECEIVE_BUFFER_NOT_EMPTY
    };

    enum State
    {
        STATE_IDLE,
        STATE_BUSY
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    UartSTM32F4xx(const Id id,
                  GpioPinSTM32F4xx& txGpioPin,
                  GpioPinSTM32F4xx& rxGpioPin);
    
private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const InterruptSTM32F4xx::Id myInterruptIdMap[];

    // Variables
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    USART_TypeDef* myUart;
    
    GpioPinSTM32F4xx& myTxGpioPin;
    
    GpioPinSTM32F4xx& myRxGpioPin;
    
    InterruptSTM32F4xx myInterrupt;

    volatile State myState;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverEnable(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from Uart
    //--------------------------------------------------------------------------
    
    Error driverConfigure(const Config& config);
    
    ComInterface::Error driverTx(const ByteArray& byteArray,
                                 const bool waitUntilDone);

    unsigned int driverRxBytesAvailable();

    ComInterface::Error driverGetRxBytes(ByteArray& byteArray,
                                         unsigned int nBytes);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void writeByte(const uint8_t byte);

    inline uint8_t readByte();

    inline void setInterruptFlagEnabled(const InterruptFlag interruptFlag,
                                        const bool enabled);

    inline bool isInterruptFlagEnabled(const InterruptFlag interruptFlag);

    inline void interruptHandler();
};

}; // namespace Plat4m

#endif // UART_STM32F4XX_H
