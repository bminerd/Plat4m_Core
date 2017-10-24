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
/// @file ImuLSM6DS0.cpp
/// @author Ben Minerd
/// @date 11/19/2015
/// @brief ImuLSM6DS0 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ImuLSM6DS0.h>
#include <SpiDeviceSt.h>
#include <I2cDevice.h>
#include <ByteArrayN.h>

using Plat4m::ImuLSM6DS0;
using Plat4m::Imu;
using Plat4m::Module;
using Plat4m::SlaveDevice;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static const uint8_t i2cBaseAddress = 0x6A; // 1101010 in binary

static const uint8_t accelFullScaleMaskMap[4] =
{
	0x00, // AccelFullScale::PLUS_MINUS_2_G
	0x10, // AccelFullScale::PLUS_MINUS_4_G
	0x18, // AccelFullScale::PLUS_MINUS_8_G
	0x08  // AccelFullScale::PLUS_MINUS_16_G
};

static const uint8_t gyroFullScaleMaskMap[3] =
{
	0x00, // AccelFullScale::PLUS_MINUS_245_DPS
	0x08, // AccelFullScale::PLUS_MINUS_500_DPS
	0x18  // AccelFullScale::PLUS_MINUS_2000_DPS
};

static const float accelFullScaleScaleFactorMap[4] =
{
	0.061, // AccelFullScale::PLUS_MINUS_2_G
	0.122, // AccelFullScale::PLUS_MINUS_4_G
	0.244, // AccelFullScale::PLUS_MINUS_8_G
	0.732  // AccelFullScale::PLUS_MINUS_16_G
};

static const float gyroFullScaleSensitivityMap[3] =
{
	8.75,  // GyroSensitivity::PLUS_MINUS_245_DPS
	17.50, // GyroSensitivity::PLUS_MINUS_500_DPS
	70.0   // GyroSensitivity::PLUS_MINUS_2000_DPS
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuLSM6DS0::ImuLSM6DS0(Plat4m::Spi& spi, Plat4m::GpioPin& chipSelectGpioPin) :
    Imu(3, 3, 0),
	mySlaveDevice(0),
	myConfig()
{
	mySlaveDevice = new SpiDeviceSt(spi, chipSelectGpioPin);
}

//------------------------------------------------------------------------------
ImuLSM6DS0::ImuLSM6DS0(const PinLevel sa0PinLevel, Plat4m::I2c& i2c) :
    Imu(3, 3, 0),
	mySlaveDevice(0),
	myConfig()
{
	mySlaveDevice =
				   new I2cDevice(i2cBaseAddress | ((uint8_t) sa0PinLevel), i2c);
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuLSM6DS0::~ImuLSM6DS0()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuLSM6DS0::Error ImuLSM6DS0::setLSM6DS0Config(const Config& config)
{
	if (!isEnabled())
	{
		return Error(ERROR_CODE_NOT_ENABLED);
	}

//	uint8_t value;
//	Error error;
//
//	// Accel configuration
//
//	// TODO CTRL_REG5_XL configuration
//
//	// CTRL_REG6_XL configuration
//
//	value = 0;
//
//	// Accel sensitivity
//	setBits(value, accelFullScaleMaskMap[config.accelFullScale]);
//
//	error = writeRegister(REGISTER_CTRL_REG6_XL, value);
//
//	if (error.getCode() != ERROR_CODE_NONE)
//	{
//		return error;
//	}
//
//	// TODO CTRL_REG7_XL configuration
//
//	// Gyro configuration
//
//	// CTRL_REG1_G configuration
//
//	value = 0;
//
//	// Gyro sensitivity
//	setBits(value, gyroFullScaleMaskMap[config.gyroFullScale]);
//
//	error = writeRegister(REGISTER_CTRL_REG1_G, value);
//
//	if (error.getCode() != ERROR_CODE_NONE)
//	{
//		return error;
//	}

	myConfig = config;

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ImuLSM6DS0::Config ImuLSM6DS0::getLSM6DS0Config() const
{
	return myConfig;
}

//------------------------------------------------------------------------------
ImuLSM6DS0::Error ImuLSM6DS0::readRegister(const Register reg, uint8_t& value)
{
	ByteArrayN<1> values;
	values.append(value);

	return readRegisters(reg, values);
}

//------------------------------------------------------------------------------
ImuLSM6DS0::Error ImuLSM6DS0::readRegisters(const Register startingRegister,
										    ByteArray& values)
{
	if (!isEnabled())
	{
		return Error(ERROR_CODE_NOT_ENABLED);
	}

	ByteArrayN<1> txByteArray;
	txByteArray.append(static_cast<uint8_t>(startingRegister));

	SlaveDevice::Error error = mySlaveDevice->transmitReceive(txByteArray,
	                                                          values);

	if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
	{
		return Error(ERROR_CODE_COMMUNICATION_FAILED);
	}

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ImuLSM6DS0::Error ImuLSM6DS0::writeRegister(const Register reg,
											const uint8_t value)
{
	if (!isEnabled())
	{
		return Error(ERROR_CODE_NOT_ENABLED);
	}

	ByteArrayN<2> txByteArray;
	txByteArray.append(static_cast<uint8_t>(reg));
	txByteArray.append(value);

	SlaveDevice::Error error = mySlaveDevice->transmit(txByteArray);

	if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
	{
		return Error(ERROR_CODE_COMMUNICATION_FAILED);
	}

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ImuLSM6DS0::driverEnable(const bool enable)
{
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Imu
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS0::driverSetConfig(const Imu::Config& config)
{
//    uint8_t value;
//    Error error;
//
//    // Accel configuration
//
//    value = 0;
//
//    // Accel full scale
//    setBits(value, myAccelFullScaleMaskMap[config.accelFullScale]);
//
//    // Accel output data rate
//    setBits(value, myAccelOutputDataRateMaskMap[config.accelOutputDataRate]);
//
//    // Accel filter bandwidth
//    setBits(value, myAccelFilterBandwidthMaskMap[config.accelFilterBandwidth]);
//
//    error = writeRegister(REGISTER_CTRL1_XL, value);
//
//    if (error.getCode() != ERROR_CODE_NONE)
//    {
//        return error;
//    }
//
//    // Gyro configuration
//
//    value = 0;
//
//    // Gyro sensitivity
//    setBits(value, myGyroFullScaleMaskMap[config.gyroFullScale]);
//
//    // Gyro output data rate
//    setBits(value, myGyroOutputDataRateMaskMap[config.gyroOutputDataRate]);
//
//    error = writeRegister(REGISTER_CTRL2_G, value);
//
//    if (error.getCode() != ERROR_CODE_NONE)
//    {
//        return error;
//    }
//
//    value = 0;
//
//    // Gyro filter bandwidth
//    setBits(value, myGyroFilterBandwidthMaskMap[config.gyroFilterBandwidth]);
//
//    error = writeRegister(REGISTER_CTRL7_G, value);
//
//    if (error.getCode() != ERROR_CODE_NONE)
//    {
//        return error;
//    }
//
//    // TODO CTRL_REG2_G configuration
//
//    // TODO CTRL_REG3_G configuration
//
//    myAccelSensitivity =
//                        myAccelFullScaleSensitivityMap[myConfig.accelFullScale];
//    myGyroSensitivity = myGyroFullScaleSensitivityMap[myConfig.gyroFullScale];

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS0::driverGetRawAccelMeasurement(
                                               RawAccelMeasurement& measurement)
{
//    ByteArrayN<6> values;
//
//    Error error;
//
//    values.setValue(0);
//    error = readRegisters(REGISTER_OUTX_L_XL, values);
//
//    if (error.getCode() != ERROR_CODE_NONE)
//    {
//        return Error(ERROR_CODE_MEASUREMENT_FAILED);
//    }
//
//    measurement.values[0] = (int32_t) (values[1] << 8) | (int32_t) (values[0]);
//    measurement.values[1] = (int32_t) (values[3] << 8) | (int32_t) (values[2]);
//    measurement.values[2] = (int32_t) (values[5] << 8) | (int32_t) (values[4]);

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS0::driverGetRawGyroMeasurement(
                                                RawGyroMeasurement& measurement)
{
//    ByteArrayN<6> values;
//
//    Error error;
//
//    values.setValue(0);
//    error = readRegisters(REGISTER_OUTX_L_G, values);
//
//    if (error.getCode() != ERROR_CODE_NONE)
//    {
//        return Error(ERROR_CODE_MEASUREMENT_FAILED);
//    }
//
//    measurement.values[0] = (int16_t) (values[1] << 8) | (int16_t) (values[0]);
//    measurement.values[1] = (int16_t) (values[3] << 8) | (int16_t) (values[2]);
//    measurement.values[2] = (int16_t) (values[5] << 8) | (int16_t) (values[4]);

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS0::driverGetRawMeasurement(RawMeasurement& measurement)
{
//    ByteArrayN<12> values;
//
//    Error error;
//
//    values.setValue(0);
//    error = readRegisters(REGISTER_OUTX_L_G, values);
//
//    if (error.getCode() != ERROR_CODE_NONE)
//    {
//        return Error(ERROR_CODE_MEASUREMENT_FAILED);
//    }
//
//    measurement.accelValues[0] =
//                             (int32_t) (values[7] << 8) | (int32_t) (values[6]);
//    measurement.accelValues[1] =
//                             (int32_t) (values[9] << 8) | (int32_t) (values[8]);
//    measurement.accelValues[2] =
//                           (int32_t) (values[11] << 8) | (int32_t) (values[10]);
//
//    measurement.gyroValues[0] =
//                             (int32_t) (values[1] << 8) | (int32_t) (values[0]);
//    measurement.gyroValues[1] =
//                             (int32_t) (values[3] << 8) | (int32_t) (values[2]);
//    measurement.gyroValues[2] =
//                             (int32_t) (values[5] << 8) | (int32_t) (values[4]);

    return Imu::Error(Imu::ERROR_CODE_NONE);
}
