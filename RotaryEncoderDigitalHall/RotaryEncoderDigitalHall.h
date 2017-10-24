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
/// @file RotaryEncoderDigitalHall.h
/// @author Ben Minerd
/// @date 1/12/2017
/// @brief RotaryEncoderDigitalHall class header file.
///

#ifndef PLAT4M_ROTARY_ENCODER_DIGITAL_HALL_H
#define PLAT4M_ROTARY_ENCODER_DIGITAL_HALL_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <RotaryEncoder.h>
#include <InputCaptureTimer.h>
#include <ArrayN.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class RotaryEncoderDigitalHall : public RotaryEncoder
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum AngleStep
    {
        ANGLE_STEP_0_TO_60_DEGREES = 0,
        ANGLE_STEP_60_TO_120_DEGREES,
        ANGLE_STEP_120_TO_180_DEGREES,
        ANGLE_STEP_180_TO_240_DEGREES,
        ANGLE_STEP_240_TO_300_DEGREES,
        ANGLE_STEP_300_TO_360_DEGREES
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    RotaryEncoderDigitalHall(GpioPin& hall1GpioPin,
                             GpioPin& hall2GpioPin,
                             GpioPin& hall3GpioPin,
                             InputCaptureTimer& hallUpdateInputCaptureTimer);

    RotaryEncoderDigitalHall(AngleUpdateCallback& angleUpdateCallback,
                             GpioPin& hall1GpioPin,
                             GpioPin& hall2GpioPin,
                             GpioPin& hall3GpioPin,
                             InputCaptureTimer& hallUpdateInputCaptureTimer);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~RotaryEncoderDigitalHall();

    //--------------------------------------------------------------------------
    // Public methods implemented from RotaryEncoder
    //--------------------------------------------------------------------------

    AngleDegrees getAngleDegreesFast();

    AngleRadians getAngleRadiansFast();

    Direction getDirectionFast();

    AngularVelocityDps getAngularVelocityDpsFast();

    AngularVelocityRadPs getAngularVelocityRadPsFast();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const AngleDegrees myMaxAngularVelocityOffsetDegrees;

    static const AngleRadians myMaxAngularVelocityOffsetRadians;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    GpioPin& myHall1GpioPin;

    GpioPin& myHall2GpioPin;

    GpioPin& myHall3GpioPin;

    InputCaptureTimer& myHallUpdateInputCaptureTimer;

    AngleStep myLastAngleStep;

    AngleDegrees myLastAngleDegrees;

    AngleRadians myLastAngleRadians;

    AngularVelocityDps myAngluarVelocityDps;

    AngularVelocityRadPs myAngluarVelocityRadPs;

    AngularAccelerationDps2 myAngluarAccelerationDps2;

    AngularAccelerationRadPs2 myAngularAccelerationRadPs2;

    AngleDegrees myAngluarVelocityOffsetDegrees;

    AngleRadians myAngluarVelocityOffsetRadians;

    Direction myDirection;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from RotaryEncoder
    //--------------------------------------------------------------------------

    Error driverGetAngleDegrees(AngleDegrees& angleDegrees);

    Error driverGetAngleRadians(AngleRadians& angleRadians);

    Error driverGetAngularVelocityDps(AngularVelocityDps& angularVelocityDps);

    Error driverGetAngularVelocityRadPs(
                                    AngularVelocityRadPs& angularVelocityRadPs);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initialize();

    void getInitialAngle();

    void hallUpdateInterruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_ROTARY_ENCODER_DIGITAL_HALL_H
