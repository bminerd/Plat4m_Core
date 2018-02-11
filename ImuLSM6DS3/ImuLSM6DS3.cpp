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
/// @file ImuLSM6DS3.cpp
/// @author Ben Minerd
/// @date 11/20/2015
/// @brief ImuLSM6DS3 class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/ImuLSM6DS3/ImuLSM6DS3.h>
#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/SpiDeviceSt/SpiDeviceSt.h>
#include <Plat4m_Core/I2cDevice.h>
#include <Plat4m_Core/ByteArrayN.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/CallbackMethodParameter.h>

using Plat4m::ImuLSM6DS3;
using Plat4m::Imu;
using Plat4m::Module;
using Plat4m::SlaveDevice;

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const uint8_t ImuLSM6DS3::myI2cBaseAddress = 0x6A; // 1101010 in binary

const float ImuLSM6DS3::myTempSensitivity = 1.0/16.0;

const float ImuLSM6DS3::myTempOffset = 25.0;

// Accel maps

const float ImuLSM6DS3::myAccelFullScaleMap[4] =
{
    2.0, /// ACCEL_FULL_SCALE_PLUS_MINUS_2_G
    4.0, /// ACCEL_FULL_SCALE_PLUS_MINUS_4_G
    8.0, /// ACCEL_FULL_SCALE_PLUS_MINUS_8_G
    16.0 /// ACCEL_FULL_SCALE_PLUS_MINUS_16_G
};

const uint8_t ImuLSM6DS3::myAccelFullScaleMaskMap[4] =
{
	0x00 << 2, /// ACCEL_FULL_SCALE_PLUS_MINUS_2_G
	0x02 << 2, /// ACCEL_FULL_SCALE_PLUS_MINUS_4_G
	0x03 << 2, /// ACCEL_FULL_SCALE_PLUS_MINUS_8_G
	0x01 << 2  /// ACCEL_FULL_SCALE_PLUS_MINUS_16_G
};

const float ImuLSM6DS3::myAccelFullScaleSensitivityMap[4] =
{
	0.061E-3, /// ACCEL_FULL_SCALE_PLUS_MINUS_2_G
	0.122E-3, /// ACCEL_FULL_SCALE_PLUS_MINUS_4_G
	0.244E-3, /// ACCEL_FULL_SCALE_PLUS_MINUS_8_G
	0.488E-3  /// ACCEL_FULL_SCALE_PLUS_MINUS_16_G
};

const uint32_t ImuLSM6DS3::myAccelOutputDataRateMap[11] =
{
    0,    /// ACCEL_OUTPUT_DATA_RATE_0_HZ
    13,   /// ACCEL_OUTPUT_DATA_RATE_13_HZ
    26,   /// ACCEL_OUTPUT_DATA_RATE_26_HZ
    52,   /// ACCEL_OUTPUT_DATA_RATE_52_HZ
    104,  /// ACCEL_OUTPUT_DATA_RATE_104_HZ
    208,  /// ACCEL_OUTPUT_DATA_RATE_208_HZ
    416,  /// ACCEL_OUTPUT_DATA_RATE_416_HZ
    833,  /// ACCEL_OUTPUT_DATA_RATE_833_HZ
    1660, /// ACCEL_OUTPUT_DATA_RATE_1660_HZ
    3330, /// ACCEL_OUTPUT_DATA_RATE_3330_HZ
    6660  /// ACCEL_OUTPUT_DATA_RATE_6660_HZ
};

const uint8_t ImuLSM6DS3::myAccelOutputDataRateMaskMap[11] =
{
	0x00 << 4, /// ACCEL_OUTPUT_DATA_RATE_0_HZ
	0x01 << 4, /// ACCEL_OUTPUT_DATA_RATE_13_HZ
	0x02 << 4, /// ACCEL_OUTPUT_DATA_RATE_26_HZ
	0x03 << 4, /// ACCEL_OUTPUT_DATA_RATE_52_HZ
	0x04 << 4, /// ACCEL_OUTPUT_DATA_RATE_104_HZ
	0x05 << 4, /// ACCEL_OUTPUT_DATA_RATE_208_HZ
	0x06 << 4, /// ACCEL_OUTPUT_DATA_RATE_416_HZ
	0x07 << 4, /// ACCEL_OUTPUT_DATA_RATE_833_HZ
	0x08 << 4, /// ACCEL_OUTPUT_DATA_RATE_1660_HZ
	0x09 << 4, /// ACCEL_OUTPUT_DATA_RATE_3330_HZ
	0x0A << 4  /// ACCEL_OUTPUT_DATA_RATE_6660_HZ
};

const uint8_t ImuLSM6DS3::myAccelAntiAliasingFilterBandwidthMaskMap[4] =
{
	0x03 << 0, /// ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_50_HZ
	0x02 << 0, /// ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_100_HZ
	0x01 << 0, /// ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_200_HZ
	0x00 << 0  /// ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_400_HZ
};

const uint8_t ImuLSM6DS3::myAccelLowPassFilterCutoffFrequencyMaskMap[4] =
{
	0x02 << 5, /// ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_9
	0x00 << 5, /// ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_50
	0x01 << 5, /// ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_100
	0x03 << 5  /// ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_400
};

// Gyro maps

const float ImuLSM6DS3::myGyroFullScaleMap[5] =
{
    125.0,  /// GYRO_FULL_SCALE_PLUS_MINUS_125_DPS
    245.0,  /// GYRO_FULL_SCALE_PLUS_MINUS_245_DPS
    500.0,  /// GYRO_FULL_SCALE_PLUS_MINUS_500_DPS
    1000.0, /// GYRO_FULL_SCALE_PLUS_MINUS_1000_DPS
    2000.0  /// GYRO_FULL_SCALE_PLUS_MINUS_2000_DPS
};

const uint8_t ImuLSM6DS3::myGyroFullScaleMaskMap[5] =
{
	0x01 << 1, /// GYRO_FULL_SCALE_PLUS_MINUS_125_DPS
	0x00 << 1, /// GYRO_FULL_SCALE_PLUS_MINUS_245_DPS
	0x02 << 1, /// GYRO_FULL_SCALE_PLUS_MINUS_500_DPS
	0x04 << 1, /// GYRO_FULL_SCALE_PLUS_MINUS_1000_DPS
	0x06 << 1  /// GYRO_FULL_SCALE_PLUS_MINUS_2000_DPS
};

const float ImuLSM6DS3::myGyroFullScaleSensitivityMap[5] =
{
	4.375E-3, /// GYRO_FULL_SCALE_PLUS_MINUS_125_DPS
	8.75E-3,  /// GYRO_FULL_SCALE_PLUS_MINUS_245_DPS
	17.50E-3, /// GYRO_FULL_SCALE_PLUS_MINUS_500_DPS
	35.0E-3,  /// GYRO_FULL_SCALE_PLUS_MINUS_1000_DPS
	70.0E-3   /// GYRO_FULL_SCALE_PLUS_MINUS_2000_DPS
};

const uint32_t ImuLSM6DS3::myGyroOutputDataRateMap[9] =
{
    0,   /// GYRO_OUTPUT_DATA_RATE_0_HZ
    13,  /// GYRO_OUTPUT_DATA_RATE_13_HZ
    26,  /// GYRO_OUTPUT_DATA_RATE_26_HZ
    52,  /// GYRO_OUTPUT_DATA_RATE_52_HZ
    104, /// GYRO_OUTPUT_DATA_RATE_104_HZ
    208, /// GYRO_OUTPUT_DATA_RATE_208_HZ
    416, /// GYRO_OUTPUT_DATA_RATE_416_HZ
    833, /// GYRO_OUTPUT_DATA_RATE_833_HZ
    1660 /// GYRO_OUTPUT_DATA_RATE_1660_HZ
};

const uint8_t ImuLSM6DS3::myGyroOutputDataRateMaskMap[9] =
{
	0x00 << 4, /// GYRO_OUTPUT_DATA_RATE_0_HZ
	0x01 << 4, /// GYRO_OUTPUT_DATA_RATE_13_HZ
	0x02 << 4, /// GYRO_OUTPUT_DATA_RATE_26_HZ
	0x03 << 4, /// GYRO_OUTPUT_DATA_RATE_52_HZ
	0x04 << 4, /// GYRO_OUTPUT_DATA_RATE_104_HZ
	0x05 << 4, /// GYRO_OUTPUT_DATA_RATE_208_HZ
	0x06 << 4, /// GYRO_OUTPUT_DATA_RATE_416_HZ
	0x07 << 4, /// GYRO_OUTPUT_DATA_RATE_833_HZ
	0x08 << 4  /// GYRO_OUTPUT_DATA_RATE_1660_HZ
};

const uint8_t ImuLSM6DS3::myGyroFilterBandwidthMaskMap[4] =
{
	0x00 << 4, /// GYRO_FILTER_BANDWIDTH_0_0081_HZ
	0x01 << 4, /// GYRO_FILTER_BANDWIDTH_0_0324_HZ
	0x02 << 4, /// GYRO_FILTER_BANDWIDTH_2_07_HZ
	0x03 << 4  /// GYRO_FILTER_BANDWIDTH_16_32_HZ
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuLSM6DS3::ImuLSM6DS3(Spi& spi,
                       GpioPin& chipSelectGpioPin,
					   ExternalInterrupt* int1ExternalInterrupt,
					   ExternalInterrupt* int2ExternalInterrupt) :
    Imu(),
	mySlaveDevice(new SpiDeviceSt(spi, chipSelectGpioPin)),
	myInt1ExternalInterrupt(int1ExternalInterrupt),
	myInt2ExternalInterrupt(int2ExternalInterrupt),
	myConfig(),
	myTemp(0),
	myTransmitByteArray1(),
	myReceiveByteArray1(),
	myTransmitByteArray2(),
	myReceiveByteArray2(),
	myMailboxArray(true)
{
    initialize();
}

//------------------------------------------------------------------------------
ImuLSM6DS3::ImuLSM6DS3(const PinLevel sa0PinLevel,
					   I2c& i2c,
					   ExternalInterrupt* int1ExternalInterrupt,
					   ExternalInterrupt* int2ExternalInterrupt) :
    Imu(),
	mySlaveDevice(
	            new I2cDevice(myI2cBaseAddress | ((uint8_t) sa0PinLevel), i2c)),
	myInt1ExternalInterrupt(int1ExternalInterrupt),
	myInt2ExternalInterrupt(int2ExternalInterrupt),
	myConfig(),
	myTemp(0),
    myTransmitByteArray1(),
    myReceiveByteArray1(),
    myTransmitByteArray2(),
    myReceiveByteArray2(),
    myMailboxArray(true)
{
    initialize();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuLSM6DS3::~ImuLSM6DS3()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ImuLSM6DS3::Error ImuLSM6DS3::setLSM6DS3Config(const Config& config)
{
	if (!isEnabled())
	{
		return Error(ERROR_CODE_NOT_ENABLED);
	}

	uint8_t value;
	Error error;

	// Accel configuration

	error = readRegister(REGISTER_CTRL1_XL, value);

	// Accel anti-aliasing filter bandwidth
	setBits(value,
			myAccelAntiAliasingFilterBandwidthMaskMap[
								      config.accelAntiAliasingFilterBandwidth]);

	error = writeRegister(REGISTER_CTRL1_XL, value);

	if (error.getCode() != ERROR_CODE_NONE)
	{
		return error;
	}

	value = 0;

	// Accel low-pass filter enabled
	if (config.isAccelLowPassFilterEnabled)
	{
		// Per LSM6DS3 datasheet, LPF2_XL_EN and HP_SLOPE_XL_EN bits need to be
		// set to enable the LPF2 accelerometer low-pass filter
		setBitSet(value, 7, config.isAccelLowPassFilterEnabled);
		setBitSet(value, 2, config.isAccelLowPassFilterEnabled);
		// Accel low-pass filter cutoff frequency
		setBits(value,
				myAccelLowPassFilterCutoffFrequencyMaskMap[
									 config.accelLowPassFilterCutoffFrequency]);

		error = writeRegister(REGISTER_CTRL8_XL, value);

		if (error.getCode() != ERROR_CODE_NONE)
		{
			return error;
		}

		// 0x3C = 0b00111100
		// - Zen_G   = 1
		// - Yen_G   = 1
		// - Xen_G   = 1
		// - FUNC_EN = 1 (needed for LPF2 accelerometer low-pass filter)
		value = 0x3C;

		error = writeRegister(REGISTER_CTRL10_C, value);

		if (error.getCode() != ERROR_CODE_NONE)
		{
			return error;
		}

		value = 0;
	}

	// Gyro high-pass filter enabled
	if (config.isGyroHighPassFilterEnabled)
	{
		setBitSet(value, 6, config.isGyroHighPassFilterEnabled);
		// Gyro high-pass filter bandwidth
		setBits(value,
				myGyroFilterBandwidthMaskMap[
									 config.gyroHighPassFilterCutoffFrequency]);

		error = writeRegister(REGISTER_CTRL7_G, value);

		if (error.getCode() != ERROR_CODE_NONE)
		{
			return error;
		}

		value = 0;
	}

	error = writeRegister(REGISTER_INT1_CTRL, 0x01);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return error;
    }

    error = writeRegister(REGISTER_INT2_CTRL, 0x02);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return error;
    }

    myConfig = config;

    if (isValidPointer(myInt1ExternalInterrupt))
    {
        ExternalInterrupt::Config config;
        config.trigger     = ExternalInterrupt::TRIGGER_RISING;
        config.activeLevel = ExternalInterrupt::ACTIVE_LEVEL_HIGH;

        myInt1ExternalInterrupt->configure(config);
    }

    if (isValidPointer(myInt2ExternalInterrupt))
    {
        ExternalInterrupt::Config config;
        config.trigger     = ExternalInterrupt::TRIGGER_RISING;
        config.activeLevel = ExternalInterrupt::ACTIVE_LEVEL_HIGH;

        myInt2ExternalInterrupt->configure(config);
    }

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ImuLSM6DS3::Config ImuLSM6DS3::getLSM6DS3Config() const
{
	return myConfig;
}

//------------------------------------------------------------------------------
ImuLSM6DS3::Error ImuLSM6DS3::readRegister(const Register reg, uint8_t& value)
{
	ByteArrayN<1> values;

	Error error = readRegisters(reg, values);

	if (error.getCode() == ERROR_CODE_NONE)
	{
	    value = values[0];
	}

	return error;
}

//------------------------------------------------------------------------------
ImuLSM6DS3::Error ImuLSM6DS3::readRegisters(const Register startingRegister,
										    ByteArray& values)
{
	if (!isEnabled())
	{
		return Error(ERROR_CODE_NOT_ENABLED);
	}

	ByteArrayN<1> txByteArray;
	txByteArray.append((uint8_t) startingRegister);

	SlaveDevice::Error error = mySlaveDevice->transmitReceive(txByteArray,
	                                                          values);

	if (error.getCode() != SlaveDevice::ERROR_CODE_NONE)
	{
		return Error(ERROR_CODE_COMMUNICATION_FAILED);
	}

	return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
ImuLSM6DS3::Error ImuLSM6DS3::writeRegister(const Register reg,
											const uint8_t value)
{
	if (!isEnabled())
	{
		return Error(ERROR_CODE_NOT_ENABLED);
	}

	ByteArrayN<2> txByteArray;
	txByteArray.append((uint8_t) reg);
	txByteArray.append(value);

	SlaveDevice::Error error = mySlaveDevice->transmit(txByteArray, true);

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
Module::Error ImuLSM6DS3::driverSetEnabled(const bool enabled)
{
	Module::Error error = mySlaveDevice->setEnabled(enabled);

	if (isValidPointer(myInt1ExternalInterrupt))
	{
		myInt1ExternalInterrupt->setEnabled(enabled);
	}

	if (isValidPointer(myInt2ExternalInterrupt))
	{
		myInt2ExternalInterrupt->setEnabled(enabled);
	}

    return error;
}

//------------------------------------------------------------------------------
// Private virtual methods implemented from Imu
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS3::driverSetConfig(const Imu::Config& config)
{
    uint8_t value;
    Error error;
    uint32_t index;

    // Software reset

//    value = 0x01;
//
//    error = writeRegister(REGISTER_CTRL3_C, value);
//
//    if (error.getCode() != ERROR_CODE_NONE)
//    {
//        return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
//    }

    // Accel configuration

    // Accel measurement range (full scale)

    value = 0;

    if (!findValueIndex(config.accelMeasurementRangeAccelerationG,
                        myAccelFullScaleMap,
                        ARRAY_SIZE(myAccelFullScaleMap),
                        index))
    {
        return Imu::Error(Imu::ERROR_CODE_ACCEL_MEASUREMENT_RANGE_INVALID);
    }

    setBits(value, myAccelFullScaleMaskMap[index]);

    setAccelSensitivity(myAccelFullScaleSensitivityMap[index]);

    // Accel output data rate

    if (!findValueIndex(config.accelMeasurementRateHz,
                        myAccelOutputDataRateMap,
                        ARRAY_SIZE(myAccelOutputDataRateMap),
                        index))
    {
        return Imu::Error(Imu::ERROR_CODE_ACCEL_MEASUREMENT_RATE_INVALID);
    }

    setBits(value, myAccelOutputDataRateMaskMap[index]);

    error = writeRegister(REGISTER_CTRL1_XL, value);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
    }

    // Gyro configuration

    value = 0;

    // Gyro measurement range (full scale)

    if (!findValueIndex(config.gyroMeasurementRangeAngularVelocityDps,
                        myGyroFullScaleMap,
                        ARRAY_SIZE(myGyroFullScaleMap),
                        index))
    {
        return Imu::Error(Imu::ERROR_CODE_GYRO_MEASUREMENT_RANGE_INVALID);
    }

    setBits(value, myGyroFullScaleMaskMap[index]);

    setGyroSensitivity(myGyroFullScaleSensitivityMap[index]);

    // Gyro output data rate

    if (!findValueIndex(config.gyroMeasurementRateHz,
                        myGyroOutputDataRateMap,
                        ARRAY_SIZE(myGyroOutputDataRateMap),
                        index))
    {
        return Imu::Error(Imu::ERROR_CODE_GYRO_MEASUREMENT_RATE_INVALID);
    }

    setBits(value, myGyroOutputDataRateMaskMap[index]);

    error = writeRegister(REGISTER_CTRL2_G, value);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
    }

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS3::driverGetRawTempMeasurement(int32_t& temp)
{
    temp = (uint32_t) myTemp;

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS3::driverGetRawAccelMeasurement(
                                               RawAccelMeasurement& measurement)
{
    if (myConfig.readMode == READ_MODE_BLOCKING)
    {
        Error error;

        myReceiveByteArray1.setValue(0);
        error = readRegisters(REGISTER_OUTX_L_XL, myReceiveByteArray1);

        if (error.getCode() != ERROR_CODE_NONE)
        {
            return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
        }
    }
    else if (myConfig.readMode == READ_MODE_MAILBOX)
    {
        if (myMailboxArray[0].error.getCode() !=
                                                MasterSlaveBus::ERROR_CODE_NONE)
        {
            return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
        }
    }

    measurement.x = (int16_t) ((((uint16_t) myReceiveByteArray1[1]) << 8) |
                                ((uint16_t) myReceiveByteArray1[0]));
    measurement.y = (int16_t) ((((uint16_t) myReceiveByteArray1[3]) << 8) |
                                ((uint16_t) myReceiveByteArray1[2]));
    measurement.z = (int16_t) ((((uint16_t) myReceiveByteArray1[5]) << 8) |
                                ((uint16_t) myReceiveByteArray1[4]));

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS3::driverGetRawGyroMeasurement(
                                                RawGyroMeasurement& measurement)
{
    if (myConfig.readMode == READ_MODE_BLOCKING)
    {
        Error error;

        myReceiveByteArray2.setValue(0);
        error = readRegisters(REGISTER_OUT_TEMP_L, myReceiveByteArray2);

        if (error.getCode() != ERROR_CODE_NONE)
        {
            return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
        }
    }
    else if (myConfig.readMode == READ_MODE_MAILBOX)
    {
        if (myMailboxArray[1].error.getCode() !=
                                                MasterSlaveBus::ERROR_CODE_NONE)
        {
            return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
        }
    }

    myTemp = (int16_t) ((((uint16_t) myReceiveByteArray2[1]) << 8) |
                         ((uint16_t) myReceiveByteArray2[0]));

    measurement.x = (int16_t) ((((uint16_t) myReceiveByteArray2[3]) << 8) |
                                ((uint16_t) myReceiveByteArray2[2]));
    measurement.y = (int16_t) ((((uint16_t) myReceiveByteArray2[5]) << 8) |
                                ((uint16_t) myReceiveByteArray2[4]));
    measurement.z = (int16_t) ((((uint16_t) myReceiveByteArray2[7]) << 8) |
                                ((uint16_t) myReceiveByteArray2[6]));

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Imu::Error ImuLSM6DS3::driverGetRawMeasurement(RawMeasurement& measurement)
{
    ByteArrayN<12> values;

    Error error;

    values.setValue(0);
    error = readRegisters(REGISTER_OUTX_L_G, values);

    if (error.getCode() != ERROR_CODE_NONE)
    {
        return Imu::Error(Imu::ERROR_CODE_COMMUNICATION_FAILED);
    }

    measurement.accelX =
           (int16_t) ((((uint16_t) values[7])  << 8) | ((uint16_t) values[6]));
    measurement.accelY =
           (int16_t) ((((uint16_t) values[9])  << 8) | ((uint16_t) values[8]));
    measurement.accelZ =
           (int16_t) ((((uint16_t) values[11]) << 8) | ((uint16_t) values[10]));
    measurement.gyroX =
           (int16_t) ((((uint16_t) values[1])  << 8) | ((uint16_t) values[0]));
    measurement.gyroY =
           (int16_t) ((((uint16_t) values[3])  << 8) | ((uint16_t) values[2]));
    measurement.gyroZ =
           (int16_t) ((((uint16_t) values[5])  << 8) | ((uint16_t) values[4]));

    return Imu::Error(Imu::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ImuLSM6DS3::initialize()
{
    myMailboxArray[0].mailboxFullCallback =
        &createCallback(this, &ImuLSM6DS3::accelMeasurementMailboxFullCallback);
    myMailboxArray[1].mailboxFullCallback =
         &createCallback(this, &ImuLSM6DS3::gyroMeasurementMailboxFullCallback);
    mySlaveDevice->setMailboxArray(myMailboxArray);

    if (isValidPointer(myInt1ExternalInterrupt))
    {
        myInt1ExternalInterrupt->setHandlerCallback(
               createCallback(this, &ImuLSM6DS3::int1ExternalInterruptHandler));
    }

    if (isValidPointer(myInt2ExternalInterrupt))
    {
        myInt2ExternalInterrupt->setHandlerCallback(
               createCallback(this, &ImuLSM6DS3::int2ExternalInterruptHandler));
    }

    setTempSensitivity(myTempSensitivity);
    setTempOffset(myTempOffset);

    myTransmitByteArray1.append((uint8_t) REGISTER_OUTX_L_XL);
    myTransmitByteArray2.append((uint8_t) REGISTER_OUT_TEMP_L);
}

//------------------------------------------------------------------------------
void ImuLSM6DS3::int1ExternalInterruptHandler(bool isActive)
{
     if (myConfig.readMode == READ_MODE_BLOCKING)
     {
         rawAccelMeasurementReady();
     }
     else if (myConfig.readMode == READ_MODE_MAILBOX)
     {
        myReceiveByteArray1.clear();
        mySlaveDevice->transmitReceive(myTransmitByteArray1,
                                       myReceiveByteArray1,
                                       0);
     }
}

//------------------------------------------------------------------------------
void ImuLSM6DS3::int2ExternalInterruptHandler(bool isActive)
{
     if (myConfig.readMode == READ_MODE_BLOCKING)
     {
         rawGyroMeasurementReady();
         rawTempMeasurementReady();
     }
     else if (myConfig.readMode == READ_MODE_MAILBOX)
     {
         myReceiveByteArray2.clear();
         mySlaveDevice->transmitReceive(myTransmitByteArray2,
                                        myReceiveByteArray2,
                                        1);
     }
}

//------------------------------------------------------------------------------
void ImuLSM6DS3::accelMeasurementMailboxFullCallback()
{
    rawAccelMeasurementReady();
}

//------------------------------------------------------------------------------
void ImuLSM6DS3::gyroMeasurementMailboxFullCallback()
{
    rawGyroMeasurementReady();
    rawTempMeasurementReady();
}
