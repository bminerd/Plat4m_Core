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
 * @file spi_driver_template.c
 * @author Ben Minerd
 * @date 8/1/12
 * @brief SPI driver layer for the STM32F2xx.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <spi_interface.h>

// plat4m
//
// Replace below with proper SPI driver header file.
// ex.
//      #include <spi_driver_stm32f10x.h>

#include <spi_driver.h>

// plat4m
//
// Include necessary manufacturer driver header files here.
// ex.
//      #include <stm32f10x_spi.h>

#include <gpio_interface.h>

#include <stm32f2xx.h>
#include <stm32f2xx_spi.h>
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
 * @brief SPI hardware type.
 */
typedef struct _spi_gpio_hardware_t_
{
    gpio_hardware_t sck;
    gpio_hardware_t miso;
    gpio_hardware_t mosi;
} spi_gpio_hardware_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief SPI interface interrupt handler.
 */
static const spi_interface_int_handler_f* intHandler;

/**
 * @brief SPI map.
 */
static SPI_TypeDef* spiMap[SPI_COUNT] =
{
    SPI1,   /// SPI_1
    SPI2,   /// SPI_2
    SPI3    /// SPI_3
};

/**
 * @brief SPI clock map.
 */
static const uint32_t clockMap[SPI_COUNT] =
{
    RCC_APB2Periph_SPI1,    /// SPI_1
    RCC_APB1Periph_SPI2,    /// SPI_2
    RCC_APB1Periph_SPI3     /// SPI_3
};

/**
 * @brief SPI GPIO hardware map.
 */
static const spi_gpio_hardware_t gpioMap[SPI_COUNT][SPI_GPIO_MAP_COUNT] =
{
    /// SPI_1
    {
        /// SPI_GPIO_MAP_1
        {
            .sck.port   = GPIO_PORT_A,
            .sck.pin    = GPIO_PIN_5,
            .miso.port  = GPIO_PORT_A,
            .miso.pin   = GPIO_PIN_6,
            .mosi.port  = GPIO_PORT_A,
            .mosi.pin   = GPIO_PIN_7
        },
        /// SPI_GPIO_MAP_2
        {
            .sck.port   = GPIO_PORT_B,
            .sck.pin    = GPIO_PIN_3,
            .miso.port  = GPIO_PORT_B,
            .miso.pin   = GPIO_PIN_4,
            .mosi.port  = GPIO_PORT_B,
            .mosi.pin   = GPIO_PIN_5
        }
    },
    /// SPI_2
    {
        /// SPI_GPIO_MAP_1
        {
            .sck.port   = GPIO_PORT_B,
            .sck.pin    = GPIO_PIN_10,
            .miso.port  = GPIO_PORT_B,
            .miso.pin   = GPIO_PIN_14,
            .mosi.port  = GPIO_PORT_B,
            .mosi.pin   = GPIO_PIN_15
        },
        /// SPI_GPIO_MAP_2
        {
            .sck.port   = GPIO_PORT_B,
            .sck.pin    = GPIO_PIN_10,
            .miso.port  = GPIO_PORT_C,
            .miso.pin   = GPIO_PIN_2,
            .mosi.port  = GPIO_PORT_C,
            .mosi.pin   = GPIO_PIN_3
        }
    },
    /// SPI_3
    {
        /// SPI_GPIO_MAP_1
        {
            .sck.port   = GPIO_PORT_B,
            .sck.pin    = GPIO_PIN_3,
            .miso.port  = GPIO_PORT_B,
            .miso.pin   = GPIO_PIN_4,
            .mosi.port  = GPIO_PORT_B,
            .mosi.pin   = GPIO_PIN_5
        },
        /// SPI_GPIO_MAP_2
        {
            .sck.port   = GPIO_PORT_C,
            .sck.pin    = GPIO_PIN_10,
            .miso.port  = GPIO_PORT_C,
            .miso.pin   = GPIO_PIN_11,
            .mosi.port  = GPIO_PORT_C,
            .mosi.pin   = GPIO_PIN_12
        }
    }
};

/**
 * @brief SPI IRQ map.
 */
static const IRQn_Type irqMap[SPI_COUNT] =
{
    SPI1_IRQn,  /// SPI_1
    SPI2_IRQn,  /// SPI_2
    SPI3_IRQn   /// SPI_3
};

/**
 * @brief SPI alternate function map.
 */
static const gpio_alternate_function_e afMap[SPI_COUNT] =
{
    GPIO_ALTERNATE_FUNCTION_SPI1,   /// SPI_1
    GPIO_ALTERNATE_FUNCTION_SPI2,   /// SPI_2
    GPIO_ALTERNATE_FUNCTION_SPI3,   /// SPI_3
};

/**
 * @brief SPI mode map.
 */
static const uint16_t modeMap[SPI_MODE_COUNT] =
{
    SPI_Mode_Master,    /// SPI_MODE_MASTER
    SPI_Mode_Slave      /// SPI_MODE_SLAVE
};

/**
 * @brief SPI data bits map.
 */
static const uint16_t dataBitsMap[SPI_DATA_BITS_COUNT] =
{
    SPI_DataSize_16b,   /// SPI_DATA_BITS_8
    SPI_DataSize_8b     /// SPI_DATA_BITS_16
};

/**
 * @brief SPI clock polarity map.
 */
static const uint16_t clockPolarityMap[SPI_CLOCK_POLARITY_COUNT] =
{
    SPI_CPOL_Low,   /// SPI_CLOCK_POLARITY_LOW
    SPI_CPOL_High   /// SPI_CLOCK_POLARITY_HIGH
};

/**
 * @brief SPI clock phase map.
 */
static const uint16_t clockPhaseMap[SPI_CLOCK_PHASE_COUNT] =
{
    SPI_CPHA_1Edge, /// SPI_CLOCK_PHASE_LEADING_EDGE
    SPI_CPHA_2Edge  /// SPI_CLOCK_PHASE_TRAILING_EDGE
};

/**
 * @brief SPI clock prescaler map.
 */
static const uint16_t clockPrescalerMap[SPI_CLOCK_PRESCALER_COUNT] =
{
    SPI_BaudRatePrescaler_2,    /// SPI_CLOCK_PRESCALER_2
    SPI_BaudRatePrescaler_4,    /// SPI_CLOCK_PRESCALER_4
    SPI_BaudRatePrescaler_8,    /// SPI_CLOCK_PRESCALER_8
    SPI_BaudRatePrescaler_16,   /// SPI_CLOCK_PRESCALER_16
    SPI_BaudRatePrescaler_32,   /// SPI_CLOCK_PRESCALER_32
    SPI_BaudRatePrescaler_64,   /// SPI_CLOCK_PRESCALER_64
    SPI_BaudRatePrescaler_128,  /// SPI_CLOCK_PRESCALER_128
    SPI_BaudRatePrescaler_256   /// SPI_CLOCK_PRESCALER_256
};

/**
 * @brief SPI data order map.
 */
static const uint16_t bitOrderMap[SPI_BIT_ORDER_COUNT] =
{
    SPI_FirstBit_LSB,   /// SPI_BIT_ORDER_LSB_FIRST
    SPI_FirstBit_MSB    /// SPI_BIT_ORDER_MSB_FIRST
};

/**
 * @brief SPI interrupt map.
 */
static const uint8_t interruptMap[SPI_INTERRUPT_COUNT] =
{
    SPI_I2S_IT_TXE, /// SPI_INTERRUPT_TX
    SPI_I2S_IT_RXNE /// SPI_INTERRUPT_RX
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local SPI driver functions here.
//
// Example:
//
//      void spi1SetEnabled(bool setEnabled);
//      void spi1Tx(uint8_t data[], uint8_t size);
//      ...

/**
 * @brief Function type that sets the given SPI enabled or disabled.
 * @param spi SPI to access.
 * @param enabled Flag that indicates if the SPI should be enabled or disabled.
 */
static void setEnabled(const spi_e spi, const bool enabled);

/**
 * @brief Function type that configures the given SPI.
 * @param spi SPI to access.
 * @param config SPI configuration.
 */
static void configure(const spi_e spi, const spi_config_t* config);

/**
 * @brief Function type that transmits the given byte over the given SPI.
 * @param spi SPI to access.
 * @param data Data to transmit.
 * @todo Support 16-bit data.
 */
static void tx(const spi_e spi, const uint8_t data);

/**
 * @brief Function type that receives a byte over the given SPI.
 * @param spi SPI to acccess.
 * @param data Data received.
 * @todo Support 16-bit data.
 */
static void rx(const spi_e spi, uint8_t* byte);

/**
 * @brief Function type that sets the given SPI interrupt enabled or disabled.
 * @param spi SPI to access.
 * @param interrupt SPI interrupt to set enabled or disabled.
 * @param enabled Flag that indicates if the interrupt should be enabled or
 * disabled.
 */
static void intSetEnabled(const spi_e spi,
                          const spi_interrupt_e interrupt,
                          const bool enabled);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void spiDriverInit(const spi_interface_int_handler_f* handler)
{
    spi_driver_t driver =
    {
        .setEnabled     = setEnabled,
        .configure      = configure,
        .tx             = tx,
        .rx             = rx,
        .intSetEnabled  = intSetEnabled
    };

    spiSetDriver(&driver);

    intHandler = handler;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(const spi_e spi, const bool enabled)
{
    switch (spi)
    {
        case SPI_1:
        {
            RCC_APB2PeriphClockCmd(clockMap[spi], (FunctionalState) enabled);

            break;
        }
        default:
        {
            RCC_APB1PeriphClockCmd(clockMap[spi], (FunctionalState) enabled);

            break;
        }
    }

    SPI_Cmd(spiMap[spi], (FunctionalState) enabled);

    NVIC_InitTypeDef nvicInit =
    {
        .NVIC_IRQChannel                      = irqMap[spi],
        .NVIC_IRQChannelPreemptionPriority    = 0,
        .NVIC_IRQChannelSubPriority           = 0,
        .NVIC_IRQChannelCmd                   = (FunctionalState) enabled
    };

    NVIC_Init(&nvicInit);
}

//------------------------------------------------------------------------------
static void configure(const spi_e spi, const spi_config_t* config)
{
    gpio_config_t gpioConfig =
    {
        .mode               = GPIO_MODE_ALTERNATE_FUNCTION,
        .resistor           = GPIO_RESISTOR_PULL_DOWN,
        .alternateFunction  = afMap[spi]
    };

    gpioHardwareSetEnabled(&(gpioMap[spi][config->map].sck), true);
    gpioHardwareSetEnabled(&(gpioMap[spi][config->map].miso), true);
    gpioHardwareSetEnabled(&(gpioMap[spi][config->map].mosi), true);
    gpioHardwareConfigure(&(gpioMap[spi][config->map].sck), &gpioConfig);
    gpioHardwareConfigure(&(gpioMap[spi][config->map].miso), &gpioConfig);
    gpioHardwareConfigure(&(gpioMap[spi][config->map].mosi), &gpioConfig);

    SPI_InitTypeDef spiInit =
    {
        .SPI_Direction          = SPI_Direction_2Lines_FullDuplex,
        .SPI_Mode               = modeMap[config->mode],
        .SPI_DataSize           = dataBitsMap[config->dataBits],
        .SPI_CPOL               = clockPolarityMap[config->clockPolarity],
        .SPI_CPHA               = clockPhaseMap[config->clockPhase],
        .SPI_NSS                = SPI_NSS_Soft,
        .SPI_BaudRatePrescaler  = clockPrescalerMap[config->clockPrescaler],
        .SPI_FirstBit           = bitOrderMap[config->bitOrder],
        .SPI_CRCPolynomial      = 7
    };

    SPI_Init(spiMap[spi], &spiInit);
}

//------------------------------------------------------------------------------
static void tx(const spi_e spi, const uint8_t data)
{
    SPI_I2S_SendData(spiMap[spi], data);
}

//------------------------------------------------------------------------------
static void rx(const spi_e spi, uint8_t* data)
{
    *data = SPI_I2S_ReceiveData(spiMap[spi]);
}

//------------------------------------------------------------------------------
static void intSetEnabled(const spi_e spi,
                          const spi_interrupt_e interrupt,
                          const bool enabled)
{
    SPI_I2S_ITConfig(spiMap[spi],
                     interruptMap[interrupt],
                     (FunctionalState) enabled);
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void SPI1_IRQHandler(void)
{
    // Transmit interrupt
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) != RESET)
    {
        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
        intHandler(SPI_1, SPI_INTERRUPT_TX);
    }

    // Receive interrupt
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
    {
        SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
        intHandler(SPI_1, SPI_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void SPI2_IRQHandler(void)
{
    // Transmit interrupt
    if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) != RESET)
    {
        SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
        intHandler(SPI_2, SPI_INTERRUPT_TX);
    }

    // Receive interrupt
    if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET)
    {
        SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
        intHandler(SPI_2, SPI_INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
void SPI3_IRQHandler(void)
{
    // Transmit interrupt
    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_TXE) != RESET)
    {
        SPI_I2S_ClearITPendingBit(SPI3, SPI_I2S_IT_TXE);
        intHandler(SPI_3, SPI_INTERRUPT_TX);
    }

    // Receive interrupt
    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) != RESET)
    {
        SPI_I2S_ClearITPendingBit(SPI3, SPI_I2S_IT_RXNE);
        intHandler(SPI_3, SPI_INTERRUPT_RX);
    }
}
