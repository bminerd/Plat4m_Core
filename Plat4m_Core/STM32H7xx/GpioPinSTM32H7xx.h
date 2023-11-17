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
/// @file GpioPinSTM32H7xx.h
/// @author Ben Minerd
/// @date 6/22/2022
/// @brief GpioPinSTM32H7xx class header file.
///

#ifndef PLAT4M_GPIO_PIN_STM32H7XX_H
#define PLAT4M_GPIO_PIN_STM32H7XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32h7xx.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/GpioPin.h>
#include <Plat4m_Core/STM32H7xx/GpioPortSTM32H7xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class GpioPinSTM32H7xx : public GpioPin
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_0 = 0,
        ID_1,
        ID_2,
        ID_3,
        ID_4,
        ID_5,
        ID_6,
        ID_7,
        ID_8,
        ID_9,
        ID_10,
        ID_11,
        ID_12,
        ID_13,
        ID_14,
        ID_15
    };

    enum AlternateFunction
    {
        ALTERNATE_FUNCTION_0   = 0,
        ALTERNATE_FUNCTION_SYS = ALTERNATE_FUNCTION_0,

        ALTERNATE_FUNCTION_1     = 1,
        ALTERNATE_FUNCTION_TIM_1 = ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_TIM_2 = ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_TIM_16 = ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_TIM_17 = ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_LPTIM_1 = ALTERNATE_FUNCTION_1,
        ALTERNATE_FUNCTION_HRTIM_1 = ALTERNATE_FUNCTION_1,

        ALTERNATE_FUNCTION_2 = 2,
        ALTERNATE_FUNCTION_SAI_1 = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM_3 = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM_4 = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM_5 = ALTERNATE_FUNCTION_2,
        ALTERNATE_FUNCTION_TIM_12 = ALTERNATE_FUNCTION_2,
        // ALTERNATE_FUNCTION_HRTIM_1 = ALTERNATE_FUNCTION_2,

        ALTERNATE_FUNCTION_3 = 3,
        ALTERNATE_FUNCTION_LPUART = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_TIM_8 = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_LPTIM_2 = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_LPTIM_3 = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_LPTIM_4 = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_LPTIM_5 = ALTERNATE_FUNCTION_3,
        // ALTERNATE_FUNCTION_HRTIM_1 = ALTERNATE_FUNCTION_3,
        ALTERNATE_FUNCTION_DFSDM_1 = ALTERNATE_FUNCTION_3,

        ALTERNATE_FUNCTION_4 = 4,
        ALTERNATE_FUNCTION_I2C_1 = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_I2C_2 = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_I2C_3 = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_I2C_4 = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_USART_1 = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_TIM_15 = ALTERNATE_FUNCTION_4,
        // ALTERNATE_FUNCTION_LPTIM_2 = ALTERNATE_FUNCTION_4,
        // ALTERNATE_FUNCTION_DFSDM_1 = ALTERNATE_FUNCTION_4,
        ALTERNATE_FUNCTION_CEC = ALTERNATE_FUNCTION_4,

        ALTERNATE_FUNCTION_5 = 5,
        ALTERNATE_FUNCTION_SPI_1 = ALTERNATE_FUNCTION_5,
        // ALTERNATE_FUNCTION_SPI_2 = ALTERNATE_FUNCTION_5,
        // ALTERNATE_FUNCTION_SPI_3 = ALTERNATE_FUNCTION_5,
        ALTERNATE_FUNCTION_SPI_4 = ALTERNATE_FUNCTION_5,
        ALTERNATE_FUNCTION_SPI_5 = ALTERNATE_FUNCTION_5,
        ALTERNATE_FUNCTION_SPI_6 = ALTERNATE_FUNCTION_5,
        // ALTERNATE_FUNCTION_CEC = ALTERNATE_FUNCTION_5,

        ALTERNATE_FUNCTION_6 = 6,
        // ALTERNATE_FUNCTION_SPI_2 = ALTERNATE_FUNCTION_6,
        // ALTERNATE_FUNCTION_SPI_3 = ALTERNATE_FUNCTION_6,
        // ALTERNATE_FUNCTION_SAI_1 = ALTERNATE_FUNCTION_6,
        ALTERNATE_FUNCTION_SAI_3 = ALTERNATE_FUNCTION_6,
        // ALTERNATE_FUNCTION_I2C_4 = ALTERNATE_FUNCTION_6,
        ALTERNATE_FUNCTION_UART_4 = ALTERNATE_FUNCTION_6,
        // ALTERNATE_FUNCTION_DFSDM_1 = ALTERNATE_FUNCTION_6,

        ALTERNATE_FUNCTION_7 = 7,
        // ALTERNATE_FUNCTION_SPI_2 = ALTERNATE_FUNCTION_7,
        // ALTERNATE_FUNCTION_SPI_3 = ALTERNATE_FUNCTION_7,
        // ALTERNATE_FUNCTION_SPI_6 = ALTERNATE_FUNCTION_7,
        // ALTERNATE_FUNCTION_USART_1 = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_USART_2 = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_USART_3 = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_USART_6 = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_UART_7 = ALTERNATE_FUNCTION_7,
        ALTERNATE_FUNCTION_SDMMC_1 = ALTERNATE_FUNCTION_7,

        ALTERNATE_FUNCTION_8 = 8,
        // ALTERNATE_FUNCTION_SPI_6 = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_SAI_2 = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_SAI_4 = ALTERNATE_FUNCTION_8,
        // ALTERNATE_FUNCTION_UART_4 = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_UART_5 = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_UART_8 = ALTERNATE_FUNCTION_8,
        // ALTERNATE_FUNCTION_LPUART = ALTERNATE_FUNCTION_8,
        // ALTERNATE_FUNCTION_SDMMC_1 = ALTERNATE_FUNCTION_8,
        ALTERNATE_FUNCTION_SPDIFRX_1 = ALTERNATE_FUNCTION_8,

        ALTERNATE_FUNCTION_9 = 9,
        // ALTERNATE_FUNCTION_SAI_4 = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_FDCAN_1 = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_FDCAN_2 = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM_13 = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_TIM_14 = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_QUADSPI = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_FMC = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_SDMMC_2 = ALTERNATE_FUNCTION_9,
        ALTERNATE_FUNCTION_LCD = ALTERNATE_FUNCTION_9,
        // ALTERNATE_FUNCTION_SPDIFRX_1 = ALTERNATE_FUNCTION_9,

        ALTERNATE_FUNCTION_10 = 10,
        // ALTERNATE_FUNCTION_SAI_2 = ALTERNATE_FUNCTION_10,
        // ALTERNATE_FUNCTION_SAI_4 = ALTERNATE_FUNCTION_10,
        // ALTERNATE_FUNCTION_TIM_8 = ALTERNATE_FUNCTION_10,
        // ALTERNATE_FUNCTION_QUADSPI = ALTERNATE_FUNCTION_10,
        // ALTERNATE_FUNCTION_SDMMC_2 = ALTERNATE_FUNCTION_10,
        ALTERNATE_FUNCTION_OTG_1_HS = ALTERNATE_FUNCTION_10,
        ALTERNATE_FUNCTION_OTG_2_FS = ALTERNATE_FUNCTION_10,
        // ALTERNATE_FUNCTION_LCD = ALTERNATE_FUNCTION_10,

        ALTERNATE_FUNCTION_11 = 11,
        // ALTERNATE_FUNCTION_I2C_4 = ALTERNATE_FUNCTION_11,
        // ALTERNATE_FUNCTION_UART_7 = ALTERNATE_FUNCTION_11,
        ALTERNATE_FUNCTION_SWPMI_1 = ALTERNATE_FUNCTION_11,
        // ALTERNATE_FUNCTION_TIM_1 = ALTERNATE_FUNCTION_11,
        // ALTERNATE_FUNCTION_TIM_8 = ALTERNATE_FUNCTION_11,
        // ALTERNATE_FUNCTION_DFSDM_1 = ALTERNATE_FUNCTION_11,
        // ALTERNATE_FUNCTION_SDMMC_2 = ALTERNATE_FUNCTION_11,
        ALTERNATE_FUNCTION_MDIOS = ALTERNATE_FUNCTION_11,
        ALTERNATE_FUNCTION_ETH = ALTERNATE_FUNCTION_11,

        ALTERNATE_FUNCTION_12 = 12,
        // ALTERNATE_FUNCTION_TIM_1 = ALTERNATE_FUNCTION_12,
        // ALTERNATE_FUNCTION_TIM_8 = ALTERNATE_FUNCTION_12,
        // ALTERNATE_FUNCTION_FMC = ALTERNATE_FUNCTION_12,
        // ALTERNATE_FUNCTION_SDMMC_1 = ALTERNATE_FUNCTION_12,
        // ALTERNATE_FUNCTION_MDIOS = ALTERNATE_FUNCTION_12,
        ALTERNATE_FUNCTION_OTG_1_FS = ALTERNATE_FUNCTION_12,
        // ALTERNATE_FUNCTION_LCD = ALTERNATE_FUNCTION_12,

        ALTERNATE_FUNCTION_13 = 13,
        // ALTERNATE_FUNCTION_TIM_1 = ALTERNATE_FUNCTION_13,
        ALTERNATE_FUNCTION_DCMI = ALTERNATE_FUNCTION_13,
        // ALTERNATE_FUNCTION_LCD = ALTERNATE_FUNCTION_13,
        ALTERNATE_FUNCTION_COMP = ALTERNATE_FUNCTION_13,

        ALTERNATE_FUNCTION_14 = 14,
        // ALTERNATE_FUNCTION_UART_5 = ALTERNATE_FUNCTION_14,
        // ALTERNATE_FUNCTION_LCD = ALTERNATE_FUNCTION_14,

        ALTERNATE_FUNCTION_15 = 15,
        // ALTERNATE_FUNCTION_SYS = ALTERNATE_FUNCTION_15
    };

    enum OutputType
    {
        OUTPUT_TYPE_PUSH_PULL = 0,
        OUTPUT_TYPE_OPEN_DRAIN
    };

    enum OutputSpeed
    {
        OUTPUT_SPEED_LOW = 0,
        OUTPUT_SPEED_MEDIUM,
        OUTPUT_SPEED_HIGH,
        OUTPUT_SPEED_VERY_HIGH
    };

    struct STM32H7xxConfig
    {
        AlternateFunction alternateFunction;
        OutputType outputType;
        OutputSpeed outputSpeed;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    GpioPinSTM32H7xx(GpioPortSTM32H7xx& gpioPort, const Id id);

    //--------------------------------------------------------------------------
    // Public methods implemented from GpioPin
    //--------------------------------------------------------------------------

    void setLevelFast(const Level level);

    Level getLevelFast();

    Level readLevelFast();

    void toggleLevelFast();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    GpioPortSTM32H7xx& getGpioPort();

    Id getId() const;

    void setSTM32H7xxConfig(STM32H7xxConfig& config);

    void setOutputType(const OutputType outputType);

    void setAlternateFunction(const AlternateFunction alternateFunction);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const std::uint32_t myModeMap[];

    static const std::uint32_t myResistorMap[];

    static const OutputSpeed myDefaultOutputSpeed;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    GpioPortSTM32H7xx& myGpioPort;

    const Id myId;

    const std::uint16_t myPinBitMask;

    STM32H7xxConfig mySTM32H7xxConfig;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from GpioPin
    //--------------------------------------------------------------------------

    GpioPin::Error driverConfigure(const Config& config);

    GpioPin::Error driverSetLevel(const Level level);

    GpioPin::Error driverGetLevel(Level& level);

    GpioPin::Error driverReadLevel(Level& level);

    GpioPin::Error driverToggleLevel();

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void setOutputSpeed(const OutputSpeed outputSpeed);
};

}; // namespace Plat4m

#endif // PLAT4M_GPIO_PIN_STM32H7XX_H
