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
// Copyright (c) 2020 Benjamin Minerd
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
/// @file interfaceIncludes.h
/// @author Ben Minerd
/// @date 5/15/2020
/// @brief Interface includes header file for Linux_GCC_Test_Build_App.
///

#ifndef PLAT4M_INTERFACE_INCLUDES_H
#define PLAT4M_INTERFACE_INCLUDES_H

// Include all Interface Layer modules

#include <Plat4m_Core/Accel.h>
#include <Plat4m_Core/Adc.h>
#include <Plat4m_Core/AllocationMemory.h>
#include <Plat4m_Core/AnalogInput.h>
#include <Plat4m_Core/Application.h>
#include <Plat4m_Core/Array.h>
#include <Plat4m_Core/ArrayN.h>
#include <Plat4m_Core/Board.h>
#include <Plat4m_Core/Buffer.h>
#include <Plat4m_Core/BufferN.h>
#include <Plat4m_Core/Button.h>
#include <Plat4m_Core/ByteArray.h>
#include <Plat4m_Core/ByteArrayN.h>
#include <Plat4m_Core/ByteArrayParser.h>
#include <Plat4m_Core/Callback.h>
#include <Plat4m_Core/CallbackFunction.h>
#include <Plat4m_Core/CallbackFunctionParameter.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/CallbackMethod2Parameters.h>
#include <Plat4m_Core/CallbackMethodParameter.h>
#include <Plat4m_Core/Can.h>
#include <Plat4m_Core/ComInterface.h>
#include <Plat4m_Core/ComInterfaceDevice.h>
#include <Plat4m_Core/ComInterfaceDeviceTemplate.h>
#include <Plat4m_Core/ComLink.h>
#include <Plat4m_Core/ComLinkTemplate.h>
#include <Plat4m_Core/Compass.h>
#include <Plat4m_Core/ComProtocol.h>
#include <Plat4m_Core/Crc.h>
#include <Plat4m_Core/CurrentSensor.h>
#include <Plat4m_Core/Dac.h>
#include <Plat4m_Core/DacIc.h>
#include <Plat4m_Core/DigitalFilter.h>
#include <Plat4m_Core/Display.h>
#include <Plat4m_Core/EnableLine.h>
#include <Plat4m_Core/EncryptedMemory.h>
#include <Plat4m_Core/ErrorTemplate.h>
#include <Plat4m_Core/ExternalInterrupt.h>
#include <Plat4m_Core/Font.h>
#include <Plat4m_Core/GpioPin.h>
#include <Plat4m_Core/GpioPort.h>
#include <Plat4m_Core/Gyro.h>
#include <Plat4m_Core/HardwareTimer.h>
#include <Plat4m_Core/I2c.h>
#include <Plat4m_Core/I2cDevice.h>
#include <Plat4m_Core/Imu.h>
#include <Plat4m_Core/InputCaptureTimer.h>
#include <Plat4m_Core/Ins.h>
#include <Plat4m_Core/Interrupt.h>
#include <Plat4m_Core/InterruptPolicy.h>
#include <Plat4m_Core/IoExpander.h>
#include <Plat4m_Core/List.h>
#include <Plat4m_Core/MasterSlaveBus.h>
#include <Plat4m_Core/Memory.h>
#include <Plat4m_Core/MemoryRegion.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/Mutex.h>
#include <Plat4m_Core/MutexLock.h>
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/PowerSupply.h>
#include <Plat4m_Core/Processor.h>
#include <Plat4m_Core/PwmOutput.h>
#include <Plat4m_Core/Queue.h>
#include <Plat4m_Core/QueueDriver.h>
#include <Plat4m_Core/RotaryEncoder.h>
#include <Plat4m_Core/RotarySwitch.h>
#include <Plat4m_Core/SerialPort.h>
#include <Plat4m_Core/SlaveDevice.h>
#include <Plat4m_Core/Spi.h>
#include <Plat4m_Core/SpiDevice.h>
#include <Plat4m_Core/Subsystem.h>
#include <Plat4m_Core/System.h>
#include <Plat4m_Core/Thread.h>
#include <Plat4m_Core/Timer.h>
#include <Plat4m_Core/Topic.h>
#include <Plat4m_Core/TopicSubscriber.h>
#include <Plat4m_Core/TopicSubscriberThread.h>
#include <Plat4m_Core/Uart.h>
#include <Plat4m_Core/Ui.h>
#include <Plat4m_Core/UiAction.h>
#include <Plat4m_Core/UiInput.h>
#include <Plat4m_Core/UiState.h>
#include <Plat4m_Core/Usb.h>
#include <Plat4m_Core/WaitCondition.h>

// Include all platform-agnostic drivers

#include <Plat4m_Core/AllocationMemoryLite/AllocationMemoryLite.h>
#include <Plat4m_Core/BoardStevalMKI160V1/BoardStevalMKI160V1.h>
#include <Plat4m_Core/BoardXNucleoIKS01A1/BoardXNucleoIKS01A1.h>
#include <Plat4m_Core/ComProtocolPlat4m/AsciiMessage.h>
#include <Plat4m_Core/ComProtocolPlat4m/AsciiMessageHandler.h>
#include <Plat4m_Core/ComProtocolPlat4m/AsciiMessageHandlerTemplate.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessage.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageFrameHandler.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandler.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerGroup.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageHandlerTemplate.h>
#include <Plat4m_Core/ComProtocolPlat4m/BinaryMessageServer.h>
#include <Plat4m_Core/ComProtocolPlat4m/ComProtocolPlat4mAscii.h>
#include <Plat4m_Core/ComProtocolPlat4m/ComProtocolPlat4mBinary.h>
#include <Plat4m_Core/ComProtocolPlat4m/Frame.h>
#include <Plat4m_Core/ComProtocolPlat4m/FrameHandler.h>
#include <Plat4m_Core/ComProtocolPlat4m/Message.h>
#include <Plat4m_Core/ComProtocolPlat4m/Packet.h>
#include <Plat4m_Core/ComProtocolPlat4m/PacketFrameHandler.h>
#include <Plat4m_Core/ImuLSM6DS0/ImuLSM6DS0.h>
#include <Plat4m_Core/ImuLSM6DS3/ImuLSM6DS3.h>
#include <Plat4m_Core/ImuServer/ImuClient.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigResponseAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetConfigResponseMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetMeasurementAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetMeasurementBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuGetMeasurementMessage.h>
#include <Plat4m_Core/ImuServer/ImuMeasurementAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuMeasurementBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuMeasurementMessage.h>
#include <Plat4m_Core/ImuServer/ImuServer.h>
#include <Plat4m_Core/ImuServer/ImuServerBinaryMessages.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigResponseAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerGetConfigResponseMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerMeasurementAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerMeasurementMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigResponseAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuServerSetConfigResponseMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigResponseAsciiMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigResponseBinaryMessage.h>
#include <Plat4m_Core/ImuServer/ImuSetConfigResponseMessage.h>
// Don't include Linux/*
#include <Plat4m_Core/SpiDeviceSt/SpiDeviceSt.h>
// Don't include STM32F4xx/*
// Don't include STM32F30x/*
// Don't include SystemFreeRtos/*
// Don't include SystemLite/*
// Don't include SystemWindows/*
// Don't include UnitTest/Unit_Test_Linux_App/*
#include <Plat4m_Core/UnitTest/ApplicationUnitTestApp.h>
#include <Plat4m_Core/UnitTest/ArrayNUnitTest.h>
#include <Plat4m_Core/UnitTest/ArrayUnitTest.h>
#include <Plat4m_Core/UnitTest/BufferUnitTest.h>
#include <Plat4m_Core/UnitTest/ByteArrayUnitTest.h>
#include <Plat4m_Core/UnitTest/ModuleUnitTest.h>
#include <Plat4m_Core/UnitTest/UnitTest.h>

#endif // PLAT4M_INTERFACE_INCLUDES_H
