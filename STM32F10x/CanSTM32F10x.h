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
/// @file CanSTM32F10x.h
/// @author Ben Minerd
/// @date 1/6/2016
/// @brief CanSTM32F10x class header file.
///

#ifndef PLAT4M_CAN_STM32F10X_H
#define PLAT4M_CAN_STM32F10X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <Can.h>
#include <GpioPinSTM32F10x.h>
#include <InterruptSTM32F10x.h>
#include <Module.h>

#include <stm32f10x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class CanSTM32F10x : public Can
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    /**
     * @brief Enumeration of SPIs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2
    };

    enum GpioRemap
    {
        GPIO_REMAP_0 = 0,
        GPIO_REMAP_1,
        GPIO_REMAP_2
    };

    /**
     * @brief Enumeration of SPI states.
     */
    enum State
    {
        STATE_IDLE,
        STATE_BUSY,
        STATE_ERROR // Split into multiple errors!
    };

    enum Interrupt
	{
    	INTERRUPT_TRANSMIT = 0,
		INTERRUPT_RECEIVE_0,
		INTERRUPT_RECEIVE_1,
		INTERRUPT_ERROR_STATUS_CHANGE
	};

    enum Mailbox
	{
    	MAILBOX_0 = 0,
		MAILBOX_1,
		MAILBOX_2
	};

    struct DriverConfig
    {
    	uint32_t timeQuantaSync;
    	uint32_t timeQuantaBitSegment1;
    	uint32_t timeQuantaBitSegment2;
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    CanSTM32F10x(const Id id,
                 GpioPinSTM32F10x& txGpioPin,
                 GpioPinSTM32F10x& rxGpioPin,
                 const GpioRemap gpioRemap);

    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------

    virtual ~CanSTM32F10x();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    CAN_TypeDef* getCan();

    GpioPinSTM32F10x& getTxGpioPin();

    GpioPinSTM32F10x& getRxGpioPin();

    void interruptHandler(const Interrupt interrupt);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const uint32_t myClockMap[];

    static const InterruptSTM32F10x::Id myInterruptIdMap[][4];

    static const uint32_t myInterruptMap[];

    static const uint32_t myIdMap[];

    static const uint32_t myFrameMap[];

    static const GpioPinSTM32F10x::OutputSpeed myDefaultOutputSpeed;

    static const uint32_t myMax16bitFilterIdCount;

    static const uint32_t myMax16bitFilterMaskIdCount;

    // Variables

    static CAN_TypeDef* myCanMap[];

    static uint32_t my16bitFilterIdCount[];
    
    static uint32_t my16bitFilterMaskIdCount[];

    static uint8_t myCan2FilterStartBank;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    CAN_TypeDef* myCan;
    
    GpioPinSTM32F10x& myTxGpioPin;
    
    GpioPinSTM32F10x& myRxGpioPin;
    
    const GpioRemap myGpioRemap;

    uint16_t myClockPrescaler;
    
    InterruptSTM32F10x myTransmitInterrupt;

    InterruptSTM32F10x myReceive0Interrupt;

    InterruptSTM32F10x myReceive1Interrupt;

    InterruptSTM32F10x myStatusChangeErrorInterrupt;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------

    Module::Error driverSetEnabled(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from Can
    //--------------------------------------------------------------------------
    
    Error driverSetConfig(const Config& config);

    Error driverSendMessage(const Message& message);
    
    Error driverAddAcceptanceFilter(const IdType idType,
    								const uint32_t filter,
    								const uint32_t mask);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void setInterruptEnabled(const Interrupt interrupt,
                                    const bool enabled);

    inline void receiveMessage(uint8_t fifoNumber);

    inline void dummyInterruptHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_CAN_STM32F10X_H
