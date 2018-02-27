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
// Copyright (c) 2016 Benjamin Minerd
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
/// @file ImuServer.cpp
/// @author Ben Minerd
/// @date 6/8/2016
/// @brief ImuServer class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ImuServer/ImuServer.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerTemplate.h>
#include <Plat4m_Core/ImuServer/ImuGetMeasurementBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuMeasurementBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuMeasurementBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ByteArrayN.h>
#include <Plat4m_Core/System.h>

using Plat4m::ImuServer;
using Plat4m::Module;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServer::ImuServer(ComProtocolPlat4mBinary& comProtocolPlat4mBinary,
                     BinaryMessageFrameHandler& binaryMessageFrameHandler) :
    BinaryMessageServer(0x0004,
                        comProtocolPlat4mBinary,
                        binaryMessageFrameHandler),
    myConfig(),
    myOutputThread(System::createThread(
					   createCallback(this, &ImuServer::outputThreadCallback))),
	myImuList(),
	myTempC(0.0),
	myAccelMeasurement(),
	myGyroMeasurement(),
	myWaitCondition(System::createWaitCondition(myOutputThread)),
	myUpdateCount(0)
{
    initialize();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuServer::~ImuServer()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ImuServer::addImu(Imu& imu)
{
    if (myImuList.size() >= 256)
    {
        return false;
    }

    Imu* pointer = &imu;
    myImuList.append(pointer);

    return true;
}

//------------------------------------------------------------------------------
ImuServer::Config ImuServer::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
ImuServer::Error ImuServer::setConfig(const Config& config)
{
    if (!isEnabled())
    {
        return Error(ERROR_CODE_NOT_ENABLED);
    }

    myConfig = config;
    myUpdateCount = 0;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void ImuServer::measurementReadyCallback()
{
	if (myConfig.outputRateDivisionFactor != 0)
	{
		myUpdateCount++;

		if (myUpdateCount == myConfig.outputRateDivisionFactor)
		{
//			myWaitCondition.notifyFast();
		    myWaitCondition.notify();

			myUpdateCount = 0;
		}
	}
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ImuServer::driverSetEnabled(const bool enabled)
{
	myOutputThread.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuServer::initialize()
{
    myOutputThread.setPriority(2);

    addMessageHandler(
    createMessageHandler<ImuServer,
                         ImuGetMeasurementMessage,
                         ImuMeasurementMessage,
                         ImuGetMeasurementBinaryMessage,
                         ImuMeasurementBinaryMessage>(
                           this, &ImuServer::imuGetMeasurementMessageCallback));

    addMessageHandler(
    createMessageHandler<ImuServer,
                         ImuSetConfigMessage,
                         ImuSetConfigResponseMessage,
                         ImuSetConfigBinaryMessage,
                         ImuSetConfigResponseBinaryMessage>(
                                this, &ImuServer::imuSetConfigMessageCallback));

    addMessageHandler(
    createMessageHandler<ImuServer,
                         ImuGetConfigMessage,
                         ImuGetConfigResponseMessage,
                         ImuGetConfigBinaryMessage,
                         ImuGetConfigResponseBinaryMessage>(
                                this, &ImuServer::imuGetConfigMessageCallback));

    addMessageHandler(
    createMessageHandler<ImuServer,
                         ImuServerSetConfigMessage,
                         ImuServerSetConfigResponseMessage,
                         ImuServerSetConfigBinaryMessage,
                         ImuServerSetConfigResponseBinaryMessage>(
                          this, &ImuServer::imuServerSetConfigMessageCallback));

    addMessageHandler(
    createMessageHandler<ImuServer,
                         ImuServerGetConfigMessage,
                         ImuServerGetConfigResponseMessage,
                         ImuServerGetConfigBinaryMessage,
                         ImuServerGetConfigResponseBinaryMessage>(
                          this, &ImuServer::imuServerGetConfigMessageCallback));
}

//------------------------------------------------------------------------------
void ImuServer::outputThreadCallback()
{
	myWaitCondition.wait();

    uint8_t i = 0;
    List<Imu*>::Iterator iterator = myImuList.iterator();

    while (iterator.hasCurrent())
    {
    	Imu* imu = iterator.current();

    	Imu::Measurement measurement;
    	Imu::Error error = imu->getMeasurement(measurement);

        ImuMeasurementMessage message;
        message.index  = i;
        message.timeUs = System::getTimeUs();
        message.accelX = measurement.xAccelerationG;
        message.accelY = measurement.yAccelerationG;
        message.accelZ = measurement.zAccelerationG;
        message.gyroX  = measurement.xAngularVelocityDps;
        message.gyroY  = measurement.yAngularVelocityDps;
        message.gyroZ  = measurement.zAngularVelocityDps;
        message.magX   = measurement.magX;
        message.magY   = measurement.magY;
        message.magZ   = measurement.magZ;
        message.tempC  = myTempC;

        ImuMeasurementBinaryMessage binaryMessage(message);
        transmitMessage(binaryMessage);

        i++;
        iterator.next();
    }
}

//------------------------------------------------------------------------------
void ImuServer::imuGetMeasurementMessageCallback(
                                        const ImuGetMeasurementMessage& request,
                                        ImuMeasurementMessage& response)
{
    uint8_t index = 0;
    List<Imu*>::Iterator iterator = myImuList.iterator();

    while (iterator.hasCurrent())
    {
        if (index == request.index)
        {
            break;
        }

        index++;
    }

    if (index != request.index)
    {
        // Error
    }

    Imu* imu = iterator.current();

    response.index  = request.index;
    response.timeUs = System::getTimeUs();
    response.tempC  = myTempC;
    response.accelX = myAccelMeasurement.xAccelerationG;
    response.accelY = myAccelMeasurement.yAccelerationG;
    response.accelZ = myAccelMeasurement.zAccelerationG;
    response.gyroX  = myGyroMeasurement.xAngularVelocityDps;
    response.gyroY  = myGyroMeasurement.yAngularVelocityDps;
    response.gyroZ  = myGyroMeasurement.zAngularVelocityDps;
    response.magX   = 0.0;
    response.magY   = 0.0;
    response.magZ   = 0.0;
}

//------------------------------------------------------------------------------
void ImuServer::imuSetConfigMessageCallback(
                                          const ImuSetConfigMessage& request,
                                          ImuSetConfigResponseMessage& response)
{
    uint8_t index = 0;
    List<Imu*>::Iterator iterator = myImuList.iterator();

    while (iterator.hasCurrent())
    {
        if (index == request.index)
        {
            break;
        }

        index++;
    }

    if (index != request.index)
    {
        // Error
    }

    Imu* imu = iterator.current();

    Imu::Config config;
    config.measurementMode     = (Imu::MeasurementMode) request.measurementMode;
    config.accelMeasurementRangeAccelerationG  = request.accelMeasurementRange;
    config.accelMeasurementRateHz = request.accelMeasurementRateHz;
    config.gyroMeasurementRangeAngularVelocityDps   = request.gyroMeasurementRange;
    config.gyroMeasurementRateHz  = request.gyroMeasurementRateHz;
    config.magMeasurementRange    = request.magMeasurementRange;
    config.magMeasurementRateHz   = request.magMeasurementRateHz;

    Imu::Error error = imu->setConfig(config);

    response.error = (uint8_t) error.getCode();
}

//------------------------------------------------------------------------------
void ImuServer::imuGetConfigMessageCallback(
                                          const ImuGetConfigMessage& request,
                                          ImuGetConfigResponseMessage& response)
{
    uint8_t index = 0;
    List<Imu*>::Iterator iterator = myImuList.iterator();

    while (iterator.hasCurrent())
    {
        if (index == request.index)
        {
            break;
        }

        index++;
    }

    if (index != request.index)
    {
        // Error
    }

    Imu* imu = iterator.current();

    Imu::Config config = imu->getConfig();

    response.index                  = request.index;
    response.measurementMode        = (uint8_t) config.measurementMode;
    response.accelMeasurementRange  = config.accelMeasurementRangeAccelerationG;
    response.accelMeasurementRateHz = config.accelMeasurementRateHz;
    response.gyroMeasurementRange   =
    							  config.gyroMeasurementRangeAngularVelocityDps;
    response.gyroMeasurementRateHz  = config.gyroMeasurementRateHz;
    response.magMeasurementRange    = config.magMeasurementRange;
    response.magMeasurementRateHz   = config.magMeasurementRateHz;
}

//------------------------------------------------------------------------------
void ImuServer::imuServerSetConfigMessageCallback(
                                    const ImuServerSetConfigMessage& request,
                                    ImuServerSetConfigResponseMessage& response)
{
    Config config;
    config.outputRateDivisionFactor = request.outputRateDivisionFactor;

    Error error = setConfig(config);

    response.error = (uint8_t) error.getCode();
}

//------------------------------------------------------------------------------
void ImuServer::imuServerGetConfigMessageCallback(
                                    const ImuServerGetConfigMessage& request,
                                    ImuServerGetConfigResponseMessage& response)
{
    Config config = getConfig();

    response.outputRateDivisionFactor = config.outputRateDivisionFactor;
}
