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
// Copyright (c) 2017-2023 Benjamin Minerd
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
/// @file Ins.h
/// @author Ben Minerd
/// @date 4/20/2017
/// @brief Ins class header file.
///

#ifndef PLAT4M_INS_H
#define PLAT4M_INS_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Sensor.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Matrice/Matrix.h>
#include <Matrice/Vector.h>
#include <Matrice/Quaternion.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType>
class Ins : public Sensor<Sample>
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_NOT_CONFIGURED,
        ERROR_CODE_COMMUNICATION_FAILED
    };

    enum MeasurementMode
    {
        MEASUREMENT_MODE_SINGLE,
        MEASUREMENT_MODE_CONTINUOUS
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> MeasurementCallback;

    struct Config
    {
        int a; // Placeholder
    };

    struct Sample
    {
        Matrice::Quaternion<RealNumber> quaternion;
    };

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error setConfig(const Config& config);

    Config getConfig() const;

    Error getEulerAnglesDegrees(AngleDegrees& yawAngleDegrees,
                                AngleDegrees& pitchAngleDegrees,
                                AngleDegrees& rollAngleDegrees);

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    Ins();

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    virtual ~Ins();

    //--------------------------------------------------------------------------
    // Protected methods
    //--------------------------------------------------------------------------

    void measurementReady();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    //--------------------------------------------------------------------------
    // Private pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error driverSetConfig(const Config& config) = 0;

    virtual Error driverGetEulerAnglesDegrees(
                                            AngleDegrees& yawAngleDegrees,
                                            AngleDegrees& pitchAngleDegrees,
                                            AngleDegrees& rollAngleDegrees) = 0;
};

}; // namespace Plat4m

#endif // PLAT4M_INS_H
