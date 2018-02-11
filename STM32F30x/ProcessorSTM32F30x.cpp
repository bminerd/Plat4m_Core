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
/// @file ProcessorSTM32F30x.cpp
/// @author Ben Minerd
/// @date 2/3/2016
/// @brief ProcessorSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f30x.h>
#include <stm32f30x_syscfg.h>
#include <stm32f30x_rcc.h>

#include <Plat4m_Core/STM32F30x/ProcessorSTM32F30x.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/Application.h>

using Plat4m::ProcessorSTM32F30x;
using Plat4m::Processor;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ProcessorSTM32F30x::Prescaler
                                 ProcessorSTM32F30x::myPllClockMultiplierMap[] =
{
    {2,  RCC_CFGR_PLLMULL2},  /// PLL_CLOCK_MULTIPLIER_2
    {3,  RCC_CFGR_PLLMULL3},  /// PLL_CLOCK_MULTIPLIER_3
    {4,  RCC_CFGR_PLLMULL4},  /// PLL_CLOCK_MULTIPLIER_4
    {5,  RCC_CFGR_PLLMULL5},  /// PLL_CLOCK_MULTIPLIER_5
    {6,  RCC_CFGR_PLLMULL6},  /// PLL_CLOCK_MULTIPLIER_6
    {7,  RCC_CFGR_PLLMULL7},  /// PLL_CLOCK_MULTIPLIER_7
    {8,  RCC_CFGR_PLLMULL8},  /// PLL_CLOCK_MULTIPLIER_8
    {9,  RCC_CFGR_PLLMULL9},  /// PLL_CLOCK_MULTIPLIER_9
    {10, RCC_CFGR_PLLMULL10}, /// PLL_CLOCK_MULTIPLIER_10
    {11, RCC_CFGR_PLLMULL11}, /// PLL_CLOCK_MULTIPLIER_11
    {12, RCC_CFGR_PLLMULL12}, /// PLL_CLOCK_MULTIPLIER_12
    {13, RCC_CFGR_PLLMULL13}, /// PLL_CLOCK_MULTIPLIER_13
    {14, RCC_CFGR_PLLMULL14}, /// PLL_CLOCK_MULTIPLIER_14
    {15, RCC_CFGR_PLLMULL15}, /// PLL_CLOCK_MULTIPLIER_15
    {16, RCC_CFGR_PLLMULL16}  /// PLL_CLOCK_MULTIPLIER_16
};

///
/// NOTE: This strange numbering comes from the fact that modifying the PLL
/// clock divider LSB also changes the HSE/2 bit
/// (see pg. 162 of datasheet)
///
const ProcessorSTM32F30x::Prescaler ProcessorSTM32F30x::myPllClockDividerMap[] =
{
    {1,  RCC_CFGR2_PREDIV1_DIV1},  /// PLL_CLOCK_DIVIDER_1
    {3,  RCC_CFGR2_PREDIV1_DIV3},  /// PLL_CLOCK_DIVIDER_3
    {4,  RCC_CFGR2_PREDIV1_DIV2},  /// PLL_CLOCK_DIVIDER_4
    {5,  RCC_CFGR2_PREDIV1_DIV5},  /// PLL_CLOCK_DIVIDER_5
    {7,  RCC_CFGR2_PREDIV1_DIV7},  /// PLL_CLOCK_DIVIDER_7
    {8,  RCC_CFGR2_PREDIV1_DIV4},  /// PLL_CLOCK_DIVIDER_8
    {9,  RCC_CFGR2_PREDIV1_DIV9},  /// PLL_CLOCK_DIVIDER_9
    {11, RCC_CFGR2_PREDIV1_DIV11}, /// PLL_CLOCK_DIVIDER_11
    {12, RCC_CFGR2_PREDIV1_DIV6},  /// PLL_CLOCK_DIVIDER_12
    {13, RCC_CFGR2_PREDIV1_DIV13}, /// PLL_CLOCK_DIVIDER_13
    {15, RCC_CFGR2_PREDIV1_DIV15}, /// PLL_CLOCK_DIVIDER_15
    {16, RCC_CFGR2_PREDIV1_DIV8},  /// PLL_CLOCK_DIVIDER_16
    {20, RCC_CFGR2_PREDIV1_DIV10}, /// PLL_CLOCK_DIVIDER_20
    {24, RCC_CFGR2_PREDIV1_DIV12}, /// PLL_CLOCK_DIVIDER_24
    {28, RCC_CFGR2_PREDIV1_DIV14}, /// PLL_CLOCK_DIVIDER_28
    {32, RCC_CFGR2_PREDIV1_DIV16}  /// PLL_CLOCK_DIVIDER_32
};

const ProcessorSTM32F30x::Prescaler
                                  ProcessorSTM32F30x::myAhbClockPrescalerMap[] =
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

const ProcessorSTM32F30x::Prescaler
                                 ProcessorSTM32F30x::myApb1ClockPrescalerMap[] =
{
    {1,  RCC_CFGR_PPRE1_DIV1}, /// APB1_PRESCALER_1
    {2,  RCC_CFGR_PPRE1_DIV2}, /// APB1_PRESCALER_2
    {4,  RCC_CFGR_PPRE1_DIV4}, /// APB1_PRESCALER_4
    {8,  RCC_CFGR_PPRE1_DIV8}, /// APB1_PRESCALER_8
    {16, RCC_CFGR_PPRE1_DIV16} /// APB1_PRESCALER_16
};

const ProcessorSTM32F30x::Prescaler
                                 ProcessorSTM32F30x::myApb2ClockPrescalerMap[] =
{
    {1,  RCC_CFGR_PPRE2_DIV1}, /// APB2_PRESCALER_1
    {2,  RCC_CFGR_PPRE2_DIV2}, /// APB2_PRESCALER_2
    {4,  RCC_CFGR_PPRE2_DIV4}, /// APB2_PRESCALER_4
    {8,  RCC_CFGR_PPRE2_DIV8}, /// APB2_PRESCALER_8
    {16, RCC_CFGR_PPRE2_DIV16} /// APB2_PRESCALER_16
};

const uint32_t ProcessorSTM32F30x::myFlashWaitStatesMap[] =
{
    0,                   /// FLASH_WAIT_STATES_0
    FLASH_ACR_LATENCY_0, /// FLASH_WAIT_STATES_1
    FLASH_ACR_LATENCY_1  /// FLASH_WAIT_STATES_2
};

const uint32_t ProcessorSTM32F30x::myFlashWaitStatesFrequencyMap[] =
{
    24000000, /// FLASH_WAIT_STATES_0
    48000000, /// FLASH_WAIT_STATES_1
    72000000  /// FLASH_WAIT_STATES_2
};

const uint32_t ProcessorSTM32F30x::myRccClockEnableMap[] =
{
    // AHB peripherals
    RCC_AHBENR_DMA1EN,    /// PERIPHERAL_DMA_1
    RCC_AHBENR_DMA2EN,    /// PERIPHERAL_DMA_2
    RCC_AHBENR_SRAMEN,    /// PERIPHERAL_SRAM
    RCC_AHBENR_FLITFEN,   /// PERIPHERAL_FLITF
    RCC_AHBENR_FMCEN,     /// PERIPHERAL_FMC
    RCC_AHBENR_CRCEN,     /// PERIPHERAL_CRC
    RCC_AHBENR_GPIOHEN,   /// PERIPHERAL_GPIO_H
    RCC_AHBENR_GPIOAEN,   /// PERIPHERAL_GPIO_A
    RCC_AHBENR_GPIOBEN,   /// PERIPHERAL_GPIO_B
    RCC_AHBENR_GPIOCEN,   /// PERIPHERAL_GPIO_C
    RCC_AHBENR_GPIODEN,   /// PERIPHERAL_GPIO_D
    RCC_AHBENR_GPIOEEN,   /// PERIPHERAL_GPIO_E
    RCC_AHBENR_GPIOFEN,   /// PERIPHERAL_GPIO_F
    RCC_AHBENR_GPIOGEN,   /// PERIPHERAL_GPIO_G
    RCC_AHBENR_TSEN,      /// PERIPHERAL_TS
    RCC_AHBENR_ADC12EN,   /// PERIPHERAL_ADC_1_2
    RCC_AHBENR_ADC34EN,   /// PERIPHERAL_ADC_3_4
    // APB2 peripherals
    RCC_APB2ENR_SYSCFGEN, /// PERIPHERAL_SYSCFG
    RCC_APB2ENR_TIM1EN,   /// PERIPHERAL_TIM_1
    RCC_APB2ENR_SPI1EN,   /// PERIPHERAL_SPI_1
    RCC_APB2ENR_TIM8EN,   /// PERIPHERAL_TIM_8
    RCC_APB2ENR_USART1EN, /// PERIPHERAL_USART_1
    RCC_APB2ENR_SPI4EN,   /// PERIPHERAL_SPI_4
    RCC_APB2ENR_TIM15EN,  /// PERIPHERAL_TIM_15
    RCC_APB2ENR_TIM16EN,  /// PERIPHERAL_TIM_16
    RCC_APB2ENR_TIM17EN,  /// PERIPHERAL_TIM_17
    RCC_APB2ENR_TIM20EN,  /// PERIPHERAL_TIM_20
    RCC_APB2ENR_HRTIM1,   /// PERIPHERAL_HRTIM_1
    // APB1 peripherals
    RCC_APB1ENR_TIM2EN,   /// PERIPHERAL_TIM_2
    RCC_APB1ENR_TIM3EN,   /// PERIPHERAL_TIM_3
    RCC_APB1ENR_TIM4EN,   /// PERIPHERAL_TIM_4
    RCC_APB1ENR_TIM6EN,   /// PERIPHERAL_TIM_6
    RCC_APB1ENR_TIM7EN,   /// PERIPHERAL_TIM_7
    RCC_APB1ENR_WWDGEN,   /// PERIPHERAL_WWDG
    RCC_APB1ENR_SPI2EN,   /// PERIPHERAL_SPI_2
    RCC_APB1ENR_SPI3EN,   /// PERIPHERAL_SPI_3
    RCC_APB1ENR_USART2EN, /// PERIPHERAL_USART_2
    RCC_APB1ENR_USART3EN, /// PERIPHERAL_USART_3
    RCC_APB1ENR_UART4EN,  /// PERIPHERAL_UART_4
    RCC_APB1ENR_UART5EN,  /// PERIPHERAL_UART_5
    RCC_APB1ENR_I2C1EN,   /// PERIPHERAL_I2C_1
    RCC_APB1ENR_I2C2EN,   /// PERIPHERAL_I2C_2
    RCC_APB1ENR_USBEN,    /// PERIPHERAL_USB
    RCC_APB1ENR_CAN1EN,   /// PERIPHERAL_CAN_1
    RCC_APB1ENR_DAC2EN,   /// PERIPHERAL_DAC_2
    RCC_APB1ENR_PWREN,    /// PERIPHERAL_PWR
    RCC_APB1ENR_DAC1EN,   /// PERIPHERAL_DAC_1
    RCC_APB1ENR_I2C3EN    /// PERIPHERAL_I2C_3
};

// Variables

ProcessorSTM32F30x::Config ProcessorSTM32F30x::myConfig;

ProcessorSTM32F30x::PllClockMultiplier ProcessorSTM32F30x::myPllClockMultiplier;

ProcessorSTM32F30x::PllClockDivider ProcessorSTM32F30x::myPllClockDivider;

ProcessorSTM32F30x::AhbClockPrescaler ProcessorSTM32F30x::myAhbClockPrescaler;

ProcessorSTM32F30x::Apb1ClockPrescaler ProcessorSTM32F30x::myApb1ClockPrescaler;

ProcessorSTM32F30x::Apb2ClockPrescaler ProcessorSTM32F30x::myApb2ClockPrescaler;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F30x::ProcessorSTM32F30x(
                                      const float coreVoltage,
                                      const uint32_t externalClockFrequencyHz) :
    Processor(coreVoltage, externalClockFrequencyHz)
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F30x::Config ProcessorSTM32F30x::getDefaultConfig()
{
    Config config;
    config.clockSource = ProcessorSTM32F30x::CLOCK_SOURCE_EXTERNAL;
    config.coreClockFrequencyHz = 72000000; // 72 MHz
    config.ahbClockFrequencyHz = 72000000; 	// 72 MHz
    config.apb1ClockFrequencyHz = 36000000; // 36 MHz
    config.apb2ClockFrequencyHz = 72000000; // 72 MHz
    config.isUsbOtgFs = false;
    config.isSdio = false;
    config.isRng = false;
    config.isEthernet = false;
    config.vectorTableAddress = FLASH_BASE;

    return config;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F30x::getCoreClockFrequencyHz()
{
    return myConfig.coreClockFrequencyHz;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F30x::getAhbClockFrequencyHz()
{
    return myConfig.ahbClockFrequencyHz;
}

//------------------------------------------------------------------------------
ProcessorSTM32F30x::AhbClockPrescaler ProcessorSTM32F30x::getAhbClockPrescaler()
{
    return myAhbClockPrescaler;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F30x::getApb1ClockFrequencyHz()
{
    return myConfig.apb1ClockFrequencyHz;
}

//------------------------------------------------------------------------------
ProcessorSTM32F30x::Apb1ClockPrescaler
                                     ProcessorSTM32F30x::getApb1ClockPrescaler()
{
    return myApb1ClockPrescaler;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F30x::getApb2ClockFrequencyHz()
{
    return myConfig.apb2ClockFrequencyHz;
}

//------------------------------------------------------------------------------
ProcessorSTM32F30x::Apb2ClockPrescaler
                                     ProcessorSTM32F30x::getApb2ClockPrescaler()
{
    return myApb2ClockPrescaler;
}

//------------------------------------------------------------------------------
void ProcessorSTM32F30x::setPeripheralClockEnabled(const Peripheral peripheral,
                                                   const bool enabled)
{
    // AHB peripheral
    if (peripheral <= PERIPHERAL_ADC_3_4)
    {
        setBitsSet(RCC->AHBENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB2 peripheral
    else if (peripheral <= PERIPHERAL_HRTIM_1)
    {
        setBitsSet(RCC->APB2ENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB1 peripheral
    else
    {
        setBitsSet(RCC->APB1ENR, myRccClockEnableMap[peripheral], enabled);
    }
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F30x::getPeripheralInputClockFrequencyHz(
                                                    const Peripheral peripheral)
{
    uint32_t inputClockFrequencyHz;

    // AHB peripheral
    if (peripheral <= PERIPHERAL_ADC_3_4)
    {
        inputClockFrequencyHz = getAhbClockFrequencyHz();
    }
    // APB2 peripheral
    else if (peripheral <= PERIPHERAL_HRTIM_1)
    {
        inputClockFrequencyHz = getApb2ClockFrequencyHz();
    }
    // APB1 peripheral
    else
    {
        inputClockFrequencyHz = getApb1ClockFrequencyHz();
    }

    switch (peripheral)
    {
        // APB2 peripherals
        case PERIPHERAL_TIM_1:  // Fall through
        case PERIPHERAL_TIM_8:  // Fall through
        case PERIPHERAL_TIM_15: // Fall through
        case PERIPHERAL_TIM_16: // Fall through
        case PERIPHERAL_TIM_17: // Fall through
        case PERIPHERAL_TIM_20: // Fall through
        {
            if (getApb2ClockPrescaler() != APB2_CLOCK_PRESCALER_1)
            {
                inputClockFrequencyHz *= 2;
            }

            break;
        }
        // APB1 peripherals
        case PERIPHERAL_TIM_2: // Fall through
        case PERIPHERAL_TIM_3: // Fall through
        case PERIPHERAL_TIM_4: // Fall through
        case PERIPHERAL_TIM_6: // Fall through
        case PERIPHERAL_TIM_7: // Fall through
        {
            if (getApb1ClockPrescaler() != APB1_CLOCK_PRESCALER_1)
            {
                inputClockFrequencyHz *= 2;
            }

            break;
        }
        default:
        {
            break;
        }
    }

    return inputClockFrequencyHz;
}

//------------------------------------------------------------------------------
// Public virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F30x::Error ProcessorSTM32F30x::setConfig(const Config& config)
{
    if (config.coreClockFrequencyHz > 72000000)
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }
    
    // Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE bits
    // 0x007F0000 = ~0xFF80FFFF
    clearBits(RCC->CFGR, (uint32_t) 0x007F0000);

    // Reset PREDIV1[3:0] bits
    // 0x0000000F = ~0xFFFFFFF0
    clearBits(RCC->CFGR2, (uint32_t) 0x0000000F);

    // Reset USARTSW[1:0], I2CSW and TIMs bits
    // 0x00FF0333 = ~0xFF00FCCC
    clearBits(RCC->CFGR3, (uint32_t) 0x00FF0333);
    
    uint32_t i;
    uint32_t j;
    uint32_t size;
    uint32_t size2;

    i = 0;
    j = 0;
    size  = ARRAY_SIZE(myPllClockDividerMap);
    size2 = ARRAY_SIZE(myPllClockMultiplierMap);

    while (i < size)
    {
        if ((getClockSourceFrequencyHz()    *
             myPllClockMultiplierMap[j].value /
             myPllClockDividerMap[i].value)     == config.coreClockFrequencyHz)
        {
            break;
        }

        if (j < size2)
        {
            j++;
        }
        else
        {
            i++;
            j++;
        }
    }

    // Check to see if a divider/multiplier combination was found
    if ((i == size) && (j == size2))
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }

    myPllClockDivider = (PllClockDivider) i;
    myPllClockMultiplier = (PllClockMultiplier) j;

    setBits(RCC->CFGR, myPllClockMultiplierMap[myPllClockMultiplier].bits);
    setBits(RCC->CFGR2, myPllClockDividerMap[myPllClockDivider].bits);

    ///
    /// HCLK/AHB = SYSCLK / prescaler
    /// - prescaler = [1, 2, 4, 8, 16, 64, 128, 256, 512]
    /// - AHB <= 72 MHz
    ///

    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
    size = ARRAY_SIZE(myAhbClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.coreClockFrequencyHz /
             myAhbClockPrescalerMap[i].value) == config.ahbClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if a prescaler was found
    if (i == size)
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }

    // If using Ethernet, check AHB clock >= 25 MHz
    if (config.isEthernet && (config.ahbClockFrequencyHz < 25000000))
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }

    myAhbClockPrescaler = (AhbClockPrescaler) i;
    setBits(RCC->CFGR, myAhbClockPrescalerMap[myAhbClockPrescaler].bits);

    ///
    /// PCLK1/APB1 = HCLK/AHB / prescaler
    /// - prescaler = [1, 2, 4, 8, 16]
    /// - APB1 <= 36 MHz (datasheet pg. 135)
    ///

    // Loop through the prescaler values to see if any give the desired APB1
    // clock speed
    size = ARRAY_SIZE(myApb1ClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.ahbClockFrequencyHz / myApb1ClockPrescalerMap[i].value) ==
            config.apb1ClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if a prescaler was found
    if (i == size)
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }

    myApb1ClockPrescaler = (Apb1ClockPrescaler) i;
    setBits(RCC->CFGR, myApb1ClockPrescalerMap[myApb1ClockPrescaler].bits);

    ///
    /// PCLK2/APB2 = HCLK/AHB / 2
    /// - prescaler = [1, 2, 4, 8, 16]
    /// - APB2 <= 72 MHz (datasheet pg. 135)
    ///

    // Loop through the prescaler values to see if any give the desired APB2
    // clock speed
    size = ARRAY_SIZE(myApb2ClockPrescalerMap);

    for (i = 0; i < ARRAY_SIZE(myApb2ClockPrescalerMap); i++)
    {
        if ((config.ahbClockFrequencyHz /
             myApb2ClockPrescalerMap[i].value) == config.apb2ClockFrequencyHz)
        {
            break;
        }
    }

    // Check to see if a prescaler was found
    if (i == size)
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }

    myApb2ClockPrescaler = (Apb2ClockPrescaler) i;
    setBits(RCC->CFGR, myApb2ClockPrescalerMap[myApb2ClockPrescaler].bits);

    if (config.clockSource == CLOCK_SOURCE_EXTERNAL)
    {
        uint32_t hseStatus = 0;
        uint32_t startUpCounter = 500;

        // Enable HSE
        setBits(RCC->CR, RCC_CR_HSEON);

        while (!hseStatus && startUpCounter--)
        {
            hseStatus = (RCC->CR & RCC_CR_HSERDY);
        }

        if (!startUpCounter)
        {
            return ERROR_CLOCK_STARTUP_FAILED;
        }

        setBits(RCC->CFGR, RCC_CFGR_PLLSRC_PREDIV1);
    }

    // Enable PLL
    setBits(RCC->CR, RCC_CR_PLLON);
    
    // Wait till PLL is ready
    while (areBitsClear(RCC->CR, RCC_CR_PLLRDY))
    {
    }
    
    // Enable Prefetch Buffer
    setBits(FLASH->ACR, FLASH_ACR_PRFTBE);

    size = ARRAY_SIZE(myFlashWaitStatesFrequencyMap);

    for (i = 0; i < size; i++)
    {
        if (config.coreClockFrequencyHz <= myFlashWaitStatesFrequencyMap[i])
        {
            break;
        }
    }

    if (i == size)
    {
        return ERROR_CLOCK_FREQUENCY_INVALID;
    }

    FlashWaitStates flashWaitStates = (FlashWaitStates) i;

    setBits(FLASH->ACR, (uint32_t) flashWaitStates);

    // Select PLL as system clock source
    clearBits(RCC->CFGR, RCC_CFGR_SW);
    setBits(RCC->CFGR, RCC_CFGR_SW_PLL);

    // Wait till PLL is used as system clock source
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }

    SCB->VTOR = (config.vectorTableAddress & (uint32_t)0x1FFFFF80);

    NVIC_SetPriorityGrouping(3);

    myConfig = config;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
ProcessorSTM32F30x::ClockSource ProcessorSTM32F30x::getCoreClockSource()
{
    return (myConfig.clockSource);
}

//------------------------------------------------------------------------------
void ProcessorSTM32F30x::setCoreClockSource(const ClockSource coreClockSource)
{
    // Set clock source
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Processor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F30x::driverReset()
{
    // Start-up routine based on ST Standard Peripheral Library
    // (system_stm32f30x.c)

    // FPU settings
    if (__FPU_USED == 1)
    {
        // set CP10 and CP11 Full Access
        setBits(SCB->CPACR, ((3UL << 10*2)|(3UL << 11*2)));
    }

    // Reset the RCC clock configuration to the default reset state
    // Set HSION bit */
    setBits(RCC->CR, (uint32_t) 0x00000001);

    // Reset CFGR register
    // 0x07803FF3 = ~0xF878FC00C
    clearBits(RCC->CFGR, (uint32_t) 0x07803FF3);

    // Reset HSEON, CSSON and PLLON bits
    // 0x01090000 = ~0xFEF6FFFF
    clearBits(RCC->CR, (uint32_t) 0x01090000);

    // Reset HSEBYP bit
    // 0x00040000 = ~0xFFFBFFFF
    clearBits(RCC->CR, (uint32_t) 0x00040000);

    // Disable all clock interrupts
    RCC->CIR = 0x00000000;
    
    // Enable SYSCFG clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    setBits(RCC->APB2RSTR, 0xFFFFFFFF);
    setBits(RCC->APB1RSTR, 0xFFFFFFFF);
    setBits(RCC->AHBRSTR,  0xFFFFFFFF);

    clearBits(RCC->APB2RSTR, 0xFFFFFFFF);
    clearBits(RCC->APB1RSTR, 0xFFFFFFFF);
    clearBits(RCC->AHBRSTR,  0xFFFFFFFF);

    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F30x::driverConfigure(const Processor::Config& config)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F30x::driverSetPowerMode(
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
uint32_t ProcessorSTM32F30x::driverGetCoreClockFrequencyHz()
{
    return (myConfig.coreClockFrequencyHz);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F30x::driverJumpToAddress(const intptr_t address)
{
    // if (address_is_in_range...)
    {
        uint32_t jumpAddress = *(__IO uint32_t*) (address + 4);
        void (*jumpToApplication)()  = (void (*)()) jumpAddress;

        // Initialize user application's Stack Pointer
        __set_MSP(* ( __IO uint32_t* ) address);

        jumpToApplication();
    }

    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void SystemInit(void)
{
    // Do nothing

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
    RCC->CIR = 0x00000000;

    // Enable SYSCFG clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    Plat4m::setBits(RCC->APB2RSTR, 0xFFFFFFFF);
    Plat4m::setBits(RCC->APB1RSTR, 0xFFFFFFFF);
    Plat4m::setBits(RCC->AHBRSTR,  0xFFFFFFFF);

    Plat4m::clearBits(RCC->APB2RSTR, 0xFFFFFFFF);
    Plat4m::clearBits(RCC->APB1RSTR, 0xFFFFFFFF);
    Plat4m::clearBits(RCC->AHBRSTR,  0xFFFFFFFF);
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
	NVIC_SystemReset();
}

//------------------------------------------------------------------------------
extern "C" void MemManage_Handler(void)
{
	// TODO: Define additional behavior
	NVIC_SystemReset();
}

//------------------------------------------------------------------------------
extern "C" void BusFault_Handler(void)
{
	// TODO: Define additional behavior
	NVIC_SystemReset();
}

//------------------------------------------------------------------------------
extern "C" void UsageFault_Handler(void)
{
	// TODO: Define additional behavior
	NVIC_SystemReset();
}

//------------------------------------------------------------------------------
extern "C" void DebugMon_Handler(void)
{
}
