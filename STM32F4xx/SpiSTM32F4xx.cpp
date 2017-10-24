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
/// @file SpiSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 4/4/13
/// @brief SpiSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <SpiSTM32F4xx.h>
#include <ProcessorSTM32F4xx.h>

#include <stm32f4xx_spi.h>
#include <stm32f4xx_rcc.h>

using Plat4m::SpiSTM32F4xx;
using Plat4m::Spi;
using Plat4m::GpioSTM32F4xx;
using Plat4m::Module;
using Plat4m::InterruptSTM32F4xx;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F4xx* interruptObjectMap[3];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

/**
 * @brief SPI clock map.
 */
const uint32_t SpiSTM32F4xx::myClockMap[] =
{
    RCC_APB2Periph_SPI1, /// ID_1
    RCC_APB1Periph_SPI2, /// ID_2
    RCC_APB1Periph_SPI3  /// ID_3
};

/**
 * @brief SPI IRQ map.
 */
const IRQn_Type SpiSTM32F4xx::myIrqMap[] =
{
    SPI1_IRQn, /// ID_1
    SPI2_IRQn, /// ID_2
    SPI3_IRQn  /// ID_3
};

/**
 * @brief SPI alternate function map.
 */
const GpioSTM32F4xx::AlternateFunction SpiSTM32F4xx::myAlternateFunctionMap[] =
{
    GpioSTM32F4xx::ALTERNATE_FUNCTION_SPI1, /// ID_1
    GpioSTM32F4xx::ALTERNATE_FUNCTION_SPI2, /// ID_2
    GpioSTM32F4xx::ALTERNATE_FUNCTION_SPI3  /// ID_3
};

/**
 * @brief SPI mode map.
 */
const uint16_t SpiSTM32F4xx::myModeMap[] =
{
    SPI_Mode_Master, /// SPI_MODE_MASTER
    SPI_Mode_Slave   /// SPI_MODE_SLAVE
};

/**
 * @brief SPI data bits map.
 */
const uint16_t SpiSTM32F4xx::myDataBitsMap[] =
{
    SPI_DataSize_8b, /// SPI_DATA_BITS_8
    SPI_DataSize_16b /// SPI_DATA_BITS_16
};

/**
 * @brief SPI clock polarity map.
 */
const uint16_t SpiSTM32F4xx::myClockPolarityMap[] =
{
    SPI_CPOL_Low, /// SPI_CLOCK_POLARITY_LOW
    SPI_CPOL_High /// SPI_CLOCK_POLARITY_HIGH
};

/**
 * @brief SPI clock phase map.
 */
const uint16_t SpiSTM32F4xx::myClockPhaseMap[] =
{
    SPI_CPHA_1Edge, /// SPI_CLOCK_PHASE_LEADING_EDGE
    SPI_CPHA_2Edge  /// SPI_CLOCK_PHASE_TRAILING_EDGE
};

/**
 * @brief SPI clock prescaler map.
 */
const SpiSTM32F4xx::Prescaler SpiSTM32F4xx::myClockPrescalerMap[] =
{
    {2,   SPI_BaudRatePrescaler_2},   /// SPI_CLOCK_PRESCALER_2
    {4,   SPI_BaudRatePrescaler_4},   /// SPI_CLOCK_PRESCALER_4
    {8,   SPI_BaudRatePrescaler_8},   /// SPI_CLOCK_PRESCALER_8
    {16,  SPI_BaudRatePrescaler_16},  /// SPI_CLOCK_PRESCALER_16
    {32,  SPI_BaudRatePrescaler_32},  /// SPI_CLOCK_PRESCALER_32
    {64,  SPI_BaudRatePrescaler_64},  /// SPI_CLOCK_PRESCALER_64
    {128, SPI_BaudRatePrescaler_128}, /// SPI_CLOCK_PRESCALER_128
    {256, SPI_BaudRatePrescaler_256}  /// SPI_CLOCK_PRESCALER_256
};

/**
 * @brief SPI data order map.
 */
const uint16_t SpiSTM32F4xx::myBitOrderMap[] =
{
    SPI_FirstBit_LSB,   /// SPI_BIT_ORDER_LSB_FIRST
    SPI_FirstBit_MSB    /// SPI_BIT_ORDER_MSB_FIRST
};

/**
 * @brief SPI interrupt map.
 */
const uint8_t SpiSTM32F4xx::myInterruptMap[] =
{
    SPI_I2S_IT_TXE,  /// INTERRUPT_TRANSMIT_BUFFER_EMPTY
    SPI_I2S_IT_RXNE, /// INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY
    SPI_I2S_IT_ERR   /// INTERRUPT_ERROR
};

const GpioSTM32F4xx::OutputSpeed SpiSTM32F4xx::myDefaultOutputSpeed =
                                              GpioSTM32F4xx::OUTPUT_SPEED_50MHZ;

/**
 * @brief SPI map.
 */
SPI_TypeDef* SpiSTM32F4xx::mySpiMap[] =
{
    SPI1, /// ID_1
    SPI2, /// ID_2
    SPI3  /// ID_3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SpiSTM32F4xx::SpiSTM32F4xx(const Id id,
                           const TransmissionMode transmissionMode,
                           GpioPinSTM32F4xx& sckGpioPin,
                           GpioPinSTM32F4xx& misoGpioPin,
                           GpioPinSTM32F4xx& mosiGpioPin) :
    Spi(transmissionMode),
    myId(id),
    mySpi(mySpiMap[id]),
    mySckGpioPin(sckGpioPin),
    myMisoGpioPin(&misoGpioPin),
    myMosiGpioPin(&mosiGpioPin),
    myClockPrescaler(),
    myState(STATE_IDLE),
    myTransfer(0)
{
}

//------------------------------------------------------------------------------
SpiSTM32F4xx::SpiSTM32F4xx(const Id id,
                           const TransmissionMode transmissionMode,
                           GpioPinSTM32F4xx& sckGpioPin,
                           GpioPinSTM32F4xx& dataGpioPin) :
    Spi(transmissionMode),
    myId(id),
    mySpi(mySpiMap[id]),
    mySckGpioPin(sckGpioPin),
    myMisoGpioPin(0),
    myMosiGpioPin(0),
    myClockPrescaler(),
    myState(STATE_IDLE),
    myTransfer(0)
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
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void SpiSTM32F4xx::interruptHandler()
{
    if (isNullPointer(myTransfer))
    {
        return;
    }

    // NOTE:
    // Putting this check here implies that an additional interrupt is required
    // to stop the transfer
    if ((myTransfer->txBuffer.isEmpty()) &&
        (((myTransfer->rxCount) == 0) ||
         ((myTransfer->rxCount) == myTransfer->rxBuffer.count())))
    {
        setInterruptEnabled(INTERRUPT_TRANSMIT_BUFFER_EMPTY, false);
        setInterruptEnabled(INTERRUPT_RECEIVE_BUFFER_NOT_EMPTY, false);

        myState = STATE_IDLE;

        return;
    }

    // TODO Change to support 16bit data
    uint8_t byte = 0;
    bool txIsEmpty, rxIsEmpty;

    txIsEmpty = myTransfer->txBuffer.isEmpty();
    rxIsEmpty = myTransfer->rxBuffer.isEmpty();

    // Transmit flag
    if (SPI_I2S_GetFlagStatus(mySpi, SPI_I2S_FLAG_TXE))
    {
        switch (myTransfer->transferMode)
        {
            case TRANSFER_MODE_TX_RX_SEQUENTIAL: // Fall through
            case TRANSFER_MODE_TX_RX_SEQUENTIAL_MAILBOX:
            {
                if (myTransfer->txBuffer.read(byte))
                {
                    writeByte(byte);
                }
                else
                {
                    writeByte(0);
                }

                break;
            }
            case TRANSFER_MODE_TX:                  // Fall through
            case TRANSFER_MODE_TX_WAIT:             // Fall through
            case TRANSFER_MODE_TX_RX_CONCURRENT:    // Fall through
            case TRANSFER_MODE_TX_RX_CONCURRENT_MAILBOX:
            {
                if (myTransfer->txBuffer.read(byte))
                {
                    writeByte(byte);
                }

                break;
            }
            default:
            {
                break; // Does this make sense?
            }
        }
    }
    // Receive flag
    else if (SPI_I2S_GetFlagStatus(mySpi, SPI_I2S_FLAG_RXNE))
    {
        bool isLastByte =
                      (myTransfer->rxCount + 1) == myTransfer->rxBuffer.count();
        byte = readByte();

        switch (myTransfer->transferMode)
        {
            case TRANSFER_MODE_TX_RX_SEQUENTIAL: // Fall through
            case TRANSFER_MODE_TX_RX_SEQUENTIAL_MAILBOX:
            {
                if (myTransfer->txBuffer.isEmpty())
                {
                    if (!(myTransfer->rxBuffer.write(byte)))
                    {
                    }
                }

                break;
            }
            case TRANSFER_MODE_TX_RX_CONCURRENT: // Fall through
            case TRANSFER_MODE_TX_RX_CONCURRENT_MAILBOX:
            {
                if (!(myTransfer->rxBuffer.write(byte)))
                {
                    // Error?
                }

                break;
            }
        }
    }
    //TODO: else if (SPI_I2S_GetFlagStatus(mySpi, ...)) More flags?
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Module
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Module::Error SpiSTM32F4xx::driverEnable(const bool enable)
{
    mySckGpioPin.enable(enable);
    
    if (isValidPointer(myMisoGpioPin))
    {
        myMisoGpioPin->enable(enable);
    }
    
    if (isValidPointer(myMosiGpioPin))
    {
        myMosiGpioPin->enable(enable);
    }
    
    switch (myId)
    {
        case ID_1:
        {
            RCC_APB2PeriphClockCmd(myClockMap[myId], (FunctionalState) enable);

            break;
        }
        default:
        {
            RCC_APB1PeriphClockCmd(myClockMap[myId], (FunctionalState) enable);

            break;
        }
    }
    
    SPI_Cmd(mySpi, (FunctionalState) enable);

    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel                   = myIrqMap[myId];
    nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
    nvicInit.NVIC_IRQChannelSubPriority        = 0;
    nvicInit.NVIC_IRQChannelCmd                = (FunctionalState) enable;

    NVIC_Init(&nvicInit);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods implemented from Spi
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Spi::Error SpiSTM32F4xx::driverSetConfig(const Config& config)
{
    GpioPin::Config gpioConfig;
    gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
    gpioConfig.resistor = GpioPin::RESISTOR_PULL_DOWN;
    
    GpioSTM32F4xx::Config gpioDriverConfig;
    gpioDriverConfig.alternateFunction = myAlternateFunctionMap[myId];
    gpioDriverConfig.outputSpeed       = myDefaultOutputSpeed;

    if (isValidPointer(myMisoGpioPin))
    {
        myMisoGpioPin->configure(gpioConfig);
        myMisoGpioPin->configureDriver(gpioDriverConfig);
    }
    
    if (isValidPointer(myMosiGpioPin))
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

    int i;
    int size = ARRAY_SIZE(myClockPrescalerMap);

    for (i = 0; i < size; i++)
    {
        if (myId == ID_1)
        {
            if ((ProcessorSTM32F4xx::getApb2ClockFrequencyHz() /
                 myClockPrescalerMap[i].value) ==
                                                        config.clockFrequencyHz)
            {
                break;
            }
        }
        else // myId == ID_2 || myId == ID_3
        {
            if ((ProcessorSTM32F4xx::getApb1ClockFrequencyHz() /
                 myClockPrescalerMap[i].value) ==
                                                        config.clockFrequencyHz)
            {
                break;
            }
        }
    }

    // True if no prescaler was found to make the desired clock frequency valid
    if (i == size)
    {
        return Spi::Error(Spi::ERROR_CODE_CLOCK_FREQUENCY_INVALID);
    }

    myClockPrescaler = (ClockPrescaler) i;

    SPI_InitTypeDef spiInit;
    spiInit.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    spiInit.SPI_Mode              = myModeMap[config.mode];
    spiInit.SPI_DataSize          = myDataBitsMap[config.dataBits];
    spiInit.SPI_CPOL              = myClockPolarityMap[config.clockPolarity];
    spiInit.SPI_CPHA              = myClockPhaseMap[config.clockPhase];
    spiInit.SPI_NSS               = SPI_NSS_Hard;
    spiInit.SPI_BaudRatePrescaler = myClockPrescalerMap[myClockPrescaler].bits;
    spiInit.SPI_FirstBit          = myBitOrderMap[config.bitOrder];
    spiInit.SPI_CRCPolynomial     = 7;

    SPI_Init(mySpi, &spiInit);
    
    return Spi::Error(Spi::ERROR_CODE_NONE);
}

/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void SpiSTM32F4xx::writeByte(const uint8_t byte)
{   
    // Direct register access to improve speed
    mySpi->DR = (uint16_t) byte;
}

//------------------------------------------------------------------------------
uint8_t SpiSTM32F4xx::readByte()
{
    // Direct register access to improve speed
    return ((uint8_t) (mySpi->DR));
}

//------------------------------------------------------------------------------
void SpiSTM32F4xx::setInterruptEnabled(const Interrupt interrupt,
                                       const bool enable)
{
    SPI_I2S_ITConfig(mySpi, myInterruptMap[interrupt], (FunctionalState) enable);
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern "C" void SPI1_IRQHandler(void)
{
    interruptObjectMap[SpiSTM32F4xx::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void SPI2_IRQHandler(void)
{
    interruptObjectMap[SpiSTM32F4xx::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void SPI3_IRQHandler(void)
{
    interruptObjectMap[SpiSTM32F4xx::ID_3]->handler();
}
