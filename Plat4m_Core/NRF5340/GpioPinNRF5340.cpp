/**
 * Copyright (c) 2021, SignalQuest LLC
 */

///
/// @file GpioPinNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief GpioPinNRF5340 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/NRF5340/GpioPinNRF5340.h>

using Plat4m::GpioPinNRF5340;
using Plat4m::GpioPortNRF5340;
using Plat4m::GpioPin;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint32_t GpioPinNRF5340::myModeMap[] =
{
    0x00000003, /// GpioPin::MODE_DIGITAL_OUTPUT_PUSH_PULL
    0x00000003, /// GpioPin::MODE_DIGITAL_OUTPUT_OPEN_DRAIN
    0x00000000  /// GpioPin::MODE_DIGITAL_INPUT
};

const uint32_t GpioPinNRF5340::myResistorMap[] =
{
    0x00000000, /// GPIO_RESISTOR_NONE
    0x0000000C, /// GPIO_RESISTOR_PULL_UP
    0x00000004  /// GPIO_RESISTOR_PULL_DOWN
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPinNRF5340::GpioPinNRF5340(GpioPortNRF5340& gpioPort, const Id id) :
    GpioPin(),
    myGpioPort(gpioPort),
    myId(id),
    myPinBitMask(1 << myId),
    myNRF5340Config()
{
    if (id >= myGpioPort.getNPins())
    {
        while (true)
        {
            // Attempting to assign incorrect pin, lock up
        }
    }
}

//------------------------------------------------------------------------------
// Public methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GpioPinNRF5340::setLevelFast(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->OUTCLR = myPinBitMask;
    }
    else
    {
        myGpioPort.getPort()->OUTSET = myPinBitMask;
    }
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinNRF5340::getLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->OUT, myPinBitMask));
}

//------------------------------------------------------------------------------
GpioPin::Level GpioPinNRF5340::readLevelFast()
{
    return ((Level) areBitsSet(myGpioPort.getPort()->IN, myPinBitMask));
}

//------------------------------------------------------------------------------
void GpioPinNRF5340::toggleLevelFast()
{
    toggleBits(myGpioPort.getPort()->OUT, myPinBitMask);
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortNRF5340& GpioPinNRF5340::getGpioPort()
{
    return myGpioPort;
}

//------------------------------------------------------------------------------
GpioPinNRF5340::Id GpioPinNRF5340::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
void GpioPinNRF5340::setNRF5340Config(NRF5340Config& config)
{
    setOutputType(config.outputType);

    myNRF5340Config = config;
}

//------------------------------------------------------------------------------
void GpioPinNRF5340::setOutputType(const OutputType outputType)
{
    clearAndSetBits(myGpioPort.getPort()->PIN_CNF[myId],
                    0x00000007 << 8,
                    outputType);
}


//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error GpioPinNRF5340::driverSetEnabled(const bool enabled)
{
    myGpioPort.setEnabled(enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from GpioPin
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPin::Error GpioPinNRF5340::driverConfigure(const Config& config)
{
    // Set mode and resistor bits
    
    // Clear mode bits for this pin
    clearAndSetBits(myGpioPort.getPort()->PIN_CNF[myId],
                    0x0000000F,
                    myModeMap[config.mode] |
                        myResistorMap[config.resistor]);
    
    if (config.mode == MODE_DIGITAL_OUTPUT_PUSH_PULL)
    {
        setOutputType(OUTPUT_TYPE_PUSH_PULL);
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinNRF5340::driverSetLevel(const Level level)
{
    if (level == LEVEL_LOW)
    {
        myGpioPort.getPort()->OUTCLR = myPinBitMask;
    }
    else
    {
        myGpioPort.getPort()->OUTSET = myPinBitMask;
    }
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinNRF5340::driverGetLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->OUT, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinNRF5340::driverReadLevel(Level& level)
{
    level = (Level) areBitsSet(myGpioPort.getPort()->IN, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
GpioPin::Error GpioPinNRF5340::driverToggleLevel()
{
    toggleBits(myGpioPort.getPort()->OUT, myPinBitMask);
    
    return Error(ERROR_CODE_NONE);
}
