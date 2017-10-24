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
/// @file I2cSTM32F4xx.h
/// @author Ben Minerd
/// @date 3/25/13
/// @brief I2cSTM32F4xx class.
///

#ifndef I2C_STM32F4XX_H
#define I2C_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <I2c.h>
#include <GpioPinSTM32F4xx.h>
#include <Module.h>
#include <InterruptSTM32F4xx.h>

#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class I2cSTM32F4xx : public I2c
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    /**
     * @brief Enumeration of I2Cs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };

    /**
	 * @brief Enumeration of I2C interrupts.
	 */
	enum Interrupt
	{
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
		EVENT_MASTER_BYTE_RECEIVING = I2C_EVENT_MASTER_BYTE_RECEIVED,
		EVENT_MASTER_BYTE_RECEIVED = I2C_EVENT_MASTER_BYTE_RECEIVED | 0x00000004
	};

	enum State
	{
		STATE_IDLE,
		STATE_BUSY,
		STATE_ERROR
	};

    struct Register
    {
        struct Cr1
        {
            enum Bit
            {
                BIT_SOFTWARE_RESET                  = (uint16_t) 0x8000,
                // Bit 14: Reserved
                BIT_SMBUS_ALERT                     = (uint16_t) 0x2000,
                BIT_PACKET_ERROR_CHECKING           = (uint16_t) 0x1000,
                BIT_ACKNOWLEDGE_PEC_POSITION        = (uint16_t) 0x0800,
                BIT_ACKNOWLEDGE_ENABLE              = (uint16_t) 0x0400,
                BIT_STOP_GENERATION                 = (uint16_t) 0x0200,
                BIT_START_GENERATION                = (uint16_t) 0x0100,
                BIT_CLOCK_STRETCHING_DISABLE        = (uint16_t) 0x0080,
                BIT_GENERAL_CALL_ENABLE             = (uint16_t) 0x0040,
                BIT_PEC_ENABLE                      = (uint16_t) 0x0020,
                BIT_ARP_ENABLE                      = (uint16_t) 0x0010,
                BIT_SMBUS_TYPE                      = (uint16_t) 0x0008,
                // Bit 2: Reserved
                BIT_SMBUS_MODE                      = (uint16_t) 0x0002,
                BIT_PERIPHERAL_ENABLE               = (uint16_t) 0x0001
            };
        };

        struct Cr2
        {
            enum Bit
            {
                // Bits 15-13: Reserved
                BIT_DMA_LAST_TRANSFER               = (uint16_t) 0x1000,
                BIT_DMA_REQUESTS_ENABLE             = (uint16_t) 0x0800,
                BIT_BUFFER_INTERRUPT_ENABLE         = (uint16_t) 0x0400,
                BIT_EVENT_INTERRUPT_ENABLE          = (uint16_t) 0x0200,
                BIT_ERROR_INTERRUPT_ENABLE          = (uint16_t) 0x0100
                // Bits 7-6: Reserved
                // Bits 5-0: Peripheral clock frequency
            };
        };

        struct Sr1
        {
            enum Bit
            {
                BIT_SMBUS_ALERT                     = (uint16_t) 0x8000,
                BIT_TIMEOUT                         = (uint16_t) 0x4000,
                // Bit 13: Reserved
                BIT_PEC_ERROR                       = (uint16_t) 0x1000,
                BIT_OVERRUN_UNDERRUN                = (uint16_t) 0x0800,
                BIT_ACKNOWLEDGE_FAILURE             = (uint16_t) 0x0400,
                BIT_ARBITRATION_LOST                = (uint16_t) 0x0200,
                BIT_BUS_ERROR                       = (uint16_t) 0x0100,
                BIT_TRANSMIT_EMPTY                  = (uint16_t) 0x0080,
                BIT_RECEIVE_NOT_EMPTY               = (uint16_t) 0x0040,
                // Bit 5: Reserved
                BIT_STOP_DETECTION                  = (uint16_t) 0x0010,
                BIT_10_BIT_HEADER_SENT              = (uint16_t) 0x0008,
                BIT_BYTE_TRANSFER_FINISHED          = (uint16_t) 0x0004,
                BIT_ADDRESS_SENT_MATCHED            = (uint16_t) 0x0002,
                BIT_START_BIT                       = (uint16_t) 0x0001
            };
        };

        struct Sr2
        {
            enum Bit
            {
                // Bits 15-8: Packet error checking register
                BIT_DUAL_FLAG                       = (uint16_t) 0x0080,
                BIT_SMBUS_HOST_READER               = (uint16_t) 0x0040,
                BIT_SMBUS_DEVICE_DEFAULT_ADDRESS    = (uint16_t) 0x0020,
                BIT_GENERAL_CALL_ADDRESS            = (uint16_t) 0x0010,
                // Bit 3: Reserved
                BIT_TRANSMITTER_RECEIVER            = (uint16_t) 0x0004,
                BIT_BUS_BUSY                        = (uint16_t) 0x0002,
                BIT_MASTER_SLAVE                    = (uint16_t) 0x0001
            };
        };

        struct Ccr
        {
            enum Bit
            {
                BIT_MASTER_MODE_SELECTION           = (uint16_t) 0x8000,
                BIT_FAST_MODE_DUTY_CYCLE            = (uint16_t) 0x4000,
                // Bits 13-12: Reserved
                // Bits 11-0: Clock control value
            };
        };
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    I2cSTM32F4xx(const Id id,
                 GpioPinSTM32F4xx& sclGpioPin,
                 GpioPinSTM32F4xx& sdaGpioPin,
                 const uint8_t ownAddress = 0xA0);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const uint32_t myClockMap[];

    static const GpioSTM32F4xx::AlternateFunction myAlternateFunctionMap[];

    static const InterruptSTM32F4xx::Id myInterruptIdMap[][2];

    static const uint16_t myAddressBitsMap[];

    static const GpioSTM32F4xx::OutputSpeed myDefaultOutputSpeed;

    static const uint16_t myInterruptMap[];

    // Variables

    static I2C_TypeDef* myI2cMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const Id myId;

    GpioPinSTM32F4xx& mySclGpioPin;

    GpioPinSTM32F4xx& mySdaGpioPin;

    const uint8_t myOwnAddress;

    I2C_TypeDef* myI2c;

    InterruptSTM32F4xx myEventInterrupt;

    InterruptSTM32F4xx myErrorInterrupt;

    volatile State myState;

    Transfer* myCurrentTransfer;
    
    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverEnable(const bool enable);

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

    inline void setInterruptEnabled(const Interrupt interrupt,
                                    const bool enable);

    inline void disableAllInterrupts();

    inline void handleMasterModeSelectEvent();

    inline void handleMasterTransmitterModeSelectedEvent();

    inline void handleMasterReceiverModeSelectedEvent();

    inline void handleMasterByteTransmittingEvent();

    inline void handleMasterByteTransmittedEvent();

    inline void handleMasterByteReceivingEvent();

    inline void handleMasterByteReceivedEvent();

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void softwareReset();

    void reset();

    void eventInterruptHandler();

    void errorInterruptHandler();
};

}; // namespace Plat4m

#endif // I2C_STM32F4XX_H
