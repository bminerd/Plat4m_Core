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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file Processor.cpp
/// @author Ben Minerd
/// @date 6/3/2013
/// @brief Processor class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Processor.h>

using Plat4m::Processor;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

Processor* Processor::myDriver               = 0;
Plat4m::Endian Processor::myEndian           = Plat4m::ENDIAN_BIG;
Plat4m::VoltageV Processor::myCoreVoltageV   = 0.0;
uint32_t Processor::myClockSourceFrequencyHz = 0;
Processor::Config Processor::myConfig;

//------------------------------------------------------------------------------
extern "C" uint32_t processorGetCoreClockFrequencyHz(void)
{
    return Processor::getCoreClockFrequencyHz();
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::VoltageV Processor::getCoreVoltageV()
{
    return myCoreVoltageV;
}

//------------------------------------------------------------------------------
uint32_t Processor::getClockSourceFrequencyHz()
{
    return myClockSourceFrequencyHz;
}

//------------------------------------------------------------------------------
Processor::Error Processor::reset()
{
    return (myDriver->driverReset());
}

//------------------------------------------------------------------------------
Processor::Error Processor::configure(const Config& config)
{
    Error error = myDriver->driverConfigure(config);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        myConfig = config;
    }

    return error;
}

//------------------------------------------------------------------------------
Processor::Error Processor::setPowerMode(const PowerMode powerMode)
{
    return (myDriver->driverSetPowerMode(powerMode));
}

//------------------------------------------------------------------------------
uint32_t Processor::getCoreClockFrequencyHz()
{
    return (myDriver->driverGetCoreClockFrequencyHz());
}

//------------------------------------------------------------------------------
Plat4m::Endian Processor::getEndian()
{
    return myEndian;
}

//------------------------------------------------------------------------------
bool Processor::isInterruptActive()
{
    return (myDriver->driverIsInterruptActive());
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::Processor(const Endian endian,
                     const VoltageV coreVoltageV,
                     const uint32_t clockSourceFrequencyHz)
{
    if (isNullPointer(myDriver))
    {
        myEndian                 = endian;
        myCoreVoltageV           = coreVoltageV;
        myClockSourceFrequencyHz = clockSourceFrequencyHz;
        myDriver                 = this;
    }
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Processor::~Processor()
{
    myDriver = 0;
}

//------------------------------------------------------------------------------
// Private virtual methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool Processor::driverIsInterruptActive()
{
    // Not implemented by subclass, default implementation

    return false;
}
