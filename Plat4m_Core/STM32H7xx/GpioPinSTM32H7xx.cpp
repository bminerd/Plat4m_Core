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
/// @file GpioPinSTM32H7xx.cpp
/// @author Ben Minerd
/// @date 6/22/2022
/// @brief GpioPinSTM32H7xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32H7xx/GpioPinSTM32H7xx.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const std::uint32_t GpioPinSTM32H7xx::myModeMap[] =
{
    1, /// GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL
    1, /// GpioPin::MODE_DIGITAL_OUTPUT_OPEN_DRAIN
    0, /// GpioPin::MODE_DIGITAL_INPUT
    3, /// GpioPin::MODE_ANALOG
    2  /// GpioPin::MODE_ALTERNATE_FUNCTION
};

const std::uint32_t GpioPinSTM32H7xx::myResistorMap[] =
{
    0, /// GpioPin::RESISTOR_NONE
    1, /// GpioPin::RESISTOR_PULL_UP
    2  /// GpioPin::RESISTOR_PULL_DOWN
};

const GpioPinSTM32H7xx::OutputSpeed GpioPinSTM32H7xx::myDefaultOutputSpeed =
                                       GpioPinSTM32H7xx::OUTPUT_SPEED_VERY_HIGH;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinSTM32H7xx::GpioPinSTM32H7xx(GpioPortSTM32H7xx& gpioPort, const Id id) :
    GpioPin(),
    myGpioPort(gpioPort),
    myId(id),
    myPinBitMask(1 << myId),
    mySTM32H7xxConfig()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32H7xx::setLevelFast(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->BSRR = myPinBitMask << 16;
    }
    else
    {
        myGpioPort.getPort()->BSRR = myPinBitMask;
    }
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32H7xx::getLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->ODR, myPinBitMask));
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinSTM32H7xx::readLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->IDR, myPinBitMask));
}

//------------------------------------------------------------------------------
void GpioPinSTM32H7xx::toggleLevelFast()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortSTM32H7xx& GpioPinSTM32H7xx::getGpioPort()
{
    return myGpioPort;
}

//------------------------------------------------------------------------------
GpioPinSTM32H7xx::Id GpioPinSTM32H7xx::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
void GpioPinSTM32H7xx::setSTM32H7xxConfig(STM32H7xxConfig& config)
{
    setAlternateFunction(config.alternateFunction);
    setOutputType(config.outputType);
    setOutputSpeed(config.outputSpeed);

    mySTM32H7xxConfig = config;
}

//------------------------------------------------------------------------------
void GpioPinSTM32H7xx::setAlternateFunction(
                                      const AlternateFunction alternateFunction)
{
    if (myId < ID_8)
    {
        clearAndSetBits(
                 myGpioPort.getPort()->AFR[0],
                 15 << (myId * 4),
                 (static_cast<std::uint32_t>(alternateFunction)) << (myId * 4));
    }
    else
    {
        clearAndSetBits(
            myGpioPort.getPort()->AFR[1],
            15 << ((myId - 8) * 4),
            (static_cast<std::uint32_t>(alternateFunction)) <<
                                                              ((myId - 8) * 4));
    }
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPinSTM32H7xx::driverSetEnabled(const bool enabled)
{
    myGpioPort.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32H7xx::driverConfigure(const Config& config)
{
    // Set mode bits for this pin
    clearAndSetBits(myGpioPort.getPort()->MODER,
                    3 << (myId * 2),
                    myModeMap[config.mode] << (myId * 2));

    // Set resistor bits for this pin
    clearAndSetBits(myGpioPort.getPort()->PUPDR,
                    3 << (myId * 2),
                    myResistorMap[config.resistor] << (myId * 2));

    if (config.mode == MODE_DIGITAL_OUTPUT_PUSH_PULL)
    {
        setOutputSpeed(myDefaultOutputSpeed);
        setOutputType(OUTPUT_TYPE_PUSH_PULL);
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32H7xx::driverSetLevel(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->BSRR = myPinBitMask << 16;
    }
    else
    {
        myGpioPort.getPort()->BSRR = myPinBitMask;
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32H7xx::driverGetLevel(Level& level)
{
    level = static_cast<Level>(areBitsSet(myGpioPort.getPort()->ODR,
                                          myPinBitMask));

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32H7xx::driverReadLevel(Level& level)
{
    level = static_cast<Level>(areBitsSet(myGpioPort.getPort()->IDR,
                                          myPinBitMask));

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinSTM32H7xx::driverToggleLevel()
{
    toggleBits(myGpioPort.getPort()->ODR, myPinBitMask);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinSTM32H7xx::setOutputType(const OutputType outputType)
{
    // Set output bits for this pin
    clearAndSetBits(myGpioPort.getPort()->OTYPER,
                    1 << myId,
                    (static_cast<std::uint32_t>(outputType)) << myId);
}

//------------------------------------------------------------------------------
void GpioPinSTM32H7xx::setOutputSpeed(const OutputSpeed outputSpeed)
{
    // Set speed bits for this pin
    clearAndSetBits(myGpioPort.getPort()->OSPEEDR,
                    3 << (myId * 2),
                    (static_cast<std::uint32_t>(outputSpeed)) << (myId * 2));
}
