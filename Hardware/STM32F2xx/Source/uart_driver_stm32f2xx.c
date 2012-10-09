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
 * @file uart_driver_stm32f2xx.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief UART driver layer for the STM32F2xx.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <uart_interface.h>

// plat4m
//
// Replace below with proper driver header file.
// ex.
//      #include <uart_driver_stm32f10x.h>

#include <uart_driver.h>

// plat4m
//
// Include necessary manufacturer driver header files here.
// ex.
//      #include <stm32f10x_usart.h>

#include <gpio_interface.h>

#include <stm32f2xx.h>
#include <stm32f2xx_usart.h>
#include <stm32f2xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief UART GPIO hardware type.
 */
typedef struct _uart_gpio_hardware_t_
{
    gpio_hardware_t tx;
    gpio_hardware_t rx;
} uart_gpio_hardware_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief UART interface interrupt handler.
 */
static const uart_interface_int_handler_f* intHandler;

/**
 * @brief USART map.
 */
static USART_TypeDef* usartMap[UART_COUNT] =
{
    USART1, /// UART_1
    USART2, /// UART_2
    USART3, /// UART_3
    UART4,  /// UART_4
    UART5,  /// UART_5
    USART6  /// UART_6
};

/**
 * @brief UART clock map.
 */
static const uint32_t clockMap[UART_COUNT] =
{
    RCC_APB2Periph_USART1,  /// UART_1
    RCC_APB1Periph_USART2,  /// UART_2
    RCC_APB1Periph_USART3,  /// UART_3
    RCC_APB1Periph_UART4,   /// UART_4
    RCC_APB1Periph_UART5,   /// UART_5
    RCC_APB2Periph_USART6   /// UART_6
};

/**
 * @brief UART GPIO hardware map.
 */
static const uart_gpio_hardware_t gpioMap[UART_COUNT][UART_GPIO_MAP_COUNT] =
{
    /// UART_1
    {
        /// UART_GPIO_MAP_1
        {
            .tx.port    = GPIO_PORT_A,
            .tx.pin     = GPIO_PIN_9,
            .rx.port    = GPIO_PORT_A,
            .rx.pin     = GPIO_PIN_10
        },
        /// UART_GPIO_MAP_2
        {
            .tx.port    = GPIO_PORT_B,
            .tx.pin     = GPIO_PIN_6,
            .rx.port    = GPIO_PORT_B,
            .rx.pin     = GPIO_PIN_7
        }
    },
    /// UART_2
    {
        /// UART_GPIO_MAP_1
        {
            .tx.port    = GPIO_PORT_A,
            .tx.pin     = GPIO_PIN_2,
            .rx.port    = GPIO_PORT_A,
            .rx.pin     = GPIO_PIN_3
        },
        /// UART_GPIO_MAP_2
        {
            .tx.port    = GPIO_PORT_D,
            .tx.pin     = GPIO_PIN_5,
            .rx.port    = GPIO_PORT_D,
            .rx.pin     = GPIO_PIN_6
        }
    },
    /// UART_3
    {
        /// UART_GPIO_MAP_1
        {
            .tx.port    = GPIO_PORT_B,
            .tx.pin     = GPIO_PIN_10,
            .rx.port    = GPIO_PORT_B,
            .rx.pin     = GPIO_PIN_11
        },
        /// UART_GPIO_MAP_2
        {
            .tx.port    = GPIO_PORT_C,
            .tx.pin     = GPIO_PIN_10,
            .rx.port    = GPIO_PORT_C,
            .rx.pin     = GPIO_PIN_11
        }
    },
    /// UART_4
    {
        /// UART_GPIO_MAP_1
        {
            .tx.port    = GPIO_PORT_A,
            .tx.pin     = GPIO_PIN_0,
            .rx.port    = GPIO_PORT_A,
            .rx.pin     = GPIO_PIN_1
        },
        /// UART_GPIO_MAP_2
        {
            .tx.port    = GPIO_PORT_C,
            .tx.pin     = GPIO_PIN_10,
            .rx.port    = GPIO_PORT_C,
            .rx.pin     = GPIO_PIN_11
        }
    },
    /// UART_5
    {
        /// UART_GPIO_MAP_1
        {
            .tx.port    = GPIO_PORT_C,
            .tx.pin     = GPIO_PIN_12,
            .rx.port    = GPIO_PORT_D,
            .rx.pin     = GPIO_PIN_2
        },
        /// UART_GPIO_MAP_2 (identical to 1)
        {
            .tx.port    = GPIO_PORT_C,
            .tx.pin     = GPIO_PIN_12,
            .rx.port    = GPIO_PORT_D,
            .rx.pin     = GPIO_PIN_2
        }
    },
    /// UART_6
    {
        /// UART_GPIO_MAP_1
        {
            .tx.port    = GPIO_PORT_C,
            .tx.pin     = GPIO_PIN_6,
            .rx.port    = GPIO_PORT_C,
            .rx.pin     = GPIO_PIN_7
        },
        /// UART_GPIO_MAP_2
        {
            .tx.port    = GPIO_PORT_G,
            .tx.pin     = GPIO_PIN_14,
            .rx.port    = GPIO_PORT_G,
            .rx.pin     = GPIO_PIN_9
        }
    }
};

/**
 * @brief UART IRQ map.
 */
static const IRQn_Type irqMap[UART_COUNT] =
{
    USART1_IRQn,    /// UART_1
    USART2_IRQn,    /// UART_2
    USART3_IRQn,    /// UART_3
    UART4_IRQn,     /// UART_4
    UART5_IRQn,     /// UART_5
    USART6_IRQn     /// UART_6
};

/**
 * @brief UART alternate function map.
 */
static const gpio_alternate_function_e afMap[UART_COUNT] =
{
    GPIO_ALTERNATE_FUNCTION_USART1, /// UART_1
    GPIO_ALTERNATE_FUNCTION_USART2, /// UART_2
    GPIO_ALTERNATE_FUNCTION_USART3, /// UART_3
    GPIO_ALTERNATE_FUNCTION_UART4,  /// UART_4
    GPIO_ALTERNATE_FUNCTION_UART5,  /// UART_5
    GPIO_ALTERNATE_FUNCTION_USART6  /// UART_6
};

/**
 * @brief UART word length map.
 */
static const uint16_t wordLengthMap[UART_WORD_BITS_COUNT] =
{
    USART_WordLength_8b,    /// UART_WORD_BITS_8
    USART_WordLength_9b     /// UART_WORD_BITS_9
};

/**
 * @brief UART stop bits map.
 */
static const uint16_t stopBitsMap[UART_STOP_BITS_COUNT] =
{
    USART_StopBits_1,   /// UART_STOP_BITS_1
    USART_StopBits_2    /// UART_STOP_BITS_2
};

/**
 * @brief UART parity map.
 */
static const uint16_t parityMap[UART_PARITY_COUNT] =
{
    USART_Parity_No,    /// UART_PARITY_NONE
    USART_Parity_Even,  /// UART_PARITY_EVEN
    USART_Parity_Odd    /// UART_PARITY_ODD
};

/**
 * @brief UART hardware flow control map.
 */
static const uint16_t flowControlMap[UART_HARDWARE_FLOW_CONTROL_COUNT] =
{
    USART_HardwareFlowControl_None  /// UART_HARDWARE_FLOW_CONTROL_NONE
};

/**
 * @brief UART interrupt map.
 */
static const uint16_t interruptMap[UART_INTERRUPT_COUNT] =
{
    USART_IT_TXE,   /// UART_INTERRUPT_TX
    USART_IT_RXNE   /// UART_INTERRUPT_RX
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local UART driver functions here.
//
// Example:
//
//      static void uart1SetEnabled(bool enabled);
//      static void uart1Tx(uint8_t data);
//      ...

/**
 * @brief Sets the given UART enabled.
 * @param id UART driver ID.
 * @param enabled Flag that indicates if the UART should be enabled or disabled.
 */
static void setEnabled(const uart_e uart, const bool enabled);

/**
 * @brief Configures the given UART.
 * @param id UART driver ID.
 * @param config UART configuration.
 */
static void configure(const uart_e uart, const uart_config_t* config);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param id UART driver ID.
 * @param byte Byte to transmit.
 */
static void tx(const uart_e uart, const uint8_t byte);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param id UART driver ID.
 * @param byte Byte received.
 */
static void rx(const uart_e uart, uint8_t* byte);

/**
 * @brief Sets the given UART interrupt enabled or disabled.
 * @param id UART driver ID.
 * @param interrupt UART interrupt to set enabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
static void intSetEnabled(const uart_e uart,
                          const uart_interrupt_e interrupt,
                          const bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartDriverInit(const uart_interface_int_handler_f* handler)
{
    // plat4m
    //
    // Initialize UART driver.
    // Add drivers to UART interface.
    
    const uart_driver_t driver =
    {
        .setEnabled     = setEnabled,
        .configure      = configure,
        .tx             = tx,
        .rx             = rx,
        .intSetEnabled  = intSetEnabled
    };

    uartSetDriver(&driver);

    intHandler = handler;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local UART driver functions declared above.

//------------------------------------------------------------------------------
static void setEnabled(const uart_e uart, const bool enabled)
{
    switch (uart)
    {
        case UART_1:
        case UART_6:
        {
            RCC_APB2PeriphClockCmd(clockMap[uart], (FunctionalState) enabled);

            break;
        }
        default:
        {
            RCC_APB1PeriphClockCmd(clockMap[uart], (FunctionalState) enabled);

            break;
        }
    }

    USART_Cmd(usartMap[uart], (FunctionalState) enabled);

    NVIC_InitTypeDef nvicInit =
    {
        .NVIC_IRQChannel                      = irqMap[uart],
        .NVIC_IRQChannelPreemptionPriority    = 0,
        .NVIC_IRQChannelSubPriority           = 0,
        .NVIC_IRQChannelCmd                   = (FunctionalState) enabled
    };

    NVIC_Init(&nvicInit);
}

//------------------------------------------------------------------------------
static void configure(const uart_e uart, const uart_config_t* config)
{
    gpio_config_t gpioConfig =
    {
        .mode               = GPIO_MODE_ALTERNATE_FUNCTION,
        .resistor           = GPIO_RESISTOR_PULL_UP,
        .alternateFunction  = afMap[uart]
    };

    gpioHardwareSetEnabled(&(gpioMap[uart][config->map].tx), true);
    gpioHardwareSetEnabled(&(gpioMap[uart][config->map].tx), true);
    gpioHardwareConfigure(&(gpioMap[uart][config->map].tx), &gpioConfig);
    gpioHardwareConfigure(&(gpioMap[uart][config->map].rx), &gpioConfig);

    USART_InitTypeDef uartInit =
    {
        .USART_BaudRate             = config->baudRate,
        .USART_WordLength           = wordLengthMap[config->wordBits],
        .USART_StopBits             = stopBitsMap[config->stopBits],
        .USART_Parity               = parityMap[config->parity],
        .USART_HardwareFlowControl  = flowControlMap[config->flowControl],
        .USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx
    };

    USART_Init(usartMap[uart], &uartInit);
}

//------------------------------------------------------------------------------
static void tx(const uart_e uart, const uint8_t byte)
{
    USART_SendData(usartMap[uart], byte);
}

//------------------------------------------------------------------------------
static void rx(const uart_e uart, uint8_t* byte)
{
    *byte = (uint8_t)USART_ReceiveData(usartMap[uart]);
}

//------------------------------------------------------------------------------
static void intSetEnabled(const uart_e uart,
                          const uart_interrupt_e interrupt,
                          const bool enabled)
{
    USART_ITConfig(usartMap[uart],
                   interruptMap[interrupt],
                   (FunctionalState) enabled);
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
        intHandler(UART_1, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        intHandler(UART_1, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART2_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_TXE);
        intHandler(UART_2, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        intHandler(UART_2, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART3_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_TXE);
        intHandler(UART_3, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        intHandler(UART_3, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART4_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(UART4, USART_IT_TXE);
        intHandler(UART_4, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        intHandler(UART_4, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART5_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(UART5, USART_IT_TXE);
        intHandler(UART_5, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        intHandler(UART_5, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART6_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(USART6, USART_IT_TXE);
        intHandler(UART_6, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        intHandler(UART_6, UART_INTERRUPT_RX);
    }
}
