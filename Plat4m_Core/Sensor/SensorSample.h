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
// Copyright (c) 2024 Benjamin Minerd
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
/// @file SensorSample.h
/// @author Ben Minerd
/// @date 11/18/2024
/// @brief SensorSample class header file.
///

#ifndef PLAT4M_SENSOR_SAMPLE_H
#define PLAT4M_SENSOR_SAMPLE_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/TimeStamp.h>
#include <Plat4m_Core/Sensor/SensorBase.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------

template <typename ValueType, std::uint32_t nDof>
struct SensorSample
{
    Plat4m::TimeStamp timeOfValidity;
    ValueType values[nDof];
    SensorBase::SampleStatus status;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    SensorSample() :
        timeOfValidity(),
        values{0},
        status(SensorBase::SAMPLE_STATUS_VALID)
    {
    }

    //--------------------------------------------------------------------------
    // Public overloaded operators
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    bool operator==(const SensorSample& sample) const
    {
        return ((values[0] == sample.values[0]) &&
                (values[1] == sample.values[1]) &&
                (values[2] == sample.values[2]) &&
                (status == sample.status));
    }

    //--------------------------------------------------------------------------
    bool operator!=(const SensorSample& sample) const
    {
        return (!(operator==(sample)));
    }
};

}; // end namespace Plat4m

#endif // PLAT4M_SENSOR_SAMPLE_H
