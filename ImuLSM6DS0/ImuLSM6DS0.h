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
/// @file ImuLSM6DS0.h
/// @author Ben Minerd
/// @date 11/19/2015
/// @brief ImuLSM6DS0 class header file.
///

#ifndef IMU_LSM6DS0_H
#define IMU_LSM6DS0_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Imu.h>
#include <ErrorTemplate.h>
#include <Module.h>
#include <Spi.h>
#include <GpioPin.h>
#include <I2c.h>
#include <SlaveDevice.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ImuLSM6DS0 : public Imu
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

    /**
     * @brief Enumeration of  registers.
     */
    enum Register
    {
    	REGISTER_ACT_THS			= 0x04,
		REGISTER_ACT_DUR			= 0x05,
		REGISTER_INT_GEN_CFG_XL		= 0x06,
		REGISTER_INT_GEN_THS_X_XL	= 0x07,
		REGISTER_INT_GEN_THS_Y_XL	= 0x08,
		REGISTER_INT_GEN_THS_Z_XL	= 0x09,
		REGISTER_INT_GEN_DUR_XL		= 0x0A,
		REGISTER_REFERENCE_G		= 0x08,
		REGISTER_INT_CTL			= 0x0C,
		REGISTER_WHO_AM_I			= 0x0F,
		REGISTER_CTRL_REG1_G		= 0x10,
		REGISTER_CTRL_REG2_G		= 0x11,
		REGISTER_CTRL_REG3_G		= 0x12,
		REGISTER_ORIENT_CFG_G		= 0x13,
		REGISTER_INT_GEN_SRC_G		= 0x14,
		REGISTER_OUT_TEMP			= 0x15, // 2-byte register
		REGISTER_STATUS_REG			= 0x17,
		REGISTER_OUT_X_G			= 0x18, // 2-byte register
		REGISTER_OUT_Y_G			= 0x1A, // 2-byte register
		REGISTER_OUT_Z_G			= 0x1C, // 2-byte register
		REGISTER_CTRL_REG4			= 0x1E,
		REGISTER_CTRL_REG5_XL		= 0x1F,
		REGISTER_CTRL_REG6_XL		= 0x20,
		REGISTER_CTRL_REG7_XL		= 0x21,
		REGISTER_CTRL_REG8			= 0x22,
		REGISTER_CTRL_REG9			= 0x23,
		REGISTER_CTRL_REG10			= 0x24,
		REGISTER_INT_GEN_SRC_XL		= 0x26,
		// STATUS_REG is a repeat of 0x17
		REGISTER_OUT_X_XL			= 0x28, // 2-byte register
		REGISTER_OUT_Y_XL			= 0x2A, // 2-byte register
		REGISTER_OUT_Z_XL			= 0x2C, // 2-byte register
		REGISTER_FIFO_CTRL			= 0x2E,
		REGISTER_FIFO_SRC			= 0x2F,
		REGISTER_INT_GEN_CFG_G		= 0x30,
		REGISTER_INT_GEN_THS_X_G	= 0x31, // 2-byte register
		REGISTER_INT_GEN_THS_Y_G	= 0x33, // 2-byte register
		REGISTER_INT_GEN_THS_Z_G	= 0x35, // 2-byte register
		REGISTER_INT_GEN_DUR_G		= 0x37
    };

    enum AccelFullScale
	{
    	ACCEL_FULL_SCALE_PLUS_MINUS_2_G = 0,
		ACCEL_FULL_SCALE_PLUS_MINUS_4_G,
		ACCEL_FULL_SCALE_PLUS_MINUS_8_G,
		ACCEL_FULL_SCALE_PLUS_MINUS_16_G
	};

    enum GyroFullScale
	{
    	GYRO_FULL_SCALE_PLUS_MINUS_245_DPS = 0,
		GYRO_FULL_SCALE_PLUS_MINUS_500_DPS,
		GYRO_FULL_SCALE_PLUS_MINUS_2000_DPS
	};

    enum PinLevel
	{
		PIN_LEVEL_LOW = 0,
		PIN_LEVEL_HIGH
	};
    
    typedef Plat4m::ErrorTemplate<ErrorCode> Error;

    struct Config
    {
    	int a; // Placeholder
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

	ImuLSM6DS0(Plat4m::Spi& spi, Plat4m::GpioPin& chipSelectGpioPin);

	ImuLSM6DS0(const PinLevel sa0PinLevel, Plat4m::I2c& i2c);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

	virtual ~ImuLSM6DS0();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Error setLSM6DS0Config(const Config& config);

    Config getLSM6DS0Config() const;

    Error readRegister(const Register reg, uint8_t& value);

    Error readRegisters(const Register startingRegister, ByteArray& values);

    Error writeRegister(const Register reg, const uint8_t value);

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Plat4m::SlaveDevice* mySlaveDevice;

	Config myConfig;

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverEnable(const bool enable);

    //--------------------------------------------------------------------------
    // Private virtual methods implemented from Imu
    //--------------------------------------------------------------------------
    
    virtual Imu::Error driverSetConfig(const Imu::Config& config);

    virtual Imu::Error driverGetRawAccelMeasurement(
                                              RawAccelMeasurement& measurement);

    virtual Imu::Error driverGetRawGyroMeasurement(
                                               RawGyroMeasurement& measurement);

    virtual Imu::Error driverGetRawMeasurement(RawMeasurement& measurement);
};

}; // namespace Plat4m

#endif // IMU_LSM6DS0_H
