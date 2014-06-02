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
 * @file SpiStm32f4xx.h
 * @author Ben Minerd
 * @date 4/4/13
 * @brief SpiStm32f4xx class.
 */

#ifndef _SPI_STM32F4XX_H_
#define _SPI_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Spi.h>
#include <GpioPinStm32f4xx.h>

#include <stm32f4xx.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define SPI_STM32F4XX_ID_COUNT 3

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Structures
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class SpiStm32f4xx : public Spi
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
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
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static SpiStm32f4xx& get(const Id id,
                             const TransmissionMode transmissionMode,
                             GpioPinStm32f4xx& sckGpioPin,
                             GpioPinStm32f4xx& misoGpioPin,
                             GpioPinStm32f4xx& mosiGpioPin);
    
    static SpiStm32f4xx& get(const Id id,
                             const TransmissionMode transmissionMode,
                             GpioPinStm32f4xx& sckGpioPin,
                             GpioPinStm32f4xx& dataGpioPin);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    
    const Id myId;
    
    SPI_TypeDef* mySpi;
    
    GpioPinStm32f4xx& mySckGpioPin;
    
    GpioPinStm32f4xx* myMisoGpioPin;
    
    GpioPinStm32f4xx* myMosiGpioPin;
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    SpiStm32f4xx(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinStm32f4xx& sckGpioPin,
                 GpioPinStm32f4xx& misoGpioPin,
                 GpioPinStm32f4xx& mosiGpioPin);
     
    SpiStm32f4xx(const Id id,
                 const TransmissionMode transmissionMode,
                 GpioPinStm32f4xx& sckGpioPin,
                 GpioPinStm32f4xx& dataGpioPin);
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Error driverEnable(const bool enable);
    
    Error driverConfigure(const Config& config);
    
    Error driverTx(const uint8_t byte);
    
    Error driverRx(uint8_t& byte, const bool isLastByte);
    
    Error driverInterruptEnable(const Interrupt interrupt, const bool enable);
};

#endif // _SPI_STM32F4XX_H_
