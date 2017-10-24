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
/// @file InterruptSTM32F30x.cpp
/// @author Ben Minerd
/// @date 3/14/2016
/// @brief InterruptSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <InterruptSTM32F30x.h>

using Plat4m::InterruptSTM32F30x;
using Plat4m::Interrupt;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const IRQn_Type InterruptSTM32F30x::myInterruptMap[] =
{
    NonMaskableInt_IRQn,     // ID_NON_MASKABLE
    MemoryManagement_IRQn,   // ID_MEMORY_MANAGEMENT
    BusFault_IRQn,           // ID_BUS_FAULT
    UsageFault_IRQn,         // ID_USAGE_FAULT
    SVCall_IRQn,             // ID_SV_CALL
    DebugMonitor_IRQn,       // ID_DEBUG_MONITOR
    PendSV_IRQn,             // ID_PEND_SV
    SysTick_IRQn,            // ID_SYS_TICK
    WWDG_IRQn,               // ID_WWDG
    PVD_IRQn,                // ID_PVD
    TAMPER_STAMP_IRQn,       // ID_TAMPER_STAMP
    RTC_WKUP_IRQn,           // ID_RTC_WAKE_UP
    FLASH_IRQn,              // ID_FLASH
    RCC_IRQn,                // ID_RCC
    EXTI0_IRQn,              // ID_EXTI_0
    EXTI1_IRQn,              // ID_EXTI_1
    EXTI2_TS_IRQn,           // ID_EXTI_2
    EXTI3_IRQn,              // ID_EXTI_3
    EXTI4_IRQn,              // ID_EXTI_4
    DMA1_Channel1_IRQn,      // ID_DMA_1_CHANNEL_1
    DMA1_Channel2_IRQn,      // ID_DMA_1_CHANNEL_2
    DMA1_Channel3_IRQn,      // ID_DMA_1_CHANNEL_3
    DMA1_Channel4_IRQn,      // ID_DMA_1_CHANNEL_4
    DMA1_Channel5_IRQn,      // ID_DMA_1_CHANNEL_5
    DMA1_Channel6_IRQn,      // ID_DMA_1_CHANNEL_6
    DMA1_Channel7_IRQn,      // ID_DMA_1_CHANNEL_7
    ADC1_2_IRQn,             // ID_ADC_1_2
    USB_HP_CAN1_TX_IRQn,     // ID_USB_HP_CAN_1_TX
    USB_LP_CAN1_RX0_IRQn,    // ID_USB_LP_CAN_1_RX0
    CAN1_RX1_IRQn,           // ID_CAN_1_RX1
    CAN1_SCE_IRQn,           // ID_CAN_1_SCE
    EXTI9_5_IRQn,            // ID_EXTI_9_5
    TIM1_BRK_TIM15_IRQn,     // ID_TIM_1_BRK_TIM_15
    TIM1_UP_TIM16_IRQn,      // ID_TIM_1_UP_TIM_16
    TIM1_TRG_COM_TIM17_IRQn, // ID_TIM_1_TRG_COM_TIM_17
    TIM1_CC_IRQn,            // ID_TIM_1_CC
    TIM2_IRQn,               // ID_TIM_2
    TIM3_IRQn,               // ID_TIM_3
    TIM4_IRQn,               // ID_TIM_4
    I2C1_EV_IRQn,            // ID_I2C_1_EV
    I2C1_ER_IRQn,            // ID_I2C_1_ER
    I2C2_EV_IRQn,            // ID_I2C_2_EV
    I2C2_ER_IRQn,            // ID_I2C_2_ER
    SPI1_IRQn,               // ID_SPI_1
    SPI2_IRQn,               // ID_SPI_2
    USART1_IRQn,             // ID_USART_1
    USART2_IRQn,             // ID_USART_2
    USART3_IRQn,             // ID_USART_3
    EXTI15_10_IRQn,          // ID_EXTI_15_10
    RTC_Alarm_IRQn,          // ID_RTC_ALARM
    USBWakeUp_IRQn,          // ID_USB_WAKE_UP
    TIM8_BRK_IRQn,           // ID_TIM_8_BRK
    TIM8_UP_IRQn,            // ID_TIM_8_UP
    TIM8_TRG_COM_IRQn,       // ID_TIM_8_TRG_COM
    TIM8_CC_IRQn,            // ID_TIM_8_CC
    ADC3_IRQn,               // ID_ADC_3
    FMC_IRQn,                // ID_FMC
    SPI3_IRQn,               // ID_SPI_3
    UART4_IRQn,              // ID_UART_4
    UART5_IRQn,              // ID_UART_5
    TIM6_DAC_IRQn,           // ID_TIM_6_DAC
    TIM7_IRQn,               // ID_TIM_7
    DMA2_Channel1_IRQn,      // ID_DMA_2_CHANNEL_1
    DMA2_Channel2_IRQn,      // ID_DMA_2_CHANNEL_2
    DMA2_Channel3_IRQn,      // ID_DMA_2_CHANNEL_3
    DMA2_Channel4_IRQn,      // ID_DMA_2_CHANNEL_4
    DMA2_Channel5_IRQn,      // ID_DMA_2_CHANNEL_5
    ADC4_IRQn,               // ID_ADC_4
    COMP1_2_3_IRQn,          // ID_COMP_1_2_3
    COMP4_5_6_IRQn,          // ID_COMP_4_5_6
    COMP7_IRQn,              // ID_COMP_7
    I2C3_EV_IRQn,            // ID_I2C_2_EV
    I2C3_ER_IRQn,            // ID_I2C_3_ER
    USB_HP_IRQn,             // ID_USB_HP
    USB_LP_IRQn,             // ID_USB_LP
    USBWakeUp_RMP_IRQn,      // ID_USB_WAKE_UP_RMP
    TIM20_BRK_IRQn,          // ID_TIM_20_BRK
    TIM20_UP_IRQn,           // ID_TIM_20_UP
    TIM20_TRG_COM_IRQn,      // ID_TIM_20_TRG_COM
    TIM20_CC_IRQn,           // ID_TIM_20_CC
    FPU_IRQn,                // ID_FPU
    SPI4_IRQn                // ID_SPI_4
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InterruptSTM32F30x::InterruptSTM32F30x(const Id id,
                                       HandlerCallback& handlerCallback) :
    Interrupt(handlerCallback),
    myId(id)
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InterruptSTM32F30x::driverSetEnabled(const bool enabled)
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
Interrupt::Error InterruptSTM32F30x::driverConfigure(const Config& config)
{
    NVIC_SetPriority(myInterruptMap[myId],
                     NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                                         config.priority,
                                         0));

    return Error(ERROR_CODE_NONE);
}
