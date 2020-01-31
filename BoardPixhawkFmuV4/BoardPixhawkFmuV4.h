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
// Copyright (c) 2019 Benjamin Minerd
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
/// @file BoardPixhawkFmuV4.h
/// @author Ben Minerd
/// @date 7/25/2019
/// @brief BoardPixhawkFmuV4 class.
///

#ifndef BOARD_PIXHAWK_FMU_V4_H
#define BOARD_PIXHAWK_FMU_V4_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Board.h>
#include <ProcessorSTM32F4xx.h>
#include <GpioPinSTM32F4xx.h>
#include <I2cSTM32F4xx.h>
#include <UartSTM32F4xx.h>
#include <Button.h>
#include <EnableLine.h>
#include <GpioPin.h>
#include <PwmOutputSTM32F4xx.h>
#include <SpiSTM32F4xx.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BoardPixhawkFmuV4 : public Board
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    enum CanId
    {
    	CAN_ID_1,
	CAN_ID_2
    };

    enum GpioPinId
    {
    	GPIO_PIN_ID_20608_CS,
	GPIO_PIN_ID_20608_DRDY,
	GPIO_PIN_ID_8266_CTS,
	GPIO_PIN_ID_8266_GPIO0,
    	GPIO_PIN_ID_8266_GPIO2,
	GPIO_PIN_ID_8266_PD,
	GPIO_PIN_ID_8266_RST,
    	GPIO_PIN_ID_8266_RTS,
    	GPIO_PIN_ID_ALARM,
	GPIO_PIN_ID_BARO_CS,
    	GPIO_PIN_ID_BATT_CURRENT_SENS,
	GPIO_PIN_ID_BATT_VOLT_SENS,
	GPIO_PIN_ID_CAN1_RX,
	GPIO_PIN_ID_CAN1_TX,
	GPIO_PIN_ID_CAN2_RX,
	GPIO_PIN_ID_CAN2_TX,
	GPIO_PIN_ID_FMU_CH1,
	GPIO_PIN_ID_FMU_CH2,
	GPIO_PIN_ID_FMU_CH3,
	GPIO_PIN_ID_FMU_CH4,
	GPIO_PIN_ID_FMU_CH5,
	GPIO_PIN_ID_FMU_CH6,
    	GPIO_PIN_ID_FMU_I2C1_SCL,
	GPIO_PIN_ID_FMU_I2C1_SDA,
	GPIO_PIN_ID_FMU_LED_BLUE,
	GPIO_PIN_ID_FMU_LED_GREEN,
	GPIO_PIN_ID_FMU_LED_RED,
    	GPIO_PIN_ID_FMU_RC_INPUT,
	GPIO_PIN_ID_FMU_SWDIO,
	GPIO_PIN_ID_FMU_SWCLK,
	GPIO_PIN_ID_FMU_USART1_RX,
    	GPIO_PIN_ID_FMU_USART1_TX,
	GPIO_PIN_ID_FMU_USART2_CTS,
	GPIO_PIN_ID_FMU_USART2_RTS,
	GPIO_PIN_ID_FMU_USART2_RX,
	GPIO_PIN_ID_FMU_USART2_TX,
	GPIO_PIN_ID_FMU_USART3_CTS,
	GPIO_PIN_ID_FMU_USART3_RTS,
	GPIO_PIN_ID_FMU_USART3_RX,
	GPIO_PIN_ID_FMU_USART3_TX,
	GPIO_PIN_ID_FMU_USART4_RX,
	GPIO_PIN_ID_FMU_USART4_TX,
	GPIO_PIN_ID_FMU_USART7_RX,
	GPIO_PIN_ID_FMU_USART7_TX,
	GPIO_PIN_ID_FMU_USART8_RX,
	GPIO_PIN_ID_FMU_USART8_TX,
	GPIO_PIN_ID_FRAM_CS,
	GPIO_PIN_ID_FRAM_MISO,
	GPIO_PIN_ID_FRAM_MOSI,
	GPIO_PIN_ID_FRAM_SCK,
	GPIO_PIN_ID_FRSKY_INV,
	GPIO_PIN_ID_HMC5983_DRDY,
	GPIO_PIN_ID_HMC5983_CS,
	GPIO_PIN_ID_LED_SAFETY,
	GPIO_PIN_ID_MPU9250_CS,
	GPIO_PIN_ID_MPU9250_DRDY,
	GPIO_PIN_ID_MPU9250_MISO,
	GPIO_PIN_ID_MPU9250_SCK,
	GPIO_PIN_ID_MPU9250_MOSI,
	GPIO_PIN_ID_OTG_FS_DM,
	GPIO_PIN_ID_OTG_FS_DP,
	GPIO_PIN_ID_RC_INPUT,
	GPIO_PIN_ID_RSSI_IN,
	GPIO_PIN_ID_SAFETY_SWITCH_EN,
	GPIO_PIN_ID_SBUS_INV,
	GPIO_PIN_ID_SDIO_CK,
	GPIO_PIN_ID_SDIO_CMD,
	GPIO_PIN_ID_SDIO_D0,
	GPIO_PIN_ID_SDIO_D1,
	GPIO_PIN_ID_SDIO_D2,
	GPIO_PIN_ID_SDIO_D3,
	GPIO_PIN_ID_SPEKTRUM_POWER,
	GPIO_PIN_ID_VBUS,
	GPIO_PIN_ID_VBUS_VALID,
	GPIO_PIN_ID_VDD_3V3_PERIPH_EN,
	GPIO_PIN_ID_VDD_3V3_SENSORS_EN,
	GPIO_PIN_ID_VDD_5V_SENS,
	GPIO_PIN_ID_VDD_BRICK_VALID
    };

    enum PwmOutputId
    {
    	PWM_OUTPUT_ID_CH_1,
	PWM_OUTPUT_ID_CH_2,
	PWM_OUTPUT_ID_CH_3,
	PWM_OUTPUT_ID_CH_4,
	PWM_OUTPUT_ID_CH_5,
	PWM_OUTPUT_ID_CH_6
    };

    enum SpiId
    {
    	SPI_ID_1,
	SPI_ID_MPU9250 = SPI_ID_1,
	SPI_ID_2,
	SPI_ID_FRAM = SPI_ID_2
    };

    enum UartId
    {
    	UART_ID_1,
	UART_ID_EPS8266 = UART_ID_1,
    	UART_ID_2,
	UART_ID_TELEMETRY_1 = UART_ID_2,
	UART_ID_3,
	UART_ID_TELEMETRY_2 = UART_ID_3,
	UART_ID_OSD         = UART_ID_3,
	UART_ID_4,
	UART_ID_7,
	UART_ID_8,
	UART_ID_FRSKY = UART_ID_8
    };


    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    BoardPixhawkFmuV4();

    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

    ~BoardPixhawkFmuV4();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

//    CanSTM32F4xx& getCan(const CanId canID);

    EnableLine& getEnableLine();

    GpioPinSTM32F4xx& getGpioPin(const GpioPinId gpioPinId);

    ProcessorSTM32F4xx& getProcessor();

    PwmOutputSTM32F4xx& getPwmOutput(const PwmOutputId pwmOutputId);

    I2cSTM32F4xx& getI2c();

//    ImuMpu9250& getImu();

    SpiSTM32F4xx& getSpi(const SpiId spiId);

    UartSTM32F4xx& getUart(const UartId uartId);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const float myProcessorCoreVoltage;

    static const uint32_t myProcessorExternalClockFrequencyHz;

    static const GpioPin::Id myGpioPinIdMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    GpioPinSTM32F4xx* myGpioPins[81];

    I2cSTM32F4xx* myI2c;

    ProcessorSTM32F4xx* myProcessor;

    PwmOutputSTM32F4xx* myPwmOutputs[6];

    SpiSTM32F4xx* mySpis[2];

    UartSTM32F4xx* myUarts[8];
};

}; // namespace Plat4m

#endif // BOARD_PIXHAWK_FMU_V4_H
