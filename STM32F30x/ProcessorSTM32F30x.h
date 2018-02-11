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
/// @file ProcessorSTM32F30x.h
/// @author Ben Minerd
/// @date 2/3/2016
/// @brief ProcessorSTM32F30x class header file.
///

#ifndef PLAT4M_PROCESSOR_STM32F30X_H
#define PLAT4M_PROCESSOR_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stdint.h>

#include <Plat4m_Core/Processor.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ProcessorSTM32F30x : public Processor
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
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
    
    enum ExternalClockFrequency
    {
        EXTERNAL_CLOCK_FREQUENCY_8_MHZ  = 8000000,
        EXTERNAL_CLOCK_FREQUENCY_16_MHZ = 16000000
    };

    enum PllClockMultiplier
    {
        PLL_CLOCK_MULTIPLIER_2 = 0,
        PLL_CLOCK_MULTIPLIER_3,
        PLL_CLOCK_MULTIPLIER_4,
        PLL_CLOCK_MULTIPLIER_5,
        PLL_CLOCK_MULTIPLIER_6,
        PLL_CLOCK_MULTIPLIER_7,
        PLL_CLOCK_MULTIPLIER_8,
        PLL_CLOCK_MULTIPLIER_9,
        PLL_CLOCK_MULTIPLIER_10,
        PLL_CLOCK_MULTIPLIER_11,
        PLL_CLOCK_MULTIPLIER_12,
        PLL_CLOCK_MULTIPLIER_13,
        PLL_CLOCK_MULTIPLIER_14,
        PLL_CLOCK_MULTIPLIER_15,
        PLL_CLOCK_MULTIPLIER_16
    };

    ///
    /// NOTE: This strange numbering comes from the fact that modifying the PLL
    /// clock divider LSB also changes the HSE/2 bit
    /// (see pg. 162 of datasheet)
    ///
    enum PllClockDivider
    {
        PLL_CLOCK_DIVIDER_1 = 0,
        PLL_CLOCK_DIVIDER_3,
        PLL_CLOCK_DIVIDER_4,
        PLL_CLOCK_DIVIDER_5,
        PLL_CLOCK_DIVIDER_7,
        PLL_CLOCK_DIVIDER_8,
        PLL_CLOCK_DIVIDER_9,
        PLL_CLOCK_DIVIDER_11,
        PLL_CLOCK_DIVIDER_12,
        PLL_CLOCK_DIVIDER_13,
        PLL_CLOCK_DIVIDER_15,
        PLL_CLOCK_DIVIDER_16,
        PLL_CLOCK_DIVIDER_20,
        PLL_CLOCK_DIVIDER_24,
        PLL_CLOCK_DIVIDER_28,
        PLL_CLOCK_DIVIDER_32
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
    
    enum FlashWaitStates
    {
        FLASH_WAIT_STATES_0 = 0,
        FLASH_WAIT_STATES_1,
        FLASH_WAIT_STATES_2
    };
    
    enum Peripheral
    {
        // AHB peripherals
        PERIPHERAL_DMA_1 = 0,
        PERIPHERAL_DMA_2,
        PERIPHERAL_SRAM,
        PERIPHERAL_FLITF,
        PERIPHERAL_FMC,
        PERIPHERAL_CRC,
        PERIPHERAL_GPIO_H,
        PERIPHERAL_GPIO_A,
        PERIPHERAL_GPIO_B,
        PERIPHERAL_GPIO_C,
        PERIPHERAL_GPIO_D,
        PERIPHERAL_GPIO_E,
        PERIPHERAL_GPIO_F,
        PERIPHERAL_GPIO_G,
        PERIPHERAL_TS,
        PERIPHERAL_ADC_1_2,
        PERIPHERAL_ADC_3_4,
        // APB2 peripherals
        PERIPHERAL_SYSCFG,
        PERIPHERAL_TIM_1,
        PERIPHERAL_SPI_1,
        PERIPHERAL_TIM_8,
        PERIPHERAL_USART_1,
        PERIPHERAL_SPI_4,
        PERIPHERAL_TIM_15,
        PERIPHERAL_TIM_16,
        PERIPHERAL_TIM_17,
        PERIPHERAL_TIM_20,
        PERIPHERAL_HRTIM_1,
        // APB1 peripherals
        PERIPHERAL_TIM_2,
        PERIPHERAL_TIM_3,
        PERIPHERAL_TIM_4,
        PERIPHERAL_TIM_6,
        PERIPHERAL_TIM_7,
        PERIPHERAL_WWDG,
        PERIPHERAL_SPI_2,
        PERIPHERAL_SPI_3,
        PERIPHERAL_USART_2,
        PERIPHERAL_USART_3,
        PERIPHERAL_UART_4,
        PERIPHERAL_UART_5,
        PERIPHERAL_I2C_1,
        PERIPHERAL_I2C_2,
        PERIPHERAL_USB,
        PERIPHERAL_CAN_1,
        PERIPHERAL_DAC_2,
        PERIPHERAL_PWR,
        PERIPHERAL_DAC_1,
        PERIPHERAL_I2C_3
    };

    //--------------------------------------------------------------------------
    // Public structures
    //--------------------------------------------------------------------------

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
        uint32_t vectorTableAddress;
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ProcessorSTM32F30x(const float coreVoltage,
                       const uint32_t externalClockFrequencyHz);

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------
    
    static Config getDefaultConfig();

    static uint32_t getCoreClockFrequencyHz();
    
    static void setCoreClockFrequencyHz(const uint32_t coreClockFrequencyHz);
    
    static uint32_t getAhbClockFrequencyHz();
    
    static AhbClockPrescaler getAhbClockPrescaler();
    
    static uint32_t getApb1ClockFrequencyHz();
    
    static Apb1ClockPrescaler getApb1ClockPrescaler();
    
    static uint32_t getApb2ClockFrequencyHz();
    
    static Apb2ClockPrescaler getApb2ClockPrescaler();
    
    static void setPeripheralClockEnabled(const Peripheral peripheral,
                                          const bool enabled);

    static uint32_t getPeripheralInputClockFrequencyHz(
                                                   const Peripheral peripheral);

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------
    
    virtual Error setConfig(const Config& config);
    
    virtual ClockSource getCoreClockSource();
    
    virtual void setCoreClockSource(const ClockSource coreClockSource);
    
private:
    
    //--------------------------------------------------------------------------
    // Private structures
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

    static const Prescaler myPllClockMultiplierMap[];

    static const Prescaler myPllClockDividerMap[];

    static const Prescaler myAhbClockPrescalerMap[];

    static const Prescaler myApb1ClockPrescalerMap[];

    static const Prescaler myApb2ClockPrescalerMap[];

    static const uint32_t myFlashWaitStatesMap[];

    static const uint32_t myFlashWaitStatesFrequencyMap[];

    static const uint32_t myRccClockEnableMap[];

    // Variables
    
    static Config myConfig;
    
    static PllClockMultiplier myPllClockMultiplier;

    static PllClockDivider myPllClockDivider;

    static AhbClockPrescaler myAhbClockPrescaler;
    
    static Apb1ClockPrescaler myApb1ClockPrescaler;
    
    static Apb2ClockPrescaler myApb2ClockPrescaler;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Processor
    //--------------------------------------------------------------------------
    
    virtual Processor::Error driverReset();
    
    virtual Processor::Error driverConfigure(const Processor::Config& config);
    
    virtual Processor::Error driverSetPowerMode(
                                          const Processor::PowerMode powerMode);

    virtual uint32_t driverGetCoreClockFrequencyHz();

    virtual Processor::Error driverJumpToAddress(const intptr_t address);
};

}; // namespace Plat4m

#endif // PLAT4M_PROCESSOR_STM32F30X_H
