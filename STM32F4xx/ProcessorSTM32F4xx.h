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
/// @file ProcessorSTM32F4xx.h
/// @author Ben Minerd
/// @date 5/31/2013
/// @brief ProcessorSTM32F4xx class header file.
///

#ifndef PLAT4M_PROCESSOR_STM32F4XX_H
#define PLAT4M_PROCESSOR_STM32F4XX_H

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

class ProcessorSTM32F4xx : public Processor
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
    
    // Move this to FlashSTM32F4xx?
    enum FlashWaitStates
    {
        FLASH_WAIT_STATES_0 = 0,
        FLASH_WAIT_STATES_1,
        FLASH_WAIT_STATES_2,
        FLASH_WAIT_STATES_3,
        FLASH_WAIT_STATES_4,
        FLASH_WAIT_STATES_5,
        FLASH_WAIT_STATES_6,
        FLASH_WAIT_STATES_7,
		FLASH_WAIT_STATES_8
    };
    
    enum Peripheral
    {
        // AHB1 peripherals
        PERIPHERAL_GPIO_A = 0,
        PERIPHERAL_GPIO_B,
        PERIPHERAL_GPIO_C,
        PERIPHERAL_GPIO_D,
        PERIPHERAL_GPIO_E,
        PERIPHERAL_GPIO_F,
        PERIPHERAL_GPIO_G,
        PERIPHERAL_GPIO_H,
        PERIPHERAL_CRC,
        PERIPHERAL_BACKUP_SRAM,
        PERIPHERAL_DMA_1,
        PERIPHERAL_DMA_2,
        PERIPHERAL_USB_OTG_HS,
        PERIPHERAL_USB_OTG_HS_ULPI,
        // AHB2 peripherals
        PERIPHERAL_CAMERA_INTERFACE,
        PERIPHERAL_USB_OTG_FS,
        // AHB3 peripherals
        PERIPHERAL_FMC,
        PERIPHERAL_QUAD_SPI,
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
        PERIPHERAL_WINDOW_WATCHDOG,
        PERIPHERAL_SPI_2,
        PERIPHERAL_SPI_3,
        PERIPHERAL_SPDIF_RX,
        PERIPHERAL_USART_2,
        PERIPHERAL_USART_3,
        PERIPHERAL_UART_4,
        PERIPHERAL_UART_5,
        PERIPHERAL_I2C_1,
        PERIPHERAL_I2C_2,
        PERIPHERAL_FMP_I2C_1,
        PERIPHERAL_CAN_1,
        PERIPHERAL_CAN_2,
        PERIPHERAL_CEC,
        PERIPHERAL_PWR,
        PERIPHERAL_DAC,
        // APB2 peripherals
        PERIPHERAL_TIM_1,
        PERIPHERAL_TIM_8,
        PERIPHERAL_USART_1,
        PERIPHERAL_USART_6,
        PERIPHERAL_ADC_1,
        PERIPHERAL_ADC_2,
        PERIPHERAL_ADC_3,
        PERIPHERAL_SDIO,
        PERIPHERAL_SPI_1,
        PERIPHERAL_SPI_4,
        PERIPHERAL_SYS_CFG,
        PERIPHERAL_TIM_9,
        PERIPHERAL_TIM_10,
        PERIPHERAL_TIM_11,
        PERIPHERAL_SAI_1,
        PERIPHERAL_SAI_2
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

    ProcessorSTM32F4xx(const float coreVoltage,
                       const uint32_t externalClockFrequencyHz);

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

    static void setPeripheralClockEnabled(const Peripheral peripheral,
                                          const bool enabled);

    static uint32_t getPeripheralInputClockFrequencyHz(
                                                   const Peripheral peripheral);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------
    
    Error setConfig(const Config& config);
    
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

    static const float myCoreVoltageMap[];

    static const uint32_t myFlashWaitStatesMap[];

    static const uint32_t myFlashWaitStatesFrequencyMap[][9];

    static const uint32_t myRccClockEnableMap[];

    // Variables

    static Config myConfig;
    
    static AhbClockPrescaler myAhbClockPrescaler;
    
    static Apb1ClockPrescaler myApb1ClockPrescaler;
    
    static Apb2ClockPrescaler myApb2ClockPrescaler;

    //--------------------------------------------------------------------------
    // Private methods implemented from Processor
    //--------------------------------------------------------------------------
    
    Processor::Error driverReset();
    
    Processor::Error driverConfigure(const Processor::Config& config);
    
    Processor::Error driverSetPowerMode(const Processor::PowerMode powerMode);

    uint32_t driverGetCoreClockFrequencyHz();
};

}; // namespace Plat4m

#endif // PLAT4M_PROCESSOR_STM32F4XX_H
