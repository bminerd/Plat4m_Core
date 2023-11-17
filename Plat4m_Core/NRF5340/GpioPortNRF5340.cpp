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
// Copyright (c) 2021-2023 Benjamin Minerd
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
/// @file GpioPortNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief GpioPortNRF5340 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/NRF5340/GpioPortNRF5340.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const std::uint32_t GpioPortNRF5340::myBitMaskMap[] =
{
    0xFFFFFFFF, /// PORT_ID_P0
    0x0000FFFF  /// PORT_ID_P1
};

const std::uint8_t GpioPortNRF5340::myNPinsMap[] =
{
    32, /// PORT_ID_P0
    16  /// PORT_ID_P1
};

const std::uint32_t GpioPortNRF5340::myModeMap[] =
{
    0x00000003, /// GPIO_MODE_DIGITAL_OUTPUT
    0x00000000, /// GPIO_MODE_DIGITAL_INPUT
    0x00000000, /// GPIO_MODE_ANALOG
    0x00000000  /// GPIO_MODE_ALTERNATE_FUNCTION
};

const std::uint32_t GpioPortNRF5340::myResistorMap[] =
{
    0x00000000, /// GPIO_RESISTOR_NONE
    0x0000000C, /// GPIO_RESISTOR_PULL_UP
    0x00000004  /// GPIO_RESISTOR_PULL_DOWN
};

// Variables

NRF_GPIO_Type* GpioPortNRF5340::myPortMap[] =
{
    NRF_P0_NS, /// PORT_ID_P0
    NRF_P1_NS  /// PORT_ID_P1
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortNRF5340::GpioPortNRF5340(const Id id) :
    GpioPort(),
    myId(id),
    myBitMask(myBitMaskMap[id]),
    myNPins(myNPinsMap[id]),
    myPort(myPortMap[id])
{
}

//------------------------------------------------------------------------------
// Public virtual methods overridden for GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPortNRF5340::setValueFast(const std::uint32_t value)
{
    myPort->OUT = (value & myBitMask);
}

//------------------------------------------------------------------------------
uint32_t GpioPortNRF5340::getValueFast()
{
    return (myPort->OUT & myBitMask);
}

//------------------------------------------------------------------------------
uint32_t GpioPortNRF5340::readValueFast()
{
    return (myPort->IN & myBitMask);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortNRF5340::Id GpioPortNRF5340::getId()
{
    return myId;
}

//------------------------------------------------------------------------------
NRF_GPIO_Type*& GpioPortNRF5340::getPort()
{
    return myPort;
}

//------------------------------------------------------------------------------
uint8_t GpioPortNRF5340::getNPins() const
{
    return myNPins;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPortNRF5340::driverSetEnabled(const bool enabled)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for GpioPort
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPort<uint32_t>::Error GpioPortNRF5340::driverConfigure(
                                                           const Config& config)
{
    // Set mode

    for (int i = 0; i < myNPins; i++)
    {
        // Set mode and resistor bits for this pin
        setBits(myPort->PIN_CNF[i],
                myModeMap[config.mode] |
                    myResistorMap[config.resistor]);
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint32_t>::Error GpioPortNRF5340::driverSetValue(
                                                      const std::uint32_t value)
{
    myPort->OUT = maskBits(value, myBitMask);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint32_t>::Error GpioPortNRF5340::driverGetValue(std::uint32_t& value)
{
    value = maskBits(myPort->OUT, myBitMask);

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPort<uint32_t>::Error GpioPortNRF5340::driverReadValue(std::uint32_t& value)
{
    value = maskBits(myPort->IN, myBitMask);

    return Error(ERROR_CODE_NONE);
}
