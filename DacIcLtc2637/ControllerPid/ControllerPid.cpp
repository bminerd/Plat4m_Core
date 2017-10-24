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
/// @file ControllerPid.cpp
/// @author Ben Minerd
/// @date 2/2/2017
/// @brief Controller class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ControllerPid.h>

using Plat4m::Control::ControllerPid;
using Plat4m::Control::Controller;

using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ControllerPid::ControllerPid(const RealNumber minSetPoint,
						     const RealNumber maxSetPoint,
						     const RealNumber minOutput,
                             const RealNumber maxOutput,
                             const RealNumber updateRateHz,
                             const RealNumber proportionalGain,
                             const RealNumber integralGain,
                             const RealNumber derivativeGain) :
    Controller(minSetPoint, maxSetPoint, minOutput, maxOutput, updateRateHz),
    myProportionalGain(proportionalGain),
    myIntegralGain(integralGain),
    myDerivativeGain(derivativeGain),
    myLastError(0),
    myIntegralError(0),
    myIntegralOutput(0),
    myProportionalOutput(0),
    myDerivativeOutput(0)
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ControllerPid::~ControllerPid()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::RealNumber ControllerPid::getProportionalGain() const
{
    return myProportionalGain;
}

//------------------------------------------------------------------------------
void ControllerPid::setProportionalGain(const RealNumber proportionalGain)
{
    myProportionalGain = proportionalGain;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber ControllerPid::getIntegralGain() const
{
    return myIntegralGain;
}

//------------------------------------------------------------------------------
void ControllerPid::setIntegralGain(const RealNumber integralGain)
{
    myIntegralGain = integralGain;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber ControllerPid::getDerivativeGain() const
{
    return myDerivativeGain;
}

//------------------------------------------------------------------------------
void ControllerPid::setDerivativeGain(const RealNumber derivativeGain)
{
    myDerivativeGain = derivativeGain;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber ControllerPid::getProportionalOutput() const
{
    return myProportionalOutput;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber ControllerPid::getIntegralOutput() const
{
    return myIntegralOutput;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber ControllerPid::getDerivativeOutput() const
{
    return myDerivativeOutput;
}

//------------------------------------------------------------------------------
// Private methods implemented from Controller
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::RealNumber ControllerPid::driverErrorUpdate(
                                                  const RealNumber error)
{
    const RealNumber dt = 1.0/getUpdateRateHz();

    myIntegralError += error;

    const RealNumber pError = error;
    const RealNumber iError = myIntegralError;
    const RealNumber dError = error - myLastError;

    myProportionalOutput = (myProportionalGain * pError);
    myIntegralOutput     = (myIntegralGain     * iError * dt);
    myDerivativeOutput   = (myDerivativeGain   * dError / dt);

    const RealNumber output = myProportionalOutput +
                                     myIntegralOutput     +
                                     myDerivativeOutput;

    myLastError = error;

    if (myIntegralError > (getMaxOutput() / (myIntegralGain * dt)))
    {
        myIntegralError = getMaxOutput() / (myIntegralGain * dt);
    }
    else if (myIntegralError < (getMinOutput() / (myIntegralGain * dt)))
    {
        myIntegralError = getMinOutput() / (myIntegralGain * dt);
    }

    return output;
}

//------------------------------------------------------------------------------
void ControllerPid::driverReset()
{
    myLastError = 0;
    myIntegralError = 0;
}
