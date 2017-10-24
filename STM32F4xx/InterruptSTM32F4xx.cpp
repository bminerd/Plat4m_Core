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
/// @file InterruptSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 6/20/2016
/// @brief InterruptSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <InterruptSTM32F4xx.h>

#include <misc.h>

using Plat4m::InterruptSTM32F4xx;
using Plat4m::Interrupt;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private constructors
//------------------------------------------------------------------------------

const IRQn_Type InterruptSTM32F4xx::myIrqNumberMap[] =
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
    TAMP_STAMP_IRQn,         // ID_TAMPER_STAMP
    RTC_WKUP_IRQn,           // ID_RTC_WAKE_UP
    FLASH_IRQn,              // ID_FLASH
    RCC_IRQn,                // ID_RCC
    EXTI0_IRQn,              // ID_EXTI_0
    EXTI1_IRQn,              // ID_EXTI_1
    EXTI2_IRQn,              // ID_EXTI_2
    EXTI3_IRQn,              // ID_EXTI_3
    EXTI4_IRQn,              // ID_EXTI_4
    DMA1_Stream0_IRQn,       // ID_DMA_1_STREAM_0
    DMA1_Stream1_IRQn,       // ID_DMA_1_STREAM_1
    DMA1_Stream2_IRQn,       // ID_DMA_1_STREAM_2
    DMA1_Stream3_IRQn,       // ID_DMA_1_STREAM_3
    DMA1_Stream4_IRQn,       // ID_DMA_1_STREAM_4
    DMA1_Stream5_IRQn,       // ID_DMA_1_STREAM_5
    DMA1_Stream6_IRQn,       // ID_DMA_1_STREAM_6
    ADC_IRQn,                // ID_ADC_1_2_3
    CAN1_TX_IRQn,            // ID_CAN_1_TX
    CAN1_RX0_IRQn,           // ID_CAN_1_RX0
    CAN1_RX1_IRQn,           // ID_CAN_1_RX1
    CAN1_SCE_IRQn,           // ID_CAN_1_SCE
    EXTI9_5_IRQn,            // ID_EXTI_9_5
    TIM1_BRK_TIM9_IRQn,      // ID_TIM_1_BRK_TIM_9
    TIM1_UP_TIM10_IRQn,      // ID_TIM_1_UP_TIM_10
    TIM1_TRG_COM_TIM11_IRQn, // ID_TIM_1_TRG_COM_TIM_11
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
    OTG_FS_WKUP_IRQn,        // ID_OTG_FS_WAKE_UP
    TIM8_BRK_TIM12_IRQn,     // ID_TIM_8_BRK_TIM_12
    TIM8_UP_TIM13_IRQn,      // ID_TIM_8_UP_TIM_13
    TIM8_TRG_COM_TIM14_IRQn, // ID_TIM_8_TRG_COM_TIM_14
    TIM8_CC_IRQn,            // ID_TIM_8_CC
    DMA1_Stream7_IRQn,       // ID_DMA_1_STREAM_7
    FSMC_IRQn,               // ID_FMC
    SDIO_IRQn,               // ID_SDIO
    TIM5_IRQn,               // ID_TIM_5
    SPI3_IRQn,               // ID_SPI_3
    UART4_IRQn,              // ID_UART_4
    UART5_IRQn,              // ID_UART_5
    TIM6_DAC_IRQn,           // ID_TIM_6_DAC
    TIM7_IRQn,               // ID_TIM_7
    DMA2_Stream0_IRQn,       // ID_DMA_2_STREAM_0
    DMA2_Stream1_IRQn,       // ID_DMA_2_STREAM_1
    DMA2_Stream2_IRQn,       // ID_DMA_2_STREAM_2
    DMA2_Stream3_IRQn,       // ID_DMA_2_STREAM_3
    DMA2_Stream4_IRQn,       // ID_DMA_2_STREAM_4
    ETH_IRQn,                // ID_ETH,
    ETH_WKUP_IRQn,           // ID_ETH_WKUP,
    CAN2_TX_IRQn,            // ID_CAN_2_TX
    CAN2_RX0_IRQn,           // ID_CAN_2_RX0
    CAN2_RX1_IRQn,           // ID_CAN_2_RX_1
    CAN2_SCE_IRQn,           // ID_CAN_2_SCE
    OTG_FS_IRQn,             // ID_OTG_FS
    DMA2_Stream5_IRQn,       // ID_DMA_2_STREAM_5
    DMA2_Stream6_IRQn,       // ID_DMA_2_STREAM_6
    DMA2_Stream7_IRQn,       // ID_DMA_2_STREAM_7
    USART6_IRQn,             // ID_USART_6
    I2C3_EV_IRQn,            // ID_I2C_3_EV
    I2C3_ER_IRQn,            // ID_I2C_3_ER
    OTG_HS_EP1_OUT_IRQn,     // ID_OTG_HS_EP1_OUT
    OTG_HS_EP1_IN_IRQn,      // ID_OTG_HS_EP1_IN
    OTG_HS_WKUP_IRQn,        // ID_OTG_HS_WAKE_UP
    DCMI_IRQn,               // ID_DCMI
    CRYP_IRQn,               // ID_CRYP,
    HASH_RNG_IRQn,           // ID_HASH_RNG,
    FPU_IRQn                 // ID_FPU
};

//------------------------------------------------------------------------------
// Private constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
InterruptSTM32F4xx::InterruptSTM32F4xx(const Id id) :
    Interrupt(),
    myId(id)
{
}

//------------------------------------------------------------------------------
InterruptSTM32F4xx::InterruptSTM32F4xx(const Id id,
                                       HandlerCallback& handlerCallback) :
    Interrupt(handlerCallback),
    myId(id)
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error InterruptSTM32F4xx::driverSetEnabled(const bool enabled)
{
    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel                   = myIrqNumberMap[myId];
    nvicInit.NVIC_IRQChannelPreemptionPriority = 0x00;
    nvicInit.NVIC_IRQChannelSubPriority        = 0x00;
    nvicInit.NVIC_IRQChannelCmd                = (FunctionalState) enabled;

    NVIC_Init(&nvicInit);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Interrupt
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Interrupt::Error InterruptSTM32F4xx::driverConfigure(const Config& config)
{
    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel                   = myIrqNumberMap[myId];
    nvicInit.NVIC_IRQChannelPreemptionPriority = config.priority;
    nvicInit.NVIC_IRQChannelSubPriority        = 0x00;
    nvicInit.NVIC_IRQChannelCmd                = ENABLE;

    NVIC_Init(&nvicInit);
    
    return Error(ERROR_CODE_NONE);
}
