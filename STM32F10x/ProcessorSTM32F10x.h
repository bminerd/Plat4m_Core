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
/// @file ProcessorSTM32F10x.h
/// @author Ben Minerd
/// @date 1/4/2016
/// @brief ProcessorSTM32F10x class header file.
///

#ifndef PLAT4M_PROCESSOR_STM32F10X_H
#define PLAT4M_PROCESSOR_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Processor.h>
#include <ErrorTemplate.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ProcessorSTM32F10x : public Processor
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_CLOCK_FREQUENCY_INVALID = 0,
        ERROR_CODE_INPUT_VOLTAGE_INVALID,
        ERROR_CODE_CLOCK_STARTUP_FAILED,
        ERROR_CODE_NONE
    };
    
    enum ClockSource
    {
        CLOCK_SOURCE_INTERNAL = 0,
        CLOCK_SOURCE_EXTERNAL
    };
    
    enum ExternalClockFrequency
	{
    	EXTERNAL_CLOCK_FREQUENCY_8_MHZ  = 8000000,
		EXTERNAL_CLOCK_FREQUENCY_16_MHZ = 16000000,
		EXTERNAL_CLOCK_FREQUENCY_25_MHZ = 25000000
	};

    enum PllClockPrescaler
    {
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_2 = 0,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_3,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_4,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_5,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_6,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_7,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_8,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_9,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_10,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_11,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_12,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_13,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_14,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_15,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_6_5 =
                                             PLL_CLOCK_PRESCALER_MULTIPLY_BY_15,
        PLL_CLOCK_PRESCALER_MULTIPLY_BY_16
    };

    enum AhbClockPrescaler
    {
        AHB_CLOCK_PRESCALER_DIVIDE_BY_1 = 0,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_2,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_4,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_8,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_16,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_64,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_128,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_256,
        AHB_CLOCK_PRESCALER_DIVIDE_BY_512
    };
    
    enum Apb1ClockPrescaler
    {
        APB1_CLOCK_PRESCALER_DIVIDE_BY_1 = 0,
        APB1_CLOCK_PRESCALER_DIVIDE_BY_2,
        APB1_CLOCK_PRESCALER_DIVIDE_BY_4,
        APB1_CLOCK_PRESCALER_DIVIDE_BY_8,
        APB1_CLOCK_PRESCALER_DIVIDE_BY_16
    };
    
    enum Apb2ClockPrescaler
    {
        APB2_CLOCK_PRESCALER_DIVIDE_BY_1 = 0,
        APB2_CLOCK_PRESCALER_DIVIDE_BY_2,
        APB2_CLOCK_PRESCALER_DIVIDE_BY_4,
        APB2_CLOCK_PRESCALER_DIVIDE_BY_8,
        APB2_CLOCK_PRESCALER_DIVIDE_BY_16
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
        PERIPHERAL_CRC,
        PERIPHERAL_FSMC,
        PERIPHERAL_SDIO,
        PERIPHERAL_USB_OTG_FS,
        PERIPHERAL_ETHERNET_MAC,
        PERIPHERAL_ETHERNET_MAC_TX,
        PERIPHERAL_ETHERNET_MAC_RX,
        // APB2 peripherals
        PERIPHERAL_AFIO,
        PERIPHERAL_GPIO_A,
        PERIPHERAL_GPIO_B,
        PERIPHERAL_GPIO_C,
        PERIPHERAL_GPIO_D,
        PERIPHERAL_GPIO_E,
        PERIPHERAL_GPIO_F,
        PERIPHERAL_GPIO_G,
        PERIPHERAL_ADC_1,
        PERIPHERAL_ADC_2,
        PERIPHERAL_TIM_1,
        PERIPHERAL_SPI_1,
        PERIPHERAL_TIM_8,
        PERIPHERAL_USART_1,
        PERIPHERAL_ADC_3,
        PERIPHERAL_TIM_15,
        PERIPHERAL_TIM_16,
        PERIPHERAL_TIM_17,
        PERIPHERAL_TIM_9,
        PERIPHERAL_TIM_10,
        PERIPHERAL_TIM_11,
        // APB1 peripherals
        PERIPHERAL_TIM_2,
        PERIPHERAL_TIM_3,
        PERIPHERAL_TIM_4,
        PERIPHERAL_TIM_5,
        PERIPHERAL_TIM_6,
        PERIPHERAL_TIM_7,
        PERIPHERAL_TIM_12,
        PERIPHERAL_TIM_13,
        PERIPHERAL_TIM_14,
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
        PERIPHERAL_CAN_2,
        PERIPHERAL_BKP,
        PERIPHERAL_PWR,
        PERIPHERAL_DAC,
        PERIPHERAL_CEC
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct STM32F10xConfig
    {
        ClockSource clockSource;
        uint32_t coreClockFrequencyHz;
        uint32_t ahbClockFrequencyHz;
        uint32_t apb1ClockFrequencyHz;
        uint32_t apb2ClockFrequencyHz;
        bool isUsbOtgFs;
        bool isSdio;
        bool isEthernet;
        uint32_t vectorTableAddress;
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ProcessorSTM32F10x(const VoltageV coreVoltageV,
                       const uint32_t externalClockFrequencyHz);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ProcessorSTM32F10x();

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------
    
    static uint32_t getCoreClockFrequencyHz();
    
    static void setCoreClockFrequencyHz(const uint32_t coreClockFrequencyHz);
    
    static uint32_t getAhbClockFrequencyHz();
    
    static AhbClockPrescaler getAhbClockPrescaler();
    
    static uint32_t getApb1ClockFrequencyHz();
    
    static Apb1ClockPrescaler getApb1ClockPrescaler();
    
    static uint32_t getApb2ClockFrequencyHz();
    
    static Apb2ClockPrescaler getApb2ClockPrescaler();
    
    static PllClockPrescaler getPllClockPrescaler();
    
    static void setPeripheralClockEnabled(const Peripheral peripheral,
                                          const bool enabled);

    static uint32_t getPeripheralInputClockFrequencyHz(
                                                   const Peripheral peripheral);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    Error setSTM32F10xConfig(const STM32F10xConfig& config);
    
    ClockSource getCoreClockSource();
    
    void setCoreClockSource(const ClockSource coreClockSource);

private:
    
    //--------------------------------------------------------------------------
    // Private types
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

    static const Prescaler myAhbClockPrescalerMap[];

    static const Prescaler myApb1ClockPrescalerMap[];

    static const Prescaler myApb2ClockPrescalerMap[];

    static const uint32_t myFlashWaitStatesMap[];

    static const uint32_t myFlashWaitStatesFrequencyMap[];

    static const Prescaler myPllClockPrescalerMap[];

    static const uint32_t myRccClockEnableMap[];

    // Variables
    
    static STM32F10xConfig mySTM32F10xConfig;
    
    static AhbClockPrescaler myAhbClockPrescaler;
    
    static Apb1ClockPrescaler myApb1ClockPrescaler;
    
    static Apb2ClockPrescaler myApb2ClockPrescaler;
    
    static PllClockPrescaler myPllClockPrescaler;

    //--------------------------------------------------------------------------
    // Private methods implemented from Processor
    //--------------------------------------------------------------------------
    
    Processor::Error driverReset();
    
    Processor::Error driverConfigure(const Config& config);
    
    Processor::Error driverSetPowerMode(const PowerMode powerMode);

    uint32_t driverGetCoreClockFrequencyHz();
};

}; // namespace Plat4m

#endif // PLAT4M_PROCESSOR_STM32F10X_H
