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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file RotaryEncoder.cpp
/// @author Ben Minerd
/// @date 1/11/2017
/// @brief RotaryEncoder class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <RotaryEncoder.h>

using Plat4m::RotaryEncoder;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void RotaryEncoder::setAngleUpdateCallback(
                                       AngleUpdateCallback& angleUpdateCallback)
{
    myAngleUpdateCallback = &angleUpdateCallback;
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoder::getAngleDegrees(AngleDegrees& angleDegrees)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    AngleDegrees tempAngleDegrees = 0.0;
    Error error = driverGetAngleDegrees(tempAngleDegrees);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        angleDegrees = tempAngleDegrees;
    }

    return error;
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoder::getAngleRadians(AngleRadians& angleRadians)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    AngleRadians tempAngleRadians = 0.0;
    Error error = driverGetAngleRadians(tempAngleRadians);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        angleRadians = tempAngleRadians;
    }

    return error;
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoder::getDirection(Direction& direction)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoder::getAngularVelocityDps(
                                         AngularVelocityDps& angularVelocityDps)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    AngularVelocityDps tempAngularVelocityDps = 0.0;
    Error error = driverGetAngularVelocityDps(tempAngularVelocityDps);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        angularVelocityDps = tempAngularVelocityDps;
    }

    return error;
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoder::getAngularVelocityRadPs(
                                     AngularVelocityRadPs& angularVelocityRadPs)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    AngularVelocityRadPs tempAngularVelocityRadPs = 0.0;
    Error error = driverGetAngularVelocityRadPs(tempAngularVelocityRadPs);

    if (error.getCode() == ERROR_CODE_NONE)
    {
        angularVelocityRadPs = tempAngularVelocityRadPs;
    }

    return error;
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RotaryEncoder::RotaryEncoder() :
	Module(),
	myAngleUpdateCallback(0)
{
}

//------------------------------------------------------------------------------
RotaryEncoder::RotaryEncoder(AngleUpdateCallback& angleUpdateCallback) :
    Module(),
    myAngleUpdateCallback(&angleUpdateCallback)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RotaryEncoder::~RotaryEncoder()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error RotaryEncoder::interfaceSetEnabled(const bool enabled)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

