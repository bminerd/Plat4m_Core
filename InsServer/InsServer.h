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
/// @file InsServer.h
/// @author Ben Minerd
/// @date 4/20/2017
/// @brief InsServer class header file.
///

#ifndef PLAT4M_INS_SERVER_H
#define PLAT4M_INS_SERVER_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

// Plat4m includes
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageServer.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/Ins.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/WaitCondition.h>
#include <Plat4m_Core/ComProtocolPlat4m/ComProtocolPlat4mBinary.h>
#include <Plat4m_Core/InsServer/InsMeasurementMessage.h>
#include <Plat4m_Core/InsServer/InsServerSetConfigMessage.h>
#include <Plat4m_Core/InsServer/InsServerSetConfigResponseMessage.h>
#include <Plat4m_Core/InsServer/InsServerGetConfigMessage.h>
#include <Plat4m_Core/InsServer/InsServerGetConfigResponseMessage.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InsServer : public BinaryMessageServer
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

    struct Config
    {
        uint32_t outputRateDivisionFactor;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    InsServer(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
              BinaryMessageFrameHandler& binaryMessageFrameHandler);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~InsServer();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    bool addIns(Ins& imu);

    Config getConfig() const;

    Error setConfig(const Config& config);

    void measurementReadyCallback();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const TimeMs myOutputThreadPeriodMs;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Config myConfig;

    List<Ins*> myInsList;

    Thread& myOutputThread;

    WaitCondition& myWaitCondition;

    // TODO: This only works for one Ins
    uint32_t myUpdateCount;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initialize();

    void outputThreadCallback();

    void insServerSetConfigMessageCallback(
                                   const InsServerSetConfigMessage& request,
                                   InsServerSetConfigResponseMessage& response);
};

}; // namespace Plat4m

#endif // PLAT4M_INS_SERVER_H
