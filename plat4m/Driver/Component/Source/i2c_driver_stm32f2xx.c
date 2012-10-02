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
 * @file i2c_driver_stm32f2xx.c
 * @author Ben Minerd
 * @date 2/25/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <i2c_interface.h>

// plat4m
//
// Replace below with proper I2C driver header file.
//
// Example:
//
//      #include <i2c_stm32f10x.h>

#include <i2c_driver_stm32f2xx.h>

// plat4m
//
// Include necessary manufacturer I2C driver header files here.
//
// Example:
//
//      #include <stm32f10x_i2c.h>

#include <gpio_interface.h>

#include <stm32f2xx.h>
#include <stm32f2xx_i2c.h>
#include <stm32f2xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C own address.
 */
#define I2C_OWN_ADDRESS (0xA0)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C GPIO hardware type.
 */
typedef struct _i2c_gpio_hardware_t_
{
    gpio_hardware_t sclHardware;
    gpio_hardware_t sdaHardware;
} i2c_gpio_hardware_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief I2C driver map.
 */
static I2C_TypeDef* i2cMap[I2C_DRIVER_ID_COUNT] =
{
    I2C1,   /// I2C_DRIVER_ID_1
    I2C2,   /// I2C_DRIVER_ID_2
    I2C3    /// I2C_DRIVER_ID_3
};

/**
 * @brief I2C clock map.
 */
static const uint32_t clockMap[I2C_DRIVER_ID_COUNT] =
{
    RCC_APB1Periph_I2C1,    /// I2C_DRIVER_ID_1
    RCC_APB1Periph_I2C2,    /// I2C_DRIVER_ID_2
    RCC_APB1Periph_I2C3     /// I2C_DRIVER_ID_3
};

/**
 * @brief I2C GPIO hardware map.
 */
static const i2c_gpio_hardware_t gpioMap[I2C_DRIVER_ID_COUNT]
                                        [I2C_DRIVER_GPIO_MAP_ID_COUNT] =
{
    /// I2C_DRIVER_ID_1
    {
        /// I2C_DRIVER_GPIO_MAP_ID_1
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_6,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_7
        },
        /// I2C_DRIVER_GPIO_MAP_ID_2
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_8,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_9
        },
        /// I2C_DRIVER_GPIO_MAP_ID_3 (same as map 2)
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_8,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_9
        }
    },
    /// I2C_DRIVER_ID_2
    {
        /// I2C_DRIVER_GPIO_MAP_ID_1
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_10,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_B,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_11
        },
        /// I2C_DRIVER_GPIO_MAP_ID_2
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_F,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_1,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_F,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_0
        },
        /// I2C_DRIVER_GPIO_MAP_ID_3
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_H,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_4,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_H,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_5
        }
    },
    /// I2C_DRIVER_ID_3
    {
        /// I2C_DRIVER_GPIO_MAP_ID_1
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_A,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_0,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_C,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_9
        },
        /// I2C_DRIVER_GPIO_MAP_ID_2
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_H,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_7,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_H,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_8
        },
        /// I2C_DRIVER_GPIO_MAP_ID_3 (same as map 2)
        {
            .sclHardware.portId   = GPIO_DRIVER_PORT_ID_H,
            .sclHardware.pinId    = GPIO_DRIVER_PIN_ID_7,
            .sdaHardware.portId   = GPIO_DRIVER_PORT_ID_H,
            .sdaHardware.pinId    = GPIO_DRIVER_PIN_ID_8
        }
    }
};

/**
 * @brief I2C alternate function map.
 */
static const gpio_driver_alternate_function_e afMap[I2C_DRIVER_ID_COUNT] =
{
    GPIO_DRIVER_ALTERNATE_FUNCTION_I2C1,    /// I2C_DRIVER_ID_1
    GPIO_DRIVER_ALTERNATE_FUNCTION_I2C2,    /// I2C_DRIVER_ID_2
    GPIO_DRIVER_ALTERNATE_FUNCTION_I2C3     /// I2C_DRIVER_ID_3
};

/**
 * @brief I2C IRQ map.
 */
static const IRQn_Type irqMap[I2C_DRIVER_ID_COUNT] =
{
    I2C1_EV_IRQn,   /// I2C_DRIVER_ID_1
    I2C2_EV_IRQn,   /// I2C_DRIVER_ID_2
    I2C3_EV_IRQn,   /// I2C_DRIVER_ID_3
};

/**
 * @brief I2C interrupt map.
 */
static const uint16_t interruptMap[I2C_INTERRUPT_COUNT] =
{
    I2C_IT_EVT, /// I2C_INTERRUPT_START
    I2C_IT_EVT, /// I2C_INTERRUPT_STOP
    I2C_IT_EVT, /// I2C_INTERRUPT_ADDRESS
    I2C_IT_BUF, /// I2C_INTERRUPT_TX
    I2C_IT_BUF  /// I2C_INTERRUPT_RX
};

/**
 * @brief I2C address bits map.
 */
static const uint16_t addressBitsMap[I2C_ADDRESS_BITS_COUNT] =
{
    I2C_AcknowledgedAddress_7bit,   /// I2C_ADDRESS_BITS_7
    I2C_AcknowledgedAddress_10bit   /// I2C_ADDRESS_BITS_10
};

/**
 * @brief Pointer to I2C interface interrupt handler.
 */
static const interface_int_handler_f* intHandler;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local I2C driver functions here.
//
// Example:
//
//      ...

/**
 * @brief Function type that sets the given I2C enabled or disabled.
 * @param driverId I2C driver ID.
 * @param enabled Flag that indicates if the I2C should be enabled or disabled.
 */
static void setEnabled(const i2c_driver_id_e driverId,
                       const bool enabled);

/**
 * @brief Function type that configures the given I2C.
 * @param driverId I2C driver ID.
 * @param config I2C configuration.
 */
static void configure(const i2c_driver_id_e driverId,
                      const i2c_config_t* config);

/**
 * @brief Function type that performs a start on the given I2C.
 * @param driverId I2C driver ID.
 */
static void masterStart(const i2c_driver_id_e driverId);

/**
 * @brief Function type that performs a stop on the given I2C.
 * @param driverId I2C driver ID.
 */
static void masterStop(const i2c_driver_id_e driverId);

/**
 * @brief Function type that transmits a slave address over the given I2C.
 * @param driverId I2C driver ID.
 * @param address I2C slave address.
 * @param mode I2C master mode.
 */
static void masterTxAddress(const i2c_driver_id_e driverId,
                            const i2c_address_t address,
                            const i2c_master_mode_e mode);

/**
 * @brief Function type that transmits a byte over the given I2C.
 * @param driverId I2C driver ID.
 * @param data Byte to transmit.
 */
static void tx(const i2c_driver_id_e driverId, const uint8_t data);

/**
 * @brief Function type that receives a byte over the given I2C.
 * @param driverId I2C driver ID.
 * @param data Byte received.
 */
static void rx(const i2c_driver_id_e driverId, uint8_t* data);

/**
 * @brief Function type that sets the given I2C interrupt enabled or disabled.
 * @param driverId I2C driver ID.
 * @param interrupt I2C interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
static void intSetEnabled(const i2c_driver_id_e driverId,
                          const i2c_interrupt_e interrupt,
                          const bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cDriverInit(const interface_int_handler_f* handler)
{
    // plat4m
    //
    // Initialize I2C driver.
    // Add drivers to I2C interface.
    //
    // Example:
    //
    //      i2c_t i2cs[] =
    //      {
    //          // I2C_1
    //          {
    //              .id             = I2C_ID_1,
    //              .setEnabled     = i2c1SetEnabled,
    //              ...
    //          },
    //          // I2C_2
    //          {
    //              .id             = I2C_ID_2,
    //              .setEnabled     = i2c2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      i2cAddDrivers(i2cs, ARRAY_SIZE(i2cs));


    const i2c_driver_t driver =
    {
        .setEnabled         = setEnabled,
        .configure          = configure,
        .masterStart        = masterStart,
        .masterStop         = masterStop,
        .masterTxAddress    = masterTxAddress,
        .tx                 = tx,
        .rx                 = rx,
        .intSetEnabled      = intSetEnabled
    };

    i2cSetDriver(&driver);

    intHandler = handler;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local I2C driver functions declared above.

//------------------------------------------------------------------------------
static void setEnabled(const i2c_driver_id_e driverId, const bool enabled)
{
    RCC_APB1PeriphClockCmd(clockMap[driverId], (FunctionalState) enabled);
    I2C_Cmd(i2cMap[driverId], (FunctionalState) enabled);

    NVIC_InitTypeDef nvicInit =
    {
        .NVIC_IRQChannel                    = irqMap[driverId],
        .NVIC_IRQChannelPreemptionPriority  = 1,
        .NVIC_IRQChannelSubPriority         = 0,
        .NVIC_IRQChannelCmd                 = (FunctionalState) enabled
    };

    NVIC_Init(&nvicInit);
}

//------------------------------------------------------------------------------
static void configure(const i2c_driver_id_e driverId,
                      const i2c_config_t* config)
{
    gpio_config_t gpioConfig =
    {
        .mode               = GPIO_MODE_ALTERNATE_FUNCTION,
        .resistor           = GPIO_RESISTOR_NONE,
        .alternateFunction  = afMap[driverId]
    };

    gpioHardwareConfigure(&(gpioMap[driverId][config->mapId].sclHardware),
                          &gpioConfig);
    gpioHardwareConfigure(&(gpioMap[driverId][config->mapId].sclHardware),
                          &gpioConfig);
    gpioHardwareSetEnabled(&(gpioMap[driverId][config->mapId].sdaHardware),
                           true);
    gpioHardwareSetEnabled(&(gpioMap[driverId][config->mapId].sdaHardware),
                           true);

    I2C_InitTypeDef i2cInit =
    {
        .I2C_ClockSpeed          = config->clockSpeed,
        .I2C_Mode                = I2C_Mode_I2C,
        .I2C_DutyCycle           = I2C_DutyCycle_2,
        .I2C_OwnAddress1         = I2C_OWN_ADDRESS,
        .I2C_Ack                 = I2C_Ack_Enable,
        .I2C_AcknowledgedAddress = addressBitsMap[config->addressBits]
    };

    I2C_Init(i2cMap[driverId], &i2cInit);
}

//------------------------------------------------------------------------------
static void masterStart(const i2c_driver_id_e driverId)
{
    I2C_GenerateSTART(i2cMap[driverId], ENABLE);
}

//------------------------------------------------------------------------------
static void masterStop(const i2c_driver_id_e driverId)
{
    I2C_GenerateSTOP(i2cMap[driverId], ENABLE);
}

//------------------------------------------------------------------------------
static void masterTxAddress(const i2c_driver_id_e driverId,
                            const i2c_address_t address,
                            const i2c_master_mode_e mode)
{
    I2C_Send7bitAddress(i2cMap[driverId], address, mode);
}

//------------------------------------------------------------------------------
static void tx(const i2c_driver_id_e driverId, const uint8_t data)
{
    I2C_SendData(i2cMap[driverId], data);
}

//------------------------------------------------------------------------------
static void rx(const i2c_driver_id_e driverId, uint8_t* data)
{
    *data = I2C_ReceiveData(i2cMap[driverId]);
}

//------------------------------------------------------------------------------
static void intSetEnabled(const i2c_driver_id_e driverId,
                          const i2c_interrupt_e interrupt,
                          const bool enabled)
{
    I2C_ITConfig(i2cMap[driverId],
                 interruptMap[interrupt],
                 (FunctionalState) enabled);
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void I2C1_EV_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C1);

    switch (event)
    {
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            intHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_START);

            break;
        }
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            intHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_ADDRESS);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
        {
            intHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_TX);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            intHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_STOP);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_RECEIVED:
        {
            intHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_RX);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void I2C1_ER_IRQHandler(void)
{
}

//------------------------------------------------------------------------------
void I2C2_EV_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C2);

    switch (event)
    {
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            intHandler(I2C_DRIVER_ID_2, I2C_INTERRUPT_START);

            break;
        }
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            intHandler(I2C_DRIVER_ID_2, I2C_INTERRUPT_ADDRESS);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
        {
            intHandler(I2C_DRIVER_ID_2, I2C_INTERRUPT_TX);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            intHandler(I2C_DRIVER_ID_2, I2C_INTERRUPT_STOP);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_RECEIVED:
        {
            intHandler(I2C_DRIVER_ID_2, I2C_INTERRUPT_RX);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void I2C2_ER_IRQHandler(void)
{
}

//------------------------------------------------------------------------------
void I2C3_EV_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C3);

    switch (event)
    {
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            intHandler(I2C_DRIVER_ID_3, I2C_INTERRUPT_START);

            break;
        }
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            intHandler(I2C_DRIVER_ID_3, I2C_INTERRUPT_ADDRESS);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
        {
            intHandler(I2C_DRIVER_ID_3, I2C_INTERRUPT_TX);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            intHandler(I2C_DRIVER_ID_3, I2C_INTERRUPT_STOP);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_RECEIVED:
        {
            intHandler(I2C_DRIVER_ID_3, I2C_INTERRUPT_RX);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void I2C3_ER_IRQHandler(void)
{
}
