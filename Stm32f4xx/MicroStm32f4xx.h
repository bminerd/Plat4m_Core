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
 * @file MicroStm32f4xx.h
 * @author Ben Minerd
 * @date 5/31/2013
 * @brief MicroStm32f4xx class.
 */

#ifndef _MICRO_STM32F4XX_H_
#define _MICRO_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Micro.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define MICRO_STM32F4XX_CORE_CLOCK_FREQUENCY_MAX ((FrequencyHz) 168000000)

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class MicroStm32f4xx : public Micro
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
    enum Error
    {
        ERROR_CLOCK_FREQUENCY_INVALID,
        ERROR_INPUT_VOLTAGE_INVALID,
        ERROR_CLOCK_STARTUP_FAILED,
        ERROR_NONE
    };
    
    enum ClockSource
    {
        CLOCK_SOURCE_INTERNAL,
        CLOCK_SOURCE_EXTERNAL
    };
    
    enum AhbClockPrescaler
    {
        AHB_CLOCK_PRESCALER_1 = 0,
        AHB_CLOCK_PRESCALER_2,
        AHB_CLOCK_PRESCALER_4,
        AHB_CLOCK_PRESCALER_8,
        AHB_CLOCK_PRESCALER_16,
        AHB_CLOCK_PRESCALER_64,
        AHB_CLOCK_PRESCALER_128,
        AHB_CLOCK_PRESCALER_256,
        AHB_CLOCK_PRESCALER_512
    };
    
    enum Apb1ClockPrescaler
    {
        APB1_CLOCK_PRESCALER_1 = 0,
        APB1_CLOCK_PRESCALER_2,
        APB1_CLOCK_PRESCALER_4,
        APB1_CLOCK_PRESCALER_8,
        APB1_CLOCK_PRESCALER_16
    };
    
    enum Apb2ClockPrescaler
    {
        APB2_CLOCK_PRESCALER_1 = 0,
        APB2_CLOCK_PRESCALER_2,
        APB2_CLOCK_PRESCALER_4,
        APB2_CLOCK_PRESCALER_8,
        APB2_CLOCK_PRESCALER_16
    };
    
    enum CoreVoltage
    {
        CORE_VOLTAGE_V1R8_TO_V2R1 = 0,
        CORE_VOLTAGE_V2R1_TO_V2R4,
        CORE_VOLTAGE_V2R4_TO_V2R7,
        CORE_VOLTAGE_V2R7_TO_V3R6
    };
    
    // Move this to FlashStm32f4xx?
    enum FlashWaitStates
    {
        FLASH_WAIT_STATES_0 = 0,
        FLASH_WAIT_STATES_1,
        FLASH_WAIT_STATES_2,
        FLASH_WAIT_STATES_3,
        FLASH_WAIT_STATES_4,
        FLASH_WAIT_STATES_5,
        FLASH_WAIT_STATES_6,
        FLASH_WAIT_STATES_7
    };
    
    /*--------------------------------------------------------------------------
     * Public structures
     *------------------------------------------------------------------------*/

    struct Config
    {
        ClockSource clockSource;
        uint32_t coreClockFrequencyHz;
        uint32_t ahbClockFrequencyHz;
        uint32_t apb1ClockFrequencyHz;
        uint32_t apb2ClockFrequencyHz;
        bool isUsbOtgFs;
        bool isSdio;
        bool isRng;
        bool isEthernet;
    };
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static MicroStm32f4xx& get(const float coreVoltage,
                               const uint32_t externalClockFrequencyHz);
    
    static uint32_t getCoreClockFrequencyHz();
    
    static void setCoreClockFrequencyHz(const uint32_t coreClockFrequencyHz);
    
    static uint32_t getAhbClockFrequencyHz();
    
    static AhbClockPrescaler getAhbClockPrescaler();
    
    static uint32_t getApb1ClockFrequencyHz();
    
    static Apb1ClockPrescaler getApb1ClockPrescaler();
    
    static uint32_t getApb2ClockFrequencyHz();
    
    static Apb2ClockPrescaler getApb2ClockPrescaler();
    
    // Add methods to check peripherals based on package!
    
    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    
    Error setDriverConfig(const Config& config);
    
    ClockSource getCoreClockSource();
    
    void setCoreClockSource(const ClockSource coreClockSource);
    
private:
    
    /*--------------------------------------------------------------------------
     * Private static data members
     *------------------------------------------------------------------------*/
    
    static Config myConfig;
    
    static AhbClockPrescaler myAhbClockPrescaler;
    
    static Apb1ClockPrescaler myApb1ClockPrescaler;
    
    static Apb2ClockPrescaler myApb2ClockPrescaler;
    
    /*--------------------------------------------------------------------------
     * Private constructors and destructors
     *------------------------------------------------------------------------*/
    
    MicroStm32f4xx(const float coreVoltage,
                   const uint32_t externalClockFrequencyHz);
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/
    
    Micro::Error driverReset();
    
    Micro::Error driverConfigure(const Micro::Config& config);
    
    Micro::Error driverSetPowerMode(const Micro::PowerMode powerMode);
};

#endif // _MICRO_STM32F4XX_H_
