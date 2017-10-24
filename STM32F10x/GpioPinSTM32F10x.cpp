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
/// @file GpioPinSTM32F10x.cpp
/// @author Ben Minerd
/// @date 12/24/15
/// @brief GpioPinSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <GpioPinSTM32F10x.h>

using Plat4m::GpioPinSTM32F10x;
using Plat4m::GpioPin;
using Plat4m::Module;
using Plat4m::GpioPortSTM32F10x;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

///
/// This map defines bit masks for GPIOx_CRL/CRH registers as defined on pg. 171
/// of the STM32F10x datasheet. ST calls these bits the Port configuration bits,
/// or CNFy.
///
const uint32_t GpioPinSTM32F10x::myModeMap[] =
{
    ((uint32_t) 0x00) << 2, /// MODE_ANALOG
    ((uint32_t) 0x01) << 2, /// MODE_DIGITAL_INPUT_FLOATING
    ((uint32_t) 0x02) << 2, /// MODE_DIGITAL_INPUT_PULL_UP_DOWN
    ((uint32_t) 0x00) << 2, /// MODE_DIGITAL_OUTPUT_PUSH_PULL
    ((uint32_t) 0x01) << 2, /// MODE_DIGITAL_OUTPUT_OPEN_DRAIN
    ((uint32_t) 0x02) << 2, /// MODE_ALTERNATE_FUNCTION_PUSH_PULL
    ((uint32_t) 0x03) << 2  /// MODE_ALTERNATE_FUNCTION_OPEN_DRAIN
};

const GpioPinSTM32F10x::OutputSpeed GpioPinSTM32F10x::myDefaultOutputSpeed =
                                                             OUTPUT_SPEED_50MHZ;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32F10x::GpioPinSTM32F10x(GpioPortSTM32F10x& gpioPort, const Id id) :
    GpioPin(),
    myGpioPort(gpioPort),
    myId(id),
    myPinBitMask(1 << myId),
    mySTM32F10xConfig()
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32F10x::~GpioPinSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32F10x::setLevelFast(const Level level)
{
    if (level == LEVEL_LOW)
	{
        myGpioPort.getPort()->BRR = myPinBitMask;
	}
    else // level == LEVEL_HIGH
	{
        myGpioPort.getPort()->BSRR = myPinBitMask;
	}
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F10x::getLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->ODR, myPinBitMask));
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32F10x::readLevelFast()
{
	return ((Level) areBitsSet(myGpioPort.getPort()->IDR, myPinBitMask));
}

//------------------------------------------------------------------------------
void GpioPinSTM32F10x::toggleLevelFast()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32F10x& GpioPinSTM32F10x::getGpioPort()
{
    return myGpioPort;
}

//------------------------------------------------------------------------------
GpioPortSTM32F10x& GpioPinSTM32F10x::getGpioPort() const
{
    return myGpioPort;
}

//------------------------------------------------------------------------------
GpioPinSTM32F10x::Id GpioPinSTM32F10x::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
void GpioPinSTM32F10x::setSTM32F10xConfig(STM32F10xConfig& config)
{
    setOutputType(config.outputType);
    setOutputSpeed(config.outputSpeed);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPinSTM32F10x::driverSetEnabled(const bool enabled)
{
	myGpioPort.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F10x::driverConfigure(const Config& config)
{
    // Set mode
    
	// CRL/CRH register

	volatile uint32_t* reg = 0;
	uint32_t bitOffset;

	// Choose CRL or CRH based on pin number
	if (myId < ID_8)
	{
		reg = &(myGpioPort.getPort()->CRL);
		bitOffset = myId * 4;
	}
	else
	{
		reg = &(myGpioPort.getPort()->CRH);
		bitOffset = (myId - 8) * 4;
	}

	// Clear configuration and mode bits
	clearBits(*reg, ((uint32_t) 0x0F) << bitOffset);

	uint32_t value = 0;

	switch (config.mode)
	{
		case MODE_DIGITAL_OUTPUT_PUSH_PULL:
		{
			setOutputSpeed(myDefaultOutputSpeed);
			setOutputType(OUTPUT_TYPE_PUSH_PULL);

			break;
		}
		case MODE_DIGITAL_OUTPUT_OPEN_DRAIN:
		{
			setOutputSpeed(myDefaultOutputSpeed);
			setOutputType(OUTPUT_TYPE_OPEN_DRAIN);

			break;
		}
		case MODE_DIGITAL_INPUT:
		{
			if (config.resistor == RESISTOR_NONE)
			{
				setBits(value,
				        myModeMap[STM32F103X_MODE_DIGITAL_INPUT_FLOATING]);
			}
			// outputType == GpioPin::RESISTOR_PULL_UP ||
			// outputType == GpioPin::RESISTOR_PULL_DOWN
			else
			{
				setBits(value,
				        myModeMap[STM32F103X_MODE_DIGITAL_INPUT_PULL_UP_DOWN]);

				if (config.resistor == RESISTOR_PULL_UP)
				{
					// The ST Standard Peripheral library does this
					// (apparently there is no explicit configuration)
					driverSetLevel(LEVEL_HIGH);
				}
				else // config.resistor == GpioPin::RESISTOR_PULL_DOWN
				{
					// The ST Standard Peripheral library does this
					// (apparently there is no explicit configuration)
					driverSetLevel(LEVEL_LOW);
				}
			}

			break;
		}
		case GpioPin::MODE_ANALOG:
		{
			setBits(value, myModeMap[STM32F103X_MODE_ANALOG]);

			break;
		}
		case GpioPin::MODE_ALTERNATE_FUNCTION:
		{
			setBits(value,
			        myModeMap[STM32F103X_MODE_ALTERNATE_FUNCTION_PUSH_PULL]);

			break;
		}
	}

	setBits(*reg, value << bitOffset);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F10x::driverSetLevel(const Level level)
{
    if (level == LEVEL_LOW)
	{
		setBits(myGpioPort.getPort()->BRR, myPinBitMask);
	}
    else // level == LEVEL_HIGH
	{
    	setBits(myGpioPort.getPort()->BSRR, myPinBitMask);
	}

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F10x::driverGetLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->ODR, myPinBitMask);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F10x::driverReadLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->IDR, myPinBitMask);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32F10x::driverToggleLevel()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32F10x::setOutputSpeed(const OutputSpeed outputSpeed)
{
	// Choose CRL or CRH based on pin number
	if (myId < ID_8)
	{
		setBits(myGpioPort.getPort()->CRL,
		        ((uint32_t) outputSpeed) << (myId * 4));
	}
	else
	{
		setBits(myGpioPort.getPort()->CRH,
		        ((uint32_t) outputSpeed) << ((myId - 8) * 4));
	}
}

//------------------------------------------------------------------------------
void GpioPinSTM32F10x::setOutputType(const OutputType outputType)
{
	// Choose CRL or CRH based on pin number
	if (myId < ID_8)
	{
		setBits(myGpioPort.getPort()->CRL,
		        ((uint32_t) outputType) << ((myId * 4) + 2));
	}
	else
	{
		setBits(myGpioPort.getPort()->CRH,
				((uint32_t) outputType) << (((myId - 8) * 4) + 2));
	}
}
