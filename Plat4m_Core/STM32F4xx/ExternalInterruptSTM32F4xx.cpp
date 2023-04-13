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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file ExternalInterruptSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 4/17/2013
/// @brief ExternalInterruptSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>

#include <Plat4m_Core/STM32F4xx/ExternalInterruptSTM32F4xx.h>
#include <Plat4m_Core/CallbackMethod.h>
#include <Plat4m_Core/CallbackFunction.h>

using Plat4m::ExternalInterruptSTM32F4xx;
using Plat4m::ExternalInterrupt;
using Plat4m::Module;
using Plat4m::InterruptSTM32F4xx;

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define IS_INTERRUPT_ACTIVE(line) (EXTI->PR & line)

#define INTERRUPT_CLEAR(line) EXTI->PR = line

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F4xx* interruptObjectMap[16];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

/**
 * @brief Interrupt map.
 */
const InterruptSTM32F4xx::Id ExternalInterruptSTM32F4xx::myInterruptIdMap[] =
{
    InterruptSTM32F4xx::ID_EXTI_0,     /// ID_0
    InterruptSTM32F4xx::ID_EXTI_1,     /// ID_1
    InterruptSTM32F4xx::ID_EXTI_2,     /// ID_2
    InterruptSTM32F4xx::ID_EXTI_3,     /// ID_3
    InterruptSTM32F4xx::ID_EXTI_4,     /// ID_4
    InterruptSTM32F4xx::ID_EXTI_9_5,   /// ID_5
    InterruptSTM32F4xx::ID_EXTI_9_5,   /// ID_6
    InterruptSTM32F4xx::ID_EXTI_9_5,   /// ID_7
    InterruptSTM32F4xx::ID_EXTI_9_5,   /// ID_8
    InterruptSTM32F4xx::ID_EXTI_9_5,   /// ID_9
    InterruptSTM32F4xx::ID_EXTI_15_10, /// ID_10
    InterruptSTM32F4xx::ID_EXTI_15_10, /// ID_11
    InterruptSTM32F4xx::ID_EXTI_15_10, /// ID_12
    InterruptSTM32F4xx::ID_EXTI_15_10, /// ID_13
    InterruptSTM32F4xx::ID_EXTI_15_10, /// ID_14
    InterruptSTM32F4xx::ID_EXTI_15_10  /// ID_15
};

/**
 * @brief Line map.
 */
static const uint32_t lineMap[] =
{
    EXTI_Line0,  /// ID_EXTERNAL_0
    EXTI_Line1,  /// ID_EXTERNAL_1
    EXTI_Line2,  /// ID_EXTERNAL_2
    EXTI_Line3,  /// ID_EXTERNAL_3
    EXTI_Line4,  /// ID_EXTERNAL_4
    EXTI_Line5,  /// ID_EXTERNAL_5
    EXTI_Line6,  /// ID_EXTERNAL_6
    EXTI_Line7,  /// ID_EXTERNAL_7
    EXTI_Line8,  /// ID_EXTERNAL_8
    EXTI_Line9,  /// ID_EXTERNAL_9
    EXTI_Line10, /// ID_EXTERNAL_10
    EXTI_Line11, /// ID_EXTERNAL_11
    EXTI_Line12, /// ID_EXTERNAL_12
    EXTI_Line13, /// ID_EXTERNAL_13
    EXTI_Line14, /// ID_EXTERNAL_14
    EXTI_Line15  /// ID_EXTERNAL_15
};

/**
 * @brief Trigger map.
 */
static const EXTITrigger_TypeDef triggerMap[] =
{
    EXTI_Trigger_Rising,        /// INTERRUPT_TRIGGER_RISING
    EXTI_Trigger_Falling,       /// INTERRUPT_TRIGGER_FALLING
    EXTI_Trigger_Rising_Falling /// INTERRUPT_TRIGGER_RISING_FALLING
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ExternalInterruptSTM32F4xx::ExternalInterruptSTM32F4xx(
                                                    GpioPinSTM32F4xx& gpioPin) :
    ExternalInterrupt(gpioPin),
    myId((Id) gpioPin.getId()),
    myGpioPortId(gpioPin.getGpioPort().getId()),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(
                          this,
                          &ExternalInterruptSTM32F4xx::interruptHandlerPrivate))
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
// Public static inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
bool ExternalInterruptSTM32F4xx::isInterruptActive(const Id id)
{
    return (EXTI->PR & ((uint32_t) id));
}

//------------------------------------------------------------------------------
void ExternalInterruptSTM32F4xx::clear(const Id id)
{
    EXTI->PR = ((uint32_t) 1) << id;
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error ExternalInterruptSTM32F4xx::driverEnable(const bool enable)
{
    SYSCFG_EXTILineConfig(myGpioPortId, myId);
    
    EXTI_InitTypeDef extiInit;
    extiInit.EXTI_Line    = lineMap[myId];
    extiInit.EXTI_Mode    = EXTI_Mode_Interrupt;
    extiInit.EXTI_LineCmd = (FunctionalState) enable;

    EXTI_Init(&extiInit);
    
    myInterrupt.enable(enable);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for ExternalInterrupt
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ExternalInterrupt::Error ExternalInterruptSTM32F4xx::driverConfigure(
                                                           const Config& config)
{
    EXTI_InitTypeDef extiInit;
    extiInit.EXTI_Line    = lineMap[myId];
    extiInit.EXTI_Mode    = EXTI_Mode_Interrupt;
    extiInit.EXTI_Trigger = triggerMap[config.trigger];
    extiInit.EXTI_LineCmd = ENABLE;

    EXTI_Init(&extiInit);
    
    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ExternalInterruptSTM32F4xx::interruptHandlerPrivate()
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
    interruptObjectMap[ExternalInterruptSTM32F4xx::ID_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI1_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F4xx::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI2_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F4xx::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI3_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F4xx::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI4_IRQHandler(void)
{
    interruptObjectMap[ExternalInterruptSTM32F4xx::ID_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI9_5_IRQHandler(void)
{
    if (IS_INTERRUPT_ACTIVE(EXTI_Line5))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_5]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line6))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_6]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line7))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_7]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line8))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_8]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line9))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_9]->handler();
    }
}

//------------------------------------------------------------------------------
extern "C" void EXTI15_10_IRQHandler(void)
{
    if (IS_INTERRUPT_ACTIVE(EXTI_Line10))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_10]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line11))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_11]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line12))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_12]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line13))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_13]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line14))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_14]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line15))
    {
        interruptObjectMap[ExternalInterruptSTM32F4xx::ID_15]->handler();
    }
}
