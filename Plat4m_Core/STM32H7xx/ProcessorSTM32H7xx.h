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
// Copyright (c) 2022-2023 Benjamin Minerd
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
/// @file ProcessorSTM32H7xx.h
/// @author Ben Minerd
/// @date 6/20/2022
/// @brief ProcessorSTM32H7xx class header file.
///

#ifndef PLAT4M_PROCESSOR_STM32H7XX_H
#define PLAT4M_PROCESSOR_STM32H7XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Processor.h>
#include <Plat4m_Core/ErrorTemplate.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ProcessorSTM32H7xx : public Processor
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_CLOCK_FREQUENCY_INVALID,
        ERROR_CODE_INPUT_VOLTAGE_INVALID,
        ERROR_CODE_CLOCK_STARTUP_FAILED,
        ERROR_CODE_NONE
    };

    using Error = ErrorTemplate<ErrorCode>;

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

    enum Pll
    {
        PLL_1 = 0,
        PLL_2,
        PLL_3
    };

    ///
    /// NOTE: This strange numbering comes from the fact that modifying the PLL
    /// clock divider LSB also changes the HSE/2 bit
    /// (see pg. 162 of datasheet)
    ///
    enum PllClockPrescaler
    {
        PLL_CLOCK_PRESCALER_0 = 0,
        PLL_CLOCK_PRESCALER_1,
        PLL_CLOCK_PRESCALER_2,
        PLL_CLOCK_PRESCALER_3,
        PLL_CLOCK_PRESCALER_4,
        PLL_CLOCK_PRESCALER_5,
        PLL_CLOCK_PRESCALER_6,
        PLL_CLOCK_PRESCALER_7,
        PLL_CLOCK_PRESCALER_8,
        PLL_CLOCK_PRESCALER_9,
        PLL_CLOCK_PRESCALER_10,
        PLL_CLOCK_PRESCALER_11,
        PLL_CLOCK_PRESCALER_12,
        PLL_CLOCK_PRESCALER_13,
        PLL_CLOCK_PRESCALER_14,
        PLL_CLOCK_PRESCALER_15,
        PLL_CLOCK_PRESCALER_16,
        PLL_CLOCK_PRESCALER_17,
        PLL_CLOCK_PRESCALER_18,
        PLL_CLOCK_PRESCALER_19,
        PLL_CLOCK_PRESCALER_20,
        PLL_CLOCK_PRESCALER_21,
        PLL_CLOCK_PRESCALER_22,
        PLL_CLOCK_PRESCALER_23,
        PLL_CLOCK_PRESCALER_24,
        PLL_CLOCK_PRESCALER_25,
        PLL_CLOCK_PRESCALER_26,
        PLL_CLOCK_PRESCALER_27,
        PLL_CLOCK_PRESCALER_28,
        PLL_CLOCK_PRESCALER_29,
        PLL_CLOCK_PRESCALER_30,
        PLL_CLOCK_PRESCALER_31,
        PLL_CLOCK_PRESCALER_32,
        PLL_CLOCK_PRESCALER_33,
        PLL_CLOCK_PRESCALER_34,
        PLL_CLOCK_PRESCALER_35,
        PLL_CLOCK_PRESCALER_36,
        PLL_CLOCK_PRESCALER_37,
        PLL_CLOCK_PRESCALER_38,
        PLL_CLOCK_PRESCALER_39,
        PLL_CLOCK_PRESCALER_40,
        PLL_CLOCK_PRESCALER_41,
        PLL_CLOCK_PRESCALER_42,
        PLL_CLOCK_PRESCALER_43,
        PLL_CLOCK_PRESCALER_44,
        PLL_CLOCK_PRESCALER_45,
        PLL_CLOCK_PRESCALER_46,
        PLL_CLOCK_PRESCALER_47,
        PLL_CLOCK_PRESCALER_48,
        PLL_CLOCK_PRESCALER_49,
        PLL_CLOCK_PRESCALER_50,
        PLL_CLOCK_PRESCALER_51,
        PLL_CLOCK_PRESCALER_52,
        PLL_CLOCK_PRESCALER_53,
        PLL_CLOCK_PRESCALER_54,
        PLL_CLOCK_PRESCALER_55,
        PLL_CLOCK_PRESCALER_56,
        PLL_CLOCK_PRESCALER_57,
        PLL_CLOCK_PRESCALER_58,
        PLL_CLOCK_PRESCALER_59,
        PLL_CLOCK_PRESCALER_60,
        PLL_CLOCK_PRESCALER_61,
        PLL_CLOCK_PRESCALER_62,
        PLL_CLOCK_PRESCALER_63
    };

    enum PllInputFrequencyRange
    {
        PLL_INPUT_FREQUENCY_RANGE_1_TO_2_MHZ = 0,
        PLL_INPUT_FREQUENCY_RANGE_2_TO_4_MHZ,
        PLL_INPUT_FREQUENCY_RANGE_4_TO_8_MHZ,
        PLL_INPUT_FREQUENCY_RANGE_8_TO_16_MHZ
    };

    enum PllVcoRange
    {
        PLL_VCO_RANGE_WIDE_192_TO_960_MHZ = 0,
        PLL_VCO_RANGE_MEDIUM_150_TO_420_MHZ
    };

    enum D1DomainCorePrescaler
    {
        D1_DOMAIN_CORE_PRESCALER_1 = 0,
        D1_DOMAIN_CORE_PRESCALER_2,
        D1_DOMAIN_CORE_PRESCALER_4,
        D1_DOMAIN_CORE_PRESCALER_8,
        D1_DOMAIN_CORE_PRESCALER_16,
        D1_DOMAIN_CORE_PRESCALER_64,
        D1_DOMAIN_CORE_PRESCALER_128,
        D1_DOMAIN_CORE_PRESCALER_256,
        D1_DOMAIN_CORE_PRESCALER_512
    };

    enum AhbPrescaler
    {
        AHB_PRESCALER_1 = 0,
        AHB_PRESCALER_2,
        AHB_PRESCALER_4,
        AHB_PRESCALER_8,
        AHB_PRESCALER_16,
        AHB_PRESCALER_64,
        AHB_PRESCALER_128,
        AHB_PRESCALER_256,
        AHB_PRESCALER_512
    };

    enum Apb1Prescaler
    {
        APB1_PRESCALER_1 = 0,
        APB1_PRESCALER_2,
        APB1_PRESCALER_4,
        APB1_PRESCALER_8,
        APB1_PRESCALER_16
    };

    enum Apb2Prescaler
    {
        APB2_PRESCALER_1 = 0,
        APB2_PRESCALER_2,
        APB2_PRESCALER_4,
        APB2_PRESCALER_8,
        APB2_PRESCALER_16
    };

    enum Apb3Prescaler
    {
        APB3_PRESCALER_1 = 0,
        APB3_PRESCALER_2,
        APB3_PRESCALER_4,
        APB3_PRESCALER_8,
        APB3_PRESCALER_16
    };

    enum Apb4Prescaler
    {
        APB4_PRESCALER_1 = 0,
        APB4_PRESCALER_2,
        APB4_PRESCALER_4,
        APB4_PRESCALER_8,
        APB4_PRESCALER_16
    };

    enum CoreVoltageScale
    {
        CORE_VOLTAGE_SCALE_0_V1R26_TO_V1R40 = 0,
        CORE_VOLTAGE_SCALE_1_V1R15_TO_V1R26,
        CORE_VOLTAGE_SCALE_2_V1R05_TO_V1R15,
        CORE_VOLTAGE_SCALE_3_V0R95_TO_V1R05
    };

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

    enum ProgrammingDelay
    {
        PROGRAMMING_DELAY_0 = 0,
        PROGRAMMING_DELAY_1,
        PROGRAMMING_DELAY_2,
    };

    enum Peripheral
    {
        // AHB1 peripherals
        PERIPHERAL_DMA_1 = 0,
        PERIPHERAL_DMA_2,
        PERIPHERAL_ADC_1_AND_ADC_2,
        PERIPHERAL_ETH_1_MAC,
        PERIPHERAL_ETH_1_TX,
        PERIPHERAL_ETH_1_RX,
        PERIPHERAL_USB_1_OTG,
        PERIPHERAL_USB_1_OTG_PHY,
        PERIPHERAL_USB_2_OTG,
        PERIPHERAL_USB_2_OTG_PHY,
        // AHB2 peripherals
        PERIPHERAL_DCMI,
        PERIPHERAL_CRYPT,
        PERIPHERAL_HASH,
        PERIPHERAL_RNG,
        PERIPHERAL_SDMMC_2_AND_SDMMC_2_DELAY,
        PERIPHERAL_SRAM_1,
        PERIPHERAL_SRAM_2,
        PERIPHERAL_SRAM_3,
        // AHB3 peripherals
        PERIPHERAL_MDMA,
        PERIPHERAL_MDMA_2,
        PERIPHERAL_JPGDEC,
        PERIPHERAL_FMC,
        PERIPHERAL_QUADSPI_AND_QUADSPI_DELAY,
        PERIPHERAL_SDMMC_1_AND_SDMMC_1_DELAY,
        // AHB4 peripherals
        PERIPHERAL_GPIO_A,
        PERIPHERAL_GPIO_B,
        PERIPHERAL_GPIO_C,
        PERIPHERAL_GPIO_D,
        PERIPHERAL_GPIO_E,
        PERIPHERAL_GPIO_F,
        PERIPHERAL_GPIO_G,
        PERIPHERAL_GPIO_H,
        PERIPHERAL_GPIO_I,
        PERIPHERAL_GPIO_J,
        PERIPHERAL_GPIO_K,
        PERIPHERAL_CRC,
        PERIPHERAL_BDMA_AND_DMA_MUX_2,
        PERIPHERAL_ADC_3,
        PERIPHERAL_HSEM,
        PERIPHERAL_BKPRAM,
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
        PERIPHERAL_LPTIM_1,
        PERIPHERAL_SPI_2,
        PERIPHERAL_SPI_3,
        PERIPHERAL_SPDIFRX,
        PERIPHERAL_USART_2,
        PERIPHERAL_USART_3,
        PERIPHERAL_UART_4,
        PERIPHERAL_UART_5,
        PERIPHERAL_I2C_1,
        PERIPHERAL_I2C_2,
        PERIPHERAL_I2C_3,
        PERIPHERAL_CEC,
        PERIPHERAL_DAC_1_AND_DAC_2,
        PERIPHERAL_UART_7,
        PERIPHERAL_UART_8,
        PERIPHERAL_CRS,
        PERIPHERAL_SWPMI,
        PERIPHERAL_OPAMP,
        PERIPHERAL_MDIOS,
        PERIPHERAL_FD_CAN,
        // APB2 peripherals
        PERIPHERAL_TIM_1,
        PERIPHERAL_TIM_8,
        PERIPHERAL_USART_1,
        PERIPHERAL_USART_6,
        PERIPHERAL_SPI_1,
        PERIPHERAL_SPI_4,
        PERIPHERAL_TIM_15,
        PERIPHERAL_TIM_16,
        PERIPHERAL_TIM_17,
        PERIPHERAL_SPI_5,
        PERIPHERAL_SAI_1,
        PERIPHERAL_SAI_2,
        PERIPHERAL_SAI_3,
        PERIPHERAL_DFSDM_1,
        PERIPHERAL_HRTIM,
        // APB3 peripherals
        PERIPHERAL_LTDC,
        PERIPHERAL_WWDG_1,
        // APB4 peripherals
        PERIPHERAL_SYS_CFG,
        PERIPHERAL_LPUART_1,
        PERIPHERAL_SPI_6,
        PERIPHERAL_I2C_4,
        PERIPHERAL_LPTIM_2,
        PERIPHERAL_LPTIM_3,
        PERIPHERAL_LPTIM_4,
        PERIPHERAL_LPTIM_5,
        PERIPHERAL_COMP12,
        PERIPHERAL_V_REF_BUF,
        PERIPHERAL_RTC_APB,
        PERIPHERAL_SAI_4
    };

    struct Registers
    {
        struct CR
        {
            enum : std::uint32_t
            {
                HSION    = bitMask<std::uint32_t>(1,  0),
                HSIKERON = bitMask<std::uint32_t>(1,  1),
                HSIRDY   = bitMask<std::uint32_t>(1,  2),
                HSIDIV   = bitMask<std::uint32_t>(2,  3),
                HSIDIVF  = bitMask<std::uint32_t>(1,  5),
                CSION    = bitMask<std::uint32_t>(1,  7),
                CSIRDY   = bitMask<std::uint32_t>(1,  8),
                CSIKERON = bitMask<std::uint32_t>(1,  9),
                HSI48ON  = bitMask<std::uint32_t>(1, 12),
                HSI48RDY = bitMask<std::uint32_t>(1, 13),
                D1CKRDY  = bitMask<std::uint32_t>(1, 14),
                D2CKRDY  = bitMask<std::uint32_t>(1, 15),
                HSEON    = bitMask<std::uint32_t>(1, 16),
                HSERDY   = bitMask<std::uint32_t>(1, 17),
                HSEBYP   = bitMask<std::uint32_t>(1, 18),
                HSECSSON = bitMask<std::uint32_t>(1, 19),
                PLL1ON   = bitMask<std::uint32_t>(1, 24),
                PLL1RDY  = bitMask<std::uint32_t>(1, 25),
                PLL2ON   = bitMask<std::uint32_t>(1, 26),
                PLL2RDY  = bitMask<std::uint32_t>(1, 27),
                PLL3ON   = bitMask<std::uint32_t>(1, 28),
                PLL3RDY  = bitMask<std::uint32_t>(1, 29)
            };
        };

        struct CFGR
        {
            enum : std::uint32_t
            {
                SW          = bitMask<std::uint32_t>(3,  0),
                SWS         = bitMask<std::uint32_t>(3,  3),
                STOPWUCK    = bitMask<std::uint32_t>(1,  6),
                STOPKERWUCK = bitMask<std::uint32_t>(1,  7),
                RTCPRE      = bitMask<std::uint32_t>(6,  8),
                HRTIMSEL    = bitMask<std::uint32_t>(1, 14),
                TIMPRE      = bitMask<std::uint32_t>(1, 15),
                MCO1PRE     = bitMask<std::uint32_t>(4, 18),
                MCO         = bitMask<std::uint32_t>(3, 22),
                MCO2PRE     = bitMask<std::uint32_t>(4, 25),
                MCO2        = bitMask<std::uint32_t>(3, 29)
            };
        };
    };

    struct Config
    {
        ClockSource clockSource;
        /// NOTE: Only silicon revisions X and V support 480 MHz, others are
        /// limited to 400 MHz. Because of the allowable intermediate frequency
        /// ranges for generating the peripheral clocks, if the 400 MHz max
        /// core clock frequency is desired, the AHBx and APBx clocks must be
        /// set to 200 MHz.
        std::uint32_t coreClockFrequencyHz;         /// 480 MHz max
        std::uint32_t d1DomainCoreClockFrequencyHz; /// 480 MHz max
        std::uint32_t ahbClockFrequencyHz;          /// 240 MHz max
        std::uint32_t apb1ClockFrequencyHz;         /// 240 MHz max
        std::uint32_t apb2ClockFrequencyHz;         /// 240 MHz max
        std::uint32_t apb3ClockFrequencyHz;         /// 240 MHz max
        std::uint32_t apb4ClockFrequencyHz;         /// 240 MHz max
        CoreVoltageScale coreVoltageScale;
        std::uint32_t vectorTableAddress;
    };

    struct InternalConfig
    {
        PllInputFrequencyRange pllInputFrequencyRange;
        PllVcoRange pllVcoRange;
        std::uint32_t pllM;
        std::uint32_t pllN;
        std::uint32_t pllP;
        std::uint32_t pllQ;
        std::uint32_t pllR;
        D1DomainCorePrescaler d1DomainCorePrescaler;
        AhbPrescaler ahbPrescaler;
        Apb1Prescaler apb1Prescaler;
        Apb2Prescaler apb2Prescaler;
        Apb3Prescaler apb3Prescaler;
        Apb4Prescaler apb4Prescaler;
        FlashWaitStates flashWaitStates;
        ProgrammingDelay programmingDelay;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    ProcessorSTM32H7xx(const float coreVoltage,
                       const std::uint32_t externalClockFrequencyHz);

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static Config getConfig();

    static InternalConfig getInternalConfig();

    static void reset();

    static void setPeripheralClockEnabled(const Peripheral peripheral,
                                          const bool enabled);

    static std::uint32_t getPeripheralInputClockFrequencyHz(
                                                   const Peripheral peripheral);

    static std::uint32_t getSysTickClockFrequencyHz();

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    virtual Error setConfig(const Config& config);

    virtual ClockSource getCoreClockSource();

    virtual void setCoreClockSource(const ClockSource coreClockSource);

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    void setInternalConfig(const InternalConfig& config);

private:

    //--------------------------------------------------------------------------
    // Private types
    //--------------------------------------------------------------------------

    struct Prescaler
    {
        std::uint32_t value;
        std::uint32_t bits;
    };

    struct FlashWaitStatesProgrammingDelay
    {
        std::uint32_t axiClockRateHz;
        ProgrammingDelay programmingDelay;
    };

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const Prescaler myD1DomainCoreClockPrescalerMap[];

    static const Prescaler myAhbClockPrescalerMap[];

    static const Prescaler myApb1ClockPrescalerMap[];

    static const Prescaler myApb2ClockPrescalerMap[];

    static const Prescaler myApb3ClockPrescalerMap[];

    static const Prescaler myApb4ClockPrescalerMap[];

    static const std::uint32_t myFlashWaitStatesMap[];

    static const FlashWaitStatesProgrammingDelay
                                             myFlashWaitStatesFrequencyMap[][6];

    static const std::uint32_t myRccClockEnableMap[];

    // Variables

    static Config myConfig;

    static InternalConfig myInternalConfig;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Processor
    //--------------------------------------------------------------------------

    virtual Processor::Error driverReset() override;

    virtual Processor::Error driverConfigure(
                                      const Processor::Config& config) override;

    virtual Processor::Error driverSetPowerMode(
                                 const Processor::PowerMode powerMode) override;

    virtual std::uint32_t driverGetCoreClockFrequencyHz() override;
};

}; // namespace Plat4m

#endif // PLAT4M_PROCESSOR_STM32H7XX_H
