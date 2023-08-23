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
/// @file InterruptSTM32H7xx.cpp
/// @author Ben Minerd
/// @date 6/27/2022
/// @brief InterruptSTM32H7xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32H7xx/InterruptSTM32H7xx.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const IRQn_Type InterruptSTM32H7xx::myInterruptMap[] =
{
    NonMaskableInt_IRQn,     /// ID_NON_MASKABLE
    HardFault_IRQn,          /// ID_HARD_FAULT
    MemoryManagement_IRQn,   /// ID_MEMORY_MANAGEMENT
    BusFault_IRQn,           /// ID_BUS_FAULT
    UsageFault_IRQn,         /// ID_USAGE_FAULT
    SVCall_IRQn,             /// ID_SV_CALL
    DebugMonitor_IRQn,       /// ID_DEBUG_MONITOR
    PendSV_IRQn,             /// ID_PEND_SV
    SysTick_IRQn,            /// ID_SYS_TICK
    WWDG_IRQn,               /// ID_WWDG
    PVD_AVD_IRQn,            /// ID_PVD_AVD
    TAMP_STAMP_IRQn,         /// ID_TAMPER_AND_TIMESTAMP
    RTC_WKUP_IRQn,           /// ID_RTC_WAKE_UP
    FLASH_IRQn,              /// ID_FLASH
    RCC_IRQn,                /// ID_RCC
    EXTI0_IRQn,              /// ID_EXTI_0
    EXTI1_IRQn,              /// ID_EXTI_1
    EXTI2_IRQn,              /// ID_EXTI_2
    EXTI3_IRQn,              /// ID_EXTI_3
    EXTI4_IRQn,              /// ID_EXTI_4
    DMA1_Stream0_IRQn,       /// ID_DMA_1_STREAM_0
    DMA1_Stream1_IRQn,       /// ID_DMA_1_STREAM_1
    DMA1_Stream2_IRQn,       /// ID_DMA_1_STREAM_2
    DMA1_Stream3_IRQn,       /// ID_DMA_1_STREAM_3
    DMA1_Stream4_IRQn,       /// ID_DMA_1_STREAM_4
    DMA1_Stream5_IRQn,       /// ID_DMA_1_STREAM_5
    DMA1_Stream6_IRQn,       /// ID_DMA_1_STREAM_6
    ADC_IRQn,                /// ID_ADC_1_AND_2
    FDCAN1_IT0_IRQn,         /// ID_FD_CAN_1_IT_0
    FDCAN2_IT0_IRQn,         /// ID_FD_CAN_2_IT_0
    FDCAN1_IT1_IRQn,         /// ID_FD_CAN_2_IT_1
    FDCAN2_IT1_IRQn,         /// ID_FD_CAN_2_IT_1
    EXTI9_5_IRQn,            /// ID_EXTI_9_TO_5
    TIM1_BRK_IRQn,           /// ID_TIM_1_BRK
    TIM1_UP_IRQn,            /// ID_TIM_1_UP
    TIM1_TRG_COM_IRQn,       /// ID_TIM_1_TRG_AND_COM
    TIM1_CC_IRQn,            /// ID_TIM_1_CC
    TIM2_IRQn,               /// ID_TIM_2
    TIM3_IRQn,               /// ID_TIM_3
    TIM4_IRQn,               /// ID_TIM_4
    I2C1_EV_IRQn,            /// ID_I2C_1_EV
    I2C1_ER_IRQn,            /// ID_I2C_1_ER
    I2C2_EV_IRQn,            /// ID_I2C_2_EV
    I2C2_ER_IRQn,            /// ID_I2C_2_ER
    SPI1_IRQn,               /// ID_SPI_1
    SPI2_IRQn,               /// ID_SPI_2
    USART1_IRQn,             /// ID_USART_1
    USART2_IRQn,             /// ID_USART_2
    USART3_IRQn,             /// ID_USART_3
    EXTI15_10_IRQn,          /// ID_EXTI_15_TO_10
    RTC_Alarm_IRQn,          /// ID_RTC_ALARM_A_AND_B
    TIM8_BRK_TIM12_IRQn,     /// ID_TIM_8_BRK_AND_TIM_12
    TIM8_UP_TIM13_IRQn,      /// ID_TIM_8_UP_AND_TIM_13
    TIM8_TRG_COM_TIM14_IRQn, /// ID_TIM_8_TRG_AND_COM_AND_TIM_14
    TIM8_CC_IRQn,            /// ID_TIM_8_CC
    DMA1_Stream7_IRQn,       /// ID_DMA_1_STREAM_7
    FMC_IRQn,                /// ID_FMC
    SDMMC1_IRQn,             /// ID_SDMMC
    TIM5_IRQn,               /// ID_TIM_5
    SPI3_IRQn,               /// ID_SPI_3
    UART4_IRQn,              /// ID_UART_4
    UART5_IRQn,              /// ID_UART_5
    TIM6_DAC_IRQn,           /// ID_TIM_6_AND_DAC_1_AND_2
    TIM7_IRQn,               /// ID_TIM_7
    DMA2_Stream0_IRQn,       /// ID_DMA_2_STREAM_0
    DMA2_Stream1_IRQn,       /// ID_DMA_2_STREAM_1
    DMA2_Stream2_IRQn,       /// ID_DMA_2_STREAM_2
    DMA2_Stream3_IRQn,       /// ID_DMA_2_STREAM_3
    DMA2_Stream4_IRQn,       /// ID_DMA_2_STREAM_4
    ETH_IRQn,                /// ID_ETH
    ETH_WKUP_IRQn,           /// ID_ETH_WKUP
    FDCAN_CAL_IRQn,          /// ID_FD_CAN
    DMA2_Stream5_IRQn,       /// ID_DMA_2_STREAM_5
    DMA2_Stream6_IRQn,       /// ID_DMA_2_STREAM_6
    DMA2_Stream7_IRQn,       /// ID_DMA_2_STREAM_7
    USART6_IRQn,             /// ID_USART_6
    I2C3_EV_IRQn,            /// ID_I2C_3_EV
    I2C3_ER_IRQn,            /// ID_I2C_3_ER
    OTG_HS_EP1_OUT_IRQn,     /// ID_USB_OTG_HS_EP_1_OUT
    OTG_HS_EP1_IN_IRQn,      /// ID_USB_OTG_HS_EP_1_IN
    OTG_HS_WKUP_IRQn,        /// ID_USB_OTG_HS_WKUP
    OTG_HS_IRQn,             /// ID_USB_OTG_HS
    DCMI_IRQn,               /// ID_DCMI
    CRYP_IRQn,               /// ID_CRYP
    HASH_RNG_IRQn,           /// ID_HASH_AND_RNG
    FPU_IRQn,                /// ID_FPU
    UART7_IRQn,              /// ID_UART_7
    UART8_IRQn,              /// ID_UART_8
    SPI4_IRQn,               /// ID_SPI_4
    SPI5_IRQn,               /// ID_SPI_5
    SPI6_IRQn,               /// ID_SPI_6
    SAI1_IRQn,               /// ID_SAI_1
    LTDC_IRQn,               /// ID_LTDC
    LTDC_ER_IRQn,            /// ID_LTDC_ER
    DMA2D_IRQn,              /// ID_DMA2D
    SAI2_IRQn,               /// ID_SAI_2
    QUADSPI_IRQn,            /// ID_QUAD_SPI
    LPTIM1_IRQn,             /// ID_LP_TIM_1
    CEC_IRQn,                /// ID_HDMI_CEC
    I2C4_EV_IRQn,            /// ID_I2C_4_EV
    I2C4_ER_IRQn,            /// ID_I2C_4_ER
    SPDIF_RX_IRQn,           /// ID_SPDIF_RX
    OTG_FS_EP1_OUT_IRQn,     /// ID_USB_OTG_FS_EP_1_OUT
    OTG_FS_EP1_IN_IRQn,      /// ID_USB_OTG_FS_EP_1_IN
    OTG_FS_WKUP_IRQn,        /// ID_USB_FS_WKUP
    OTG_FS_IRQn,             /// ID_USB_FS
    DMAMUX1_OVR_IRQn,        /// ID_DMA_MUX_1_OVR
    HRTIM1_Master_IRQn,      /// ID_HR_TIM_1_MASTER
    HRTIM1_TIMA_IRQn,        /// ID_HR_TIM_1_TIM_A
    HRTIM1_TIMB_IRQn,        /// ID_HR_TIM_1_TIM_B
    HRTIM1_TIMC_IRQn,        /// ID_HR_TIM_1_TIM_C
    HRTIM1_TIMD_IRQn,        /// ID_HR_TIM_1_TIM_D
    HRTIM1_TIME_IRQn,        /// ID_HR_TIM_1_TIM_E
    HRTIM1_FLT_IRQn,         /// ID_HR_TIM_1_FLT
    DFSDM1_FLT0_IRQn,        /// ID_DFSDM_1_FLT_0
    DFSDM1_FLT1_IRQn,        /// ID_DFSDM_1_FLT_1
    DFSDM1_FLT2_IRQn,        /// ID_DFSDM_1_FLT_2
    DFSDM1_FLT3_IRQn,        /// ID_DFSDM_1_FLT_3
    SAI3_IRQn,               /// ID_SAI_3
    SWPMI1_IRQn,             /// ID_SWMPI_1
    TIM15_IRQn,              /// ID_TIM_15
    TIM16_IRQn,              /// ID_TIM_16
    TIM17_IRQn,              /// ID_TIM_17
    MDIOS_WKUP_IRQn,         /// ID_MDIOS_WKUP
    MDIOS_IRQn,              /// ID_MDIOS
    JPEG_IRQn,               /// ID_JPEG
    MDMA_IRQn,               /// ID_MDMA
    SDMMC2_IRQn,             /// ID_SDMMC_2
    HSEM1_IRQn,              /// ID_HSEM_1
    ADC3_IRQn,               /// ID_ADC_3
    DMAMUX2_OVR_IRQn,        /// ID_DMA_MUX_2_OVR
    BDMA_Channel0_IRQn,      /// ID_BDMA_CHANNEL_0
    BDMA_Channel1_IRQn,      /// ID_BDMA_CHANNEL_1
    BDMA_Channel2_IRQn,      /// ID_BDMA_CHANNEL_2
    BDMA_Channel3_IRQn,      /// ID_BDMA_CHANNEL_3
    BDMA_Channel4_IRQn,      /// ID_BDMA_CHANNEL_4
    BDMA_Channel5_IRQn,      /// ID_BDMA_CHANNEL_5
    BDMA_Channel6_IRQn,      /// ID_BDMA_CHANNEL_6
    BDMA_Channel7_IRQn,      /// ID_BDMA_CHANNEL_7
    COMP_IRQn,               /// ID_COMP
    LPTIM2_IRQn,             /// ID_LP_TIM_2
    LPTIM3_IRQn,             /// ID_LP_TIM_3
    LPTIM4_IRQn,             /// ID_LP_TIM_4
    LPTIM5_IRQn,             /// ID_LP_TIM_5
    LPUART1_IRQn,            /// ID_LP_UART_1
    CRS_IRQn,                /// ID_CRS
    ECC_IRQn,                /// ID_ECC
    SAI4_IRQn,               /// ID_SAI_4
    WAKEUP_PIN_IRQn          /// ID_WAKEUP_PIN
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InterruptSTM32H7xx::InterruptSTM32H7xx(const Id id,
                                       HandlerCallback& handlerCallback) :
    Interrupt(handlerCallback),
    myId(id)
{
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InterruptSTM32H7xx::driverSetEnabled(const bool enabled)
{
    if (enabled)
    {
        NVIC_EnableIRQ(myInterruptMap[myId]);
    }
    else
    {
        NVIC_DisableIRQ(myInterruptMap[myId]);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Interrupt
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::Error InterruptSTM32H7xx::driverConfigure(const Config& config)
{
    NVIC_SetPriority(myInterruptMap[myId],
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                                         config.priority,
                                         0));

    return Error(ERROR_CODE_NONE);
}
