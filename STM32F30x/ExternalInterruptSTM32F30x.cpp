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
/// @file ExternalInterruptSTM32F30x.cpp
/// @author Ben Minerd
/// @date 2/4/2016
/// @brief ExternalInterruptSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <ExternalInterruptSTM32F30x.h>
#include <CallbackMethod.h>
#include <CallbackFunction.h>

using Plat4m::ExternalInterruptSTM32F30x;
using Plat4m::ExternalInterrupt;
using Plat4m::Module;
using Plat4m::InterruptSTM32F30x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F30x* interruptObjectMap[16];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

/**
 * @brief Interrupt map.
 */
const InterruptSTM32F30x::Id ExternalInterruptSTM32F30x::myInterruptIdMap[] =
{
    InterruptSTM32F30x::ID_EXTI_0,     /// ID_0
    InterruptSTM32F30x::ID_EXTI_1,     /// ID_1
    InterruptSTM32F30x::ID_EXTI_2,     /// ID_2
    InterruptSTM32F30x::ID_EXTI_3,     /// ID_3
    InterruptSTM32F30x::ID_EXTI_4,     /// ID_4
    InterruptSTM32F30x::ID_EXTI_9_5,   /// ID_5
    InterruptSTM32F30x::ID_EXTI_9_5,   /// ID_6
    InterruptSTM32F30x::ID_EXTI_9_5,   /// ID_7
    InterruptSTM32F30x::ID_EXTI_9_5,   /// ID_8
    InterruptSTM32F30x::ID_EXTI_9_5,   /// ID_9
    InterruptSTM32F30x::ID_EXTI_15_10, /// ID_10
    InterruptSTM32F30x::ID_EXTI_15_10, /// ID_11
    InterruptSTM32F30x::ID_EXTI_15_10, /// ID_12
    InterruptSTM32F30x::ID_EXTI_15_10, /// ID_13
    InterruptSTM32F30x::ID_EXTI_15_10, /// ID_14
    InterruptSTM32F30x::ID_EXTI_15_10  /// ID_15
};

/**
 * @brief Line map.
 */
const uint32_t ExternalInterruptSTM32F30x::myLineMap[] =
{
    EXTI_Line0,  /// ID_0
    EXTI_Line1,  /// ID_1
    EXTI_Line2,  /// ID_2
    EXTI_Line3,  /// ID_3
    EXTI_Line4,  /// ID_4
    EXTI_Line5,  /// ID_5
    EXTI_Line6,  /// ID_6
    EXTI_Line7,  /// ID_7
    EXTI_Line8,  /// ID_8
    EXTI_Line9,  /// ID_9
    EXTI_Line10, /// ID_10
    EXTI_Line11, /// ID_11
    EXTI_Line12, /// ID_12
    EXTI_Line13, /// ID_13
    EXTI_Line14, /// ID_14
    EXTI_Line15  /// ID_15
};

/**
 * @brief Trigger map.
 */
const EXTITrigger_TypeDef ExternalInterruptSTM32F30x::myTriggerMap[] =
{
    EXTI_Trigger_Rising,        /// INTERRUPT_TRIGGER_RISING
    EXTI_Trigger_Falling,       /// INTERRUPT_TRIGGER_FALLING
    EXTI_Trigger_Rising_Falling /// INTERRUPT_TRIGGER_RISING_FALLING
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ExternalInterruptSTM32F30x::ExternalInterruptSTM32F30x(
                                                    GpioPinSTM32F30x& gpioPin) :
    ExternalInterrupt(gpioPin),
    myId((Id) gpioPin.getId()),
    myGpioPortId(gpioPin.getGpioPort().getId()),
    myInterrupt(myInterruptIdMap[myId],
           createCallback(this,
                          &ExternalInterruptSTM32F30x::interruptHandlerPrivate))
{
    if (isNullPointer(interruptObjectMap[myId]))
    {
        interruptObjectMap[myId] = &myInterrupt;
    }
    else
    {
        // Instantiating same peripheral twice, lockup the system
        while (true)
        {
        }
    }
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ExternalInterruptSTM32F30x::isInterruptActive(const Id id)
{
    return areBitsSet(EXTI->PR, ((uint32_t) 1) << id);
}

//------------------------------------------------------------------------------
void ExternalInterruptSTM32F30x::clear(const Id id)
{
    setBits(EXTI->PR, ((uint32_t) 1) << id);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ExternalInterruptSTM32F30x::driverEnable(const bool enable)
{
    SYSCFG_EXTILineConfig(myGpioPortId, myId);

    EXTI_InitTypeDef extiInit;
    extiInit.EXTI_Line    = myLineMap[myId];
    extiInit.EXTI_Mode    = EXTI_Mode_Interrupt;
    extiInit.EXTI_LineCmd = (FunctionalState) enable;

    EXTI_Init(&extiInit);

    myInterrupt.enable(enable);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods implemented from ExternalInterrupt
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ExternalInterrupt::Error ExternalInterruptSTM32F30x::driverConfigure(
														   const Config& config)
{
    EXTI_InitTypeDef extiInit;
    extiInit.EXTI_Line    = myLineMap[myId];
    extiInit.EXTI_Mode    = EXTI_Mode_Interrupt;
    extiInit.EXTI_Trigger = myTriggerMap[config.trigger];
    extiInit.EXTI_LineCmd = ENABLE;

    EXTI_Init(&extiInit);

    Interrupt::Config interruptConfig;
    interruptConfig.priority = 7;
    myInterrupt.configure(interruptConfig);

    return ExternalInterrupt::Error(ExternalInterrupt::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ExternalInterruptSTM32F30x::interruptHandlerPrivate()
{
    interruptHandler();
    clear(myId);
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void EXTI0_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F30x::ID_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI1_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F30x::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI2_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F30x::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI3_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F30x::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI4_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F30x::ID_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI9_5_IRQHandler(void)
{
    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                              ExternalInterruptSTM32F30x::ID_5))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_5]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                              ExternalInterruptSTM32F30x::ID_6))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_6]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                              ExternalInterruptSTM32F30x::ID_7))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_7]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                              ExternalInterruptSTM32F30x::ID_8))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_8]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                              ExternalInterruptSTM32F30x::ID_9))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_9]->handler();
    }
}

//------------------------------------------------------------------------------
extern "C" void EXTI15_10_IRQHandler(void)
{
    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                             ExternalInterruptSTM32F30x::ID_10))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_10]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                             ExternalInterruptSTM32F30x::ID_11))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_11]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                             ExternalInterruptSTM32F30x::ID_12))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_12]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                             ExternalInterruptSTM32F30x::ID_13))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_13]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                             ExternalInterruptSTM32F30x::ID_14))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_14]->handler();
    }

    if (ExternalInterruptSTM32F30x::isInterruptActive(
                                             ExternalInterruptSTM32F30x::ID_15))
    {
        interruptObjectMap[ExternalInterruptSTM32F30x::ID_15]->handler();
    }
}
