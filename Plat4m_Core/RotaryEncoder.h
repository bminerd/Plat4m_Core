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
/// @file RotaryEncoder.h
/// @author Ben Minerd
/// @date 1/11/2017
/// @brief RotaryEncoder class header file.
///

#ifndef PLAT4M_ROTARY_ENCODER_H
#define PLAT4M_ROTARY_ENCODER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class RotaryEncoder : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_DIRECTION_INVALID
    };

    enum Direction
    {
        DIRECTION_CLOCKWISE = 0,
        DIRECTION_COUNTERCLOCKWISE
    };

    typedef Callback<> AngleUpdateCallback;

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public pure virtual methods
    //--------------------------------------------------------------------------

    virtual AngleDegrees getAngleDegreesFast() = 0;

    virtual AngleRadians getAngleRadiansFast() = 0;

    virtual Direction getDirectionFast() = 0;

    virtual AngularVelocityDps getAngularVelocityDpsFast() = 0;

    virtual AngularVelocityRadPs getAngularVelocityRadPsFast() = 0;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void setAngleUpdateCallback(AngleUpdateCallback& angleUpdateCallback);

    Error getAngleDegrees(AngleDegrees& angleDegrees);

    Error getAngleRadians(AngleRadians& angleRadians);

    Error getDirection(Direction& direction);

    Error getAngularVelocityDps(AngularVelocityDps& angularVelocityDps);

    Error getAngularVelocityRadPs(AngularVelocityRadPs& angularVelocityRadPs);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    RotaryEncoder();

    RotaryEncoder(AngleUpdateCallback& angleUpdateCallback);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~RotaryEncoder();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    AngleUpdateCallback* myAngleUpdateCallback;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error interfaceSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverGetAngleDegrees(AngleDegrees& angleDegrees) = 0;

    virtual Error driverGetAngleRadians(AngleRadians& angleRadians) = 0;

    virtual Error driverGetAngularVelocityDps(
                                    AngularVelocityDps& angularVelocityDps) = 0;

    virtual Error driverGetAngularVelocityRadPs(
                                AngularVelocityRadPs& angularVelocityRadPs) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_ROTARY_ENCODER_H
