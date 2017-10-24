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
/// @file ProcessorSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/24/2015
/// @brief ProcessorSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ProcessorSTM32F10x.h>
#include <System.h>
#include <Application.h>

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>

using Plat4m::ProcessorSTM32F10x;
using Plat4m::Processor;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F10x::Prescaler
                                  ProcessorSTM32F10x::myAhbClockPrescalerMap[] =
{
    {1,   RCC_CFGR_HPRE_DIV1},   /// AHB_PRESCALER_DIVIDE_BY_1
    {2,   RCC_CFGR_HPRE_DIV2},   /// AHB_PRESCALER_DIVIDE_BY_2
    {4,   RCC_CFGR_HPRE_DIV4},   /// AHB_PRESCALER_DIVIDE_BY_4
    {8,   RCC_CFGR_HPRE_DIV8},   /// AHB_PRESCALER_DIVIDE_BY_8
    {16,  RCC_CFGR_HPRE_DIV16},  /// AHB_PRESCALER_DIVIDE_BY_16
    {64,  RCC_CFGR_HPRE_DIV64},  /// AHB_PRESCALER_DIVIDE_BY_64
    {128, RCC_CFGR_HPRE_DIV128}, /// AHB_PRESCALER_DIVIDE_BY_128
    {256, RCC_CFGR_HPRE_DIV256}, /// AHB_PRESCALER_DIVIDE_BY_256
    {512, RCC_CFGR_HPRE_DIV512}  /// AHB_PRESCALER_DIVIDE_BY_512
};

const ProcessorSTM32F10x::Prescaler
                                 ProcessorSTM32F10x::myApb1ClockPrescalerMap[] =
{
    {1,  RCC_CFGR_PPRE1_DIV1}, /// APB1_PRESCALER_DIVIDE_BY_1
    {2,  RCC_CFGR_PPRE1_DIV2}, /// APB1_PRESCALER_DIVIDE_BY_2
    {4,  RCC_CFGR_PPRE1_DIV4}, /// APB1_PRESCALER_DIVIDE_BY_4
    {8,  RCC_CFGR_PPRE1_DIV8}, /// APB1_PRESCALER_DIVIDE_BY_8
    {16, RCC_CFGR_PPRE1_DIV16} /// APB1_PRESCALER_DIVIDE_BY_16
};

const ProcessorSTM32F10x::Prescaler
                                 ProcessorSTM32F10x::myApb2ClockPrescalerMap[] =
{
    {1,  RCC_CFGR_PPRE2_DIV1}, /// APB2_PRESCALER_DIVIDE_BY_1
    {2,  RCC_CFGR_PPRE2_DIV2}, /// APB2_PRESCALER_DIVIDE_BY_2
    {4,  RCC_CFGR_PPRE2_DIV4}, /// APB2_PRESCALER_DIVIDE_BY_4
    {8,  RCC_CFGR_PPRE2_DIV8}, /// APB2_PRESCALER_DIVIDE_BY_8
    {16, RCC_CFGR_PPRE2_DIV16} /// APB2_PRESCALER_DIVIDE_BY_16
};

const uint32_t ProcessorSTM32F10x::myFlashWaitStatesMap[] =
{
	FLASH_ACR_LATENCY_0, /// FLASH_WAIT_STATES_0
	FLASH_ACR_LATENCY_1, /// FLASH_WAIT_STATES_1
	FLASH_ACR_LATENCY_2  /// FLASH_WAIT_STATES_2
};

const uint32_t ProcessorSTM32F10x::myFlashWaitStatesFrequencyMap[] =
{
	24000000, /// FLASH_WAIT_STATES_0
	48000000, /// FLASH_WAIT_STATES_1
	72000000  /// FLASH_WAIT_STATES_2
};

const ProcessorSTM32F10x::Prescaler
                                  ProcessorSTM32F10x::myPllClockPrescalerMap[] =
{
    {2, RCC_CFGR_PLLMULL2},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_2
    {3, RCC_CFGR_PLLMULL3},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_3
	{4, RCC_CFGR_PLLMULL4},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_4
	{5, RCC_CFGR_PLLMULL5},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_5
	{6, RCC_CFGR_PLLMULL6},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_6
	{7, RCC_CFGR_PLLMULL7},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_7
	{8, RCC_CFGR_PLLMULL8},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_8
	{9, RCC_CFGR_PLLMULL9},   /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_9
    {10, RCC_CFGR_PLLMULL10}, /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_10
    {11, RCC_CFGR_PLLMULL11}, /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_11
    {12, RCC_CFGR_PLLMULL12}, /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_12
    {13, RCC_CFGR_PLLMULL13}, /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_13
    {14, RCC_CFGR_PLLMULL14}, /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_14
    {15, RCC_CFGR_PLLMULL15}, /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_15
    {16, RCC_CFGR_PLLMULL16}  /// PLL_CLOCK_PRESCALER_MULTIPLY_BY_16
};

#ifdef STM32F10X_LD

#endif // STM32F10X_LD

#ifdef STM32F10X_LD_VL

#endif // STM32F10X_LD_VL

#ifdef STM32F10X_MD

#endif // STM32F10X_MD

#ifdef STM32F10X_MD_VL

#endif // STM32F10X_MD_VL

#ifdef STM32F10X_HD

const uint32_t ProcessorSTM32F10x::myRccClockEnableMap[] =
{
    /// AHB peripherals
    RCC_AHBENR_DMA1EN,    /// PERIPHERAL_DMA_1
    RCC_AHBENR_DMA2EN,    /// PERIPHERAL_DMA_2
    RCC_AHBENR_SRAMEN,    /// PERIPHERAL_SRAM
    RCC_AHBENR_FLITFEN,   /// PERIPHERAL_FLITF
    RCC_AHBENR_CRCEN,     /// PERIPHERAL_CRC
    RCC_AHBENR_FSMCEN,    /// PERIPHERAL_FSMC
    RCC_AHBENR_SDIOEN,    /// PERIPHERAL_SDIO
    0,                    /// PERIPHERAL_USB_OTG_FS
    0,                    /// PERIPHERAL_ETHERNET_MAC
    0,                    /// PERIPHERAL_ETHERNET_MAC_TX
    0,                    /// PERIPHERAL_ETHERNET_MAC_RX
    /// APB2 peripherals
    RCC_APB2ENR_AFIOEN,   /// PERIPHERAL_AFIO
    RCC_APB2ENR_IOPAEN,   /// PERIPHERAL_GPIO_A
    RCC_APB2ENR_IOPBEN,   /// PERIPHERAL_GPIO_B
    RCC_APB2ENR_IOPCEN,   /// PERIPHERAL_GPIO_C
    RCC_APB2ENR_IOPDEN,   /// PERIPHERAL_GPIO_D
    RCC_APB2ENR_IOPEEN,   /// PERIPHERAL_GPIO_E
    RCC_APB2ENR_IOPFEN,   /// PERIPHERAL_GPIO_F
    RCC_APB2ENR_IOPGEN,   /// PERIPHERAL_GPIO_G
    RCC_APB2ENR_ADC1EN,   /// PERIPHERAL_ADC_1
    RCC_APB2ENR_ADC2EN,   /// PERIPHERAL_ADC_2
    RCC_APB2ENR_TIM1EN,   /// PERIPHERAL_TIM_1
    RCC_APB2ENR_SPI1EN,   /// PERIPHERAL_SPI_1
    RCC_APB2ENR_TIM8EN,   /// PERIPHERAL_TIM_8
    RCC_APB2ENR_USART1EN, /// PERIPHERAL_USART_1
    RCC_APB2ENR_ADC3EN,   /// PERIPHERAL_ADC_3
    0,                    /// PERIPHERAL_TIM_15
    0,                    /// PERIPHERAL_TIM_16
    0,                    /// PERIPHERAL_TIM_17
    0,                    /// PERIPHERAL_TIM_9
    0,                    /// PERIPHERAL_TIM_10
    0,                    /// PERIPHERAL_TIM_11
    /// APB1 peripherals
    RCC_APB1ENR_TIM2EN,   /// PERIPHERAL_TIM_2
    RCC_APB1ENR_TIM3EN,   /// PERIPHERAL_TIM_3
    RCC_APB1ENR_TIM4EN,   /// PERIPHERAL_TIM_4
    RCC_APB1ENR_TIM5EN,   /// PERIPHERAL_TIM_5
    RCC_APB1ENR_TIM6EN,   /// PERIPHERAL_TIM_6
    RCC_APB1ENR_TIM7EN,   /// PERIPHERAL_TIM_7
    0,                    /// PERIPHERAL_TIM_12
    0,                    /// PERIPHERAL_TIM_13
    0,                    /// PERIPHERAL_TIM_14
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
    0,                    /// PERIPHERAL_CAN_2
    RCC_APB1ENR_BKPEN,    /// PERIPHERAL_BKP
    RCC_APB1ENR_PWREN,    /// PERIPHERAL_PWR
    RCC_APB1ENR_DACEN,    /// PERIPHERAL_DAC
    0 /// PERIPHERAL_CEC
};

#endif // STM32F10X_HD

#ifdef STM32F10X_HD_VL

#endif // STM32F10X_HD_VL

#ifdef STM32F10X_XL

#endif // STM32F10X_XL

#ifdef STM32F10X_CL

const uint32_t ProcessorSTM32F10x::myRccClockEnableMap[] =
{
    /// AHB peripherals
    RCC_AHBENR_DMA1EN, /// PERIPHERAL_DMA_1
    RCC_AHBENR_DMA2EN, /// PERIPHERAL_DMA_2
    RCC_AHBENR_SRAMEN, /// PERIPHERAL_SRAM
    RCC_AHBENR_FLITFEN, /// PERIPHERAL_FLITF
    RCC_AHBENR_CRCEN, /// PERIPHERAL_CRC
    0,                  /// PERIPHERAL_FSMC
    0,                  /// PERIPHERAL_SDIO
    RCC_AHBENR_OTGFSEN, /// PERIPHERAL_USB_OTG_FS
    RCC_AHBENR_ETHMACEN, /// PERIPHERAL_ETHERNET_MAC
    RCC_AHBENR_ETHMACTXEN, /// PERIPHERAL_ETHERNET_MAC_TX
    RCC_AHBENR_ETHMACRXEN, /// PERIPHERAL_ETHERNET_MAC_RX
    /// APB2 peripherals
    RCC_APB2ENR_AFIOEN, /// PERIPHERAL_AFIO
    RCC_APB2ENR_IOPAEN, /// PERIPHERAL_GPIO_A
    RCC_APB2ENR_IOPBEN, /// PERIPHERAL_GPIO_B
    RCC_APB2ENR_IOPCEN, /// PERIPHERAL_GPIO_C
    RCC_APB2ENR_IOPDEN, /// PERIPHERAL_GPIO_D
    RCC_APB2ENR_IOPEEN, /// PERIPHERAL_GPIO_E
    0,                  /// PERIPHERAL_GPIO_F
    0,                  /// PERIPHERAL_GPIO_G
    RCC_APB2ENR_ADC1EN, /// PERIPHERAL_ADC_1
    RCC_APB2ENR_ADC2EN, /// PERIPHERAL_ADC_2
    RCC_APB2ENR_TIM1EN, /// PERIPHERAL_TIM_1
    RCC_APB2ENR_SPI1EN, /// PERIPHERAL_SPI_1
    0,                  /// PERIPHERAL_TIM_8
    RCC_APB2ENR_USART1EN, /// PERIPHERAL_USART_1
    0,                  /// PERIPHERAL_ADC_3
    0,                  /// PERIPHERAL_TIM_15
    0,                  /// PERIPHERAL_TIM_16
    0,                  /// PERIPHERAL_TIM_17
    0,                  /// PERIPHERAL_TIM_9
    0,                  /// PERIPHERAL_TIM_10
    0,                  /// PERIPHERAL_TIM_11
    /// APB1 peripherals
    RCC_APB1ENR_TIM2EN, /// PERIPHERAL_TIM_2
    RCC_APB1ENR_TIM3EN, /// PERIPHERAL_TIM_3
    RCC_APB1ENR_TIM4EN, /// PERIPHERAL_TIM_4
    RCC_APB1ENR_TIM5EN, /// PERIPHERAL_TIM_4
    RCC_APB1ENR_TIM6EN, /// PERIPHERAL_TIM_6
    RCC_APB1ENR_TIM7EN, /// PERIPHERAL_TIM_7
    0, /// PERIPHERAL_TIM_12
    0, /// PERIPHERAL_TIM_13
    0, /// PERIPHERAL_TIM_14
    RCC_APB1ENR_WWDGEN, /// PERIPHERAL_WWDG
    RCC_APB1ENR_SPI2EN, /// PERIPHERAL_SPI_2
    RCC_APB1ENR_SPI3EN, /// PERIPHERAL_SPI_3
    RCC_APB1ENR_USART2EN, /// PERIPHERAL_USART_2
    RCC_APB1ENR_USART3EN, /// PERIPHERAL_USART_3
    RCC_APB1ENR_UART4EN, /// PERIPHERAL_UART_4
    RCC_APB1ENR_UART5EN, /// PERIPHERAL_UART_5
    RCC_APB1ENR_I2C1EN, /// PERIPHERAL_I2C_1
    RCC_APB1ENR_I2C2EN, /// PERIPHERAL_I2C_2
    0, /// PERIPHERAL_USB
    RCC_APB1ENR_CAN1EN, /// PERIPHERAL_CAN_1
    RCC_APB1ENR_CAN2EN, /// PERIPHERAL_CAN_2
    RCC_APB1ENR_BKPEN, /// PERIPHERAL_BKP
    RCC_APB1ENR_PWREN, /// PERIPHERAL_PWR
    RCC_APB1ENR_DACEN, /// PERIPHERAL_DAC
    0 /// PERIPHERAL_CEC
};

#endif // STM32F10X_CL

// Variables

ProcessorSTM32F10x::STM32F10xConfig ProcessorSTM32F10x::mySTM32F10xConfig;

ProcessorSTM32F10x::AhbClockPrescaler ProcessorSTM32F10x::myAhbClockPrescaler;

ProcessorSTM32F10x::Apb1ClockPrescaler ProcessorSTM32F10x::myApb1ClockPrescaler;

ProcessorSTM32F10x::Apb2ClockPrescaler ProcessorSTM32F10x::myApb2ClockPrescaler;

ProcessorSTM32F10x::PllClockPrescaler ProcessorSTM32F10x::myPllClockPrescaler;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F10x::ProcessorSTM32F10x(
                                      const VoltageV coreVoltageV,
                                      const uint32_t externalClockFrequencyHz) :
    Processor(coreVoltageV, externalClockFrequencyHz)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F10x::~ProcessorSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F10x::getCoreClockFrequencyHz()
{
    return (mySTM32F10xConfig.coreClockFrequencyHz);
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F10x::getAhbClockFrequencyHz()
{
    return (mySTM32F10xConfig.ahbClockFrequencyHz);
}

//------------------------------------------------------------------------------
ProcessorSTM32F10x::AhbClockPrescaler ProcessorSTM32F10x::getAhbClockPrescaler()
{
    return myAhbClockPrescaler;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F10x::getApb1ClockFrequencyHz()
{
    return (mySTM32F10xConfig.apb1ClockFrequencyHz);
}

//------------------------------------------------------------------------------
ProcessorSTM32F10x::Apb1ClockPrescaler
                                     ProcessorSTM32F10x::getApb1ClockPrescaler()
{
    return myApb1ClockPrescaler;
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F10x::getApb2ClockFrequencyHz()
{
    return (mySTM32F10xConfig.apb2ClockFrequencyHz);
}

//------------------------------------------------------------------------------
ProcessorSTM32F10x::Apb2ClockPrescaler
								     ProcessorSTM32F10x::getApb2ClockPrescaler()
{
    return myApb2ClockPrescaler;
}

//------------------------------------------------------------------------------
ProcessorSTM32F10x::PllClockPrescaler ProcessorSTM32F10x::getPllClockPrescaler()
{
    return myPllClockPrescaler;
}


//------------------------------------------------------------------------------
void ProcessorSTM32F10x::setPeripheralClockEnabled(const Peripheral peripheral,
                                                   const bool enabled)
{
    // Check to see if this peripheral is supported for this specific micro
    if (myRccClockEnableMap[peripheral] == 0)
    {
        // Trying to enable a peripheral that doesn't exist, lockup
        while (true);
    }

    // AHB peripheral
    if (peripheral <= PERIPHERAL_ETHERNET_MAC_RX)
    {
        setBitsSet(RCC->AHBENR, myRccClockEnableMap[peripheral], enabled);
    }
    // APB2 peripheral
    else if (peripheral <= PERIPHERAL_TIM_11)
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
uint32_t ProcessorSTM32F10x::getPeripheralInputClockFrequencyHz(
                                                    const Peripheral peripheral)
{
    // Check to see if this peripheral is supported for this specific micro
    if (myRccClockEnableMap[peripheral] == 0)
    {
        // Trying to get clock frequency for a peripheral that doesn't exist,
        // lockup
        while (true);
    }

    uint32_t inputClockFrequencyHz;

    // AHB peripheral
    if (peripheral <= PERIPHERAL_ETHERNET_MAC_RX)
    {
        inputClockFrequencyHz = getAhbClockFrequencyHz();
    }
    // APB2 peripheral
    else if (peripheral <= PERIPHERAL_TIM_11)
    {
        inputClockFrequencyHz = getApb2ClockFrequencyHz();
    }
    // APB1 peripheral
    else
    {
        inputClockFrequencyHz = getApb1ClockFrequencyHz();
    }

    ///
    /// Timers have special clock multipliers depending on the APB1/2 clock
    /// prescaler (see the Clock Tree in the reference manual).
    ///
    switch (peripheral)
    {
        // APB2 peripherals
        case PERIPHERAL_TIM_1:  // Fall through
        case PERIPHERAL_TIM_8:  // Fall through
        case PERIPHERAL_TIM_9:  // Fall through
        case PERIPHERAL_TIM_10: // Fall through
        case PERIPHERAL_TIM_11: // Fall through
        {
            if (getApb2ClockPrescaler() != APB2_CLOCK_PRESCALER_DIVIDE_BY_1)
            {
                inputClockFrequencyHz *= 2;
            }

            break;
        }
        // APB1 peripherals
        case PERIPHERAL_TIM_2:  // Fall through
        case PERIPHERAL_TIM_3:  // Fall through
        case PERIPHERAL_TIM_4:  // Fall through
        case PERIPHERAL_TIM_5:  // Fall through
        case PERIPHERAL_TIM_6:  // Fall through
        case PERIPHERAL_TIM_7:  // Fall through
        case PERIPHERAL_TIM_12: // Fall through
        case PERIPHERAL_TIM_13: // Fall through
        case PERIPHERAL_TIM_14: // Fall through
        {
            if (getApb1ClockPrescaler() != APB1_CLOCK_PRESCALER_DIVIDE_BY_1)
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
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ProcessorSTM32F10x::Error ProcessorSTM32F10x::setSTM32F10xConfig(
										          const STM32F10xConfig& config)
{
    if (config.coreClockFrequencyHz > 72000000)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    ///
    /// NOTE:
    /// This method performs calculations of PLL configuration values. To make
    /// the calculations straight-forward, the external clock frequency is
    /// required to be 8, 16, or 25 MHz. Any other external clock frequencies
    /// will require the following code to be modified.
    ///

    // TODO: Add an error condition that keeps the HSI running if the HSE fails
    // TODO: Make sure these settings are consistent for all operating voltages

    // Calculate PLL and PLL2 dividers and multipliers

    // TODO Get rid of these #ifdefs by implementing subdrivers of this class

#ifdef STM32F10X_CL

	clearBits(RCC->CFGR2, (RCC_CFGR2_PREDIV2 |
                           RCC_CFGR2_PLL2MUL |
                           RCC_CFGR2_PREDIV1 |
                           RCC_CFGR2_PREDIV1SRC));

	switch (getClockSourceFrequencyHz())
	{
		case EXTERNAL_CLOCK_FREQUENCY_8_MHZ:
		{
			///
			/// HSE = 8 MHz
			/// PLL2 configuration: PLL2CLK = (HSE / 1) * 8 = 64 MHz
			/// PREDIV1 configuration: PREDIV1CLK = PLL2 / 8 = 8 MHz
			///
			setBits(RCC->CFGR2, (RCC_CFGR2_PREDIV2_DIV1    |
                                 RCC_CFGR2_PLL2MUL8        |
                                 RCC_CFGR2_PREDIV1SRC_PLL2 |
                                 RCC_CFGR2_PREDIV1_DIV8));

			break;
		}
		case EXTERNAL_CLOCK_FREQUENCY_16_MHZ:
		{
			///
			/// HSE = 16 MHz
			/// PLL2 configuration: PLL2CLK = (HSE / 2) * 8 = 64 MHz
			/// PREDIV1 configuration: PREDIV1CLK = PLL2 / 8 = 8 MHz
			///
			setBits(RCC->CFGR2, (RCC_CFGR2_PREDIV2_DIV2    |
                                 RCC_CFGR2_PLL2MUL8        |
                                 RCC_CFGR2_PREDIV1SRC_PLL2 |
                                 RCC_CFGR2_PREDIV1_DIV8));

			break;
		}
		case EXTERNAL_CLOCK_FREQUENCY_25_MHZ:
		{
			///
			/// HSE = 25 MHz
			/// PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz
			/// PREDIV1 configuration: PREDIV1CLK = PLL2 / 5 = 8 MHz
			///
			setBits(RCC->CFGR2, (RCC_CFGR2_PREDIV2_DIV5    |
                                 RCC_CFGR2_PLL2MUL8        |
                                 RCC_CFGR2_PREDIV1SRC_PLL2 |
                                 RCC_CFGR2_PREDIV1_DIV5));

			break;
		}
		default:
		{
		    //
		    return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
		}
	}

#endif // STM32F10X_CL

	int i;
	int size;

	///
	/// This calculation relies on PREDIV1 being equal to 8 MHz
	/// PLL configuration: PLLCLK = PREDIV1 * 9 = 72 MHz;
	///

	uint32_t preDiv1ClockFrequencyHz = 8000000;

    // Loop through the prescaler values to see if any give the desired PLL
    // clock speed
	size = ARRAY_SIZE(myPllClockPrescalerMap);

	for (i = 0; i < size; i++)
	{
		if ((preDiv1ClockFrequencyHz * myPllClockPrescalerMap[i].value) ==
			config.coreClockFrequencyHz)
		{
			break;
		}
	}

	// Check to see if a prescaler was found
	if (i == size)
	{
	    return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
	}

	myPllClockPrescaler = (PllClockPrescaler) i;

	clearBits(RCC->CFGR, (RCC_CFGR_PLLXTPRE |
					      RCC_CFGR_PLLSRC   |
					      RCC_CFGR_PLLMULL));

#ifdef STM32F10X_CL

	setBits(RCC->CFGR, (RCC_CFGR_PLLXTPRE_PREDIV1 |
	                    myPllClockPrescalerMap[myPllClockPrescaler].bits));

#endif // STM32F10X_CL
    
#ifdef STM32F10X_HD

	// TODO: This forces CLOCK_SOURCE_EXTERNAL to be used

    setBits(RCC->CFGR,
            (RCC_CFGR_PLLSRC |
             myPllClockPrescalerMap[myPllClockPrescaler].bits));

#endif // STM32F10X_HD
    
    ///
    /// HCLK/AHB = SYSCLK / prescaler
    /// - prescaler = [1, 2, 4, 8, 16]
    /// - AHB <= 72 MHz
    ///
    
    // Loop through the prescaler values to see if any give the desired AHB
    // clock speed
	size = ARRAY_SIZE(myAhbClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if ((config.coreClockFrequencyHz / myAhbClockPrescalerMap[i].value) ==
            config.ahbClockFrequencyHz)
        {
            break;
        }
    }
    
    // Check to see if a prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }
    
    // If using Ethernet, check AHB clock >= 25 MHz
    if (config.isEthernet && (config.ahbClockFrequencyHz < 25000000))
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
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
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
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
        if ((config.ahbClockFrequencyHz / myApb2ClockPrescalerMap[i].value) ==
            config.apb2ClockFrequencyHz)
        {
            break;
        }
    }
    
    // Check to see if a prescaler was found
    if (i == size)
    {
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
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
            return Error(ERROR_CODE_CLOCK_STARTUP_FAILED);
        }

#ifdef STM32F10X_CL

    	setBits(RCC->CFGR, RCC_CFGR_PLLSRC_PREDIV1);

#endif // STM32F10X_CL

    }

#ifdef STM32F10X_CL
    
	// Enable PLL2
	setBits(RCC->CR, RCC_CR_PLL2ON);

	// Wait till PLL2 is ready
	while (areBitsClear(RCC->CR, RCC_CR_PLL2RDY))
	{
	}

#endif // STM32F10X_CL

    // Enable the main PLL
    setBits(RCC->CR, RCC_CR_PLLON);
    
    // Wait until the main PLL is ready
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
        return Error(ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }
    
    FlashWaitStates flashWaitStates = (FlashWaitStates) i;
    
    setBits(FLASH->ACR, (uint32_t) flashWaitStates);
    
    // Clear SW bits
    clearBits(RCC->CFGR, RCC_CFGR_SW);
    setBits(RCC->CFGR, RCC_CFGR_SW_PLL);
    
    // Wait till the main PLL is used as system clock source
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    {
    }
    
    // Vector Table Relocation in Internal FLASH
    SCB->VTOR = config.vectorTableAddress;
    
    mySTM32F10xConfig = config;
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Processor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F10x::driverReset()
{
    // Reset the RCC clock configuration to the default reset state
    // Set HSION bit
	setBits(RCC->CR, (uint32_t) 0x00000001);

    // Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
	// 0x0700FFFF = ~0xF8FF0000
    clearBits(RCC->CFGR, (uint32_t) 0x0700FFFF);

    // Reset HSEON, CSSON and PLLON bits
    // 0x01090000 = ~0xFEF6FFFF
    clearBits(RCC->CR, (uint32_t) 0x01090000);

    // Reset HSEBYP bit
    // 0x00040000 = ~0xFFFBFFFF
    clearBits(RCC->CR, (uint32_t) 0x00040000);

    // Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
    // 0x007F0000 = ~0xFF80FFFF
    clearBits(RCC->CFGR, (uint32_t) 0x007F0000);

    // Disable all interrupts
    RCC->CIR = (uint32_t) 0x00FF0000;
    
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F10x::driverConfigure(
												const Processor::Config& config)
{
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Processor::Error ProcessorSTM32F10x::driverSetPowerMode(
                                           const Processor::PowerMode powerMode)
{
    switch (powerMode)
    {
        case POWER_MODE_SLEEP:
        {
            __WFE();
        }
    }
    
    return Processor::Error(Processor::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t ProcessorSTM32F10x::driverGetCoreClockFrequencyHz()
{
    return (mySTM32F10xConfig.coreClockFrequencyHz);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void SystemInit(void)
{
    // Do nothing
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
