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
 * @file gpio_driver_template.c
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
//      #include <i2c_driver_stm32f10x.h>

#include <i2c_driver_stm32f2xx.h>

// plat4m
//
// Include necessary manufacturer I2C driver header files here.
//
// Example:
//
//      #include <stm32f10x_i2c.h>

#include <stm32f2xx_i2c.h>
#include <stm32f2xx_rcc.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define I2C_OWN_ADDRESS (0xA0)

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
 * TODO Comment!
 */
static uint8_t masterModeMap[2] =
{
    I2C_Direction_Transmitter,
    I2C_Direction_Receiver
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local I2C driver functions here.
//
// Example:
//
//      static void i2c1SetEnabled(bool enabled);
//      static void i2c1MasterStart(void);
//      ...

/**
 * TODO Comment!
 */
static void i2c1SetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static inline void i2c1MasterStart(void);

/**
 * TODO Comment!
 */
static inline void i2c1MasterStop(void);

/**
 * TODO Comment!
 */
static inline void i2c1MasterTxAddress(uint8_t address,
                                       i2c_master_mode_e mode);

/**
 * TODO Comment!
 */
static inline void i2c1Tx(uint8_t data);

/**
 * TODO Comment!
 */
static inline void i2c1Rx(uint8_t* data);

/**
 * TODO Comment!
 */
static inline void i2c1TxIntSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
static inline void i2c1RxIntSetEnabled(bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void i2cDriverInit(void)
{
    // plat4m
    //
    // Initialize I2C driver.
    // Add drivers to I2C interface.
    //
    // Example:
    //
    //      i2c_driver_t i2cs[] =
    //      {
    //          // I2C_1
    //          {
    //              .id             = I2C_DRIVER_ID_1,
    //              .setEnabled     = i2c1SetEnabled,
    //              ...
    //          },
    //          // I2C_2
    //          {
    //              .id             = I2C_DRIVER_ID_2,
    //              .setEnabled     = i2c2SetEnabled,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      i2cAddDrivers(i2cs, ARRAY_SIZE(i2cs));


    i2c_driver_t i2cs[] =
    {
        // I2C_1
        {
            .id                 = I2C_DRIVER_ID_1,
            .address            = I2C_OWN_ADDRESS,
            .setEnabled         = i2c1SetEnabled,
            .masterStart        = i2c1MasterStart,
            .masterStop         = i2c1MasterStop,
            .masterTxAddress    = i2c1MasterTxAddress,
            .tx                 = i2c1Tx,
            .rx                 = i2c1Rx,
            .txIntSetEnabled    = i2c1TxIntSetEnabled,
            .rxIntSetEnabled    = i2c1RxIntSetEnabled
        }
    };

    i2cAddDrivers(i2cs, ARRAY_SIZE(i2cs));
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

// plat4m
//
// Define here local I2C driver functions declared above.

//------------------------------------------------------------------------------
static void i2c1SetEnabled(bool enabled)
{
    GPIO_InitTypeDef gpioInit;
    NVIC_InitTypeDef nvicInit;
    I2C_InitTypeDef i2cInit;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

    gpioInit.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;

    nvicInit.NVIC_IRQChannel                    = I2C1_EV_IRQn;
    nvicInit.NVIC_IRQChannelPreemptionPriority  = 1;
    nvicInit.NVIC_IRQChannelSubPriority         = 0;

    i2cInit.I2C_Mode                = I2C_Mode_I2C;
    i2cInit.I2C_DutyCycle           = I2C_DutyCycle_2;
    i2cInit.I2C_OwnAddress1         = I2C_OWN_ADDRESS;
    i2cInit.I2C_Ack                 = I2C_Ack_Enable;
    i2cInit.I2C_ClockSpeed          = 100000;
    i2cInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    if (enabled)
    {
        gpioInit.GPIO_Mode  = GPIO_Mode_AF;
        gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
        gpioInit.GPIO_OType = GPIO_OType_OD;
        gpioInit.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB, &gpioInit);

        // Connect PXx to I2C_SCL
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

        // Connect PXx to I2C_SDA
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

        nvicInit.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&nvicInit);

        I2C_Init(I2C1, &i2cInit);
        I2C_ITConfig(I2C1, (I2C_IT_ERR | I2C_IT_EVT), ENABLE);
        I2C_Cmd(I2C1, ENABLE);
    }
    else
    {
        I2C_Cmd(I2C1, DISABLE);
        I2C_ITConfig(I2C1, (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR), DISABLE);
        I2C_DeInit(I2C1);

        nvicInit.NVIC_IRQChannelCmd = DISABLE;
        NVIC_Init(&nvicInit);
    }
}

//------------------------------------------------------------------------------
static inline void i2c1MasterStart(void)
{
    I2C_GenerateSTART(I2C1, ENABLE);

    I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
}

//------------------------------------------------------------------------------
static inline void i2c1MasterStop(void)
{
    I2C_GenerateSTOP(I2C1, ENABLE);
}

//------------------------------------------------------------------------------
static inline void i2c1MasterTxAddress(uint8_t address, i2c_master_mode_e mode)
{
    I2C_Send7bitAddress(I2C1, address, masterModeMap[mode]);
}

//------------------------------------------------------------------------------
static inline void i2c1Tx(uint8_t data)
{
    I2C_SendData(I2C1, data);
}

//------------------------------------------------------------------------------
static inline void i2c1Rx(uint8_t* data)
{
}

//------------------------------------------------------------------------------
static inline void i2c1TxIntSetEnabled(bool enabled)
{
    I2C_ITConfig(I2C1, I2C_IT_BUF, (FunctionalState) enabled);
}

//------------------------------------------------------------------------------
static inline void i2c1RxIntSetEnabled(bool enabled)
{
    // Do nothing for now!
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void I2C1_EV_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C1);

    switch (event)
    {
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            i2cIntHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_START);

            break;
        }
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            i2cIntHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_ADDRESS);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
        {
            i2cIntHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_TX);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            i2cIntHandler(I2C_DRIVER_ID_1, I2C_INTERRUPT_TX);

            break;
        }
    }
}

//------------------------------------------------------------------------------
extern void I2C1_ER_IRQHandler(void)
{
}
