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
/// @file InterruptSTM32F30x.h
/// @author Ben Minerd
/// @date 3/14/2016
/// @brief InterruptSTM32F30x class header file.
///

#ifndef PLAT4M_INTERRUPT_STM32F30X_H
#define PLAT4M_INTERRUPT_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Interrupt.h>
     
#include <stm32f30x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InterruptSTM32F30x : public Interrupt
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum Id
    {
        ID_NON_MASKABLE = 0,
        ID_MEMORY_MANAGEMENT,
        ID_BUS_FAULT,
        ID_USAGE_FAULT,
        ID_SV_CALL,
        ID_DEBUG_MONITOR,
        ID_PEND_SV,
        ID_SYS_TICK,
        ID_WWDG,
        ID_PVD,
        ID_TAMPER_STAMP,
        ID_RTC_WAKE_UP,
        ID_FLASH,
        ID_RCC,
        ID_EXTI_0,
        ID_EXTI_1,
        ID_EXTI_2,
        ID_EXTI_3,
        ID_EXTI_4,
        ID_DMA_1_CHANNEL_1,
        ID_DMA_1_CHANNEL_2,
        ID_DMA_1_CHANNEL_3,
        ID_DMA_1_CHANNEL_4,
        ID_DMA_1_CHANNEL_5,
        ID_DMA_1_CHANNEL_6,
        ID_DMA_1_CHANNEL_7,
        ID_ADC_1_2,
        ID_USB_HP_CAN_1_TX,
        ID_USB_LP_CAN_1_RX0,
        ID_CAN_1_RX1,
        ID_CAN_1_SCE,
        ID_EXTI_9_5,
        ID_TIM_1_BRK_TIM_15,
        ID_TIM_1_UP_TIM_16,
        ID_TIM_1_TRG_COM_TIM_17,
        ID_TIM_1_CC,
        ID_TIM_2,
        ID_TIM_3,
        ID_TIM_4,
        ID_I2C_1_EV,
        ID_I2C_1_ER,
        ID_I2C_2_EV,
        ID_I2C_2_ER,
        ID_SPI_1,
        ID_SPI_2,
        ID_USART_1,
        ID_USART_2,
        ID_USART_3,
        ID_EXTI_15_10,
        ID_RTC_ALARM,
        ID_USB_WAKE_UP,
        ID_TIM_8_BRK,
        ID_TIM_8_UP,
        ID_TIM_8_TRG_COM,
        ID_TIM_8_CC,
        ID_ADC_3,
        ID_FMC,
        ID_SPI_3,
        ID_UART_4,
        ID_UART_5,
        ID_TIM_6_DAC,
        ID_TIM_7,
        ID_DMA_2_CHANNEL_1,
        ID_DMA_2_CHANNEL_2,
        ID_DMA_2_CHANNEL_3,
        ID_DMA_2_CHANNEL_4,
        ID_DMA_2_CHANNEL_5,
        ID_ADC_4,
        ID_COMP_1_2_3,
        ID_COMP_4_5_6,
        ID_COMP_7,
        ID_I2C_3_EV,
        ID_I2C_3_ER,
        ID_USB_HP,
        ID_USB_LP,
        ID_USB_WAKE_UP_RMP,
        ID_TIM_20_BRK,
        ID_TIM_20_UP,
        ID_TIM_20_TRG_COM,
        ID_TIM_20_CC,
        ID_FPU,
        ID_SPI_4
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    InterruptSTM32F30x(const Id id, HandlerCallback& handlerCallback);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const IRQn_Type myInterruptMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from Interrupt
    //--------------------------------------------------------------------------
    
    Error driverConfigure(const Config& config);
};

}; // namespace Plat4m

#endif // PLAT4M_INTERRUPT_STM32F30X_H
