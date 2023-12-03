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
// Copyright (c) 2023 Benjamin Minerd
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
/// @file GnssReceiver.h
/// @author Ben Minerd
/// @date 12/4/2023
/// @brief GnssReceiver class header file.
///

#ifndef PLAT4M_GNSS_RECEIVER_H
#define PLAT4M_GNSS_RECEIVER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Sensor/Sensor.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/TimeStamp.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

template <typename ValueType, std::uint32_t nDof>
class GnssReceiver : public Sensor<Sample>
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_PARAMETER_INVALID,
        ERROR_CODE_NOT_ENABLED,
        ERROR_CODE_COMMUNICATION_FAILED
    };

    using Error = ErrorTemplate<ErrorCode>;

    struct Sample
    {
        TimeStamp timeStamp;
        ValueType latitude;
        ValueType longitude;
        ValueType altitude;
    };

    struct Config
    {
        int a; // Placeholder
    };

    //--------------------------------------------------------------------------
    // Public virtual methods
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    Error setConfig(const Config& config)
    {
        if (!isEnabled())
        {
            return Error(ERROR_CODE_NOT_ENABLED);
        }

        Error error = subclassConfigure(config);

        if (error.getCode() == ERROR_CODE_NONE)
        {
            myConfig = config;
        }

        return error;
    }

protected:

    //--------------------------------------------------------------------------
    // Protected constructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    GnssReceiver() :
        Sensor<Sample>(),
        myConfig()
    {
    }

    //--------------------------------------------------------------------------
    // Protected virtual destructors
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual ~GnssReceiver()
    {
    }

    //--------------------------------------------------------------------------
    // Protected pure virtual methods
    //--------------------------------------------------------------------------

    virtual Error subclassSetConfig(const Config& config) = 0;

    //--------------------------------------------------------------------------
    // Protected pure virtual methods (Deprecated)
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    virtual Error driverConfigure(const Config& config)
    {
    }

    //--------------------------------------------------------------------------
    virtual Error driverGetMeasurement(Measurement& measurement)
    {
    }

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;
};

}; // namespace Plat4m

#endif // PLAT4M_GNSS_RECEIVER_H
