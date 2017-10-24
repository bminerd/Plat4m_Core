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
/// @file Controller.cpp
/// @author Ben Minerd
/// @date 2/2/2017
/// @brief Controller class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Controller.h>

using Plat4m::Control::Controller;

using Plat4m::Module;

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::getMinOutput() const
{
    return myMinOutput;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::getMaxOutput() const
{
    return myMaxOutput;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::getUpdateRateHz() const
{
    return myUpdateRateHz;
}

//------------------------------------------------------------------------------
void Controller::setUpdateCallback(UpdateCallback& updateCallback)
{
    myUpdateCallback = &updateCallback;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::getSetPoint() const
{
    return mySetPoint;
}

//------------------------------------------------------------------------------
void Controller::setSetPoint(const RealNumber setPoint)
{
    mySetPoint = setPoint;

    if (mySetPoint < myMinSetPoint)
    {
    	mySetPoint = myMinSetPoint;
    }
    else if (mySetPoint > myMaxSetPoint)
    {
    	mySetPoint = myMaxSetPoint;
    }
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::getFeedback() const
{
    return myFeedback;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::getError() const
{
    return myError;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::getOutput() const
{
    return myOutput;
}

//------------------------------------------------------------------------------
Plat4m::RealNumber Controller::update(const RealNumber feedback)
{
    if (!isEnabled())
    {
        return 0;
    }

    myFeedback = feedback;

    myError = mySetPoint - myFeedback;

    if (myControlMode == CONTROL_MODE_CLOSED_LOOP)
    {
        myOutput = driverErrorUpdate(myError);
    }
    else if (myControlMode == CONTROL_MODE_OPEN_LOOP)
    {
    	myOutput = mySetPoint;
    }

    if (myOutput < myMinOutput)
    {
        myOutput = myMinOutput;
    }
    else if (myOutput > myMaxOutput)
    {
        myOutput = myMaxOutput;
    }

    if (isValidPointer(myUpdateCallback))
    {
        myUpdateCallback->call();
    }

    return myOutput;
}

//------------------------------------------------------------------------------
void Controller::setControlMode(const ControlMode controlMode)
{
    // TODO Locking mechanism?

    myControlMode = controlMode;
}

//------------------------------------------------------------------------------
void Controller::reset()
{
    mySetPoint = 0;
    myFeedback = 0;
    myError = 0;
    myOutput = 0;

    driverReset();
}

//------------------------------------------------------------------------------
// Protected constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Controller::Controller(const RealNumber minSetPoint,
		   	   	   	   const RealNumber maxSetPoint,
		   	   	   	   const RealNumber minOutput,
                       const RealNumber maxOutput,
                       const RealNumber updateRateHz) :
    Module(),
	myMinSetPoint(minSetPoint),
	myMaxSetPoint(maxSetPoint),
    myMinOutput(minOutput),
    myMaxOutput(maxOutput),
    myUpdateRateHz(updateRateHz),
    myUpdateCallback(0),
    mySetPoint(0.0),
    myFeedback(0.0),
    myError(0.0),
    myOutput(0.0),
    myControlMode(CONTROL_MODE_CLOSED_LOOP)
{
}

//------------------------------------------------------------------------------
// Protected virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Controller::~Controller()
{
}

//------------------------------------------------------------------------------
// Protected inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Controller::setOutput(const RealNumber output)
{
    myOutput = output;
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error Controller::interfaceSetEnabled(const bool enabled)
{
    if (enabled)
    {
        reset();
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}
