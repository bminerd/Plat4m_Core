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
/// @file ControllerPid.h
/// @author Ben Minerd
/// @date 10/4/2016
/// @brief ControllerPid class header file.
///

#ifndef PLAT4M_CONTROLLER_PID_H
#define PLAT4M_CONTROLLER_PID_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Controller.h>
#include <ErrorTemplate.h>

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

class ControllerPid : public Controller
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE = 0
    };

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    ControllerPid(const RealNumber minSetPoint,
    		      const RealNumber maxSetPoint,
				  const RealNumber minOutput,
                  const RealNumber maxOutput,
                  const RealNumber updateRateHz,
                  const RealNumber proportionalGain,
                  const RealNumber integralGain,
                  const RealNumber derivativeGain);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~ControllerPid();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    RealNumber getProportionalGain() const;

    void setProportionalGain(const RealNumber proportionalGain);

    RealNumber getIntegralGain() const;

    void setIntegralGain(const RealNumber integralGain);

    RealNumber getDerivativeGain() const;

    void setDerivativeGain(const RealNumber derivativeGain);

    RealNumber getProportionalOutput() const;

    RealNumber getIntegralOutput() const;

    RealNumber getDerivativeOutput() const;

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    RealNumber myProportionalGain;

    RealNumber myIntegralGain;

    RealNumber myDerivativeGain;

    RealNumber myLastError;

    RealNumber myIntegralError;

    RealNumber myProportionalOutput;

    RealNumber myIntegralOutput;

    RealNumber myDerivativeOutput;

    //--------------------------------------------------------------------------
    // Private methods implemented from Controller
    //--------------------------------------------------------------------------

    virtual RealNumber driverErrorUpdate(const RealNumber error);

    virtual void driverReset();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------
};

}; // namespace Control

}; // namespace Plat4m

#endif // PLAT4M_CONTROLLER_PID_H
