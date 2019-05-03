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
/// @file main.cpp
/// @author Ben Minerd
/// @date 11/13/2017
/// @brief Main source file for Blinky_STM32F4xx_App.
///

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/STM32F4xx/ProcessorSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/GpioPortSTM32F4xx.h>
#include <Plat4m_Core/STM32F4xx/GpioPinSTM32F4xx.h>

using namespace Plat4m;

static ProcessorSTM32F4xx processor(3.3, 8000000);

static const ProcessorSTM32F4xx::Config processorConfig =
{
	ProcessorSTM32F4xx::CLOCK_SOURCE_EXTERNAL, // .clockSource
	72000000, 								   // .coreClockFrequencyHz, 72 MHz
	72000000, 								   // .ahbClockFrequencyHz,  72 MHz
	36000000, 								   // .apb1ClockFrequencyHz, 36 MHz
	72000000, 								   // .apb2ClockFrequencyHz, 72 MHz
	false,									   // .isUsbOtgFs
	false,									   // .isSdio
	false,									   // .isRng
	false,									   // .isEthernet
	FLASH_BASE								   // .vectorTableAddress
};

static GpioPortSTM32F4xx ledGpioPort(GpioPortSTM32F4xx::ID_A);

static GpioPinSTM32F4xx ledGpioPin(ledGpioPort, GpioPinSTM32F4xx::ID_5);

//------------------------------------------------------------------------------
int main()
{
	// Configure Processor

	processor.reset();
	processor.setConfig(processorConfig);

	// Configure LED GpioPin

	GpioPin::Config gpioConfig;
	gpioConfig.mode     = GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL;
	gpioConfig.resistor = GpioPin::RESISTOR_PULL_DOWN;

	ledGpioPin.enable(true);
	ledGpioPin.configure(gpioConfig);

	// Toggle the LED

	while (true)
	{
		ledGpioPin.toggleLevel();

		int32_t blinkLoopCount = 5000000;

		while (blinkLoopCount > 0)
		{
			blinkLoopCount--;
		}
	}

    return 0;
}
