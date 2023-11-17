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
/// @file InterruptSTM32H7xx.h
/// @author Ben Minerd
/// @date 6/27/2022
/// @brief InterruptSTM32H7xx class header file.
///

#ifndef PLAT4M_INTERRUPT_STM32H7XX_H
#define PLAT4M_INTERRUPT_STM32H7XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32h7xx.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Interrupt.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InterruptSTM32H7xx : public Interrupt
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_NON_MASKABLE = 0,
        ID_HARD_FAULT,
        ID_MEMORY_MANAGEMENT,
        ID_BUS_FAULT,
        ID_USAGE_FAULT,
        ID_SV_CALL,
        ID_DEBUG_MONITOR,
        ID_PEND_SV,
        ID_SYS_TICK,
        ID_WWDG,
        ID_PVD_AVD,
        ID_TAMPER_AND_TIMESTAMP,
        ID_RTC_WAKE_UP,
        ID_FLASH,
        ID_RCC,
        ID_EXTI_0,
        ID_EXTI_1,
        ID_EXTI_2,
        ID_EXTI_3,
        ID_EXTI_4,
        ID_DMA_1_STREAM_0,
        ID_DMA_1_STREAM_1,
        ID_DMA_1_STREAM_2,
        ID_DMA_1_STREAM_3,
        ID_DMA_1_STREAM_4,
        ID_DMA_1_STREAM_5,
        ID_DMA_1_STREAM_6,
        ID_ADC_1_AND_2,
        ID_FD_CAN_1_IT_0,
        ID_FD_CAN_2_IT_0,
        ID_FD_CAN_1_IT_1,
        ID_FD_CAN_2_IT_1,
        ID_EXTI_9_TO_5,
        ID_TIM_1_BRK,
        ID_TIM_1_UP,
        ID_TIM_1_TRG_AND_COM,
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
        ID_EXTI_15_TO_10,
        ID_RTC_ALARM_A_AND_B,
        ID_TIM_8_BRK_AND_TIM_12,
        ID_TIM_8_UP_AND_TIM_13,
        ID_TIM_8_TRG_AND_COM_AND_TIM_14,
        ID_TIM_8_CC,
        ID_DMA_1_STREAM_7,
        ID_FMC,
        ID_SDMMC,
        ID_TIM_5,
        ID_SPI_3,
        ID_UART_4,
        ID_UART_5,
        ID_TIM_6_AND_DAC_1_AND_2,
        ID_TIM_7,
        ID_DMA_2_STREAM_0,
        ID_DMA_2_STREAM_1,
        ID_DMA_2_STREAM_2,
        ID_DMA_2_STREAM_3,
        ID_DMA_2_STREAM_4,
        ID_ETH,
        ID_ETH_WKUP,
        ID_FD_CAN,
        ID_DMA_2_STREAM_5,
        ID_DMA_2_STREAM_6,
        ID_DMA_2_STREAM_7,
        ID_USART_6,
        ID_I2C_3_EV,
        ID_I2C_3_ER,
        ID_USB_OTG_HS_EP_1_OUT,
        ID_USB_OTG_HS_EP_1_IN,
        ID_USB_OTG_HS_WKUP,
        ID_USB_OTG_HS,
        ID_DCMI,
        ID_CRYP,
        ID_HASH_AND_RNG,
        ID_FPU,
        ID_UART_7,
        ID_UART_8,
        ID_SPI_4,
        ID_SPI_5,
        ID_SPI_6,
        ID_SAI_1,
        ID_LTDC,
        ID_LTDC_ER,
        ID_DMA2D,
        ID_SAI_2,
        ID_QUAD_SPI,
        ID_LP_TIM_1,
        ID_HDMI_CEC,
        ID_I2C_4_EV,
        ID_I2C_4_ER,
        ID_SPDIF_RX,
        ID_USB_OTG_FS_EP_1_OUT,
        ID_USB_OTG_FS_EP_1_IN,
        ID_USB_FS_WKUP,
        ID_USB_FS,
        ID_DMA_MUX_1_OVR,
        ID_HR_TIM_1_MASTER,
        ID_HR_TIM_1_TIM_A,
        ID_HR_TIM_1_TIM_B,
        ID_HR_TIM_1_TIM_C,
        ID_HR_TIM_1_TIM_D,
        ID_HR_TIM_1_TIM_E,
        ID_HR_TIM_1_FLT,
        ID_DFSDM_1_FLT_0,
        ID_DFSDM_1_FLT_1,
        ID_DFSDM_1_FLT_2,
        ID_DFSDM_1_FLT_3,
        ID_SAI_3,
        ID_SWMPI_1,
        ID_TIM_15,
        ID_TIM_16,
        ID_TIM_17,
        ID_MDIOS_WKUP,
        ID_MDIOS,
        ID_JPEG,
        ID_MDMA,
        ID_SDMMC_2,
        ID_HSEM_1,
        ID_ADC_3,
        ID_DMA_MUX_2_OVR,
        ID_BDMA_CHANNEL_0,
        ID_BDMA_CHANNEL_1,
        ID_BDMA_CHANNEL_2,
        ID_BDMA_CHANNEL_3,
        ID_BDMA_CHANNEL_4,
        ID_BDMA_CHANNEL_5,
        ID_BDMA_CHANNEL_6,
        ID_BDMA_CHANNEL_7,
        ID_COMP,
        ID_LP_TIM_2,
        ID_LP_TIM_3,
        ID_LP_TIM_4,
        ID_LP_TIM_5,
        ID_LP_UART_1,
        ID_CRS,
        ID_ECC,
        ID_SAI_4,
        ID_WAKEUP_PIN
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    InterruptSTM32H7xx(const Id id, HandlerCallback& handlerCallback);

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
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled) override;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Interrupt
    //--------------------------------------------------------------------------

    virtual Error driverConfigure(const Config& config) override;
};

}; // namespace Plat4m

#endif // PLAT4M_INTERRUPT_STM32F30X_H
