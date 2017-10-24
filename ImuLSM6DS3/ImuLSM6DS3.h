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
/// @file ImuLSM6DS3.h
/// @author Ben Minerd
/// @date 11/20/2015
/// @brief ImuLSM6DS3 class header file.
///

#ifndef PLAT4M_IMU_LSM6DS3_H
#define PLAT4M_IMU_LSM6DS3_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Imu.h>
#include <ErrorTemplate.h>
#include <Module.h>
#include <Spi.h>
#include <GpioPin.h>
#include <I2c.h>
#include <ExternalInterrupt.h>
#include <SlaveDevice.h>
#include <ByteArrayN.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ImuLSM6DS3 : public Imu
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
	enum ErrorCode
	{
		ERROR_CODE_NONE,
		ERROR_CODE_NOT_ENABLED,
		ERROR_CODE_CONFIG_FAILED,
		ERROR_CODE_MEASUREMENT_FAILED,
		ERROR_CODE_COMMUNICATION_FAILED
	};

	enum PinLevel
	{
		PIN_LEVEL_LOW = 0,
		PIN_LEVEL_HIGH
	};

    enum Register
    {
        REGISTER_FUNC_CFG_ACCESS		= 0x01,
		REGISTER_SENSOR_SYNC_TIME_FRAME	= 0x04,
		REGISTER_FIFO_CTRL1				= 0x06,
		REGISTER_FIFO_CTRL2				= 0x07,
		REGISTER_FIFO_CTRL3				= 0x08,
		REGISTER_FIFO_CTRL4				= 0x09,
		REGISTER_FIFO_CTRL5				= 0x0A,
		REGISTER_ORIENT_CFG_G			= 0x0B,
		REGISTER_INT1_CTRL				= 0x0D,
		REGISTER_INT2_CTRL				= 0x0E,
		REGISTER_WHO_AM_I				= 0x0F,
		REGISTER_CTRL1_XL				= 0x10,
		REGISTER_CTRL2_G				= 0x11,
		REGISTER_CTRL3_C				= 0x12,
		REGISTER_CTRL4_C				= 0x13,
		REGISTER_CTRL5_C				= 0x14,
		REGISTER_CTRL6_C				= 0x15,
		REGISTER_CTRL7_G				= 0x16,
		REGISTER_CTRL8_XL				= 0x17,
		REGISTER_CTRL9_XL				= 0x18,
		REGISTER_CTRL10_C				= 0x19,
		REGISTER_MASTER_CONFIG			= 0x1A,
		REGISTER_WAKE_UP_SRC			= 0x1B,
		REGISTER_TAP_SRC				= 0x1C,
		REGISTER_D6D_SRC				= 0x1D,
		REGISTER_STATUS_REG				= 0x1E,
		REGISTER_OUT_TEMP_L				= 0x20,
		REGISTER_OUT_TEMP_H				= 0x21,
		REGISTER_OUTX_L_G				= 0x22,
		REGISTER_OUTX_H_G				= 0x23,
		REGISTER_OUTY_L_G				= 0x24,
		REGISTER_OUTY_H_G				= 0x25,
		REGISTER_OUTZ_L_G				= 0x26,
		REGISTER_OUTZ_H_G				= 0x27,
		REGISTER_OUTX_L_XL				= 0x28,
		REGISTER_OUTX_H_XL				= 0x29,
		REGISTER_OUTY_L_XL				= 0x2A,
		REGISTER_OUTY_H_XL				= 0x2B,
		REGISTER_OUTZ_L_XL				= 0x2C,
		REGISTER_OUTZ_H_XL				= 0x2D,
		REGISTER_SENSORHUB1_REG			= 0x2E,
		REGISTER_SENSORHUB2_REG			= 0x2F,
		REGISTER_SENSORHUB3_REG			= 0x30,
		REGISTER_SENSORHUB4_REG			= 0x31,
		REGISTER_SENSORHUB5_REG			= 0x32,
		REGISTER_SENSORHUB6_REG			= 0x33,
		REGISTER_SENSORHUB7_REG			= 0x34,
		REGISTER_SENSORHUB8_REG			= 0x35,
		REGISTER_SENSORHUB9_REG			= 0x36,
		REGISTER_SENSORHUB10_REG		= 0x37,
		REGISTER_SENSORHUB11_REG		= 0x38,
		REGISTER_SENSORHUB12_REG		= 0x39,
		REGISTER_FIFO_STATUS1			= 0x3A,
		REGISTER_FIFO_STATUS2			= 0x3B,
		REGISTER_FIFO_STATUS3			= 0x3C,
		REGISTER_FIFO_STATUS4			= 0x3D,
		REGISTER_FIFO_DATA_OUT_L		= 0x3E,
		REGISTER_FIFO_DATA_OUT_H		= 0x3F,
		REGISTER_TIMESTAMP0_REG			= 0x40,
		REGISTER_TIMESTAMP1_REG			= 0x41,
		REGISTER_TIMESTAMP2_REG			= 0x42,
		REGISTER_STEP_TIMESTAMP_L		= 0x49,
		REGISTER_STEP_TIMESTAMP_H		= 0x4A,
		REGISTER_STEP_COUNTER_L			= 0x4B,
		REGISTER_STEP_COUNTER_H			= 0x4C,
		REGISTER_SENSORHUB13_REG		= 0x4D,
		REGISTER_SENSORHUB14_REG		= 0x4E,
		REGISTER_SENSORHUB15_REG		= 0x4F,
		REGISTER_SENSORHUB16_REG		= 0x50,
		REGISTER_SENSORHUB17_REG		= 0x51,
		REGISTER_SENSORHUB18_REG		= 0x52,
		REGISTER_FUNC_SRC				= 0x53,
		REGISTER_TAP_CFG				= 0x58,
		REGISTER_TAP_THS_6D				= 0x59,
		REGISTER_INT_DUR2				= 0x5A,
		REGISTER_WAKE_UP_THS			= 0x5B,
		REGISTER_WAKE_UP_DUR			= 0x5C,
		REGISTER_FREE_FALL				= 0x5D,
		REGISTER_MD1_CFG				= 0x5E,
		REGISTER_MD2_CFG				= 0x5F,
		REGISTER_OUT_MAG_RAW_X_L		= 0x66,
		REGISTER_OUT_MAG_RAW_X_H		= 0x67,
		REGISTER_OUT_MAG_RAW_Y_L		= 0x68,
		REGISTER_OUT_MAG_RAW_Y_H		= 0x69,
		REGISTER_OUT_MAG_RAW_Z_L		= 0x6A,
		REGISTER_OUT_MAG_RAW_Z_H		= 0x6B
    };

    enum AccelFullScale
	{
    	ACCEL_FULL_SCALE_PLUS_MINUS_2_G = 0,
		ACCEL_FULL_SCALE_PLUS_MINUS_4_G,
		ACCEL_FULL_SCALE_PLUS_MINUS_8_G,
		ACCEL_FULL_SCALE_PLUS_MINUS_16_G
	};

    enum AccelOutputDataRate
	{
    	ACCEL_OUTPUT_DATA_RATE_0_HZ = 0,
		ACCEL_OUTPUT_DATA_RATE_13_HZ,
		ACCEL_OUTPUT_DATA_RATE_26_HZ,
		ACCEL_OUTPUT_DATA_RATE_52_HZ,
		ACCEL_OUTPUT_DATA_RATE_104_HZ,
		ACCEL_OUTPUT_DATA_RATE_208_HZ,
		ACCEL_OUTPUT_DATA_RATE_416_HZ,
		ACCEL_OUTPUT_DATA_RATE_833_HZ,
		ACCEL_OUTPUT_DATA_RATE_1660_HZ,
		ACCEL_OUTPUT_DATA_RATE_3330_HZ,
		ACCEL_OUTPUT_DATA_RATE_6660_HZ
	};

    enum AccelAntiAliasingFilterBandwidth
	{
    	ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_50_HZ = 0,
		ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_100_HZ,
		ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_200_HZ,
		ACCEL_ANTI_ALIASING_FILTER_BANDWIDTH_400_HZ
	};

    enum AccelLowPassFilterCutoffFrequency
	{
    	ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_9 = 0,
		ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_50,
		ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_100,
		ACCEL_LOW_PASS_FILTER_CUTOFF_FREQUENCY_ODR_XL_DIVIDE_BY_400
	};

    enum GyroFullScale
	{
		GYRO_FULL_SCALE_PLUS_MINUS_125_DPS = 0,
		GYRO_FULL_SCALE_PLUS_MINUS_245_DPS,
		GYRO_FULL_SCALE_PLUS_MINUS_500_DPS,
		GYRO_FULL_SCALE_PLUS_MINUS_1000_DPS,
		GYRO_FULL_SCALE_PLUS_MINUS_2000_DPS
	};

    enum GyroOutputDataRate
	{
    	GYRO_OUTPUT_DATA_RATE_0_HZ = 0,
		GYRO_OUTPUT_DATA_RATE_13_HZ,
		GYRO_OUTPUT_DATA_RATE_26_HZ,
		GYRO_OUTPUT_DATA_RATE_52_HZ,
		GYRO_OUTPUT_DATA_RATE_104_HZ,
		GYRO_OUTPUT_DATA_RATE_208_HZ,
		GYRO_OUTPUT_DATA_RATE_416_HZ,
		GYRO_OUTPUT_DATA_RATE_833_HZ,
		GYRO_OUTPUT_DATA_RATE_1660_HZ
	};

    enum GyroHighPassFilterCutoffFrequency
	{
    	GYRO_HIGH_PASS_FILTER_CUTOFF_FREQUENCY_0_0081_HZ = 0,
		GYRO_HIGH_PASS_FILTER_CUTOFF_FREQUENCY_0_0324_HZ,
		GYRO_HIGH_PASS_FILTER_CUTOFF_FREQUENCY_2_07_HZ,
		GYRO_HIGH_PASS_FILTER_CUTOFF_FREQUENCY_16_32_HZ
	};
    
    enum ReadMode
    {
        READ_MODE_BLOCKING = 0,
        READ_MODE_MAILBOX
    };

    typedef ErrorTemplate<ErrorCode> Error;

    struct Config
    {
    	AccelAntiAliasingFilterBandwidth accelAntiAliasingFilterBandwidth;
    	bool isAccelLowPassFilterEnabled;
    	AccelLowPassFilterCutoffFrequency accelLowPassFilterCutoffFrequency;
    	bool isGyroHighPassFilterEnabled;
    	GyroHighPassFilterCutoffFrequency gyroHighPassFilterCutoffFrequency;
    	ReadMode readMode;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

	ImuLSM6DS3(Spi& spi,
	           GpioPin& chipSelectGpioPin,
			   ExternalInterrupt* int1ExternalInterrupt = 0,
			   ExternalInterrupt* int2ExternalInterrupt = 0);

	ImuLSM6DS3(const PinLevel sa0PinLevel,
			   I2c& i2c,
			   ExternalInterrupt* int1ExternalInterrupt = 0,
			   ExternalInterrupt* int2ExternalInterrupt = 0);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

	virtual ~ImuLSM6DS3();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error setLSM6DS3Config(const Config& config);

    Config getLSM6DS3Config() const;

    Error readRegister(const Register reg, uint8_t& value);

    Error readRegisters(const Register startingRegister, ByteArray& values);

    Error writeRegister(const Register reg, const uint8_t value);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const uint8_t myI2cBaseAddress;

    static const float myTempSensitivity;

    static const float myTempOffset;

    static const float myAccelFullScaleMap[];

    static const uint8_t myAccelFullScaleMaskMap[];

    static const float myAccelFullScaleSensitivityMap[];

    static const uint32_t myAccelOutputDataRateMap[];

    static const uint8_t myAccelOutputDataRateMaskMap[];

    static const uint8_t myAccelAntiAliasingFilterBandwidthMaskMap[];

    static const uint8_t myAccelLowPassFilterCutoffFrequencyMaskMap[];

    static const float myGyroFullScaleMap[];

    static const uint8_t myGyroFullScaleMaskMap[];

    static const float myGyroFullScaleSensitivityMap[];

    static const uint32_t myGyroOutputDataRateMap[];

    static const uint8_t myGyroOutputDataRateMaskMap[];

    static const uint8_t myGyroFilterBandwidthMaskMap[];
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    SlaveDevice* mySlaveDevice;

    ExternalInterrupt* myInt1ExternalInterrupt;

    ExternalInterrupt* myInt2ExternalInterrupt;

	Config myConfig;

	int16_t myTemp;

    ByteArrayN<4> myTransmitByteArray1;

    ByteArrayN<6> myReceiveByteArray1;

    ByteArrayN<2> myTransmitByteArray2;

    ByteArrayN<8> myReceiveByteArray2;

    ArrayN<MasterSlaveBus::Mailbox, 2> myMailboxArray;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Imu
    //--------------------------------------------------------------------------
    
    virtual Imu::Error driverSetConfig(const Imu::Config& config);

    virtual Imu::Error driverGetRawTempMeasurement(int32_t& temp);

    virtual Imu::Error driverGetRawAccelMeasurement(
                                              RawAccelMeasurement& measurement);

    virtual Imu::Error driverGetRawGyroMeasurement(
                                               RawGyroMeasurement& measurement);

    virtual Imu::Error driverGetRawMeasurement(RawMeasurement& measurement);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initialize();

    void int1ExternalInterruptHandler(bool isActive);

    void int2ExternalInterruptHandler(bool isActive);

    void accelMeasurementMailboxFullCallback();

    void gyroMeasurementMailboxFullCallback();
};

}; // namespace Plat4m

#endif // PLAT4M_IMU_LSM6DS3_H
