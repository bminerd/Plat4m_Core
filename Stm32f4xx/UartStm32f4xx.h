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
 * @file UartStm32f4xx.h
 * @author Ben Minerd
 * @date 5/14/13
 * @brief UartStm32f4xx class.
 */

#ifndef _UART_STM32F4XX_H_
#define _UART_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Uart.h>
#include <GpioPinStm32f4xx.h>

#include <stm32f4xx_usart.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define UART_STM32F4XX_ID_COUNT 6

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class UartStm32f4xx : public Uart
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
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
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static UartStm32f4xx& get(const Id id,
                              GpioPinStm32f4xx& txGpioPin,
                              GpioPinStm32f4xx& rxGpioPin);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    UartStm32f4xx(const Id id,
                  GpioPinStm32f4xx& txGpioPin,
                  GpioPinStm32f4xx& rxGpioPin);

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const Id myId;
    
    USART_TypeDef* myUart;
    
    GpioPinStm32f4xx& myTxGpioPin;
    
    GpioPinStm32f4xx& myRxGpioPin;
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverTx(const uint8_t byte);
    
    Error driverRx(uint8_t& byte);
    
    Error driverInterruptEnable(const Interrupt interrupt, const bool enable);
};

#endif // _UART_STM32F4XX_H_
