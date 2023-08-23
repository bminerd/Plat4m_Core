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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file ProcessorSTM32H7xx.cpp
/// @author Ben Minerd
/// @date 6/20/2022
/// @brief ProcessorSTM32H7xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>

#include <stm32h7xx.h>
#include <cachel1_armv7.h>

#include <Plat4m_Core/STM32H7xx/ProcessorSTM32H7xx.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/Application.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ProcessorSTM32H7xx::Prescaler
                         ProcessorSTM32H7xx::myD1DomainCoreClockPrescalerMap[] =
{
    {1,    0 << 8}, /// D1_DOMAIN_CORE_PRESCALER_1
    {2,    8 << 8}, /// D1_DOMAIN_CORE_PRESCALER_2
    {4,    9 << 8}, /// D1_DOMAIN_CORE_PRESCALER_4
    {8,   10 << 8}, /// D1_DOMAIN_CORE_PRESCALER_8
    {16,  11 << 8}, /// D1_DOMAIN_CORE_PRESCALER_16
    {64,  12 << 8}, /// D1_DOMAIN_CORE_PRESCALER_64
    {128, 13 << 8}, /// D1_DOMAIN_CORE_PRESCALER_128
    {256, 14 << 8}, /// D1_DOMAIN_CORE_PRESCALER_256
    {512, 15 << 8}  /// D1_DOMAIN_CORE_PRESCALER_512
};

const ProcessorSTM32H7xx::Prescaler
                                  ProcessorSTM32H7xx::myAhbClockPrescalerMap[] =
{
    {1,    0}, /// AHB_PRESCALER_1
    {2,    8}, /// AHB_PRESCALER_2
    {4,    9}, /// AHB_PRESCALER_4
    {8,   10}, /// AHB_PRESCALER_8
    {16,  11}, /// AHB_PRESCALER_16
    {64,  12}, /// AHB_PRESCALER_64
    {128, 13}, /// AHB_PRESCALER_128
    {256, 14}, /// AHB_PRESCALER_256
    {512, 15}  /// AHB_PRESCALER_512
};

const ProcessorSTM32H7xx::Prescaler
                                 ProcessorSTM32H7xx::myApb1ClockPrescalerMap[] =
{
    {1,  0 << 8}, /// APB1_PRESCALER_1
    {2,  4 << 8}, /// APB1_PRESCALER_2
    {4,  5 << 8}, /// APB1_PRESCALER_3
    {8,  6 << 8}, /// APB1_PRESCALER_8
    {16, 7 << 8}  /// APB1_PRESCALER_16
};

const ProcessorSTM32H7xx::Prescaler
                                 ProcessorSTM32H7xx::myApb2ClockPrescalerMap[] =
{
    {1,  0 << 4}, /// APB2_PRESCALER_1
    {2,  4 << 4}, /// APB2_PRESCALER_2
    {4,  5 << 4}, /// APB2_PRESCALER_3
    {8,  6 << 4}, /// APB2_PRESCALER_8
    {16, 7 << 4}  /// APB2_PRESCALER_16
};

const ProcessorSTM32H7xx::Prescaler
                                 ProcessorSTM32H7xx::myApb3ClockPrescalerMap[] =
{
    {1,  0 << 4}, /// APB3_PRESCALER_1
    {2,  4 << 4}, /// APB3_PRESCALER_2
    {4,  5 << 4}, /// APB3_PRESCALER_4
    {8,  6 << 4}, /// APB3_PRESCALER_8
    {16, 7 << 4}  /// APB3_PRESCALER_16
};

const ProcessorSTM32H7xx::Prescaler
                                 ProcessorSTM32H7xx::myApb4ClockPrescalerMap[] =
{
    {1,  0 << 4}, /// APB4_PRESCALER_1
    {2,  4 << 4}, /// APB4_PRESCALER_2
    {4,  5 << 4}, /// APB4_PRESCALER_3
    {8,  6 << 4}, /// APB4_PRESCALER_8
    {16, 7 << 4}  /// APB4_PRESCALER_16
};

const ProcessorSTM32H7xx::FlashWaitStatesProgrammingDelay
                        ProcessorSTM32H7xx::myFlashWaitStatesFrequencyMap[][6] =
{
    { /// CORE_VOLTAGE_SCALE_0_V1R26_TO_V1R40
        { 70000000, PROGRAMMING_DELAY_0}, /// FLASH_WAIT_STATES_0
        {140000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_1
        {185000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_2
        {210000000, PROGRAMMING_DELAY_2}, /// FLASH_WAIT_STATES_2 <-- Note repeat of FLASH_WAIT_STATE_2
        {225000000, PROGRAMMING_DELAY_2}, /// FLASH_WAIT_STATES_3
        {240000000, PROGRAMMING_DELAY_2}  /// FLASH_WAIT_STATES_4
    },
    { /// CORE_VOLTAGE_SCALE_1_V1R15_TO_V1R26
        { 70000000, PROGRAMMING_DELAY_0}, /// FLASH_WAIT_STATES_0
        {140000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_1
        {185000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_2
        {210000000, PROGRAMMING_DELAY_2}, /// FLASH_WAIT_STATES_2
        {225000000, PROGRAMMING_DELAY_2}, /// FLASH_WAIT_STATES_3
        {225000000, PROGRAMMING_DELAY_2}  /// FLASH_WAIT_STATES_4
    },
    { /// CORE_VOLTAGE_SCALE_2_V1R05_TO_V1R15
        { 55000000, PROGRAMMING_DELAY_0}, /// FLASH_WAIT_STATES_0
        {110000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_1
        {165000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_2
        {165000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_2
        {225000000, PROGRAMMING_DELAY_2}, /// FLASH_WAIT_STATES_3
        {225000000, PROGRAMMING_DELAY_2}  /// FLASH_WAIT_STATES_4
    },
    { /// CORE_VOLTAGE_SCALE_3_V0R95_TO_V1R05
        { 45000000, PROGRAMMING_DELAY_0}, /// FLASH_WAIT_STATES_0
        { 90000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_1
        {135000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_2
        {135000000, PROGRAMMING_DELAY_1}, /// FLASH_WAIT_STATES_2
        {180000000, PROGRAMMING_DELAY_2}, /// FLASH_WAIT_STATES_3
        {225000000, PROGRAMMING_DELAY_2}  /// FLASH_WAIT_STATES_4
    }
};

const std::uint32_t ProcessorSTM32H7xx::myRccClockEnableMap[] =
{
    // AHB1 peripherals
    bitMask<std::uint32_t>(1,  0), /// PERIPHERAL_DMA_1
    bitMask<std::uint32_t>(1,  1), /// PERIPHERAL_DMA_2
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_ADC_1_AND_ADC_2
    bitMask<std::uint32_t>(1, 15), /// PERIPHERAL_ETH_1_MAC
    bitMask<std::uint32_t>(1, 16), /// PERIPHERAL_ETH_1_TX
    bitMask<std::uint32_t>(1, 17), /// PERIPHERAL_ETH_1_RX
    bitMask<std::uint32_t>(1, 25), /// PERIPHERAL_USB_1_OTG
    bitMask<std::uint32_t>(1, 26), /// PERIPHERAL_USB_1_OTG_PHY
    bitMask<std::uint32_t>(1, 27), /// PERIPHERAL_USB_2_OTG
    bitMask<std::uint32_t>(1, 28), /// PERIPHERAL_USB_2_OTG_PHY
    // AHB2 peripherals
    bitMask<std::uint32_t>(1,  0), /// PERIPHERAL_DCMI
    bitMask<std::uint32_t>(1,  4), /// PERIPHERAL_CRYPT
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_HASH
    bitMask<std::uint32_t>(1,  6), /// PERIPHERAL_RNG
    bitMask<std::uint32_t>(1,  9), /// PERIPHERAL_SDMMC_2_AND_SDMMC_2_DELAY
    bitMask<std::uint32_t>(1, 29), /// PERIPHERAL_SRAM_1
    bitMask<std::uint32_t>(1, 30), /// PERIPHERAL_SRAM_2
    bitMask<std::uint32_t>(1, 31), /// PERIPHERAL_SRAM_3
    // AHB3 peripherals
    bitMask<std::uint32_t>(1,  0), /// PERIPHERAL_MDMA
    bitMask<std::uint32_t>(1,  4), /// PERIPHERAL_DMA_2
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_JPGDEC
    bitMask<std::uint32_t>(1, 12), /// PERIPHERAL_FMC
    bitMask<std::uint32_t>(1, 14), /// PERIPHERAL_QUADSPI_AND_QUADSPI_DELAY
    bitMask<std::uint32_t>(1, 16), /// PERIPHERAL_SDMMC_1_AND_SDMMC_1_DELAY
    // AHB4 peripherals
    bitMask<std::uint32_t>(1,  0), /// PERIPHERAL_GPIO_A
    bitMask<std::uint32_t>(1,  1), /// PERIPHERAL_GPIO_B
    bitMask<std::uint32_t>(1,  2), /// PERIPHERAL_GPIO_C
    bitMask<std::uint32_t>(1,  3), /// PERIPHERAL_GPIO_D
    bitMask<std::uint32_t>(1,  4), /// PERIPHERAL_GPIO_E
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_GPIO_F
    bitMask<std::uint32_t>(1,  6), /// PERIPHERAL_GPIO_G
    bitMask<std::uint32_t>(1,  7), /// PERIPHERAL_GPIO_H
    bitMask<std::uint32_t>(1,  8), /// PERIPHERAL_GPIO_I
    bitMask<std::uint32_t>(1,  9), /// PERIPHERAL_GPIO_J
    bitMask<std::uint32_t>(1, 10), /// PERIPHERAL_GPIO_K
    bitMask<std::uint32_t>(1, 19), /// PERIPHERAL_CRC
    bitMask<std::uint32_t>(1, 21), /// PERIPHERAL_BDMA_AND_DMA_MUX_2
    bitMask<std::uint32_t>(1, 24), /// PERIPHERAL_ADC_3
    bitMask<std::uint32_t>(1, 25), /// PERIPHERAL_HSEM
    bitMask<std::uint32_t>(1, 28), /// PERIPHERAL_BKPRAM
    // APB1 peripherals (low register)
    bitMask<std::uint32_t>(1,  0), /// PERIPHERAL_TIM_2
    bitMask<std::uint32_t>(1,  1), /// PERIPHERAL_TIM_3
    bitMask<std::uint32_t>(1,  2), /// PERIPHERAL_TIM_4
    bitMask<std::uint32_t>(1,  3), /// PERIPHERAL_TIM_5
    bitMask<std::uint32_t>(1,  4), /// PERIPHERAL_TIM_6
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_TIM_7
    bitMask<std::uint32_t>(1,  6), /// PERIPHERAL_TIM_12
    bitMask<std::uint32_t>(1,  7), /// PERIPHERAL_TIM_13
    bitMask<std::uint32_t>(1,  8), /// PERIPHERAL_TIM_14
    bitMask<std::uint32_t>(1,  9), /// PERIPHERAL_LPTIM_1
    bitMask<std::uint32_t>(1, 14), /// PERIPHERAL_SPI_2
    bitMask<std::uint32_t>(1, 15), /// PERIPHERAL_SPI_3
    bitMask<std::uint32_t>(1, 16), /// PERIPHERAL_SPDIFRX
    bitMask<std::uint32_t>(1, 17), /// PERIPHERAL_USART_2
    bitMask<std::uint32_t>(1, 18), /// PERIPHERAL_USART_3
    bitMask<std::uint32_t>(1, 19), /// PERIPHERAL_UART_4
    bitMask<std::uint32_t>(1, 20), /// PERIPHERAL_UART_5
    bitMask<std::uint32_t>(1, 21), /// PERIPHERAL_I2C_1
    bitMask<std::uint32_t>(1, 22), /// PERIPHERAL_I2C_2
    bitMask<std::uint32_t>(1, 22), /// PERIPHERAL_I2C_3
    bitMask<std::uint32_t>(1, 27), /// PERIPHERAL_CEC
    bitMask<std::uint32_t>(1, 29), /// PERIPHERAL_DAC_1_AND_DAC_2
    bitMask<std::uint32_t>(1, 30), /// PERIPHERAL_UART_7
    bitMask<std::uint32_t>(1, 31), /// PERIPHERAL_UART_8
    // APB1 peripherals (High register)
    bitMask<std::uint32_t>(1,  1), /// PERIPHERAL_CRS
    bitMask<std::uint32_t>(1,  2), /// PERIPHERAL_SWPMI
    bitMask<std::uint32_t>(1,  4), /// PERIPHERAL_OPAMP
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_MDIOS
    bitMask<std::uint32_t>(1,  8), /// PERIPHERAL_FD_CAN
    // APB2 peripherals
    bitMask<std::uint32_t>(1,  0), /// PERIPHERAL_TIM_1
    bitMask<std::uint32_t>(1,  1), /// PERIPHERAL_TIM_8
    bitMask<std::uint32_t>(1,  4), /// PERIPHERAL_USART_1
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_USART_6
    bitMask<std::uint32_t>(1, 12), /// PERIPHERAL_SPI_1
    bitMask<std::uint32_t>(1, 13), /// PERIPHERAL_SPI_4
    bitMask<std::uint32_t>(1, 16), /// PERIPHERAL_TIM_15
    bitMask<std::uint32_t>(1, 17), /// PERIPHERAL_TIM_16
    bitMask<std::uint32_t>(1, 18), /// PERIPHERAL_TIM_17
    bitMask<std::uint32_t>(1, 20), /// PERIPHERAL_SPI_5
    bitMask<std::uint32_t>(1, 22), /// PERIPHERAL_SAI_1
    bitMask<std::uint32_t>(1, 23), /// PERIPHERAL_SAI_2
    bitMask<std::uint32_t>(1, 24), /// PERIPHERAL_SAI_3
    bitMask<std::uint32_t>(1, 28), /// PERIPHERAL_DFSDM_1
    bitMask<std::uint32_t>(1, 29), /// PERIPHERAL_HRTIM
    // APB3 peripherals
    bitMask<std::uint32_t>(1,  3), /// PERIPHERAL_LTDC
    bitMask<std::uint32_t>(1,  6), /// PERIPHERAL_WWDG_1
    // APB4 peripherals
    bitMask<std::uint32_t>(1,  1), /// PERIPHERAL_SYS_CFG
    bitMask<std::uint32_t>(1,  3), /// PERIPHERAL_LPUART_1
    bitMask<std::uint32_t>(1,  5), /// PERIPHERAL_SPI_6
    bitMask<std::uint32_t>(1,  7), /// PERIPHERAL_I2C_4
    bitMask<std::uint32_t>(1,  9), /// PERIPHERAL_LPTIM_2
    bitMask<std::uint32_t>(1, 10), /// PERIPHERAL_LPTIM_3
    bitMask<std::uint32_t>(1, 11), /// PERIPHERAL_LPTIM_4
    bitMask<std::uint32_t>(1, 12), /// PERIPHERAL_LPTIM_5
    bitMask<std::uint32_t>(1, 14), /// PERIPHERAL_COMP12
    bitMask<std::uint32_t>(1, 15), /// PERIPHERAL_V_REF_BUF
    bitMask<std::uint32_t>(1, 16), /// PERIPHERAL_RTC_APB
    bitMask<std::uint32_t>(1, 21)  /// PERIPHERAL_SAI_4
};

// Variables

ProcessorSTM32H7xx::Config ProcessorSTM32H7xx::myConfig;

ProcessorSTM32H7xx::InternalConfig ProcessorSTM32H7xx::myInternalConfig;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32H7xx::ProcessorSTM32H7xx(
                                 const float coreVoltage,
                                 const std::uint32_t externalClockFrequencyHz) :
    Processor(ENDIAN_LITTLE, coreVoltage, externalClockFrequencyHz)
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32H7xx::Config ProcessorSTM32H7xx::getConfig()
{
    return myConfig;
}

//------------------------------------------------------------------------------
ProcessorSTM32H7xx::InternalConfig ProcessorSTM32H7xx::getInternalConfig()
{
    return myInternalConfig;
}

//------------------------------------------------------------------------------
void ProcessorSTM32H7xx::reset()
{
    // FPU settings
    if (__FPU_USED == 1)
    {
        // set CP10 and CP11 Full Access
        Plat4m::setBits(SCB->CPACR, ((3UL << 10*2)|(3UL << 11*2)));
    }

    // Reset the RCC clock configuration to the default reset state
    // Set HSION bit */
    Plat4m::setBits(RCC->CR, (uint32_t) 0x00000001);

    // Reset CFGR register
    // 0x07803FF3 = ~0xF878FC00C
    Plat4m::clearBits(RCC->CFGR, (uint32_t) 0x07803FF3);

    // Reset HSEON, CSSON and PLLON bits
    // 0x01090000 = ~0xFEF6FFFF
    Plat4m::clearBits(RCC->CR, (uint32_t) 0x01090000);

    // Reset HSEBYP bit
    // 0x00040000 = ~0xFFFBFFFF
    Plat4m::clearBits(RCC->CR, (uint32_t) 0x00040000);

    // Disable all clock interrupts
    RCC->CIER = 0x00000000;

    // Enable SYSCFG clock
    ProcessorSTM32H7xx::setPeripheralClockEnabled(
                                         ProcessorSTM32H7xx::PERIPHERAL_SYS_CFG,
                                         true);

    if((DBGMCU->IDCODE & 0xFFFF0000U) < 0x20000000U)
    {
        /* if stm32h7 revY*/
        /* Change  the switch matrix read issuing capability to 1 for the AXI SRAM target (Target 7) */
        *((__IO uint32_t*)0x51008108) = 0x000000001U;
    }

    // Plat4m::setBits(RCC->APB4RSTR, 0xFFFFFFFF);
    // Plat4m::setBits(RCC->APB3RSTR, 0xFFFFFFFF);
    // Plat4m::setBits(RCC->APB2RSTR, 0xFFFFFFFF);
    // Plat4m::setBits(RCC->APB1HRSTR, 0xFFFFFFFF);
    // Plat4m::setBits(RCC->APB1LRSTR, 0xFFFFFFFF);
    // Plat4m::setBits(RCC->AHB4RSTR,  0xFFFFFFFF);
    // Plat4m::setBits(RCC->AHB3RSTR,  0xFFFFFFFF);
    // Plat4m::setBits(RCC->AHB2RSTR,  0xFFFFFFFF);
    // Plat4m::setBits(RCC->AHB1RSTR,  0xFFFFFFFF);

    // Plat4m::clearBits(RCC->APB4RSTR, 0xFFFFFFFF);
    // Plat4m::clearBits(RCC->APB3RSTR, 0xFFFFFFFF);
    // Plat4m::clearBits(RCC->APB2RSTR, 0xFFFFFFFF);
    // Plat4m::clearBits(RCC->APB1HRSTR, 0xFFFFFFFF);
    // Plat4m::clearBits(RCC->APB1LRSTR, 0xFFFFFFFF);
    // Plat4m::clearBits(RCC->AHB4RSTR,  0xFFFFFFFF);
    // Plat4m::clearBits(RCC->AHB3RSTR,  0xFFFFFFFF);
    // Plat4m::clearBits(RCC->AHB2RSTR,  0xFFFFFFFF);
    // Plat4m::clearBits(RCC->AHB1RSTR,  0xFFFFFFFF);
}

//------------------------------------------------------------------------------
void ProcessorSTM32H7xx::setPeripheralClockEnabled(const Peripheral peripheral,
                                                   const bool enabled)
{
    // AHB1 peripheral
    if (peripheral <= PERIPHERAL_USB_2_OTG_PHY)
    {
        setBitsSet(RCC->AHB1ENR, myRccClockEnableMap[peripheral], enabled);
    }
    // AHB2 peripheral
    else if (peripheral <= PERIPHERAL_SRAM_3)
    {
        setBitsSet(RCC->AHB2ENR, myRccClockEnableMap[peripheral], enabled);
    }
    // AHB3 peripheral
    else if (peripheral <= PERIPHERAL_SDMMC_1_AND_SDMMC_1_DELAY)
    {
        setBitsSet(RCC->AHB3ENR, myRccClockEnableMap[peripheral], enabled);
    }
    // AHB4 peripheral
    else if (peripheral <= PERIPHERAL_BKPRAM)
    {
        setBitsSet(RCC->AHB4ENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB1 peripheral (low register)
    else if (peripheral <= PERIPHERAL_UART_8)
    {
        setBitsSet(RCC->APB1LENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB1 peripheral (high register)
    else if (peripheral <= PERIPHERAL_FD_CAN)
    {
        setBitsSet(RCC->APB1HENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB2 peripheral
    else if (peripheral <= PERIPHERAL_HRTIM)
    {
        setBitsSet(RCC->APB2ENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB3 peripheral
    else if (peripheral <= PERIPHERAL_WWDG_1)
    {
        setBitsSet(RCC->APB3ENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB4 peripheral
    else if (peripheral <= PERIPHERAL_SAI_4)
    {
        setBitsSet(RCC->APB4ENR, myRccClockEnableMap[peripheral], enabled);
    }
}

//------------------------------------------------------------------------------
std::uint32_t ProcessorSTM32H7xx::getPeripheralInputClockFrequencyHz(
                                                    const Peripheral peripheral)
{
    std::uint32_t inputClockFrequencyHz;

    // AHB1, AHB2, AHB3, or AHB4 peripheral
    if (peripheral <= PERIPHERAL_BKPRAM)
    {
        inputClockFrequencyHz = myConfig.ahbClockFrequencyHz;
    }
    // APB1 peripheral
    else if (peripheral <= PERIPHERAL_FD_CAN)
    {
        inputClockFrequencyHz = myConfig.apb1ClockFrequencyHz;
    }
    // APB2 peripheral
    else if (peripheral <= PERIPHERAL_HRTIM)
    {
        inputClockFrequencyHz = myConfig.apb2ClockFrequencyHz;
    }
    // APB3 peripheral
    else if (peripheral <= PERIPHERAL_WWDG_1)
    {
        inputClockFrequencyHz = myConfig.apb3ClockFrequencyHz;
    }
    // APB4 peripheral
    else // if (peripheral <= PERIPHERAL_SAI_4)
    {
        inputClockFrequencyHz = myConfig.apb4ClockFrequencyHz;
    }

    return inputClockFrequencyHz;
}

//------------------------------------------------------------------------------
std::uint32_t ProcessorSTM32H7xx::getSysTickClockFrequencyHz()
{
    return (myConfig.coreClockFrequencyHz / 8);
}

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32H7xx::Error ProcessorSTM32H7xx::setConfig(const Config& config)
{
    // Currently disabled due to hard fault
    // SCB_EnableICache();

    // Currently disabled due to hard fault
    // SCB_EnableDCache();

    if (config.coreClockFrequencyHz > 480000000)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    // Calculate PLL_M, PLL_N, PLL_P, and PLL_Q factors

    myInternalConfig.pllInputFrequencyRange =
                                           PLL_INPUT_FREQUENCY_RANGE_1_TO_2_MHZ;

    std::uint32_t vcoInputFrequencyHz;

    if (config.coreClockFrequencyHz > 420000000)
    {
        vcoInputFrequencyHz = 2000000;
        myInternalConfig.pllVcoRange = PLL_VCO_RANGE_WIDE_192_TO_960_MHZ;
    }
    else
    {
        vcoInputFrequencyHz = 1000000;
        myInternalConfig.pllVcoRange = PLL_VCO_RANGE_MEDIUM_150_TO_420_MHZ;
    }

    ///
    /// PLL_M calculation (datasheet pg. ???)
    ///
    /// VCO input frequency = PLL input clock frequency / PLL_M
    /// - 1 MHz <= VCO input frequency <= 2 MHz*
    ///   *Datasheet recommends PLL_VCO = 2 MHz to reduce PLL jitter, but this
    ///   isn't possible for 168 MHz operation
    /// - 1 <= PLL_M <= 63
    ///
    std::uint32_t pllM = (getClockSourceFrequencyHz() / vcoInputFrequencyHz);

    if (pllM > 63)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.pllM = pllM;

    ///
    /// PLL_N calculation (datasheet pg. ???)
    ///
    /// VCO output frequency = VCO input frequency * PLL_N
    /// - 192 MHz <= VCO output frequency <= 432 MHz (datasheet pg. 126)
    /// - 3 <= PLL_N <= 511
    ///
    std::uint32_t pllN = 0;

    ///
    /// PLL_P calculation (datasheet pg. ???)
    ///
    /// SYSCLK = PLL_VCO / PLL_P
    /// - PLL_P = [0, 2, 4, 6, ... , 128] (even values only)
    ///
    std::uint32_t vcoOutputFrequencyHz;
    std::uint32_t pllP = 0;

    while (pllP < 128)
    {
        std::uint32_t pllPFactor = pllP + 1;

        std::uint32_t pllNFactor =
               (config.coreClockFrequencyHz / vcoInputFrequencyHz) * pllPFactor;
        vcoOutputFrequencyHz = vcoInputFrequencyHz * pllNFactor;

        if ((vcoOutputFrequencyHz / pllPFactor) == config.coreClockFrequencyHz)
        {
            pllN = pllNFactor - 1;

            break;
        }

        if (pllP == 0)
        {
            pllP++;
        }
        else
        {
            pllP += 2;
        }
    }

    // Check to see if a valid PLL_P value was found
    if (pllP > 127)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    // Check to see if a valid PLL_N value was found
    if ((pllN < 3) || (pllN > 511))
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.pllN = pllN;
    myInternalConfig.pllP = pllP;

    // PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N

    /// PLL_Q calculation
    /// PLL_R calculation

    setBits(RCC->CR, Registers::CR::HSEON);

    while (!(areBitsSet(RCC->CR, Registers::CR::HSERDY)))
    {
    }

    clearAndSetBits(RCC->PLLCKSELR, 0x3F << 4, myInternalConfig.pllM << 4);
    setBits(RCC->PLLCKSELR, 2);
    setBits(RCC->PLLCFGR, myInternalConfig.pllInputFrequencyRange << 2);
    setBits(RCC->PLLCFGR, myInternalConfig.pllVcoRange << 1);
    clearAndSetBits(RCC->PLL1DIVR, 0xFFFF, myInternalConfig.pllN);
    clearAndSetBits(RCC->PLL1DIVR, 0x7F << 9, (myInternalConfig.pllP) << 9);

    // Enable PLL1
    setBit(RCC->PLLCFGR, 16);

    std::uint32_t size = 0;
    std::uint32_t i = 0;

    ///
    /// D1 Domain Core Clock = sys_ck / prescaler
    /// - prescaler = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    /// - Core Clock <= 480 MHz
    ///

    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    size = arraySize(myD1DomainCoreClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.coreClockFrequencyHz /
             myD1DomainCoreClockPrescalerMap[i].value) ==
                                            config.d1DomainCoreClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if no prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.d1DomainCorePrescaler =
                                          static_cast<D1DomainCorePrescaler>(i);
    setBits(RCC->D1CFGR, myD1DomainCoreClockPrescalerMap[i].bits);

    ///
    /// AHB Clock = sys_d1cpre_ck / prescaler
    /// - prescaler = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    /// - AHB <= 240 MHz
    ///

    if (config.ahbClockFrequencyHz > 240000000)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    size = arraySize(myAhbClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.d1DomainCoreClockFrequencyHz /
             myAhbClockPrescalerMap[i].value) == config.ahbClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if no prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.ahbPrescaler = static_cast<AhbPrescaler>(i);
    setBits(RCC->D1CFGR, myAhbClockPrescalerMap[i].bits);

    ///
    /// APB1 Clock = AHB / prescaler
    /// - prescaler = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    /// - APB1 <= 240 MHz
    ///

    if (config.apb1ClockFrequencyHz > 240000000)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    size = arraySize(myApb1ClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.ahbClockFrequencyHz /
             myApb1ClockPrescalerMap[i].value) == config.apb1ClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if no prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.apb1Prescaler = static_cast<Apb1Prescaler>(i);
    setBits(RCC->D2CFGR, myApb1ClockPrescalerMap[i].bits);

    ///
    /// APB2 Clock = AHB / prescaler
    /// - prescaler = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    /// - APB2 <= 240 MHz
    ///

    if (config.apb2ClockFrequencyHz > 240000000)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    size = arraySize(myApb2ClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.ahbClockFrequencyHz /
             myApb2ClockPrescalerMap[i].value) == config.apb2ClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if no prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.apb2Prescaler = static_cast<Apb2Prescaler>(i);
    setBits(RCC->D2CFGR, myApb2ClockPrescalerMap[i].bits);

    ///
    /// APB3 Clock = AHB / prescaler
    /// - prescaler = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    /// - APB3 <= 240 MHz
    ///

    if (config.apb3ClockFrequencyHz > 240000000)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    size = arraySize(myApb3ClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.ahbClockFrequencyHz /
             myApb3ClockPrescalerMap[i].value) == config.apb3ClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if no prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.apb3Prescaler = static_cast<Apb3Prescaler>(i);
    setBits(RCC->D1CFGR, myApb3ClockPrescalerMap[i].bits);

    ///
    /// APB4 Clock = AHB / prescaler
    /// - prescaler = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    /// - APB4 <= 240 MHz
    ///

    if (config.apb4ClockFrequencyHz > 240000000)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    size = arraySize(myApb4ClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.ahbClockFrequencyHz /
             myApb4ClockPrescalerMap[i].value) == config.apb4ClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if no prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myInternalConfig.apb4Prescaler = static_cast<Apb4Prescaler>(i);
    setBits(RCC->D3CFGR, myApb4ClockPrescalerMap[i].bits);

    if (config.clockSource == CLOCK_SOURCE_EXTERNAL)
    {
        bool isHseRunning = false;
        std::uint32_t startUpCounter = 500;

        // Enable HSE
        setBits(RCC->CR, 1 << 16);

        while (!isHseRunning && startUpCounter--)
        {
            isHseRunning = isBitSet(RCC->CR, 17);
        }

        if (!startUpCounter)
        {
            return Error(ERROR_CODE_CLOCK_STARTUP_FAILED);
        }

        // setBits(RCC->CFGR, RCC_CFGR_PLLSRC_PREDIV1);
    }

    // Enable PLL
    setBits(RCC->CR, Registers::CR::PLL1ON);

    // Wait till PLL is ready
    while (areBitsClear(RCC->CR, Registers::CR::PLL1RDY))
    {
    }

    size = arraySize(myFlashWaitStatesFrequencyMap[0]);

    const std::uint32_t scale =
                            static_cast<std::uint32_t>(config.coreVoltageScale);

    for (i = 0; i < size; i++)
    {
        if (config.ahbClockFrequencyHz <=
                         myFlashWaitStatesFrequencyMap[scale][i].axiClockRateHz)
        {
            break;
        }
    }

    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    if (i < 3)
    {
        myInternalConfig.flashWaitStates = static_cast<FlashWaitStates>(i);
    }
    else
    {
        myInternalConfig.flashWaitStates = static_cast<FlashWaitStates>(i - 1);
    }

    myInternalConfig.programmingDelay =
                       myFlashWaitStatesFrequencyMap[scale][i].programmingDelay;

    setBits(FLASH->ACR,
            static_cast<std::uint32_t>(myInternalConfig.flashWaitStates));
    setBits(
           FLASH->ACR,
           static_cast<std::uint32_t>( myInternalConfig.programmingDelay << 4));

    // Select PLL as system clock source
    // clearAndSetBits(RCC->CFGR, 7, 3);
    setBits(RCC->CFGR, 3);

    // Wait till PLL is used as system clock source
    while (!(areBitsSet(RCC->CFGR, 3 << 3)))
    {
    }

    SCB->VTOR = (config.vectorTableAddress & (uint32_t)0x1FFFFF80);

    NVIC_SetPriorityGrouping(3);

    myConfig = config;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ProcessorSTM32H7xx::ClockSource ProcessorSTM32H7xx::getCoreClockSource()
{
    return (myConfig.clockSource);
}

//------------------------------------------------------------------------------
void ProcessorSTM32H7xx::setCoreClockSource(const ClockSource coreClockSource)
{
    // Set clock source
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Processor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32H7xx::driverReset()
{
    ProcessorSTM32H7xx::reset();

    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32H7xx::driverConfigure(
                                                const Processor::Config& config)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32H7xx::driverSetPowerMode(
                                           const Processor::PowerMode powerMode)
{
    switch (powerMode)
    {
        case Processor::POWER_MODE_SLEEP:
        {
            __WFE();
            
            break;
        }
        default:
        {
            break;
        }
    }
    
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
std::uint32_t ProcessorSTM32H7xx::driverGetCoreClockFrequencyHz()
{
    return (myConfig.coreClockFrequencyHz);
}

// //------------------------------------------------------------------------------
// Processor::Error ProcessorSTM32H7xx::driverJumpToAddress(const intptr_t address)
// {
//     // if (address_is_in_range...)
//     {
//         std::uint32_t jumpAddress = *(__IO std::uint32_t*) (address + 4);
//         void (*jumpToApplication)()  = (void (*)()) jumpAddress;

//         // Initialize user application's Stack Pointer
//         __set_MSP(* ( __IO std::uint32_t* ) address);

//         jumpToApplication();
//     }

//     return Processor::Error(Processor::ERROR_CODE_NONE);
// }

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void SystemInit(void)
{
    ProcessorSTM32H7xx::reset();
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void NMI_Handler(void)
{
}

//------------------------------------------------------------------------------
extern "C" void HardFault_Handler(void)
{
	// TODO: Define additional behavior
	// NVIC_SystemReset();

    while (true)
    {
        // Do nothing
    }
}

//------------------------------------------------------------------------------
extern "C" void MemManage_Handler(void)
{
	// TODO: Define additional behavior
	// NVIC_SystemReset();
}

//------------------------------------------------------------------------------
extern "C" void BusFault_Handler(void)
{
	// TODO: Define additional behavior
	// NVIC_SystemReset();
}

//------------------------------------------------------------------------------
extern "C" void UsageFault_Handler(void)
{
	// TODO: Define additional behavior
	// NVIC_SystemReset();
}

//------------------------------------------------------------------------------
extern "C" void DebugMon_Handler(void)
{
}
