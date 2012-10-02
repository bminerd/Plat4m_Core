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
 * @file uart_driver_template.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief TODO Comment!
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

#include <uart_driver_stm32f2xx.h>

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
    gpio_hardware_t txHardware;
    gpio_hardware_t rxHardware;
} uart_gpio_hardware_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief USART driver map.
 */
static USART_TypeDef* usartMap[UART_DRIVER_ID_COUNT] =
{
    USART1, /// UART_DRIVER_ID_1
    USART2, /// UART_DRIVER_ID_2
    USART3, /// UART_DRIVER_ID_3
    UART4,  /// UART_DRIVER_ID_4
    UART5,  /// UART_DRIVER_ID_5
    USART6  /// UART_DRIVER_ID_6
};

/**
 * @brief UART clock map.
 */
static const uint32_t clockMap[UART_DRIVER_ID_COUNT] =
{
    RCC_APB2Periph_USART1,  /// UART_DRIVER_ID_1
    RCC_APB1Periph_USART2,  /// UART_DRIVER_ID_2
    RCC_APB1Periph_USART3,  /// UART_DRIVER_ID_3
    RCC_APB1Periph_UART4,   /// UART_DRIVER_ID_4
    RCC_APB1Periph_UART5,   /// UART_DRIVER_ID_5
    RCC_APB2Periph_USART6   /// UART_DRIVER_ID_6
};

/**
 * @brief UART GPIO hardware map.
 */
static const uart_gpio_hardware_t gpioMap[UART_DRIVER_ID_COUNT]
                                         [UART_DRIVER_GPIO_MAP_ID_COUNT] =
{
    /// UART_DRIVER_ID_1
    {
        /// UART_DRIVER_GPIO_MAP_ID_1
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_A,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_9,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_A,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_10
        },
        /// UART_DRIVER_GPIO_MAP_ID_2
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_6,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_7
        }
    },
    /// UART_DRIVER_ID_2
    {
        /// UART_DRIVER_GPIO_MAP_ID_1
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_A,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_2,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_A,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_3
        },
        /// UART_DRIVER_GPIO_MAP_ID_2
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_D,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_5,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_D,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_6
        }
    },
    /// UART_DRIVER_ID_3
    {
        /// UART_DRIVER_GPIO_MAP_ID_1
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_10,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_11
        },
        /// UART_DRIVER_GPIO_MAP_ID_2
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_10,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_11
        }
    },
    /// UART_DRIVER_ID_4
    {
        /// UART_DRIVER_GPIO_MAP_ID_1
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_A,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_0,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_A,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_1
        },
        /// UART_DRIVER_GPIO_MAP_ID_2
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_10,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_11
        }
    },
    /// UART_DRIVER_ID_5
    {
        /// UART_DRIVER_GPIO_MAP_ID_1
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_12,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_D,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_2
        },
        /// UART_DRIVER_GPIO_MAP_ID_2 (identical to 1)
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_12,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_D,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_2
        }
    },
    /// UART_DRIVER_ID_6
    {
        /// UART_DRIVER_GPIO_MAP_ID_1
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_6,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_7
        },
        /// UART_DRIVER_GPIO_MAP_ID_2
        {
            .txHardware.portId   = GPIO_DRIVER_PORT_ID_G,
            .txHardware.pinId    = GPIO_DRIVER_PIN_ID_14,
            .rxHardware.portId   = GPIO_DRIVER_PORT_ID_G,
            .rxHardware.pinId    = GPIO_DRIVER_PIN_ID_9
        }
    }
};

/**
 * @brief UART alternate function map.
 */
static const gpio_driver_alternate_function_e afMap[UART_DRIVER_ID_COUNT] =
{
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART1,  /// UART_DRIVER_ID_1
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART2,  /// UART_DRIVER_ID_2
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART3,  /// UART_DRIVER_ID_3
    GPIO_DRIVER_ALTERNATE_FUNCTION_UART4,   /// UART_DRIVER_ID_4
    GPIO_DRIVER_ALTERNATE_FUNCTION_UART5,   /// UART_DRIVER_ID_5
    GPIO_DRIVER_ALTERNATE_FUNCTION_USART6   /// UART_DRIVER_ID_6
};

/**
 * @brief UART IRQ map.
 */
static const IRQn_Type irqMap[UART_DRIVER_ID_COUNT] =
{
    USART1_IRQn,    /// UART_DRIVER_ID_1
    USART2_IRQn,    /// UART_DRIVER_ID_2
    USART3_IRQn,    /// UART_DRIVER_ID_3
    UART4_IRQn,     /// UART_DRIVER_ID_4
    UART5_IRQn,     /// UART_DRIVER_ID_5
    USART6_IRQn     /// UART_DRIVER_ID_6
};

/**
 * @brief UART interrupt map.
 */
static const uint16_t interruptMap[UART_INTERRUPT_COUNT] =
{
    USART_IT_TXE,   /// UART_INTERRUPT_TX
    USART_IT_RXNE   /// UART_INTERRUPT_RX
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
 * @brief Pointer to UART interface interrupt handler.
 */
static const interface_int_handler_f* intHandler;

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
 * @param driverId UART driver ID.
 * @param enabled Flag that indicates if the UART should be enabled or disabled.
 */
static void setEnabled(const uart_driver_id_e driverId, const bool enabled);

/**
 * @brief Configures the given UART.
 * @param driverId UART driver ID.
 * @param config UART configuration.
 */
static void configure(const uart_driver_id_e driverId,
                      const uart_config_t* config);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param driverId UART driver ID.
 * @param data Byte to transmit.
 * @return true if the function was successful, false if not.
 */
static void tx(const uart_driver_id_e driverId, uint8_t data);

/**
 * @brief Transmits the given byte array over the given UART.
 * @param driverId UART driver ID.
 * @param data Byte received.
 */
static void rx(const uart_driver_id_e driverId, uint8_t* data);

/**
 * @brief Sets the given UART interrupt enabled or disabled.
 * @param driverId UART driver ID.
 * @param interrupt UART interrupt to set enabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
static void intSetEnabled(const uart_driver_id_e driverId,
                          const uart_interrupt_e interrupt,
                          const bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uartDriverInit(const interface_int_handler_f* handler)
{
    // plat4m
    //
    // Initialize UART driver.
    // Add drivers to UART interface.
    //
    // Example:
    //
    //      uart_driver_t uarts[] =
    //      {
    //          // UART_1
    //          {
    //              .id         = UART_DRIVER_ID_1,
    //              .setEnabled = uart1SetEnabled,
    //              ...
    //          },
    //          // UART_2
    //          {
    //              .id         = UART_DRIVER_ID_2,
    //              .setEnabled = uart2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      uartAddDrivers(uarts, ARRAY_SIZE(uarts));
    
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
static void setEnabled(const uart_driver_id_e driverId, const bool enabled)
{
    switch (driverId)
    {
        case UART_DRIVER_ID_1:
        case UART_DRIVER_ID_6:
        {
            RCC_APB2PeriphClockCmd(clockMap[driverId],
                                   (FunctionalState) enabled);

            break;
        }
        default:
        {
            RCC_APB1PeriphClockCmd(clockMap[driverId],
                                   (FunctionalState) enabled);

            break;
        }
    }

    USART_Cmd(usartMap[driverId], (FunctionalState) enabled);

    NVIC_InitTypeDef nvicInit =
    {
        .NVIC_IRQChannel                      = irqMap[driverId],
        .NVIC_IRQChannelPreemptionPriority    = 0,
        .NVIC_IRQChannelSubPriority           = 0,
        .NVIC_IRQChannelCmd                   = (FunctionalState) enabled
    };

    NVIC_Init(&nvicInit);
}

//------------------------------------------------------------------------------
static void configure(const uart_driver_id_e driverId,
                      const uart_config_t* config)
{
    gpio_config_t gpioConfig =
    {
        .mode               = GPIO_MODE_ALTERNATE_FUNCTION,
        .resistor           = GPIO_RESISTOR_PULL_UP,
        .alternateFunction  = afMap[driverId]
    };

    gpioHardwareConfigure(&(gpioMap[driverId][config->mapId].txHardware),
                          &gpioConfig);
    gpioHardwareConfigure(&(gpioMap[driverId][config->mapId].rxHardware),
                          &gpioConfig);
    gpioHardwareSetEnabled(&(gpioMap[driverId][config->mapId].txHardware),
                           true);
    gpioHardwareSetEnabled(&(gpioMap[driverId][config->mapId].txHardware),
                           true);

    USART_InitTypeDef uartInit =
    {
        .USART_BaudRate             = config->baudRate,
        .USART_WordLength           = wordLengthMap[config->wordBits],
        .USART_StopBits             = stopBitsMap[config->stopBits],
        .USART_Parity               = parityMap[config->parity],
        .USART_HardwareFlowControl  = flowControlMap[config->flowControl],
        .USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx
    };

    USART_Init(usartMap[driverId], &uartInit);
}

//------------------------------------------------------------------------------
static void tx(const uart_driver_id_e driverId, uint8_t data)
{
    USART_SendData(usartMap[driverId], data);
}

//------------------------------------------------------------------------------
static void rx(const uart_driver_id_e driverId, uint8_t* data)
{
    *data = (uint8_t)USART_ReceiveData(usartMap[driverId]);
}

//------------------------------------------------------------------------------
static void intSetEnabled(const uart_driver_id_e driverId,
                          const uart_interrupt_e interrupt,
                          const bool enabled)
{
    USART_ITConfig(usartMap[driverId],
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
        intHandler(UART_DRIVER_ID_1, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        intHandler(UART_DRIVER_ID_1, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART2_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_TXE);
        intHandler(UART_DRIVER_ID_2, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        intHandler(UART_DRIVER_ID_2, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART3_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_TXE);
        intHandler(UART_DRIVER_ID_3, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        intHandler(UART_DRIVER_ID_3, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART4_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(UART4, USART_IT_TXE);
        intHandler(UART_DRIVER_ID_4, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        intHandler(UART_DRIVER_ID_4, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART5_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(UART5, USART_IT_TXE);
        intHandler(UART_DRIVER_ID_5, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        intHandler(UART_DRIVER_ID_5, UART_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void USART6_IRQHandler(void)
{
    // Transmit interrupt
    if (USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
    {
        USART_ClearITPendingBit(USART6, USART_IT_TXE);
        intHandler(UART_DRIVER_ID_6, UART_INTERRUPT_TX);
    }

    // Receive interrupt
    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        intHandler(UART_DRIVER_ID_6, UART_INTERRUPT_RX);
    }
}
