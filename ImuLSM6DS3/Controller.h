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
/// @file Controller.h
/// @author Ben Minerd
/// @date 10/4/2016
/// @brief Controller class header file.
///

#ifndef PLAT4M_CONTROLLER_H
#define PLAT4M_CONTROLLER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Module.h>
#include <ErrorTemplate.h>
#include <Callback.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

namespace Control
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class Controller : public Module
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0
    };

    enum ControlMode
    {
        CONTROL_MODE_CLOSED_LOOP = 0,
        CONTROL_MODE_OPEN_LOOP
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> UpdateCallback;

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    RealNumber getMinOutput() const;

    RealNumber getMaxOutput() const;

    RealNumber getUpdateRateHz() const;

    void setUpdateCallback(UpdateCallback& updateCallback);

    RealNumber getSetPoint() const;

    void setSetPoint(const RealNumber setPoint);

    RealNumber getFeedback() const;

    RealNumber getError() const;

    RealNumber getOutput() const;

    RealNumber update(const RealNumber feedback);

    void setControlMode(const ControlMode controlMode);

    void reset();

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    Controller(const RealNumber minSetPoint,
    		   const RealNumber maxSetPoint,
    		   const RealNumber minOutput,
               const RealNumber maxOutput,
               const RealNumber updateRateHz);

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Controller();

    //--------------------------------------------------------------------------
    // Protected inline methods
    //--------------------------------------------------------------------------

    inline void setOutput(const RealNumber output);

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const RealNumber myMinSetPoint;

    const RealNumber myMaxSetPoint;

    const RealNumber myMinOutput;

    const RealNumber myMaxOutput;

    const RealNumber myUpdateRateHz;

    UpdateCallback* myUpdateCallback;

    RealNumber mySetPoint;

    RealNumber myFeedback;

    RealNumber myError;

    RealNumber myOutput;

    ControlMode myControlMode;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error interfaceSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private virtual methods
    //--------------------------------------------------------------------------

    virtual RealNumber driverErrorUpdate(const RealNumber error) = 0;

    virtual void driverReset() = 0;
};

}; // namespace Control

}; // namespace Plat4m

#endif // PLAT4M_CONTROLLER_H
