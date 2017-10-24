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
/// @file RotaryEncoderDigitalHall.cpp
/// @author Ben Minerd
/// @date 1/12/2017
/// @brief RotaryEncoderDigitalHall class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <RotaryEncoderDigitalHall.h>
#include <CallbackMethod.h>

using Plat4m::RotaryEncoderDigitalHall;
using Plat4m::RotaryEncoder;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const Plat4m::AngleDegrees
             RotaryEncoderDigitalHall::myMaxAngularVelocityOffsetDegrees = 60.0;

const Plat4m::AngleRadians
                   RotaryEncoderDigitalHall::myMaxAngularVelocityOffsetRadians =
                                                            60.0 * (M_PI/180.0);

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RotaryEncoderDigitalHall::RotaryEncoderDigitalHall(
                               GpioPin& hall1GpioPin,
                               GpioPin& hall2GpioPin,
                               GpioPin& hall3GpioPin,
                               InputCaptureTimer& hallUpdateInputCaptureTimer) :
	RotaryEncoder(),
	myHall1GpioPin(hall1GpioPin),
	myHall2GpioPin(hall2GpioPin),
	myHall3GpioPin(hall3GpioPin),
	myHallUpdateInputCaptureTimer(hallUpdateInputCaptureTimer),
	myLastAngleStep(ANGLE_STEP_0_TO_60_DEGREES),
	myLastAngleDegrees(0.0),
	myLastAngleRadians(0.0),
	myAngluarVelocityDps(0.0),
	myAngluarVelocityRadPs(0.0),
	myAngluarAccelerationDps2(0.0),
	myAngularAccelerationRadPs2(0.0),
	myAngluarVelocityOffsetDegrees(0.0),
	myAngluarVelocityOffsetRadians(0.0),
	myDirection(DIRECTION_CLOCKWISE)
{
    initialize();
}

//------------------------------------------------------------------------------
RotaryEncoderDigitalHall::RotaryEncoderDigitalHall(
                               AngleUpdateCallback& angleUpdateCallback,
                               GpioPin& hall1GpioPin,
                               GpioPin& hall2GpioPin,
                               GpioPin& hall3GpioPin,
                               InputCaptureTimer& hallUpdateInputCaptureTimer) :
    RotaryEncoder(angleUpdateCallback),
    myHall1GpioPin(hall1GpioPin),
    myHall2GpioPin(hall2GpioPin),
    myHall3GpioPin(hall3GpioPin),
    myHallUpdateInputCaptureTimer(hallUpdateInputCaptureTimer),
    myLastAngleStep(ANGLE_STEP_0_TO_60_DEGREES),
    myLastAngleDegrees(0.0),
    myLastAngleRadians(0.0),
    myAngluarVelocityDps(0.0),
    myAngluarVelocityRadPs(0.0),
    myAngluarVelocityOffsetDegrees(0.0),
    myAngluarVelocityOffsetRadians(0.0),
    myDirection(DIRECTION_CLOCKWISE)
{
    initialize();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RotaryEncoderDigitalHall::~RotaryEncoderDigitalHall()
{
}

//------------------------------------------------------------------------------
// Public methods implemented from RotaryEncoder
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Plat4m::AngleDegrees RotaryEncoderDigitalHall::getAngleDegreesFast()
{
    RealNumber elapsedTimeS = myHallUpdateInputCaptureTimer.getElapsedTimeS();

    myAngluarVelocityOffsetDegrees =
    			(myAngluarVelocityDps * elapsedTimeS) +
    			(0.5 * myAngluarAccelerationDps2 * elapsedTimeS * elapsedTimeS);

    if (myAngluarVelocityOffsetDegrees > myMaxAngularVelocityOffsetDegrees)
    {
        myAngluarVelocityOffsetDegrees = myMaxAngularVelocityOffsetDegrees;
    }
    else if (myAngluarVelocityOffsetDegrees <
                                             -myMaxAngularVelocityOffsetDegrees)
    {
        myAngluarVelocityOffsetDegrees = -myMaxAngularVelocityOffsetDegrees;
    }

    Plat4m::AngleDegrees angleDegrees =
                            myLastAngleDegrees + myAngluarVelocityOffsetDegrees;

    if (angleDegrees > 360.0)
    {
        angleDegrees -= 360.0;
    }
    else if (angleDegrees < 0.0)
    {
        angleDegrees += 360.0;
    }

    return angleDegrees;
}

//------------------------------------------------------------------------------
Plat4m::AngleRadians RotaryEncoderDigitalHall::getAngleRadiansFast()
{
    RealNumber elapsedTimeS = myHallUpdateInputCaptureTimer.getElapsedTimeS();

    myAngluarVelocityOffsetRadians =
    		  (myAngluarVelocityRadPs * elapsedTimeS) +
			  (0.5 * myAngularAccelerationRadPs2 * elapsedTimeS * elapsedTimeS);

    if (myAngluarVelocityOffsetRadians > myMaxAngularVelocityOffsetRadians)
    {
        myAngluarVelocityOffsetRadians = myMaxAngularVelocityOffsetRadians;
    }
    else if (myAngluarVelocityOffsetRadians <
                                             -myMaxAngularVelocityOffsetRadians)
    {
        myAngluarVelocityOffsetRadians = -myMaxAngularVelocityOffsetRadians;
    }

    Plat4m::AngleRadians angleRadians =
                            myLastAngleRadians + myAngluarVelocityOffsetRadians;

    if (angleRadians > 2.0*M_PI)
    {
        angleRadians -= 2.0*M_PI;
    }
    else if (angleRadians < 0.0)
    {
        angleRadians += 2.0*M_PI;
    }

    return angleRadians;
}

//------------------------------------------------------------------------------
RotaryEncoder::Direction RotaryEncoderDigitalHall:: getDirectionFast()
{
    return myDirection;
}

//------------------------------------------------------------------------------
Plat4m::AngularVelocityDps RotaryEncoderDigitalHall::getAngularVelocityDpsFast()
{
    return myAngluarVelocityDps;
}

//------------------------------------------------------------------------------
Plat4m::AngularVelocityRadPs
                         RotaryEncoderDigitalHall::getAngularVelocityRadPsFast()
{
    return myAngluarVelocityRadPs;
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error RotaryEncoderDigitalHall::driverSetEnabled(const bool enabled)
{
    myHall1GpioPin.setEnabled(enabled);
    myHall2GpioPin.setEnabled(enabled);
    myHall3GpioPin.setEnabled(enabled);

    myHallUpdateInputCaptureTimer.setEnabled(enabled);

    if (enabled)
    {
        getInitialAngle();
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from RotaryEncoder
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoderDigitalHall::driverGetAngleDegrees(
                                                     AngleDegrees& angleDegrees)
{
    angleDegrees = getAngleDegreesFast();

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoderDigitalHall::driverGetAngleRadians(
                                                     AngleRadians& angleRadians)
{
    angleRadians = getAngleRadiansFast();

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoderDigitalHall::driverGetAngularVelocityDps(
                                         AngularVelocityDps& angularVelocityDps)
{
    angularVelocityDps = myAngluarVelocityDps;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
RotaryEncoder::Error RotaryEncoderDigitalHall::driverGetAngularVelocityRadPs(
                                     AngularVelocityRadPs& angularVelocityRadPs)
{
    angularVelocityRadPs = myAngluarVelocityRadPs;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void RotaryEncoderDigitalHall::initialize()
{
    myHallUpdateInputCaptureTimer.setCaptureInterruptCallback(
         createCallback(this,
                        &RotaryEncoderDigitalHall::hallUpdateInterruptHandler));
}

//------------------------------------------------------------------------------
void RotaryEncoderDigitalHall::getInitialAngle()
{
    GpioPin::Level hall1Level = myHall1GpioPin.readLevelFast();
    GpioPin::Level hall2Level = myHall2GpioPin.readLevelFast();
    GpioPin::Level hall3Level = myHall3GpioPin.readLevelFast();

    if ((hall1Level == GpioPin::LEVEL_HIGH) &&
        (hall2Level == GpioPin::LEVEL_HIGH) &&
        (hall3Level == GpioPin::LEVEL_LOW))
    {
        myLastAngleDegrees = 0.0;
        myLastAngleStep = ANGLE_STEP_0_TO_60_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_HIGH) &&
             (hall2Level == GpioPin::LEVEL_LOW)  &&
             (hall3Level == GpioPin::LEVEL_LOW))
    {
        myLastAngleDegrees = 60.0;
        myLastAngleStep = ANGLE_STEP_60_TO_120_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_HIGH) &&
             (hall2Level == GpioPin::LEVEL_LOW)  &&
             (hall3Level == GpioPin::LEVEL_HIGH))
    {
        myLastAngleDegrees = 120.0;
        myLastAngleStep = ANGLE_STEP_120_TO_180_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_LOW) &&
             (hall2Level == GpioPin::LEVEL_LOW) &&
             (hall3Level == GpioPin::LEVEL_HIGH))
    {
        myLastAngleDegrees = 180.0;
        myLastAngleStep = ANGLE_STEP_180_TO_240_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_LOW)  &&
             (hall2Level == GpioPin::LEVEL_HIGH) &&
             (hall3Level == GpioPin::LEVEL_HIGH))
    {
        myLastAngleDegrees = 240.0;
        myLastAngleStep = ANGLE_STEP_240_TO_300_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_LOW)  &&
             (hall2Level == GpioPin::LEVEL_HIGH) &&
             (hall3Level == GpioPin::LEVEL_LOW))
    {
        myLastAngleDegrees = 300.0;
        myLastAngleStep = ANGLE_STEP_300_TO_360_DEGREES;
    }

    myLastAngleRadians = degreesToRadians(myLastAngleDegrees);

    myAngluarVelocityOffsetRadians = 0.0;
    myAngluarVelocityOffsetDegrees = 0.0;

    myAngluarVelocityDps = 0.0;
    myAngluarVelocityRadPs = 0.0;
}

//------------------------------------------------------------------------------
void RotaryEncoderDigitalHall::hallUpdateInterruptHandler()
{
    GpioPin::Level hall1Level = myHall1GpioPin.readLevelFast();
    GpioPin::Level hall2Level = myHall2GpioPin.readLevelFast();
    GpioPin::Level hall3Level = myHall3GpioPin.readLevelFast();

    RealNumber elapsedTimeS =
                         myHallUpdateInputCaptureTimer.getElapsedCaptureTimeS();

    if ((hall1Level == GpioPin::LEVEL_HIGH) &&
        (hall2Level == GpioPin::LEVEL_HIGH) &&
        (hall3Level == GpioPin::LEVEL_LOW))
    {
        if (myLastAngleStep == ANGLE_STEP_300_TO_360_DEGREES)
        {
            myLastAngleDegrees = 0.0;
            myDirection = DIRECTION_CLOCKWISE;
        }
        else if (myLastAngleStep == ANGLE_STEP_60_TO_120_DEGREES)
        {
            myLastAngleDegrees = 60.0;
            myDirection = DIRECTION_COUNTERCLOCKWISE;
        }
        else
        {
            // Error, discontinuity in angles
            getInitialAngle();
        }

        myLastAngleStep = ANGLE_STEP_0_TO_60_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_HIGH) &&
             (hall2Level == GpioPin::LEVEL_LOW)  &&
             (hall3Level == GpioPin::LEVEL_LOW))
    {
        if (myLastAngleStep == ANGLE_STEP_0_TO_60_DEGREES)
        {
            myLastAngleDegrees = 60.0;
            myDirection = DIRECTION_CLOCKWISE;
        }
        else if (myLastAngleStep == ANGLE_STEP_120_TO_180_DEGREES)
        {
            myLastAngleDegrees = 120.0;
            myDirection = DIRECTION_COUNTERCLOCKWISE;
        }
        else
        {
            // Error, discontinuity in angles
            getInitialAngle();
        }

        myLastAngleStep = ANGLE_STEP_60_TO_120_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_HIGH) &&
             (hall2Level == GpioPin::LEVEL_LOW)  &&
             (hall3Level == GpioPin::LEVEL_HIGH))
    {
        if (myLastAngleStep == ANGLE_STEP_60_TO_120_DEGREES)
        {
            myLastAngleDegrees = 120.0;
            myDirection = DIRECTION_CLOCKWISE;
        }
        else if (myLastAngleStep == ANGLE_STEP_180_TO_240_DEGREES)
        {
            myLastAngleDegrees = 180.0;
            myDirection = DIRECTION_COUNTERCLOCKWISE;
        }
        else
        {
            // Error, discontinuity in angles
            getInitialAngle();
        }

        myLastAngleStep = ANGLE_STEP_120_TO_180_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_LOW) &&
             (hall2Level == GpioPin::LEVEL_LOW) &&
             (hall3Level == GpioPin::LEVEL_HIGH))
    {
        if (myLastAngleStep == ANGLE_STEP_120_TO_180_DEGREES)
        {
            myLastAngleDegrees = 180.0;
            myDirection = DIRECTION_CLOCKWISE;
        }
        else if (myLastAngleStep == ANGLE_STEP_240_TO_300_DEGREES)
        {
            myLastAngleDegrees = 240.0;
            myDirection = DIRECTION_COUNTERCLOCKWISE;
        }
        else
        {
            // Error, discontinuity in angles
            getInitialAngle();
        }

        myLastAngleStep = ANGLE_STEP_180_TO_240_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_LOW)  &&
             (hall2Level == GpioPin::LEVEL_HIGH) &&
             (hall3Level == GpioPin::LEVEL_HIGH))
    {
        if (myLastAngleStep == ANGLE_STEP_180_TO_240_DEGREES)
        {
            myLastAngleDegrees = 240.0;
            myDirection = DIRECTION_CLOCKWISE;
        }
        else if (myLastAngleStep == ANGLE_STEP_300_TO_360_DEGREES)
        {
            myLastAngleDegrees = 300.0;
            myDirection = DIRECTION_COUNTERCLOCKWISE;
        }
        else
        {
            // Error, discontinuity in angles
            getInitialAngle();
        }

        myLastAngleStep = ANGLE_STEP_240_TO_300_DEGREES;
    }
    else if ((hall1Level == GpioPin::LEVEL_LOW)  &&
             (hall2Level == GpioPin::LEVEL_HIGH) &&
             (hall3Level == GpioPin::LEVEL_LOW))
    {
        if (myLastAngleStep == ANGLE_STEP_240_TO_300_DEGREES)
        {
            myLastAngleDegrees = 300.0;
            myDirection = DIRECTION_CLOCKWISE;
        }
        else if (myLastAngleStep == ANGLE_STEP_0_TO_60_DEGREES)
        {
            myLastAngleDegrees = 0.0;
            myDirection = DIRECTION_COUNTERCLOCKWISE;
        }
        else
        {
            // Error, discontinuity in angles
            getInitialAngle();
        }

        myLastAngleStep = ANGLE_STEP_300_TO_360_DEGREES;
    }

    myLastAngleRadians = degreesToRadians(myLastAngleDegrees);

    AngularVelocityDps angularVelocityDps = 0.0;

    if (myDirection == DIRECTION_CLOCKWISE)
    {
    	angularVelocityDps = 60.0 / elapsedTimeS;
    }
    else
    {
    	angularVelocityDps = -60.0 / elapsedTimeS;
    }

	myAngluarAccelerationDps2 = angularVelocityDps - myAngluarVelocityDps;
    myAngluarVelocityDps = angularVelocityDps;

    myAngluarVelocityRadPs 		= dpsToRadPs(myAngluarVelocityDps);
    myAngularAccelerationRadPs2 = dpsToRadPs(myAngluarAccelerationDps2);

    myAngluarVelocityOffsetRadians = 0.0;
    myAngluarVelocityOffsetDegrees = 0.0;
}
