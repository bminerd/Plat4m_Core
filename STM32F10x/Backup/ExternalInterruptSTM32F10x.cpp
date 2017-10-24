/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file interrupt_driver_stm32f4xx.c
 * @author Ben Minerd
 * @date 11/26/12
 * @brief Interrupt driver layer for the STM32F4xx.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <ExternalInterruptStm32f4xx.h>

using Plat4m::ExternalInterruptStm32f4xx;
using Plat4m::ExternalInterrupt;
using Plat4m::Module;

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#define IS_INTERRUPT_ACTIVE(line) (EXTI->PR & line)

#define INTERRUPT_CLEAR(line) EXTI->PR = line

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Interrupt map.
 */
static const IRQn_Type interruptMap[] =
{
    EXTI0_IRQn,     /// ID_EXTERNAL_0
    EXTI1_IRQn,     /// ID_EXTERNAL_1
    EXTI2_IRQn,     /// ID_EXTERNAL_2
    EXTI3_IRQn,     /// ID_EXTERNAL_3
    EXTI4_IRQn,     /// ID_EXTERNAL_4
    EXTI9_5_IRQn,   /// ID_EXTERNAL_5
    EXTI9_5_IRQn,   /// ID_EXTERNAL_6
    EXTI9_5_IRQn,   /// ID_EXTERNAL_7
    EXTI9_5_IRQn,   /// ID_EXTERNAL_8
    EXTI9_5_IRQn,   /// ID_EXTERNAL_9
    EXTI15_10_IRQn, /// ID_EXTERNAL_10
    EXTI15_10_IRQn, /// ID_EXTERNAL_11
    EXTI15_10_IRQn, /// ID_EXTERNAL_12
    EXTI15_10_IRQn, /// ID_EXTERNAL_13
    EXTI15_10_IRQn, /// ID_EXTERNAL_14
    EXTI15_10_IRQn  /// ID_EXTERNAL_15
};

/**
 * @brief Line map.
 */
static const uint32_t lineMap[] =
{
    EXTI_Line0,     /// ID_EXTERNAL_0
    EXTI_Line1,     /// ID_EXTERNAL_1
    EXTI_Line2,     /// ID_EXTERNAL_2
    EXTI_Line3,     /// ID_EXTERNAL_3
    EXTI_Line4,     /// ID_EXTERNAL_4
    EXTI_Line5,     /// ID_EXTERNAL_5
    EXTI_Line6,     /// ID_EXTERNAL_6
    EXTI_Line7,     /// ID_EXTERNAL_7
    EXTI_Line8,     /// ID_EXTERNAL_8
    EXTI_Line9,     /// ID_EXTERNAL_9
    EXTI_Line10,    /// ID_EXTERNAL_10
    EXTI_Line11,    /// ID_EXTERNAL_11
    EXTI_Line12,    /// ID_EXTERNAL_12
    EXTI_Line13,    /// ID_EXTERNAL_13
    EXTI_Line14,    /// ID_EXTERNAL_14
    EXTI_Line15     /// ID_EXTERNAL_15
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

static ExternalInterruptStm32f4xx* objectMap[16];

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ExternalInterruptStm32f4xx& ExternalInterruptStm32f4xx::get(GpioPinStm32f4xx& gpioPin)
{
    Id id = (Id) gpioPin.getPinId();
    
    if (IS_NULL_POINTER(objectMap[id]))
    {
        objectMap[id] = new ExternalInterruptStm32f4xx(gpioPin);
    }
    else
    {
        // Error?
    }
    
    return *(objectMap[id]);
}

/*------------------------------------------------------------------------------
 * Private constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ExternalInterruptStm32f4xx::ExternalInterruptStm32f4xx(GpioPinStm32f4xx& gpioPin) :
    ExternalInterrupt(gpioPin),
    myId((Id) gpioPin.getPinId()),
    myGpioPortId(gpioPin.getPortId())
{
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error ExternalInterruptStm32f4xx::driverEnable(const bool enable)
{
    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel                    = interruptMap[myId];
    nvicInit.NVIC_IRQChannelPreemptionPriority  = 0x0F;
    nvicInit.NVIC_IRQChannelSubPriority         = 0x0F;
    nvicInit.NVIC_IRQChannelCmd                 = (FunctionalState) enable;

    NVIC_Init(&nvicInit);
    
    SYSCFG_EXTILineConfig(myGpioPortId, myId);
    
    EXTI_InitTypeDef extiInit;
    extiInit.EXTI_Line      = lineMap[myId];
    extiInit.EXTI_Mode      = EXTI_Mode_Interrupt;
    extiInit.EXTI_LineCmd   = (FunctionalState) enable;

    EXTI_Init(&extiInit);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods implemented from ExternalInterrupt
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ExternalInterrupt::Error ExternalInterruptStm32f4xx::driverConfigure(
														   const Config& config)
{
    EXTI_InitTypeDef extiInit;
    extiInit.EXTI_Line      = lineMap[myId];
    extiInit.EXTI_Mode      = EXTI_Mode_Interrupt;
    extiInit.EXTI_Trigger   = triggerMap[config.trigger];
    extiInit.EXTI_LineCmd   = ENABLE;

    EXTI_Init(&extiInit);
    
    return ExternalInterrupt::Error(ExternalInterrupt::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern "C" void EXTI0_IRQHandler(void)
{
    INTERRUPT_CLEAR(EXTI_Line0);
    objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI1_IRQHandler(void)
{
    INTERRUPT_CLEAR(EXTI_Line1);
    objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI2_IRQHandler(void)
{
    INTERRUPT_CLEAR(EXTI_Line2);
    objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI3_IRQHandler(void)
{
    INTERRUPT_CLEAR(EXTI_Line3);
    objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI4_IRQHandler(void)
{
    INTERRUPT_CLEAR(EXTI_Line4);
    objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void EXTI9_5_IRQHandler(void)
{
    if (IS_INTERRUPT_ACTIVE(EXTI_Line5))
    {
        INTERRUPT_CLEAR(EXTI_Line5);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_5]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line6))
    {
        INTERRUPT_CLEAR(EXTI_Line6);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_6]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line7))
    {
        INTERRUPT_CLEAR(EXTI_Line7);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_7]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line8))
    {
        INTERRUPT_CLEAR(EXTI_Line8);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_8]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line9))
    {
        INTERRUPT_CLEAR(EXTI_Line9);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_9]->handler();
    }
}

//------------------------------------------------------------------------------
extern "C" void EXTI15_10_IRQHandler(void)
{
    if (IS_INTERRUPT_ACTIVE(EXTI_Line10))
    {
        INTERRUPT_CLEAR(EXTI_Line10);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_10]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line11))
    {
        INTERRUPT_CLEAR(EXTI_Line11);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_11]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line12))
    {
        INTERRUPT_CLEAR(EXTI_Line12);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_12]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line13))
    {
        INTERRUPT_CLEAR(EXTI_Line13);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_13]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line14))
    {
        INTERRUPT_CLEAR(EXTI_Line14);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_14]->handler();
    }

    if (IS_INTERRUPT_ACTIVE(EXTI_Line15))
    {
        INTERRUPT_CLEAR(EXTI_Line15);
        objectMap[ExternalInterruptStm32f4xx::ID_EXTERNAL_15]->handler();
    }
}
