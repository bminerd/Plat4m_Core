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
/// @file InterruptSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/15/2016
/// @brief InterruptSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <InterruptSTM32F10x.h>

using Plat4m::InterruptSTM32F10x;
using Plat4m::Interrupt;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

#ifdef STM32F10X_LD

#endif // STM32F10X_LD

#ifdef STM32F10X_LD_VL

#endif // STM32F10X_LD_VL

#ifdef STM32F10X_MD

#endif // STM32F10X_MD

#ifdef STM32F10X_MD_VL

#endif // STM32F10X_MD_VL

#ifdef STM32F10X_HD

const IRQn_Type InterruptSTM32F10x::myInterruptMap[] =
{
    NonMaskableInt_IRQn,   /// ID_NON_MASKABLE
    MemoryManagement_IRQn, /// ID_MEMORY_MANAGEMENT
    BusFault_IRQn,         /// ID_BUS_FAULT
    UsageFault_IRQn,       /// ID_USAGE_FAULT
    SVCall_IRQn,           /// ID_SV_CALL
    DebugMonitor_IRQn,     /// ID_DEBUG_MONITOR
    PendSV_IRQn,           /// ID_PEND_SV
    SysTick_IRQn,          /// ID_SYS_TICK
    WWDG_IRQn,             /// ID_WWDG
    PVD_IRQn,              /// ID_PVD
    TAMPER_IRQn,           /// ID_STAMP
    RTC_IRQn,              /// ID_RTC
    FLASH_IRQn,            /// ID_FLASH
    RCC_IRQn,              /// ID_RCC
    EXTI0_IRQn,            /// ID_EXTI_0
    EXTI1_IRQn,            /// ID_EXTI_1
    EXTI2_IRQn,            /// ID_EXTI_2
    EXTI3_IRQn,            /// ID_EXTI_3
    EXTI4_IRQn,            /// ID_EXTI_4
    DMA1_Channel1_IRQn,    /// ID_DMA_1_CHANNEL_1
    DMA1_Channel2_IRQn,    /// ID_DMA_1_CHANNEL_2
    DMA1_Channel3_IRQn,    /// ID_DMA_1_CHANNEL_3
    DMA1_Channel4_IRQn,    /// ID_DMA_1_CHANNEL_4
    DMA1_Channel5_IRQn,    /// ID_DMA_1_CHANNEL_5
    DMA1_Channel6_IRQn,    /// ID_DMA_1_CHANNEL_6
    DMA1_Channel7_IRQn,    /// ID_DMA_1_CHANNEL_7
    ADC1_2_IRQn,           /// ID_ADC_1_2
    USB_HP_CAN1_TX_IRQn,   /// ID_USB_HP_CAN_1_TX
    USB_LP_CAN1_RX0_IRQn,  /// ID_USB_LP_CAN_1_RX0
    CAN1_RX1_IRQn,         /// ID_CAN_1_RX1
    CAN1_SCE_IRQn,         /// ID_CAN_1_SCE
    EXTI9_5_IRQn,          /// ID_EXTI_9_5
    TIM1_BRK_IRQn,         /// ID_TIM_1_BRK
    TIM1_UP_IRQn,          /// ID_TIM_1_UP
    TIM1_TRG_COM_IRQn,     /// ID_TIM_1_TRG_COM
    TIM1_CC_IRQn,          /// ID_TIM_1_CC
    TIM2_IRQn,             /// ID_TIM_2
    TIM3_IRQn,             /// ID_TIM_3
    TIM4_IRQn,             /// ID_TIM_4
    I2C1_EV_IRQn,          /// ID_I2C_1_EV
    I2C1_ER_IRQn,          /// ID_I2C_1_ER
    I2C2_EV_IRQn,          /// ID_I2C_2_EV
    I2C2_ER_IRQn,          /// ID_I2C_2_ER
    SPI1_IRQn,             /// ID_SPI_1
    SPI2_IRQn,             /// ID_SPI_2
    USART1_IRQn,           /// ID_USART_1
    USART2_IRQn,           /// ID_USART_2
    USART3_IRQn,           /// ID_USART_3
    EXTI15_10_IRQn,        /// ID_EXTI_15_10
    RTCAlarm_IRQn,         /// ID_RTC_ALARM
    USBWakeUp_IRQn,        /// ID_USB_OTG_FS_WAKE_UP
    TIM8_BRK_IRQn,         /// ID_TIM_8_BRK
    TIM8_UP_IRQn,          /// ID_TIM_8_UP
    TIM8_TRG_COM_IRQn,     /// ID_TIM_8_TRG_COM
    TIM8_CC_IRQn,          /// ID_TIM_8_CC
    ADC3_IRQn,             /// ID_ADC_3
    FSMC_IRQn,             /// ID_FSMC
    SDIO_IRQn,             /// ID_SDIO
    TIM5_IRQn,             /// ID_TIM_5
    SPI3_IRQn,             /// ID_SPI_3
    UART4_IRQn,            /// ID_UART_4
    UART5_IRQn,            /// ID_UART_5
    TIM6_IRQn,             /// ID_TIM_6
    TIM7_IRQn,             /// ID_TIM_7
    DMA2_Channel1_IRQn,    /// ID_DMA_2_CHANNEL_1
    DMA2_Channel2_IRQn,    /// ID_DMA_2_CHANNEL_2
    DMA2_Channel3_IRQn,    /// ID_DMA_2_CHANNEL_3
    DMA2_Channel4_5_IRQn,  /// ID_DMA_2_CHANNEL_4_5
    (IRQn_Type) 127,       /// ID_DMA_2_CHANNEL_5
    (IRQn_Type) 127,       /// ID_ETHERNET
    (IRQn_Type) 127,       /// ID_ETHERNET_WAKE_UP
    (IRQn_Type) 127,       /// ID_CAN_2_TX
    (IRQn_Type) 127,       /// ID_CAN_2_RX0
    (IRQn_Type) 127,       /// ID_CAN_2_RX1
    (IRQn_Type) 127,       /// ID_CAN_2_SCE
    (IRQn_Type) 127        /// ID_USB_OTG_FS
};

#endif // STM32F10X_HD

#ifdef STM32F10X_HD_VL

#endif // STM32F10X_HD_VL

#ifdef STM32F10X_XL

#endif // STM32F10X_XL


#ifdef STM32F10X_CL

const IRQn_Type InterruptSTM32F10x::myInterruptMap[] =
{
    NonMaskableInt_IRQn,   /// ID_NON_MASKABLE
    MemoryManagement_IRQn, /// ID_MEMORY_MANAGEMENT
    BusFault_IRQn,         /// ID_BUS_FAULT
    UsageFault_IRQn,       /// ID_USAGE_FAULT
    SVCall_IRQn,           /// ID_SV_CALL
    DebugMonitor_IRQn,     /// ID_DEBUG_MONITOR
    PendSV_IRQn,           /// ID_PEND_SV
    SysTick_IRQn,          /// ID_SYS_TICK
    WWDG_IRQn,             /// ID_WWDG
    PVD_IRQn,              /// ID_PVD
    TAMPER_IRQn,           /// ID_STAMP
    RTC_IRQn,              /// ID_RTC
    FLASH_IRQn,            /// ID_FLASH
    RCC_IRQn,              /// ID_RCC
    EXTI0_IRQn,            /// ID_EXTI_0
    EXTI1_IRQn,            /// ID_EXTI_1
    EXTI2_IRQn,            /// ID_EXTI_2
    EXTI3_IRQn,            /// ID_EXTI_3
    EXTI4_IRQn,            /// ID_EXTI_4
    DMA1_Channel1_IRQn,    /// ID_DMA_1_CHANNEL_1
    DMA1_Channel2_IRQn,    /// ID_DMA_1_CHANNEL_2
    DMA1_Channel3_IRQn,    /// ID_DMA_1_CHANNEL_3
    DMA1_Channel4_IRQn,    /// ID_DMA_1_CHANNEL_4
    DMA1_Channel5_IRQn,    /// ID_DMA_1_CHANNEL_5
    DMA1_Channel6_IRQn,    /// ID_DMA_1_CHANNEL_6
    DMA1_Channel7_IRQn,    /// ID_DMA_1_CHANNEL_7
    ADC1_2_IRQn,           /// ID_ADC_1_2
    CAN1_TX_IRQn,          /// ID_USB_HP_CAN_1_TX
    CAN1_RX0_IRQn,         /// ID_USB_LP_CAN_1_RX0
    CAN1_RX1_IRQn,         /// ID_CAN_1_RX1
    CAN1_SCE_IRQn,         /// ID_CAN_1_SCE
    EXTI9_5_IRQn,          /// ID_EXTI_9_5
    TIM1_BRK_IRQn,         /// ID_TIM_1_BRK_TIM_15
    TIM1_UP_IRQn,          /// ID_TIM_1_UP_TIM_16
    TIM1_TRG_COM_IRQn,     /// ID_TIM_1_TRG_COM_TIM_17
    TIM1_CC_IRQn,          /// ID_TIM_1_CC
    TIM2_IRQn,             /// ID_TIM_2
    TIM3_IRQn,             /// ID_TIM_3
    TIM4_IRQn,             /// ID_TIM_4
    I2C1_EV_IRQn,          /// ID_I2C_1_EV
    I2C1_ER_IRQn,          /// ID_I2C_1_ER
    I2C2_EV_IRQn,          /// ID_I2C_2_EV
    I2C2_ER_IRQn,          /// ID_I2C_2_ER
    SPI1_IRQn,             /// ID_SPI_1
    SPI2_IRQn,             /// ID_SPI_2
    USART1_IRQn,           /// ID_USART_1
    USART2_IRQn,           /// ID_USART_2
    USART3_IRQn,           /// ID_USART_3
    EXTI15_10_IRQn,        /// ID_EXTI_15_10
    RTCAlarm_IRQn,         /// ID_RTC_ALARM
    OTG_FS_WKUP_IRQn,      /// ID_USB_WAKE_UP
    127,                   /// ID_TIM_8_BRK
    127,                   /// ID_TIM_8_UP
    127,                   /// ID_TIM_8_TRG_COM
    127,                   /// ID_TIM_8_CC
    127,                   /// ID_ADC_3
    127,                   /// ID_FSMC
    127,                   /// ID_SDIO
    TIM5_IRQn,             /// ID_TIM_5
    SPI3_IRQn,             /// ID_SPI_3
    UART4_IRQn,            /// ID_UART_4
    UART5_IRQn,            /// ID_UART_5
    TIM6_IRQn,             /// ID_TIM_6
    TIM7_IRQn,             /// ID_TIM_7
    DMA2_Channel1_IRQn,    /// ID_DMA_2_CHANNEL_1
    DMA2_Channel2_IRQn,    /// ID_DMA_2_CHANNEL_2
    DMA2_Channel3_IRQn,    /// ID_DMA_2_CHANNEL_3
    DMA2_Channel4_IRQn,    /// ID_DMA_2_CHANNEL_4
    DMA2_Channel5_IRQn,    /// ID_DMA_2_CHANNEL_5
    ETH_IRQn,              /// ID_ETHERNET
    ETH_WKUP_IRQn,         /// ID_ETHERNET_WAKE_UP
    CAN2_TX_IRQn,          /// ID_CAN_2_TX
    CAN2_RX0_IRQn,         /// ID_CAN_2_RX0
    CAN2_RX1_IRQn,         /// ID_CAN_2_RX1
    CAN2_SCE_IRQn,         /// ID_CAN_2_SCE
    OTG_FS_IRQn            /// ID_USB_OTG_FS
};

#endif // STM32F10X_CL

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InterruptSTM32F10x::InterruptSTM32F10x(const Id id,
                                       HandlerCallback& handlerCallback) :
    Interrupt(handlerCallback),
    myId(id)
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InterruptSTM32F10x::driverSetEnabled(const bool enabled)
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
// Private methods implemented from Interrupt
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::Error InterruptSTM32F10x::driverConfigure(const Config& config)
{
    NVIC_SetPriority(myInterruptMap[myId],
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                                         config.priority,
                                         0));

    return Error(ERROR_CODE_NONE);
}
