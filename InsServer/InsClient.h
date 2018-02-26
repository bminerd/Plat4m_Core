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
// Copyright (c) 2018 Benjamin Minerd
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
/// @file InsClient.h
/// @author Ben Minerd
/// @date 4/20/2017
/// @brief InsClient class header file.
///

#ifndef PLAT4M_INS_CLIENT_H
#define PLAT4M_INS_CLIENT_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/Ins.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/WaitCondition.h>
#include <Plat4m_Core/ComProtocolPlat4m/ComProtocolPlat4mBinary.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageFrameHandler.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerGroup.h>
#include <Plat4m_Core/InsServer/InsMeasurementMessage.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InsClient : public Ins
{
public:

    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum ErrorCode
    {
        ERROR_CODE_NONE,
        ERROR_CODE_NOT_ENABLED
    };

    typedef ErrorTemplate<ErrorCode> Error;

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    InsClient(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
              BinaryMessageFrameHandler& binaryMessageFrameHandler);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~InsClient();

private:

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    ComProtocolPlat4mBinary& myComProtocolPlat4mBinary;

    BinaryMessageFrameHandler& myBinaryMessageFrameHandler;

    BinaryMessageHandlerGroup myBinaryMessageHandlerGroup;

    Ins::Measurement myMeasurement;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from Ins
    //--------------------------------------------------------------------------

    Ins::Error driverSetConfig(const Config& config);

    Ins::Error driverGetMeasurement(Measurement& measurement);

    Ins::Error driverGetEulerAnglesDegrees(AngleDegrees& yawAngleDegrees,
                                           AngleDegrees& pitchAngleDegrees,
                                           AngleDegrees& rollAngleDegrees);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initialize();

    void insMeasurementMessageCallback(const InsMeasurementMessage& message);
};

}; // namespace Plat4m

#endif // PLAT4M_INS_SERVER_H
