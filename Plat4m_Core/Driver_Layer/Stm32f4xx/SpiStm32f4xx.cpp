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
 * @file SpiStm32f4xx.cpp
 * @author Ben Minerd
 * @date 4/4/13
 * @brief SpiStm32f4xx class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <SpiStm32f4xx.h>

#include <stm32f4xx_spi.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief SPI map.
 */
static SPI_TypeDef* spiMap[SPI_STM32F4XX_ID_COUNT] =
{
    SPI1,   /// ID_1
    SPI2,   /// ID_2
    SPI3    /// ID_3
};

/**
 * @brief SPI clock map.
 */
static const uint32_t clockMap[SPI_STM32F4XX_ID_COUNT] =
{
    RCC_APB2Periph_SPI1,    /// ID_1
    RCC_APB1Periph_SPI2,    /// ID_2
    RCC_APB1Periph_SPI3     /// ID_3
};

/**
 * @brief SPI IRQ map.
 */
static const IRQn_Type irqMap[SPI_STM32F4XX_ID_COUNT] =
{
    SPI1_IRQn,  /// ID_1
    SPI2_IRQn,  /// ID_2
    SPI3_IRQn   /// ID_3
};

/**
 * @brief SPI alternate function map.
 */
static const GpioStm32f4xx::AlternateFunction afMap[SPI_STM32F4XX_ID_COUNT] =
{
    GpioStm32f4xx::ALTERNATE_FUNCTION_SPI1, /// ID_1
    GpioStm32f4xx::ALTERNATE_FUNCTION_SPI2, /// ID_2
    GpioStm32f4xx::ALTERNATE_FUNCTION_SPI3  /// ID_3
};

/**
 * @brief SPI mode map.
 */
static const uint16_t modeMap[] =
{
    SPI_Mode_Master,    /// SPI_MODE_MASTER
    SPI_Mode_Slave      /// SPI_MODE_SLAVE
};

/**
 * @brief SPI data bits map.
 */
static const uint16_t dataBitsMap[] =
{
    SPI_DataSize_8b,    /// SPI_DATA_BITS_8
    SPI_DataSize_16b    /// SPI_DATA_BITS_16
};

/**
 * @brief SPI clock polarity map.
 */
static const uint16_t clockPolarityMap[] =
{
    SPI_CPOL_Low,   /// SPI_CLOCK_POLARITY_LOW
    SPI_CPOL_High   /// SPI_CLOCK_POLARITY_HIGH
};

/**
 * @brief SPI clock phase map.
 */
static const uint16_t clockPhaseMap[] =
{
    SPI_CPHA_1Edge, /// SPI_CLOCK_PHASE_LEADING_EDGE
    SPI_CPHA_2Edge  /// SPI_CLOCK_PHASE_TRAILING_EDGE
};

/**
 * @brief SPI clock prescaler map.
 */
static const uint16_t clockPrescalerMap[] =
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
static const uint16_t bitOrderMap[] =
{
    SPI_FirstBit_LSB,   /// SPI_BIT_ORDER_LSB_FIRST
    SPI_FirstBit_MSB    /// SPI_BIT_ORDER_MSB_FIRST
};

/**
 * @brief SPI interrupt map.
 */
static const uint8_t interruptMap[] =
{
    SPI_I2S_IT_TXE, /// SPI_INTERRUPT_TX
    SPI_I2S_IT_RXNE /// SPI_INTERRUPT_RX
};

static const GpioStm32f4xx::OutputSpeed outputSpeed =
                                              GpioStm32f4xx::OUTPUT_SPEED_50MHZ;

static SpiStm32f4xx* objectMap[SPI_STM32F4XX_ID_COUNT];

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SpiStm32f4xx& SpiStm32f4xx::get(const Id id,
                                const TransmissionMode transmissionMode,
                                GpioPinStm32f4xx& sckGpioPin,
                                GpioPinStm32f4xx& misoGpioPin,
                                GpioPinStm32f4xx& mosiGpioPin)
{
    if (IS_NULL_POINTER(objectMap[id]))
    {
        objectMap[id] = new SpiStm32f4xx(id,
                                         transmissionMode,
                                         sckGpioPin,
                                         misoGpioPin,
                                         mosiGpioPin);
    }
    else
    {
        // Error!
    }
    
    return *(objectMap[id]);
}

//------------------------------------------------------------------------------
SpiStm32f4xx& SpiStm32f4xx::get(const Id id,
                                const TransmissionMode transmissionMode,
                                GpioPinStm32f4xx& sckGpioPin,
                                GpioPinStm32f4xx& dataGpioPin)
{
    if (IS_NULL_POINTER(objectMap[id]))
    {
        objectMap[id] = new SpiStm32f4xx(id,
                                         transmissionMode,
                                         sckGpioPin,
                                         dataGpioPin);
    }
    else
    {
        // Error!
    }
    
    return *(objectMap[id]);
}

/*------------------------------------------------------------------------------
 * Private constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
SpiStm32f4xx::SpiStm32f4xx(const Id id,
                           const TransmissionMode transmissionMode,
                           GpioPinStm32f4xx& sckGpioPin,
                           GpioPinStm32f4xx& misoGpioPin,
                           GpioPinStm32f4xx& mosiGpioPin) :
    Spi(transmissionMode),
    myId(id),
    mySpi(spiMap[id]),
    mySckGpioPin(sckGpioPin),
    myMisoGpioPin(&misoGpioPin),
    myMosiGpioPin(&mosiGpioPin)
{
}

//------------------------------------------------------------------------------
SpiStm32f4xx::SpiStm32f4xx(const Id id,
                           const TransmissionMode transmissionMode,
                           GpioPinStm32f4xx& sckGpioPin,
                           GpioPinStm32f4xx& dataGpioPin) :
    Spi(transmissionMode),
    myId(id),
    mySpi(spiMap[id]),
    mySckGpioPin(sckGpioPin),
    myMisoGpioPin(NULL_POINTER),
    myMosiGpioPin(NULL_POINTER)
{
    switch (transmissionMode)
    {
        case TRANSMISSION_MODE_RX:
        {
            myMisoGpioPin = &dataGpioPin;
            
            break;
        }
        case TRANSMISSION_MODE_TX:
        {
            myMosiGpioPin = &dataGpioPin;
            
            break;
        }
    }
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Spi::Error SpiStm32f4xx::driverEnable(const bool enable)
{
    mySckGpioPin.enable(enable);
    
    if (IS_VALID_POINTER(myMisoGpioPin))
    {
        myMisoGpioPin->enable(enable);
    }
    
    if (IS_VALID_POINTER(myMosiGpioPin))
    {
        myMosiGpioPin->enable(enable);
    }
    
    switch (myId)
    {
        case ID_1:
        {
            RCC_APB2PeriphClockCmd(clockMap[myId], (FunctionalState) enable);

            break;
        }
        default:
        {
            RCC_APB1PeriphClockCmd(clockMap[myId], (FunctionalState) enable);

            break;
        }
    }
    
    SPI_Cmd(mySpi, (FunctionalState) enable);

    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel                    = irqMap[myId];
    nvicInit.NVIC_IRQChannelPreemptionPriority  = 0;
    nvicInit.NVIC_IRQChannelSubPriority         = 0;
    nvicInit.NVIC_IRQChannelCmd                 = (FunctionalState) enable;

    NVIC_Init(&nvicInit);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Spi::Error SpiStm32f4xx::driverConfigure(const Config& config)
{
    GpioPin::Config gpioConfig;
    gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
    gpioConfig.resistor = GpioPin::RESISTOR_PULL_DOWN;
    
    GpioStm32f4xx::Config gpioDriverConfig;
    gpioDriverConfig.alternateFunction  = afMap[myId];
    gpioDriverConfig.outputSpeed        = outputSpeed;

    if (IS_VALID_POINTER(myMisoGpioPin))
    {
        myMisoGpioPin->configure(gpioConfig);
        myMisoGpioPin->configureDriver(gpioDriverConfig);
    }
    
    if (IS_VALID_POINTER(myMosiGpioPin))
    {
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;
        myMosiGpioPin->configure(gpioConfig);
        myMosiGpioPin->configureDriver(gpioDriverConfig);
    }
    
    if ((config.clockPolarity) == CLOCK_POLARITY_HIGH)
    {
        gpioConfig.resistor = GpioPin::RESISTOR_PULL_UP;
    }
    
    mySckGpioPin.configure(gpioConfig);
    mySckGpioPin.configureDriver(gpioDriverConfig);

    SPI_InitTypeDef spiInit;
//    spiInit.SPI_Direction           = SPI_Direction_2Lines_FullDuplex;
    spiInit.SPI_Direction           = SPI_Direction_1Line_Tx;
    spiInit.SPI_Mode                = modeMap[config.mode];
    spiInit.SPI_DataSize            = dataBitsMap[config.dataBits];
    spiInit.SPI_CPOL                = clockPolarityMap[config.clockPolarity];
    spiInit.SPI_CPHA                = clockPhaseMap[config.clockPhase];
    spiInit.SPI_NSS                 = SPI_NSS_Hard;
    spiInit.SPI_BaudRatePrescaler   = clockPrescalerMap[config.clockPrescaler];
    spiInit.SPI_FirstBit            = bitOrderMap[config.bitOrder];
    spiInit.SPI_CRCPolynomial       = 7;

    SPI_Init(mySpi, &spiInit);
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Spi::Error SpiStm32f4xx::driverTx(const uint8_t byte)
{   
    // Direct register access to improve speed
    mySpi->DR = (uint16_t) byte;
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Spi::Error SpiStm32f4xx::driverRx(uint8_t& byte, const bool isLastByte)
{
    // Direct register access to improve speed
    byte = (uint8_t) (mySpi->DR);
    
    if (isLastByte)
    {
        while (SPI_I2S_GetFlagStatus(mySpi, SPI_I2S_FLAG_BSY) == SET);
    }
    
    return ERROR_NONE;
}

//------------------------------------------------------------------------------
Spi::Error SpiStm32f4xx::driverInterruptEnable(const Interrupt interrupt,
                                               const bool enable)
{
    SPI_I2S_ITConfig(mySpi, interruptMap[interrupt], (FunctionalState) enable);
    
    if (interrupt == INTERRUPT_TX)
    {
        // This should be checked to make sure it doesn't wait too long (could
        // block other interrupts)
        while (SPI_I2S_GetFlagStatus(mySpi, SPI_I2S_FLAG_BSY));
    }
    
    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern "C" void SPI1_IRQHandler(void)
{
    // Transmit flag
    if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
    {
        // Should this be here?
        //SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
        objectMap[SpiStm32f4xx::ID_1]->interruptHandler(Spi::INTERRUPT_TX);
    }
    // Receive flag
    else if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
    {
        // Should this be here?
        //SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
        objectMap[SpiStm32f4xx::ID_1]->interruptHandler(Spi::INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
extern "C" void SPI2_IRQHandler(void)
{
    // Transmit flag
    if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE))
    {
        // Should this be here?
        //SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
        objectMap[SpiStm32f4xx::ID_2]->interruptHandler(Spi::INTERRUPT_TX);
    }
    // Receive flag
    else if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE))
    {
        // Should this be here?
        //SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
        objectMap[SpiStm32f4xx::ID_2]->interruptHandler(Spi::INTERRUPT_RX);
    }
}

//------------------------------------------------------------------------------
extern "C" void SPI3_IRQHandler(void)
{
    // Transmit interrupt
    if (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE))
    {
        // Should this be here?
        //SPI_I2S_ClearITPendingBit(SPI3, SPI_I2S_IT_TXE);
        objectMap[SpiStm32f4xx::ID_3]->interruptHandler(Spi::INTERRUPT_TX);
    }
    // Receive interrupt
    else if (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE))
    {
        // Should this be here?
        //SPI_I2S_ClearITPendingBit(SPI3, SPI_I2S_IT_RXNE);
        objectMap[SpiStm32f4xx::ID_3]->interruptHandler(Spi::INTERRUPT_RX);
    }
}