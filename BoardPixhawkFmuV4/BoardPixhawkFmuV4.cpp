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
/// @file BoardPixhawkFmuV4.cpp
/// @author Ben Minerd
/// @date 7/25/2019
/// @brief BoardPixhawkFmuV4 class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <BoardPixhawkFmuV4.h>

using Plat4m::BoardPixhawkFmuV4;
using Plat4m::GpioPin;
using Plat4m::GpioSTM32F4xx;
using Plat4m::GpioPinSTM32F4xx;
using Plat4m::Button;
using Plat4m::EnableLine;
using Plat4m::ProcessorSTM32F4xx;
using Plat4m::I2cSTM32F4xx;
using Plat4m::UartSTM32F4xx;

//------------------------------------------------------------------------------
// Private static data member initialization
//------------------------------------------------------------------------------

const float BoardPixhawkFmuV4::myProcessorCoreVoltage                 = 3.3;
const uint32_t BoardPixhawkFmuV4::myProcessorExternalClockFrequencyHz
                                                           = 24000000; // 24 MHz

const GpioPin::Id BoardPixhawkFmuV4::myGpioPinIdMap[] =
{
	/// GPIO_PIN_ID_20608_CS
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_15},
	/// GPIO_PIN_ID_20608_DRDY
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_14},
	/// GPIO_PIN_ID_8266_CTS
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_10},
	/// GPIO_PIN_ID_8266_GPIO0
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_2},
	/// GPIO_PIN_ID_8266_GPIO2
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_4},
	/// GPIO_PIN_ID_8266_PD
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_5},
	/// GPIO_PIN_ID_8266_RST
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_6},
	/// GPIO_PIN_ID_8266_RTS
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_8},
	/// GPIO_PIN_ID_ALARM
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_15},
	/// GPIO_PIN_ID_BARO_CS
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_7},
	/// GPIO_PIN_ID_BATT_CURRENT_SENS
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_3},
	/// GPIO_PIN_ID_BATT_VOLT_SENS
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_2},
	/// GPIO_PIN_ID_CAN1_RX
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_0},
	/// GPIO_PIN_ID_CAN1_TX
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_1},
	/// GPIO_PIN_ID_CAN2_RX
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_12},
	/// GPIO_PIN_ID_CAN2_TX
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_13},
	/// GPIO_PIN_ID_FMU_CH1
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_14},
	/// GPIO_PIN_ID_FMU_CH2
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_13},
	/// GPIO_PIN_ID_FMU_CH3
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_11},
	/// GPIO_PIN_ID_FMU_CH4
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_9},
	/// GPIO_PIN_ID_FMU_CH5
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_13},
	/// GPIO_PIN_ID_FMU_CH6,
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_14},
	/// GPIO_PIN_ID_FMU_I2C1_SCL
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_8},
	/// GPIO_PIN_ID_FMU_I2C1_SDA
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_9},
	/// GPIO_PIN_ID_FMU_LED_BLUE
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_3},
	/// GPIO_PIN_ID_FMU_LED_GREEN
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_1},
	/// GPIO_PIN_ID_FMU_LED_RED
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_11},
	/// GPIO_PIN_ID_FMU_RC_INPUT
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_7},
	/// GPIO_PIN_ID_FMU_SWDIO
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_13},
	/// GPIO_PIN_ID_FMU_SWCLK
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_14},
	/// GPIO_PIN_ID_FMU_USART1_RX
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_7},
	/// GPIO_PIN_ID_FMU_USART1_TX
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_6},
	/// GPIO_PIN_ID_FMU_USART2_CTS
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_3},
	/// GPIO_PIN_ID_FMU_USART2_RTS
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_4},
	/// GPIO_PIN_ID_FMU_USART2_RX
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_6},
	/// GPIO_PIN_ID_FMU_USART2_TX
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_5},
	/// GPIO_PIN_ID_FMU_USART3_CTS
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_11},
	/// GPIO_PIN_ID_FMU_USART3_RTS
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_12},
	/// GPIO_PIN_ID_FMU_USART3_RX
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_9},
	/// GPIO_PIN_ID_FMU_USART3_TX
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_8},
	/// GPIO_PIN_ID_FMU_USART4_RX
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_1},
	/// GPIO_PIN_ID_FMU_USART4_TX
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_0},
	/// GPIO_PIN_ID_FMU_USART7_RX
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_7},
	/// GPIO_PIN_ID_FMU_USART7_TX
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_8},
	/// GPIO_PIN_ID_FMU_USART8_RX
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_0},
	/// GPIO_PIN_ID_FMU_USART8_TX
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_1},
	/// GPIO_PIN_ID_FRAM_CS
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_10},
	/// GPIO_PIN_ID_FRAM_MISO
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_14},
	/// GPIO_PIN_ID_FRAM_MOSI
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_15},
	/// GPIO_PIN_ID_FRAM_SCK
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_10},
	/// GPIO_PIN_ID_FRSKY_INV
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_10},
	/// GPIO_PIN_ID_HMC5983_DRDY
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_12},
	/// GPIO_PIN_ID_HMC5983_CS
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_15},
	/// GPIO_PIN_ID_LED_SAFETY
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_3},
	/// GPIO_PIN_ID_MPU9250_CS
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_2},
	/// GPIO_PIN_ID_MPU9250_DRDY
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_15},
	/// GPIO_PIN_ID_MPU9250_MISO
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_6},
	/// GPIO_PIN_ID_MPU9250_SCK
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_5},
	/// GPIO_PIN_ID_MPU9250_MOSI
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_7},
	/// GPIO_PIN_ID_OTG_FS_DM
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_11},
	/// GPIO_PIN_ID_OTG_FS_DP
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_12},
	/// GPIO_PIN_ID_RC_INPUT
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_0},
	/// GPIO_PIN_ID_RSSI_IN
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_1},
	/// GPIO_PIN_ID_SAFETY_SWITCH_EN
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_4},
	/// GPIO_PIN_ID_SBUS_INV
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_13},
	/// GPIO_PIN_ID_SDIO_CK
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_12},
	/// GPIO_PIN_ID_SDIO_CMD
	{GpioSTM32F4xx::PORT_ID_D, GpioSTM32F4xx::PIN_ID_2},
	/// GPIO_PIN_ID_SDIO_D0
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_8},
	/// GPIO_PIN_ID_SDIO_D1
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_9},
	/// GPIO_PIN_ID_SDIO_D2
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_10},
	/// GPIO_PIN_ID_SDIO_D3
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_11},
	/// GPIO_PIN_ID_SPEKTRUM_POWER
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_4},
	/// GPIO_PIN_ID_VBUS
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_9},
	/// GPIO_PIN_ID_VBUS_VALID
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_0},
	/// GPIO_PIN_ID_VDD_3V3_PERIPH_EN
	{GpioSTM32F4xx::PORT_ID_C, GpioSTM32F4xx::PIN_ID_5},
	/// GPIO_PIN_ID_VDD_3V3_SENSORS_EN
	{GpioSTM32F4xx::PORT_ID_E, GpioSTM32F4xx::PIN_ID_3},
	/// GPIO_PIN_ID_VDD_5V_SENS
	{GpioSTM32F4xx::PORT_ID_A, GpioSTM32F4xx::PIN_ID_4},
	/// GPIO_PIN_ID_VDD_BRICK_VALID
	{GpioSTM32F4xx::PORT_ID_B, GpioSTM32F4xx::PIN_ID_5}
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardPixhawkFmuV4::BoardPixhawkFmuV4() :
    Board(),
    myUserButton(0),
	myUserButtonEnableLine(0),
	myGpioPins(),
	myI2c(0),
	myProcessor(0),
	myUart(0)
{
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardPixhawkFmuV4::~BoardPixhawkFmuV4()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Button& BoardPixhawkFmuV4::getButton()
{
    if (isNullPointer(myUserButton))
    {
        myUserButton = new Button(getEnableLine());
    }

    return (*myUserButton);
}

//------------------------------------------------------------------------------
EnableLine& BoardPixhawkFmuV4::getEnableLine()
{
    if (isNullPointer(myUserButtonEnableLine))
    {
        myUserButtonEnableLine =
                             new EnableLine(EnableLine::MODE_INPUT,
                                            EnableLine::ACTIVE_LEVEL_HIGH,
                                            getGpioPin(GPIO_PIN_ID_USER_BUTTON),
                                            false);
    }

    return (*myUserButtonEnableLine);
}

//------------------------------------------------------------------------------
GpioPinSTM32F4xx& BoardPixhawkFmuV4::getGpioPin(const GpioPinId gpioPinId)
{
    if (isNullPointer(myGpioPins[gpioPinId]))
    {
        myGpioPins[gpioPinId] = new GpioPinSTM32F4xx(myGpioPinIdMap[gpioPinId]);
    }

    return (*(myGpioPins[gpioPinId]));
}

//------------------------------------------------------------------------------
ProcessorSTM32F4xx& BoardPixhawkFmuV4::getProcessor()
{
    if (isNullPointer(myProcessor))
    {
        myProcessor =
                    new ProcessorSTM32F4xx(myProcessorCoreVoltage,
                                           myProcessorExternalClockFrequencyHz);
    }

    return (*myProcessor);
}

//------------------------------------------------------------------------------
I2cSTM32F4xx& BoardPixhawkFmuV4::getI2c()
{
    if (isNullPointer(myI2c))
    {
        GpioPinSTM32F4xx& i2c1Scl = getGpioPin(GPIO_PIN_ID_I2C_SCL);
        GpioPinSTM32F4xx& i2c1Sda = getGpioPin(GPIO_PIN_ID_I2C_SDA);

        myI2c = new I2cSTM32F4xx(I2cSTM32F4xx::ID_1, i2c1Scl, i2c1Sda);
    }

    return (*myI2c);
}

//------------------------------------------------------------------------------
UartSTM32F4xx& BoardPixhawkFmuV4::getUart(const UartId uartId)
{
	if (isNullPointer(myUarts[uartId]))
	{
		switch (uartId)
		{
			case UART_ID_1:
			{
				GpioPinSTM32F4xx& uart1Tx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART1_TX);
				GpioPinSTM32F4xx& uart1Rx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART1_RX);

				myUarts[uartId] = new UartSTM32F4xx(UartSTM32F4xx::ID_1,
										   	   	    uart1Tx,
													uart1Rx);

				break;
			}
			case UART_ID_2:
			{
				GpioPinSTM32F4xx& uart2Tx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART2_TX);
				GpioPinSTM32F4xx& uart2Rx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART2_RX);

				myUarts[uartId] = new UartSTM32F4xx(UartSTM32F4xx::ID_2,
												    uart2Tx,
													uart2Rx);

				break;
			}
			case UART_ID_3:
			{
				GpioPinSTM32F4xx& uart3Tx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART3_TX);
				GpioPinSTM32F4xx& uart3Rx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART3_RX);

				myUarts[uartId] = new UartSTM32F4xx(UartSTM32F4xx::ID_3,
									   	   	   	    uart3Tx,
													uart3Rx);

				break;
			}
			case UART_ID_4:
			{
				GpioPinSTM32F4xx& uart4Tx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART4_TX);
				GpioPinSTM32F4xx& uart4Rx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART4_RX);

				myUarts[uartId] = new UartSTM32F4xx(UartSTM32F4xx::ID_4,
										   	   	    uart4Tx,
													uart4Rx);

				break;
			}
			case UART_ID_7:
			{
				GpioPinSTM32F4xx& uart7Tx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART7_TX);
				GpioPinSTM32F4xx& uart7Rx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART7_RX);

				myUarts[uartId] = new UartSTM32F4xx(UartSTM32F4xx::ID_7,
										   	        uart7Tx,
													uart7Rx);

				break;
			}
			case UART_ID_8:
			{
				GpioPinSTM32F4xx& uart8Tx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART8_TX);
				GpioPinSTM32F4xx& uart8Rx =
										  getGpioPin(GPIO_PIN_ID_FMU_USART8_RX);

				myUarts[uartId] = new UartSTM32F4xx(UartSTM32F4xx::ID_8,
										   	   	    uart8Tx,
													uart8Rx);

				break;
			}
			default:
			{
				break;
			}
		}
    }

	return (*myUarts[uartId]);
}
