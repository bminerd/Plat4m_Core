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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file ProcessorSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 5/31/2013
/// @brief ProcessorSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f4xx.h>

#include <Plat4m_Core/STM32F4xx/ProcessorSTM32F4xx.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/Application.h>

using Plat4m::ProcessorSTM32F4xx;
using Plat4m::Processor;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ProcessorSTM32F4xx::Prescaler
                                  ProcessorSTM32F4xx::myAhbClockPrescalerMap[] =
{
    {1,   RCC_CFGR_HPRE_DIV1},   /// AHB_PRESCALER_1
    {2,   RCC_CFGR_HPRE_DIV2},   /// AHB_PRESCALER_2
    {4,   RCC_CFGR_HPRE_DIV4},   /// AHB_PRESCALER_4
    {8,   RCC_CFGR_HPRE_DIV8},   /// AHB_PRESCALER_8
    {16,  RCC_CFGR_HPRE_DIV16},  /// AHB_PRESCALER_16
    {64,  RCC_CFGR_HPRE_DIV64},  /// AHB_PRESCALER_64
    {128, RCC_CFGR_HPRE_DIV128}, /// AHB_PRESCALER_128
    {256, RCC_CFGR_HPRE_DIV256}, /// AHB_PRESCALER_256
    {512, RCC_CFGR_HPRE_DIV512}  /// AHB_PRESCALER_512
};

const ProcessorSTM32F4xx::Prescaler
                                 ProcessorSTM32F4xx::myApb1ClockPrescalerMap[] =
{
    {1,  RCC_CFGR_PPRE1_DIV1}, /// APB1_PRESCALER_1
    {2,  RCC_CFGR_PPRE1_DIV2}, /// APB1_PRESCALER_2
    {4,  RCC_CFGR_PPRE1_DIV4}, /// APB1_PRESCALER_4
    {8,  RCC_CFGR_PPRE1_DIV8}, /// APB1_PRESCALER_8
    {16, RCC_CFGR_PPRE1_DIV16} /// APB1_PRESCALER_16
};

const ProcessorSTM32F4xx::Prescaler
                                 ProcessorSTM32F4xx::myApb2ClockPrescalerMap[] =
{
    {1,  RCC_CFGR_PPRE2_DIV1}, /// APB2_PRESCALER_1
    {2,  RCC_CFGR_PPRE2_DIV2}, /// APB2_PRESCALER_2
    {4,  RCC_CFGR_PPRE2_DIV4}, /// APB2_PRESCALER_4
    {8,  RCC_CFGR_PPRE2_DIV8}, /// APB2_PRESCALER_8
    {16, RCC_CFGR_PPRE2_DIV16} /// APB2_PRESCALER_16
};

const float ProcessorSTM32F4xx::myCoreVoltageMap[] =
{
    1.8,
    2.1,
    2.4,
    2.7,
    3.6
};

const uint32_t ProcessorSTM32F4xx::myFlashWaitStatesMap[] =
{
    FLASH_ACR_LATENCY_0WS, /// FLASH_WAIT_STATES_0
    FLASH_ACR_LATENCY_1WS, /// FLASH_WAIT_STATES_1
    FLASH_ACR_LATENCY_2WS, /// FLASH_WAIT_STATES_2
    FLASH_ACR_LATENCY_3WS, /// FLASH_WAIT_STATES_3
    FLASH_ACR_LATENCY_4WS, /// FLASH_WAIT_STATES_4
    FLASH_ACR_LATENCY_5WS, /// FLASH_WAIT_STATES_5
    FLASH_ACR_LATENCY_6WS, /// FLASH_WAIT_STATES_6
    FLASH_ACR_LATENCY_7WS, /// FLASH_WAIT_STATES_7
};

const uint32_t ProcessorSTM32F4xx::myFlashWaitStatesFrequencyMap[][9] =
{
    /// INPUT_VOLTAGE_V1R8_TO_V2R1
    {
        20000000,  /// FLASH_WAIT_STATES_0
        40000000,  /// FLASH_WAIT_STATES_1
        60000000,  /// FLASH_WAIT_STATES_2
        80000000,  /// FLASH_WAIT_STATES_3
        100000000, /// FLASH_WAIT_STATES_4
        120000000, /// FLASH_WAIT_STATES_5
        140000000, /// FLASH_WAIT_STATES_6
        160000000, /// FLASH_WAIT_STATES_7
        168000000  /// FLASH_WAIT_STATES_8
    },
    /// INPUT_VOLTAGE_V2R1_TO_V2R4
    {
        22000000,  /// FLASH_WAIT_STATES_0
        44000000,  /// FLASH_WAIT_STATES_1
        66000000,  /// FLASH_WAIT_STATES_2
        88000000,  /// FLASH_WAIT_STATES_3
        110000000, /// FLASH_WAIT_STATES_4
        132000000, /// FLASH_WAIT_STATES_5
        154000000, /// FLASH_WAIT_STATES_6
        168000000, /// FLASH_WAIT_STATES_7
        180000000  /// FLASH_WAIT_STATES_8
    },
    /// INPUT_VOLTAGE_V2R4_TO_V2R7
    {
        24000000,  /// FLASH_WAIT_STATES_0
        48000000,  /// FLASH_WAIT_STATES_1
        72000000,  /// FLASH_WAIT_STATES_2
        96000000,  /// FLASH_WAIT_STATES_3
        120000000, /// FLASH_WAIT_STATES_4
        144000000, /// FLASH_WAIT_STATES_5
        168000000, /// FLASH_WAIT_STATES_6
        180000000, /// FLASH_WAIT_STATES_7
        0          /// FLASH_WAIT_STATES_8
    },
    /// INPUT_VOLTAGE_V2R7_TO_V3R6
    {
        30000000,  /// FLASH_WAIT_STATES_0
        60000000,  /// FLASH_WAIT_STATES_1
        90000000,  /// FLASH_WAIT_STATES_2
        120000000, /// FLASH_WAIT_STATES_3
        150000000, /// FLASH_WAIT_STATES_4
        180000000, /// FLASH_WAIT_STATES_5
        0,         /// FLASH_WAIT_STATES_6
        0,         /// FLASH_WAIT_STATES_7
        0          /// FLASH_WAIT_STATES_8
    }
};

ProcessorSTM32F4xx::Config ProcessorSTM32F4xx::myConfig;

ProcessorSTM32F4xx::AhbClockPrescaler ProcessorSTM32F4xx::myAhbClockPrescaler;

ProcessorSTM32F4xx::Apb1ClockPrescaler ProcessorSTM32F4xx::myApb1ClockPrescaler;

ProcessorSTM32F4xx::Apb2ClockPrescaler ProcessorSTM32F4xx::myApb2ClockPrescaler;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F4xx::ProcessorSTM32F4xx(
                                      const float coreVoltage,
                                      const uint32_t externalClockFrequencyHz) :
    Processor(ENDIAN_BIG, coreVoltage, externalClockFrequencyHz)
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F4xx::getCoreClockFrequencyHz()
{
    return myConfig.coreClockFrequencyHz;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F4xx::getAhbClockFrequencyHz()
{
    return myConfig.ahbClockFrequencyHz;
}

//------------------------------------------------------------------------------
ProcessorSTM32F4xx::AhbClockPrescaler ProcessorSTM32F4xx::getAhbClockPrescaler()
{
    return myAhbClockPrescaler;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F4xx::getApb1ClockFrequencyHz()
{
    return myConfig.apb1ClockFrequencyHz;
}

//------------------------------------------------------------------------------
ProcessorSTM32F4xx::Apb1ClockPrescaler
                                     ProcessorSTM32F4xx::getApb1ClockPrescaler()
{
    return myApb1ClockPrescaler;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F4xx::getApb2ClockFrequencyHz()
{
    return myConfig.apb2ClockFrequencyHz;
}

//------------------------------------------------------------------------------
ProcessorSTM32F4xx::Apb2ClockPrescaler
                                     ProcessorSTM32F4xx::getApb2ClockPrescaler()
{
    return myApb2ClockPrescaler;
}

//------------------------------------------------------------------------------
void ProcessorSTM32F4xx::setPeripheralClockEnabled(const Peripheral peripheral,
                                                   const bool enabled)
{
    // Note: Register and bit positions come from STM32F446xx reference manual
    // pages 143-150

    // AHB1 peripheral
    if (peripheral <= PERIPHERAL_GPIO_H)
    {
        setBitsSet(RCC->AHB1ENR, ((uint32_t) 1) << peripheral, enabled);
    }
    else if (peripheral == PERIPHERAL_CRC)
    {
        setBitsSet(RCC->AHB1ENR, ((uint32_t) 1) << 12, enabled);
    }
    else if (peripheral == PERIPHERAL_BACKUP_SRAM)
    {
        setBitsSet(RCC->AHB1ENR, ((uint32_t) 1) << 18, enabled);
    }
    else if (peripheral == PERIPHERAL_DMA_1)
    {
        setBitsSet(RCC->AHB1ENR, ((uint32_t) 1) << 21, enabled);
    }
    else if (peripheral == PERIPHERAL_DMA_2)
    {
        setBitsSet(RCC->AHB1ENR, ((uint32_t) 1) << 22, enabled);
    }
    else if (peripheral == PERIPHERAL_USB_OTG_HS)
    {
        setBitsSet(RCC->AHB1ENR, ((uint32_t) 1) << 29, enabled);
    }
    else if (peripheral == PERIPHERAL_USB_OTG_HS_ULPI)
    {
        setBitsSet(RCC->AHB1ENR, ((uint32_t) 1) << 30, enabled);
    }
    // AHB2 peripheral
    else if (peripheral == PERIPHERAL_CAMERA_INTERFACE)
    {
        setBitsSet(RCC->AHB2ENR, (uint32_t) 1, enabled);
    }
    else if (peripheral == PERIPHERAL_USB_OTG_FS)
    {
        setBitsSet(RCC->AHB2ENR, ((uint32_t) 1) << 7, enabled);
    }
    // AHB3 peripheral
    else if (peripheral == PERIPHERAL_FMC)
    {
        setBitsSet(RCC->AHB3ENR, (uint32_t) 1, enabled);
    }
    else if (peripheral == PERIPHERAL_QUAD_SPI)
    {
        setBitsSet(RCC->AHB3ENR, ((uint32_t) 1) << 1, enabled);
    }
    // APB1 peripheral
    else if (peripheral <= PERIPHERAL_TIM_14)
    {
        uint32_t offset = peripheral - PERIPHERAL_TIM_2;

        setBitsSet(RCC->APB1ENR, ((uint32_t) 1) << offset, enabled);
    }
    else if (peripheral == PERIPHERAL_WINDOW_WATCHDOG)
    {
        setBitsSet(RCC->APB1ENR, ((uint32_t) 1) << 11, enabled);
    }
    else if (peripheral <= PERIPHERAL_DAC)
    {
        uint32_t offset = (peripheral - PERIPHERAL_SPI_2) + 14;

        setBitsSet(RCC->APB1ENR, ((uint32_t) 1) << offset, enabled);
    }
    // APB2 peripheral
    else if (peripheral <= PERIPHERAL_TIM_8)
    {
        setBitsSet(RCC->APB2ENR, ((uint32_t) 1) << peripheral, enabled);
    }
    else if (peripheral <= PERIPHERAL_USART_6)
    {
        uint32_t offset = (peripheral - PERIPHERAL_USART_6) + 4;

        setBitsSet(RCC->APB2ENR, ((uint32_t) 1) << offset, enabled);
    }
    else if (peripheral <= PERIPHERAL_SYS_CFG)
    {
        uint32_t offset = (peripheral - PERIPHERAL_ADC_1) + 8;

        setBitsSet(RCC->APB2ENR, ((uint32_t) 1) << offset, enabled);
    }
    else if (peripheral <= PERIPHERAL_TIM_11)
    {
        uint32_t offset = (peripheral - PERIPHERAL_TIM_9) + 16;

        setBitsSet(RCC->APB2ENR, ((uint32_t) 1) << offset, enabled);
    }
    else if (peripheral <= PERIPHERAL_SAI_2)
    {
        uint32_t offset = (peripheral - PERIPHERAL_SAI_1) + 22;

        setBitsSet(RCC->APB2ENR, ((uint32_t) 1) << offset, enabled);
    }
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F4xx::getPeripheralInputClockFrequencyHz(
                                                    const Peripheral peripheral)
{
    uint32_t inputClockFrequencyHz;

    // AHB peripheral
    if (peripheral <= PERIPHERAL_USB_OTG_HS_ULPI)
    {
        inputClockFrequencyHz = getAhbClockFrequencyHz();
    }
    // APB1 peripheral
    else if (peripheral <= PERIPHERAL_DAC)
    {
        inputClockFrequencyHz = getApb1ClockFrequencyHz();

        if ((peripheral <= PERIPHERAL_TIM_14) &&
            (getApb1ClockPrescaler() != APB1_CLOCK_PRESCALER_1))
        {
            inputClockFrequencyHz *= 2;
        }
    }
    // APB2 peripheral
    else
    {
        inputClockFrequencyHz = getApb2ClockFrequencyHz();

        if (((peripheral <= PERIPHERAL_TIM_8)      ||
             ((peripheral >= PERIPHERAL_TIM_9) &&
              (peripheral <= PERIPHERAL_TIM_14)))      &&
            (getApb2ClockPrescaler() != APB2_CLOCK_PRESCALER_1))
        {
            inputClockFrequencyHz *= 2;
        }
    }

    return inputClockFrequencyHz;
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F4xx::Error ProcessorSTM32F4xx::setConfig(const Config& config)
{
//    if (config.coreClockFrequencyHz > 168000000)
//    {
//        return ERROR_CLOCK_FREQUENCY_INVALID;
//    }
    
    // Calculate PLL_M, PLL_N, PLL_P, and PLL_Q factors
    
    ///
    /// PLL_M calculation (datasheet pg. 126)
    ///
    /// VCO input frequency = PLL input clock frequency / PLL_M
    /// - 1 MHz <= VCO input frequency <= 2 MHz*
    ///   *Datasheet recommends PLL_VCO = 2 MHz to reduce PLL jitter, but this
    ///   isn't possible for 168 MHz operation
    /// - 2 <= PLL_M <= 63
    ///
    uint32_t pllM = (getClockSourceFrequencyHz() / 1000000);
    uint32_t vcoInputFrequencyHz = (getClockSourceFrequencyHz() / pllM);
    
    ///
    /// PLL_N calculation (datasheet pg. 126)
    ///
    /// VCO output frequency = VCO input frequency * PLL_N
    /// - 192 MHz <= VCO output frequency <= 432 MHz (datasheet pg. 126)
    /// - 192 <= PLL_N <= 432
    ///
    uint32_t pllN;
    
    ///
    /// PLL_P calculation (datasheet pg. 126)
    ///
    /// SYSCLK = PLL_VCO / PLL_P
    /// - PLL_P = [2, 4, 6, 8]
    ///
    uint32_t vcoOutputFrequencyHz;
    uint32_t pllP;
    
    for (pllP = 2; pllP <= 8; pllP += 2)
    {
        pllN = (config.coreClockFrequencyHz / 1000000) * pllP;
        vcoOutputFrequencyHz = vcoInputFrequencyHz * pllN;
        
        if ((vcoOutputFrequencyHz / pllP) == config.coreClockFrequencyHz)
        {
            break;
        }
    }
    
    // Check to see if a valid PLL_P value was found
    if (pllP > 8)
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }
    
    // PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
    
    ///
    /// PLL_Q calculation (datasheet pg. 125)
    ///
    /// USB OTG FS, SDIO and RNG Clock = (PLL_VCO / PLL_Q)
    /// - USB OTG FS needs 48 MHz clock
    /// - SDIO and RNG need <= 48 MHz clock
    /// - 2 <= PLL_Q <= 15
    ///
    uint32_t pllQ = 2;
    
    if (config.isUsbOtgFs || config.isSdio || config.isEthernet)
    {
        for (pllQ = 2; pllQ <= 15; pllQ++)
        {
            if (config.isUsbOtgFs &&
                ((vcoOutputFrequencyHz / pllQ) == 48000000))
            {
                break;
            }
            else if ((config.isSdio || config.isRng) &&
                     !(config.isUsbOtgFs)            &&
                     ((vcoOutputFrequencyHz / pllQ) <= 48000000))
            {
                break;
            }
        }
        
        if (pllQ > 15)
        {
            return ERROR_CLOCK_FREQUENCY_INVALID;
        }
    }
    
    ///
    /// HCLK/AHB = SYSCLK / prescaler
    /// - prescaler = [1, 2, 4, 8, 16]
    /// - AHB <= 168 MHz
    ///
    
    int i;
    
    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    for (i = 0; i < ARRAY_SIZE(myAhbClockPrescalerMap); i++)
    {
        if ((config.coreClockFrequencyHz / myAhbClockPrescalerMap[i].value) ==
            config.ahbClockFrequencyHz)
        {
            break;
        }
    }
    
    // Check to see if a prescaler was found
    if (i == ARRAY_SIZE(myAhbClockPrescalerMap))
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }
    
    // If using Ethernet, check AHB clock >= 25 MHz
    if (config.isEthernet && (config.ahbClockFrequencyHz < 25000000))
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }
    
    myAhbClockPrescaler = (AhbClockPrescaler) i;
    RCC->CFGR |= myAhbClockPrescalerMap[myAhbClockPrescaler].bits;
    
    ///
    /// PCLK1/APB1 = HCLK/AHB / prescaler
    /// - prescaler = [1, 2, 4, 8, 16]
    /// - APB2 <= 84 MHz (datasheet pg. 128)
    ///
    
    // Loop through the prescaler values to see if any give the desired APB1
    // clock speed
    for (i = 0; i < ARRAY_SIZE(myApb1ClockPrescalerMap); i++)
    {
        if ((config.ahbClockFrequencyHz / myApb1ClockPrescalerMap[i].value) ==
            config.apb1ClockFrequencyHz)
        {
            break;
        }
    }
    
    // Check to see if a prescaler was found
    if (i == ARRAY_SIZE(myApb1ClockPrescalerMap))
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }
    
    myApb1ClockPrescaler = (Apb1ClockPrescaler) i;
    RCC->CFGR |= myApb1ClockPrescalerMap[myApb1ClockPrescaler].bits;
    
    ///
    /// PCLK2/APB2 = HCLK/AHB / 2
    /// - prescaler = [1, 2, 4, 8, 16]
    /// - APB2 <= 84 MHz (datasheet pg. 128)
    ///
    
    // Loop through the prescaler values to see if any give the desired APB2
    // clock speed
    for (i = 0; i < ARRAY_SIZE(myApb2ClockPrescalerMap); i++)
    {
        if ((config.ahbClockFrequencyHz / myApb2ClockPrescalerMap[i].value) ==
            config.apb2ClockFrequencyHz)
        {
            break;
        }
    }
    
    // Check to see if a prescaler was found
    if (i == ARRAY_SIZE(myApb2ClockPrescalerMap))
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }
    
    myApb2ClockPrescaler = (Apb2ClockPrescaler) i;
    RCC->CFGR |= myApb2ClockPrescalerMap[myApb2ClockPrescaler].bits;
    
    // Setup the PLL based on the chosen PLL_M, PLL_N, PLL_P, and PLL_Q values
    RCC->PLLCFGR = pllM                        |
                   (pllN << 6)                 |
                   (((pllP >> 1) - 1) << 16)   |
                   (pllQ << 24)                |
                   (7) << 28;
    
    if (config.clockSource == CLOCK_SOURCE_EXTERNAL)
    {
        uint32_t hseStatus = 0;
        uint32_t startUpCounter = 500;
        
        // Enable HSE
        RCC->CR |=  RCC_CR_HSEON;
        
        while (!hseStatus && startUpCounter--)
        {
            hseStatus = (RCC->CR & RCC_CR_HSERDY);
        }
        
        if (!startUpCounter)
        {
            return ERROR_CLOCK_STARTUP_FAILED;
        }
        
        RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
    }
    
    // Enable the main PLL
    RCC->CR |= RCC_CR_PLLON;
    
    // Wait until the main PLL is ready
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);
    
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN;
    
    // Should this be moved up?
    
    if (getCoreVoltageV() < myCoreVoltageMap[0])
    {
        return ERROR_INPUT_VOLTAGE_INVALID;
    }
    
    for (i = 1; i < ARRAY_SIZE(myCoreVoltageMap); i++)
    {
        if (getCoreVoltageV() <= myCoreVoltageMap[i])
        {
            break;
        }
    }
    
    // Check if valid input voltage range was found
    if (i == ARRAY_SIZE(myCoreVoltageMap))
    {
        return ERROR_INPUT_VOLTAGE_INVALID;
    }
    
    CoreVoltage coreVoltage = (CoreVoltage) (i - 1);
    
    if (coreVoltage > CORE_VOLTAGE_V1R8_TO_V2R1)
    {
        FLASH->ACR |= FLASH_ACR_PRFTEN;
    }
    
    for (i = 0; i < ARRAY_SIZE(myFlashWaitStatesFrequencyMap[0]); i++)
    {
        if (config.coreClockFrequencyHz <=
            myFlashWaitStatesFrequencyMap[coreVoltage][i])
        {
            break;
        }
    }
    
    if (i == ARRAY_SIZE(myFlashWaitStatesFrequencyMap[0]))
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }
    
    FlashWaitStates flashWaitStates = (FlashWaitStates) i;
    
    FLASH->ACR |= (uint32_t) flashWaitStates;
    
    // Clear SW bits
    RCC->CFGR &= (uint32_t) ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    
    // Wait till the main PLL is used as system clock source
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    // Vector Table Relocation in Internal FLASH
    SCB->VTOR = FLASH_BASE;

    // I/O compensation cell
    if (coreVoltage >= CORE_VOLTAGE_V2R4_TO_V2R7)
    {
//        // Enable compensation cell
//        SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD;
//        
//        volatile uint32_t sys = SYSCFG->CMPCR;
//        
//        // Wait until compensation cell is ready
//        while ((SYSCFG->CMPCR & SYSCFG_CMPCR_READY) != SYSCFG_CMPCR_READY)
//        {
//            sys = SYSCFG->CMPCR;
//        }
    }

    NVIC_SetPriorityGrouping(3);
    
    myConfig = config;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Processor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F4xx::driverReset()
{
    // FPU settings
    if (__FPU_USED == 1)
    {
        // set CP10 and CP11 Full Access
        SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    }

    // Reset the RCC clock configuration to the default reset state
    // Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    // Reset CFGR register
    RCC->CFGR = 0x00000000;

    // Reset HSEON, CSSON and PLLON bits
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    // Reset PLLCFGR register
    RCC->PLLCFGR = 0x24003010;

    // Reset HSEBYP bit
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    // Disable all interrupts
    RCC->CIR = 0x00000000;
    
    setPeripheralClockEnabled(PERIPHERAL_SYS_CFG, true);
    
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F4xx::driverConfigure(
                                                const Processor::Config& config)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F4xx::driverSetPowerMode(
                                           const Processor::PowerMode powerMode)
{
    switch (powerMode)
    {
        case Processor::POWER_MODE_SLEEP:
        {
            __WFE();
        }
    }
    
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F4xx::driverGetCoreClockFrequencyHz()
{
    return (myConfig.coreClockFrequencyHz);
}

//------------------------------------------------------------------------------
bool ProcessorSTM32F4xx::driverIsInterruptActive()
{
    return ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0);
    // return (NVIC->IABR != 0);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void SystemInit(void)
{
    // FPU settings
    if (__FPU_USED == 1)
    {
        // set CP10 and CP11 Full Access
        SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    }

    // Reset the RCC clock configuration to the default reset state
    // Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    // Reset CFGR register
    RCC->CFGR = 0x00000000;

    // Reset HSEON, CSSON and PLLON bits
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    // Reset PLLCFGR register
    RCC->PLLCFGR = 0x24003010;

    // Reset HSEBYP bit
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    // Disable all interrupts
    RCC->CIR = 0x00000000;

    ProcessorSTM32F4xx::setPeripheralClockEnabled(
										 ProcessorSTM32F4xx::PERIPHERAL_SYS_CFG,
										 true);

    Plat4m::setBits(RCC->AHB1RSTR, 0xFFFFFFFF);
    Plat4m::setBits(RCC->AHB2RSTR, 0xFFFFFFFF);
    Plat4m::setBits(RCC->AHB3RSTR, 0xFFFFFFFF);
    Plat4m::setBits(RCC->APB2RSTR, 0xFFFFFFFF);
    Plat4m::setBits(RCC->APB1RSTR, 0xFFFFFFFF);

    Plat4m::clearBits(RCC->AHB1RSTR, 0xFFFFFFFF);
    Plat4m::clearBits(RCC->AHB2RSTR, 0xFFFFFFFF);
    Plat4m::clearBits(RCC->AHB3RSTR, 0xFFFFFFFF);
    Plat4m::clearBits(RCC->APB2RSTR, 0xFFFFFFFF);
    Plat4m::clearBits(RCC->APB1RSTR, 0xFFFFFFFF);
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
    // Go to infinite loop when Hard Fault exception occurs
    FOREVER;
}

//------------------------------------------------------------------------------
extern "C" void MemManage_Handler(void)
{
    // Go to infinite loop when Memory Manage exception occurs
    FOREVER;
}

//------------------------------------------------------------------------------
extern "C" void BusFault_Handler(void)
{
    // Go to infinite loop when Bus Fault exception occurs
    FOREVER;
}

//------------------------------------------------------------------------------
extern "C" void UsageFault_Handler(void)
{
    // Go to infinite loop when Usage Fault exception occurs
    FOREVER;
}

//------------------------------------------------------------------------------
extern "C" void DebugMon_Handler(void)
{
}
