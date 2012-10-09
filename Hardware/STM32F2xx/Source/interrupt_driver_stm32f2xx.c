/*------------------------------------------------------------------------------
 *                  __                           ___
 *                 || |             __          //  |
 *       _______   || |   _______  || |__      //   |    _____  ___
 *      ||  ___ \  || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      || |  || | || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |__|| | || | || |__|| | || |     // /_|| |_  || | || | || |
 *      ||  ____/  || |  \\____  | || |__  //_____   _| || | || | || |
 *      || |       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *      || |
 *      ||_|
 *
 * Copyright (c) 2012 Ben Minerd. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *----------------------------------------------------------------------------*/

/**
 * @file interrupt_driver_stm32f2xx.c
 * @author Ben Minerd
 * @date 8/2/12
 * @brief  TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <interrupt_interface.h>
#include <interrupt_driver.h>

#include <stm32f2xx.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Interface interrupt handler.
 */
static const interface_int_handler_f* intHandler;

/**
 * @brief Interrupt map.
 */
static const IRQn_Type interruptMap[INTERRUPT_COUNT] =
{
    EXTI0_IRQn,     /// INTERRUPT_EXTERNAL_0
    EXTI1_IRQn,     /// INTERRUPT_EXTERNAL_1
    EXTI2_IRQn,     /// INTERRUPT_EXTERNAL_2
    EXTI3_IRQn,     /// INTERRUPT_EXTERNAL_3
    EXTI4_IRQn,     /// INTERRUPT_EXTERNAL_4
    EXTI9_5_IRQn,   /// INTERRUPT_EXTERNAL_5
    EXTI9_5_IRQn,   /// INTERRUPT_EXTERNAL_6
    EXTI9_5_IRQn,   /// INTERRUPT_EXTERNAL_7
    EXTI9_5_IRQn,   /// INTERRUPT_EXTERNAL_8
    EXTI9_5_IRQn,   /// INTERRUPT_EXTERNAL_9
    EXTI15_10_IRQn, /// INTERRUPT_EXTERNAL_10
    EXTI15_10_IRQn, /// INTERRUPT_EXTERNAL_11
    EXTI15_10_IRQn, /// INTERRUPT_EXTERNAL_12
    EXTI15_10_IRQn, /// INTERRUPT_EXTERNAL_13
    EXTI15_10_IRQn, /// INTERRUPT_EXTERNAL_14
    EXTI15_10_IRQn  /// INTERRUPT_EXTERNAL_15
};

/**
 * @brief Line map.
 */
static const uint32_t lineMap[INTERRUPT_COUNT] =
{
    EXTI_Line0,     /// INTERRUPT_EXTERNAL_0
    EXTI_Line1,     /// INTERRUPT_EXTERNAL_1
    EXTI_Line2,     /// INTERRUPT_EXTERNAL_2
    EXTI_Line3,     /// INTERRUPT_EXTERNAL_3
    EXTI_Line4,     /// INTERRUPT_EXTERNAL_4
    EXTI_Line5,     /// INTERRUPT_EXTERNAL_5
    EXTI_Line6,     /// INTERRUPT_EXTERNAL_6
    EXTI_Line7,     /// INTERRUPT_EXTERNAL_7
    EXTI_Line8,     /// INTERRUPT_EXTERNAL_8
    EXTI_Line9,     /// INTERRUPT_EXTERNAL_9
    EXTI_Line10,    /// INTERRUPT_EXTERNAL_10
    EXTI_Line11,    /// INTERRUPT_EXTERNAL_11
    EXTI_Line12,    /// INTERRUPT_EXTERNAL_12
    EXTI_Line13,    /// INTERRUPT_EXTERNAL_13
    EXTI_Line14,    /// INTERRUPT_EXTERNAL_14
    EXTI_Line15     /// INTERRUPT_EXTERNAL_15
};

/**
 * @brief Trigger map.
 */
static const EXTITrigger_TypeDef triggerMap[INTERRUPT_TRIGGER_COUNT] =
{
    EXTI_Trigger_Rising,        /// INTERRUPT_TRIGGER_RISING
    EXTI_Trigger_Falling,       /// INTERRUPT_TRIGGER_FALLING
    EXTI_Trigger_Rising_Falling /// INTERRUPT_TRIGGER_RISING_FALLING
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/


/**
 * @brief Sets the given interrupt enabled or disabled.
 * @param interrupt Interrupt to access.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
static void setEnabled(const interrupt_e interrupt, const bool enabled);

/**
 * @brief Configures the given interrupt.
 * @param interrupt Interrupt to access.
 * @param config Interrupt configuration.
 */
static void configure(const interrupt_e interrupt,
                      const interrupt_config_t* config);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void interruptDriverInit(const interface_int_handler_f* handler)
{
    interrupt_driver_t driver =
    {
        .setEnabled = setEnabled,
        .configure  = configure
    };
    
    interruptSetDriver(&driver);

    intHandler = handler;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(const interrupt_e interrupt, const bool enabled)
{
    NVIC_InitTypeDef nvicInit =
    {
        .NVIC_IRQChannel                    = interruptMap[interrupt],
        .NVIC_IRQChannelPreemptionPriority  = 0x0F,
        .NVIC_IRQChannelSubPriority         = 0x0F,
        .NVIC_IRQChannelCmd                 = (FunctionalState) enabled
    };

    NVIC_Init(&nvicInit);
}

//------------------------------------------------------------------------------
static void configure(const interrupt_e interrupt,
                      const interrupt_config_t* config)
{
    SYSCFG_EXTILineConfig(config->hardware.port, config->hardware.pin);

    EXTI_InitTypeDef extiInit =
    {
        .EXTI_Line      = lineMap[config->hardware.pin],
        .EXTI_Mode      = EXTI_Mode_Interrupt,
        .EXTI_Trigger   = triggerMap[config->trigger],
        .EXTI_LineCmd   = ENABLE
    };

    EXTI_Init(&extiInit);
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void EXTI0_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line0);
    intHandler(INTERRUPT_EXTERNAL_0);
}

//------------------------------------------------------------------------------
void EXTI1_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line1);
    intHandler(INTERRUPT_EXTERNAL_1);
}

//------------------------------------------------------------------------------
void EXTI2_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line2);
    intHandler(INTERRUPT_EXTERNAL_2);
}

//------------------------------------------------------------------------------
void EXTI3_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line3);
    intHandler(INTERRUPT_EXTERNAL_3);
}

//------------------------------------------------------------------------------
void EXTI4_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line4);
    intHandler(INTERRUPT_EXTERNAL_4);
}

//------------------------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
        intHandler(INTERRUPT_EXTERNAL_5);
    }

    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        intHandler(INTERRUPT_EXTERNAL_6);
    }

    if (EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        intHandler(INTERRUPT_EXTERNAL_7);
    }

    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        intHandler(INTERRUPT_EXTERNAL_8);
    }

    if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line9);
        intHandler(INTERRUPT_EXTERNAL_9);
    }
}

//------------------------------------------------------------------------------
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10);
        intHandler(INTERRUPT_EXTERNAL_10);
    }

    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line11);
        intHandler(INTERRUPT_EXTERNAL_11);
    }

    if (EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line12);
        intHandler(INTERRUPT_EXTERNAL_12);
    }

    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line13);
        intHandler(INTERRUPT_EXTERNAL_13);
    }

    if (EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line14);
        intHandler(INTERRUPT_EXTERNAL_14);
    }

    if (EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line15);
        intHandler(INTERRUPT_EXTERNAL_15);
    }
}
