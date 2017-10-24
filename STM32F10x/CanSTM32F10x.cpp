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
/// @file CanSTM32F10x.cpp
/// @author Ben Minerd
/// @date 1/6/2016
/// @brief CanSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m.h>
#include <CanSTM32F10x.h>
#include <Buffer.h>
#include <ProcessorSTM32F10x.h>
#include <System.h>
#include <CallbackMethod.h>

#include <stm32f10x_can.h>
#include <stm32f10x_rcc.h>

using Plat4m::CanSTM32F10x;
using Plat4m::Can;
using Plat4m::GpioPinSTM32F10x;
using Plat4m::InterruptSTM32F10x;
using Plat4m::Module;
using Plat4m::GpioPin;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F10x* interruptObjectMap[2][4];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const uint32_t CanSTM32F10x::myClockMap[] =
{
	RCC_APB1Periph_CAN1, /// CanSTM32F10x::ID_1
	RCC_APB1Periph_CAN2  /// CanSTM32F10x::ID_2
};

const InterruptSTM32F10x::Id CanSTM32F10x::myInterruptIdMap[][4] =
{
	/// ID_1
	{
	    InterruptSTM32F10x::ID_CAN_1_TX,  /// INTERRUPT_TRANSMIT
	    InterruptSTM32F10x::ID_CAN_1_RX0, /// INTERRUPT_RECEIVE_0
	    InterruptSTM32F10x::ID_CAN_1_RX1, /// INTERRUPT_RECEIVE_1
	    InterruptSTM32F10x::ID_CAN_1_SCE  /// INTERRUPT_ERROR_STATUS_CHANGE
	},
	/// ID_2
	{
	    InterruptSTM32F10x::ID_CAN_2_TX,  /// INTERRUPT_TRANSMIT
	    InterruptSTM32F10x::ID_CAN_2_RX0, /// INTERRUPT_RECEIVE_0
	    InterruptSTM32F10x::ID_CAN_2_RX1, /// INTERRUPT_RECEIVE_1
	    InterruptSTM32F10x::ID_CAN_2_SCE  /// INTERRUPT_ERROR_STATUS_CHANGE
	}
};

const uint32_t CanSTM32F10x::myInterruptMap[] =
{
	CAN_IT_TME
	// Add others...
};

const uint32_t CanSTM32F10x::myIdMap[] =
{
	CAN_Id_Standard, /// Can::ID_TYPE_BASE
	CAN_Id_Extended  /// Can::ID_TYPE_EXTENDED
};

const uint32_t CanSTM32F10x::myFrameMap[] =
{
	CAN_RTR_Data,  /// Can::FRAME_TYPE_DATA
	CAN_RTR_Remote /// Can::FRAME_TYPE_REMOTE
};

const GpioPinSTM32F10x::OutputSpeed CanSTM32F10x::myDefaultOutputSpeed =
                                           GpioPinSTM32F10x::OUTPUT_SPEED_50MHZ;

const uint32_t CanSTM32F10x::myMax16bitFilterIdCount = 28 * 2;

const uint32_t CanSTM32F10x::myMax16bitFilterMaskIdCount = 28 * 2;

// Variables

/**
 * @brief Can map.
 */
CAN_TypeDef* CanSTM32F10x::myCanMap[] =
{
    CAN1, /// CanSTM32F10x::ID_1
    CAN2  /// CanSTM32F10x::ID_2
};

uint32_t CanSTM32F10x::my16bitFilterIdCount[2] =
{
	0, // CanSTM32F10x::ID_1
	0  // CanSTM32F10x::ID_2
};

uint32_t CanSTM32F10x::my16bitFilterMaskIdCount[2] =
{
	0, // CanSTM32F10x::ID_1
	0  // CanSTM32F10x::ID_2
};

uint8_t CanSTM32F10x::myCan2FilterStartBank = 15;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanSTM32F10x::CanSTM32F10x(const Id id,
                           GpioPinSTM32F10x& txGpioPin,
                           GpioPinSTM32F10x& rxGpioPin,
                           const GpioRemap gpioRemap) :
    Can(),
    myId(id),
    myCan(myCanMap[myId]),
    myTxGpioPin(txGpioPin),
    myRxGpioPin(rxGpioPin),
    myGpioRemap(gpioRemap),
    myClockPrescaler(0),
    myTransmitInterrupt(
                    myInterruptIdMap[myId][INTERRUPT_TRANSMIT],
                    createCallback(this, &CanSTM32F10x::dummyInterruptHandler)),
    myReceive0Interrupt(
                    myInterruptIdMap[myId][INTERRUPT_RECEIVE_0],
                    createCallback(this, &CanSTM32F10x::dummyInterruptHandler)),
    myReceive1Interrupt(
                    myInterruptIdMap[myId][INTERRUPT_RECEIVE_1],
                    createCallback(this, &CanSTM32F10x::dummyInterruptHandler)),
    myStatusChangeErrorInterrupt(
                     myInterruptIdMap[myId][INTERRUPT_ERROR_STATUS_CHANGE],
                     createCallback(this, &CanSTM32F10x::dummyInterruptHandler))
{
    if (isValidPointer(interruptObjectMap[myId][INTERRUPT_TRANSMIT]))
    {
        // Trying to instantiate peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId][INTERRUPT_TRANSMIT]  = &myTransmitInterrupt;
    interruptObjectMap[myId][INTERRUPT_RECEIVE_0] = &myReceive0Interrupt;
    interruptObjectMap[myId][INTERRUPT_RECEIVE_1] = &myReceive1Interrupt;
    interruptObjectMap[myId][INTERRUPT_ERROR_STATUS_CHANGE]  =
                                                  &myStatusChangeErrorInterrupt;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CanSTM32F10x::~CanSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
CAN_TypeDef* CanSTM32F10x::getCan()
{
	return myCan;
}

//------------------------------------------------------------------------------
GpioPinSTM32F10x& CanSTM32F10x::getTxGpioPin()
{
	return myTxGpioPin;
}

//------------------------------------------------------------------------------
GpioPinSTM32F10x& CanSTM32F10x::getRxGpioPin()
{
	return myRxGpioPin;
}

//------------------------------------------------------------------------------
void CanSTM32F10x::interruptHandler(const Interrupt interrupt)
{
	switch (interrupt)
	{
	    case INTERRUPT_TRANSMIT:
	    {
	        break;
	    }
        case INTERRUPT_RECEIVE_0:
        {
            receiveMessage(0);

            break;
        }
        case INTERRUPT_RECEIVE_1:
        {
            receiveMessage(1);

            break;
        }
        case INTERRUPT_ERROR_STATUS_CHANGE:
        {
            break;
        }
	}
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error CanSTM32F10x::driverSetEnabled(const bool enabled)
{
	myTxGpioPin.setEnabled(enabled);
	myRxGpioPin.setEnabled(enabled);

	RCC_APB1PeriphClockCmd(myClockMap[myId], (FunctionalState) enabled);

    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannelPreemptionPriority  = 0;
    nvicInit.NVIC_IRQChannelSubPriority         = 0;
    nvicInit.NVIC_IRQChannelCmd                 = (FunctionalState) enabled;

    myTransmitInterrupt.setEnabled(enabled);
    myReceive0Interrupt.setEnabled(enabled);
    myReceive1Interrupt.setEnabled(enabled);
    myStatusChangeErrorInterrupt.setEnabled(enabled);

	// TODO: For now
	CAN_ITConfig(myCan, CAN_IT_FMP0, (FunctionalState) enabled);
	CAN_ITConfig(myCan, CAN_IT_FMP1, (FunctionalState) enabled);

	CAN_DBGFreeze(myCan, DISABLE); // For now

	if (enabled)
	{
		CAN_OperatingModeRequest(myCan, CAN_OperatingMode_Normal);
	}
	else
	{
		CAN_OperatingModeRequest(myCan, CAN_OperatingMode_Sleep);
	}

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from Can
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Can::Error CanSTM32F10x::driverSetConfig(const Config& config)
{
	CAN_OperatingModeRequest(myCan, CAN_OperatingMode_Initialization);

    GpioPin::Config gpioConfig;
    gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
    
    GpioPinSTM32F10x::STM32F10xConfig gpioDriverConfig;
    gpioDriverConfig.outputType  = GpioPinSTM32F10x::OUTPUT_TYPE_PUSH_PULL;
    gpioDriverConfig.outputSpeed = myDefaultOutputSpeed;
    
    myTxGpioPin.configure(gpioConfig);
    myTxGpioPin.setSTM32F10xConfig(gpioDriverConfig);
    
    gpioConfig.mode     = GpioPin::MODE_DIGITAL_INPUT;
    gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;

    myRxGpioPin.configure(gpioConfig);

    if (myGpioRemap == GPIO_REMAP_1)
    {
        GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
    }
    else if (myGpioRemap == GPIO_REMAP_2)
    {
        GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE);
    }

    unsigned int timeQuantaNominal = config.timeQuantaBitSegment1 +
                                     config.timeQuantaBitSegment2 + 1;

    uint32_t clockFrequencyHz = config.baudRate * timeQuantaNominal;

    uint16_t clockPrescaler = 1;

    while (clockPrescaler <= 1024)
    {
		if (((ProcessorSTM32F10x::getApb1ClockFrequencyHz()) /
			 (clockPrescaler)) 									==
														       clockFrequencyHz)
		{
			break;
		}

		clockPrescaler++;
    }

    // True if no prescaler was found to make the desired clock frequency valid
    if (clockPrescaler == 1025)
    {
    	return Can::Error(Can::ERROR_CODE_BAUD_RATE_INVALID);
    }

    myClockPrescaler = clockPrescaler;

    CAN_InitTypeDef CanInit;
    CanInit.CAN_Prescaler = myClockPrescaler;
    CanInit.CAN_Mode      = CAN_Mode_Normal;
    CanInit.CAN_SJW	      = config.timeQuantaSyncJumpWidth - 1;
    CanInit.CAN_BS1		  = config.timeQuantaBitSegment1 - 1;
    CanInit.CAN_BS2		  = config.timeQuantaBitSegment2 - 1;
    CanInit.CAN_TTCM      =
    				 (FunctionalState) config.timeTriggeredCommunicationEnabled;
    CanInit.CAN_ABOM	  = DISABLE; // TODO: Implement these features
    CanInit.CAN_AWUM	  = DISABLE;
    CanInit.CAN_NART	  = DISABLE;
    CanInit.CAN_RFLM	  = DISABLE;
    CanInit.CAN_TXFP	  = DISABLE;

    CAN_Init(myCan, &CanInit);
    
    // Sets the CAN2 acceptance filter start bank to 15 (0-13 are CAN1, 14-27
    // are for CAN2)
    CAN_SlaveStartBank(myCan2FilterStartBank);

    CAN_OperatingModeRequest(myCan, CAN_OperatingMode_Normal);

    return Can::Error(Can::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Can::Error CanSTM32F10x::driverSendMessage(const Message& message)
{
	CanTxMsg canTxMsg;

	canTxMsg.StdId = message.id;
	canTxMsg.ExtId = 0;
    canTxMsg.IDE   = myIdMap[message.idType];
    canTxMsg.RTR   = myFrameMap[message.frameType];
    canTxMsg.DLC   = message.data.getSize();
    memcpy(canTxMsg.Data, message.data.getItems(), canTxMsg.DLC);

    uint8_t mailboxNumber = CAN_Transmit(myCan, &canTxMsg);

	return Can::Error(Can::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Can::Error CanSTM32F10x::driverAddAcceptanceFilter(const IdType idType,
												   const uint32_t filter,
												   const uint32_t mask)
{
	CAN_FilterInitTypeDef canFilterInit;

	unsigned int filter16BitWidth = 0;
	unsigned int mask16BitWidth = 0;

	if (idType == ID_TYPE_BASE)
	{
		filter16BitWidth = 1;
	}
	else // idType == ID_TYPE_EXTENDED
	{
		filter16BitWidth = 2;
	}

	unsigned int offset;

	if (myId == ID_1)
	{
		offset = 0;
	}
	else // myId == ID_2
	{
		offset = myCan2FilterStartBank;
	}

	if (filter == mask) // Filter-only mode
	{
		canFilterInit.CAN_FilterMode = CAN_FilterMode_IdList;
		canFilterInit.CAN_FilterNumber =
									  offset + (my16bitFilterIdCount[myId] / 2);

		unsigned int remainder = ((my16bitFilterIdCount[myId] +
								   my16bitFilterMaskIdCount[myId]) % 4);

		// If the width of the filter in 16-bit words is greater than the
		// distance to the next filter, the next filter must be used
		if ((4 - remainder) < filter16BitWidth)
		{
			canFilterInit.CAN_FilterNumber += 1;
			my16bitFilterMaskIdCount[myId] += (2 - remainder);

			remainder = ((my16bitFilterIdCount[myId] +
					      my16bitFilterMaskIdCount[myId]) % 4);
		}

		switch (remainder)
		{
			case 0:
			{
				if (filter16BitWidth == 2)
				{
					canFilterInit.CAN_FilterIdHigh = (uint16_t) (filter >> 16);
					canFilterInit.CAN_FilterIdLow  = (uint16_t) filter;
				}
				else // filter16BitWidth == 1
				{
					canFilterInit.CAN_FilterIdHigh = 0;
					canFilterInit.CAN_FilterIdLow  = (uint16_t) (filter << 5);
				}

				canFilterInit.CAN_FilterMaskIdHigh = 0;
				canFilterInit.CAN_FilterMaskIdLow  = 0;

				break;
			}
			case 1:
			{
				canFilterInit.CAN_FilterIdHigh = (uint16_t) filter;

				break;
			}
			case 2:
			{
				if (filter16BitWidth == 2)
				{
					canFilterInit.CAN_FilterMaskIdHigh =
													  (uint16_t) (filter >> 16);
					canFilterInit.CAN_FilterMaskIdLow = (uint16_t) filter;
				}
				else // filter16BitWidth == 1
				{
					canFilterInit.CAN_FilterMaskIdHigh = 0;
					canFilterInit.CAN_FilterMaskIdLow  =
													   (uint16_t) (filter << 5);
				}

				mask16BitWidth = filter16BitWidth;
				filter16BitWidth = 0;

				break;
			}
			case 3:
			{
				canFilterInit.CAN_FilterMaskIdHigh = (uint16_t) filter;

				mask16BitWidth = filter16BitWidth;
				filter16BitWidth = 0;

				break;
			}
			default:
			{
				// Should never get here

				break;
			}
		}
	}
	else // Filter+mask mode
	{
		mask16BitWidth = filter16BitWidth;

		canFilterInit.CAN_FilterMode = CAN_FilterMode_IdMask;
		canFilterInit.CAN_FilterNumber =
									  offset + (my16bitFilterIdCount[myId] / 2);

		// Was last filter used in filter-only mode?
		if (my16bitFilterIdCount[myId] > my16bitFilterMaskIdCount[myId])
		{
			canFilterInit.CAN_FilterNumber += 1;
			my16bitFilterMaskIdCount[myId] +=
				  (my16bitFilterIdCount[myId] - my16bitFilterMaskIdCount[myId]);
		}

		unsigned int remainder = (my16bitFilterIdCount[myId] % 2);

		// If the width of the filter in 16-bit words is greater than the
		// distance to the next filter, the next filter must be used
		if ((4 - remainder) < filter16BitWidth)
		{
			canFilterInit.CAN_FilterNumber += 1;
			my16bitFilterIdCount[myId]     += (2 - remainder);
			my16bitFilterMaskIdCount[myId] += (2 - remainder);

			remainder = (my16bitFilterIdCount[myId] % 2);
		}

		switch (remainder)
		{
			case 0:
			{
				if (filter16BitWidth == 2) // TODO: Fix this!
				{
					canFilterInit.CAN_FilterIdHigh = (uint16_t) (filter << 5);
					canFilterInit.CAN_FilterIdLow  = (uint16_t) (filter << 5);

					canFilterInit.CAN_FilterMaskIdHigh =
													    (uint16_t) (mask >> 16);
					canFilterInit.CAN_FilterMaskIdLow = (uint16_t) mask;
				}
				else // filter16BitWidth == 1
				{
					canFilterInit.CAN_FilterIdLow  = (uint16_t) (filter << 5);
					canFilterInit.CAN_FilterIdHigh = (uint16_t) (mask << 5);

                    canFilterInit.CAN_FilterMaskIdLow  = 0;
                    canFilterInit.CAN_FilterMaskIdHigh = 0;
				}

				break;
			}
			case 1:
			{
				canFilterInit.CAN_FilterMaskIdLow = (uint16_t) filter;
				canFilterInit.CAN_FilterMaskIdLow  = (uint16_t) mask;

				break;
			}
			default:
			{
				// Should never get here

				break;
			}
		}
	}

	my16bitFilterIdCount[myId]     += filter16BitWidth;
	my16bitFilterMaskIdCount[myId] += mask16BitWidth;

	canFilterInit.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	canFilterInit.CAN_FilterScale          = filter16BitWidth - 1;
	canFilterInit.CAN_FilterActivation     = ENABLE;

	CAN_FilterInit(&canFilterInit);

	return Can::Error(Can::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void CanSTM32F10x::setInterruptEnabled(const Interrupt interrupt,
                                       const bool enabled)
{
    CAN_ITConfig(myCan, myInterruptMap[interrupt], (FunctionalState) enabled);
}

//------------------------------------------------------------------------------
void CanSTM32F10x::receiveMessage(uint8_t fifoNumber)
{
    if (CAN_MessagePending(myCan, fifoNumber) > 0)
    {
        Message message;

        CanRxMsg canRxMsg;
        CAN_Receive(myCan, fifoNumber, &canRxMsg);

        if (canRxMsg.IDE == myIdMap[ID_TYPE_BASE])
        {
            message.id     = canRxMsg.StdId;
            message.idType = ID_TYPE_BASE;
        }
        else // canRxMsg.IDE == myIdMap[ID_TYPE_EXTENDED]
        {
            message.id     = canRxMsg.ExtId;
            message.idType = ID_TYPE_EXTENDED;
        }

        if (canRxMsg.RTR == myFrameMap[FRAME_TYPE_DATA])
        {
            message.frameType = FRAME_TYPE_DATA;
        }
        else // canRxMsg.IDE == frameMap[FRAME_TYPE_REMOTE]
        {
            message.frameType = FRAME_TYPE_REMOTE;
        }

        memcpy(message.data.getData(), canRxMsg.Data, canRxMsg.DLC);

        message.data.setSize(canRxMsg.DLC);

        interfaceMessageReceived(message);

        CAN_FIFORelease(myCan, fifoNumber);
    }
}

//------------------------------------------------------------------------------
void CanSTM32F10x::dummyInterruptHandler()
{
    // TODO: Fill me in
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void CAN1_TX_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_1]
                      [CanSTM32F10x::INTERRUPT_TRANSMIT]->handler();
}

//------------------------------------------------------------------------------
extern "C" void CAN1_RX0_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_1]
                      [CanSTM32F10x::INTERRUPT_RECEIVE_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void CAN1_RX1_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_1]
                      [CanSTM32F10x::INTERRUPT_RECEIVE_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void CAN1_SCE_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_1]
                      [CanSTM32F10x::INTERRUPT_ERROR_STATUS_CHANGE]->handler();
}

//------------------------------------------------------------------------------
extern "C" void CAN2_TX_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_2]
                      [CanSTM32F10x::INTERRUPT_TRANSMIT]->handler();
}

//------------------------------------------------------------------------------
extern "C" void CAN2_RX0_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_2]
                      [CanSTM32F10x::INTERRUPT_RECEIVE_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void CAN2_RX1_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_2]
                      [CanSTM32F10x::INTERRUPT_RECEIVE_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void CAN2_SCE_IRQHandler(void)
{
    interruptObjectMap[CanSTM32F10x::ID_2]
                      [CanSTM32F10x::INTERRUPT_ERROR_STATUS_CHANGE]->handler();
}
