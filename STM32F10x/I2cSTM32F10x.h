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
/// @file I2cSTM32F10x.h
/// @author Ben Minerd
/// @date 1/6/2016
/// @brief I2cSTM32F10x class header file.
///

#ifndef PLAT4M_I2C_STM32F10X_H
#define PLAT4M_I2C_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <I2c.h>
#include <GpioPinSTM32F10x.h>
#include <Module.h>
#include <ProcessorSTM32F10x.h>
#include <InterruptSTM32F10x.h>

#include <stm32f10x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class I2cSTM32F10x : public I2c
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------

    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };

	enum Interrupt
	{
		// I2C master interrupts
		INTERRUPT_EVENT = 0,
		INTERRUPT_BUFFER,
		INTERRUPT_ERROR
	};

	enum Event
	{
		EVENT_MASTER_MODE_SELECT = I2C_EVENT_MASTER_MODE_SELECT,
		EVENT_MASTER_TRANSMITTER_MODE_SELECTED = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED,
		EVENT_MASTER_RECEIVER_MODE_SELECTED = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED,
		EVENT_MASTER_BYTE_TRANSMITTING = I2C_EVENT_MASTER_BYTE_TRANSMITTING,
		EVENT_MASTER_BYTE_TRANSMITTED = I2C_EVENT_MASTER_BYTE_TRANSMITTED,
		EVENT_MASTER_BYTE_RECEIVED = I2C_EVENT_MASTER_BYTE_RECEIVED,
		EVENT_MASTER_BYTE_RECEIVED_BTF = I2C_EVENT_MASTER_BYTE_RECEIVED | 0x00000004
	};

	enum State
	{
		STATE_IDLE,
		STATE_BUSY,
		STATE_ERROR
	};
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    I2cSTM32F10x(const Id id,
                 GpioPinSTM32F10x& sclGpioPin,
                 GpioPinSTM32F10x& sdaGpioPin);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~I2cSTM32F10x();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F10x::Peripheral myPeripheralMap[];

    static const InterruptSTM32F10x::Id myInterruptIdMap[][2];

    static const uint16_t myInterruptMap[];

    static const uint16_t myAddressBitsMap[];

    static const GpioPinSTM32F10x::OutputSpeed myDefaultOutputSpeed;

    static const uint8_t myOwnAddress;

    // Variables

    static I2C_TypeDef* myI2cMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    I2C_TypeDef* myI2c;

    GpioPinSTM32F10x& mySclGpioPin;

    GpioPinSTM32F10x& mySdaGpioPin;

    volatile State myState;

    Transfer* myTransfer;
    
    InterruptSTM32F10x myEventInterrupt;

    InterruptSTM32F10x myErrorInterrupt;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods implemented from I2c
    //--------------------------------------------------------------------------

    I2c::Error driverSetConfig(const Config& config);

    I2c::Error driverMasterTransfer(Transfer& transfer);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void masterStart();

    inline void masterStop();

    inline void masterWriteAddress(const uint8_t address,
    							   const MasterMode masterMode);

    inline void writeByte(const uint8_t byte);

    inline uint8_t readByte();

    inline void enableAcknowledge(const bool enable);

    inline void enableAcknowledgePec(const bool enable);

    inline void interruptEnable(const Interrupt interrupt, const bool enable);

    inline void handleI2cEventMasterModeSelect();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void softwareReset();

    void reset();

    void eventInterruptHandler();

    void errorInterruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_I2C_STM32F10X_H
