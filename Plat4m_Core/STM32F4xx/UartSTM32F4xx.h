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
/// @file UartSTM32F4xx.h
/// @author Ben Minerd
/// @date 5/14/2013
/// @brief UartSTM32F4xx class header file.
///

#ifndef PLAT4M_UART_STM32F4XX_H
#define PLAT4M_UART_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <stm32f4xx.h>

#include <Plat4m_Core/Uart.h>
#include <Plat4m_Core/STM32F4xx/GpioPinSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/InterruptSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/ProcessorSTM32F4xx.h>

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
                  GpioPinSTM32F4xx& transmitGpioPin,
                  GpioPinSTM32F4xx& receiveGpioPin);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~UartSTM32F4xx();

    State getState();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F4xx::Peripheral myPeripheralMap[];

    static const InterruptSTM32F4xx::Id myInterruptIdMap[];

    // Variables
    
    static USART_TypeDef* myUartMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    USART_TypeDef* myUart;
    
    GpioPinSTM32F4xx& myTransmitGpioPin;
    
    GpioPinSTM32F4xx& myReceiveGpioPin;
    
    InterruptSTM32F4xx myInterrupt;

    volatile State myState;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from Uart
    //--------------------------------------------------------------------------
    
    Error driverSetConfig(const Config& config);
    
    ComInterface::Error driverTransmitBytes(const ByteArray& byteArray,
                                            const bool waitUntilDone);

    uint32_t driverGetReceivedBytesCount();

    ComInterface::Error driverGetReceivedBytes(ByteArray& byteArray,
                                               const uint32_t nBytes);

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

#endif // PLAT4M_UART_STM32F4XX_H
