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
 * @file i2c_driver_stm32f4xx.c
 * @author Ben Minerd
 * @date 11/26/12
 * @brief I2C driver layer for the STM32F4xx.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <I2cStm32f4xx.h>

#include <stm32f4xx_i2c.h>
#include <stm32f4xx_rcc.h>

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

static const char* idStringMap[] =
{
    "1",    /// I2cStm32f4xx::ID_1
    "2",    /// I2cStm32f4xx::ID_2
    "3"     /// I2cStm32f4xx::ID_3
};

/**
 * @brief I2C driver map.
 */
static I2C_TypeDef* i2cMap[] =
{
    I2C1,   /// I2cStm32f4xx::ID_1
    I2C2,   /// I2cStm32f4xx::ID_2
    I2C3    /// I2cStm32f4xx::ID_3
};

/**
 * @brief I2C clock map.
 */
static const uint32_t clockMap[] =
{
    RCC_APB1Periph_I2C1,    /// I2cStm32f4xx::ID_1
    RCC_APB1Periph_I2C2,    /// I2cStm32f4xx::ID_2
    RCC_APB1Periph_I2C3     /// I2cStm32f4xx::ID_3
};

/**
 * @brief I2C alternate function map.
 */
static const GpioStm32f4xx::AlternateFunction afMap[] =
{
    GpioStm32f4xx::ALTERNATE_FUNCTION_I2C1,  /// I2cStm32f4xx::ID_1
    GpioStm32f4xx::ALTERNATE_FUNCTION_I2C2,  /// I2cStm32f4xx::ID_2
    GpioStm32f4xx::ALTERNATE_FUNCTION_I2C3   /// I2cStm32f4xx::ID_3
};

/**
 * @brief I2C IRQ map.
 */
static const IRQn_Type irqMap[3][2] =
{
    /// I2cStm32f4xx::ID_1
    {
        I2C1_EV_IRQn,
        I2C1_ER_IRQn
    },
    /// I2cStm32f4xx::ID_2
    {
        I2C2_EV_IRQn,
        I2C2_ER_IRQn
    },
    /// I2cStm32f4xx::ID_3
    {
        I2C3_EV_IRQn,
        I2C3_ER_IRQn
    }
};

/**
 * @brief I2C interrupt map.
 */
static const uint16_t interruptMap[] =
{
    I2C_IT_EVT, /// I2c::INTERRUPT_MASTER_START_TRANSMITTED
    I2C_IT_EVT, /// I2c::INTERRUPT_MASTER_TRANSMITTER_SELECTED
    I2C_IT_EVT, /// I2c::INTERRUPT_MASTER_RECEIVER_SELECTED
    I2C_IT_BUF, /// I2c::INTERRUPT_BYTE_TRANSMITTING
    I2C_IT_EVT, /// I2c::INTERRUPT_BYTE_TRANSMITTED
    I2C_IT_BUF, /// I2c::INTERRUPT_BYTE_RECEIVED
    I2C_IT_EVT, /// I2C::INTERRUPT_SLAVE_STOP_RECEIVED
    I2C_IT_ERR  /// I2C::INTERRUPT_NACK_RECEIVED
};

/**
 * @brief I2C address bits map.
 */
static const uint16_t addressBitsMap[] =
{
    I2C_AcknowledgedAddress_7bit,   /// I2c::ADDRESS_BITS_7
    I2C_AcknowledgedAddress_10bit   /// I2c::ADDRESS_BITS_10
};

static const GpioStm32f4xx::OutputSpeed outputSpeed =
                                              GpioStm32f4xx::OUTPUT_SPEED_50MHZ;

static const uint8_t ownAddress = 0xA0;

static I2cStm32f4xx* objectMap[3];

/*------------------------------------------------------------------------------
 * Public static methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
I2cStm32f4xx& I2cStm32f4xx::get(const Id id,
                                GpioPinStm32f4xx& sclGpioPin,
                                GpioPinStm32f4xx& sdaGpioPin)
{
    if (IS_NULL_POINTER(objectMap[id]))
    {
        objectMap[id] = new I2cStm32f4xx(id, sclGpioPin, sdaGpioPin);
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
I2cStm32f4xx::I2cStm32f4xx(const Id id,
                           GpioPinStm32f4xx& sclGpioPin,
                           GpioPinStm32f4xx& sdaGpioPin) :
    I2c(idStringMap[id]),
    myId(id),
    myI2c(i2cMap[id]),
    mySclGpioPin(sclGpioPin),
    mySdaGpioPin(sdaGpioPin)
{
}

/*------------------------------------------------------------------------------
 * Private implemented methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverEnable(const bool enable)
{
    mySclGpioPin.enable(enable);
    mySdaGpioPin.enable(enable);

    RCC_APB1PeriphClockCmd(clockMap[myId], (FunctionalState) enable);

    if (enable)
    {
        // Generic GpioPin configuration
        GpioPin::Config gpioConfig;
        gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
        gpioConfig.resistor = GpioPin::RESISTOR_NONE;

        mySclGpioPin.configure(gpioConfig);
        mySdaGpioPin.configure(gpioConfig);

        // Specific GpioPinStm32f4xx configuration
        GpioStm32f4xx::Config gpioDriverConfig;
        gpioDriverConfig.alternateFunction  = afMap[myId];
        gpioDriverConfig.outputSpeed        = outputSpeed;

        mySclGpioPin.configureDriver(gpioDriverConfig);
        mySdaGpioPin.configureDriver(gpioDriverConfig);

        driverReset();
    }

    I2C_Cmd(myI2c, (FunctionalState) enable);

    NVIC_InitTypeDef nvicInit;
    nvicInit.NVIC_IRQChannel                    = irqMap[myId][0];
    nvicInit.NVIC_IRQChannelPreemptionPriority  = 1;
    nvicInit.NVIC_IRQChannelSubPriority         = 0;
    nvicInit.NVIC_IRQChannelCmd                 = (FunctionalState) enable;

    NVIC_Init(&nvicInit);

    nvicInit.NVIC_IRQChannel = irqMap[myId][1];

    NVIC_Init(&nvicInit);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverConfigure(const Config& config)
{
    I2C_InitTypeDef i2cInit;
    i2cInit.I2C_ClockSpeed          = config.clockSpeedHz;
    i2cInit.I2C_Mode                = I2C_Mode_I2C;
    i2cInit.I2C_DutyCycle           = I2C_DutyCycle_2;
    i2cInit.I2C_OwnAddress1         = ownAddress;
    i2cInit.I2C_Ack                 = I2C_Ack_Enable;
    i2cInit.I2C_AcknowledgedAddress = addressBitsMap[config.addressBits];

    I2C_Init(myI2c, &i2cInit);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverMasterStart()
{
    BIT_SET(myI2c->CR1, Register::Cr1::BIT_START_GENERATION);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverMasterStop()
{
    BIT_SET(myI2c->CR1, Register::Cr1::BIT_STOP_GENERATION);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverMasterTxAddress(const uint8_t address,
                                               const MasterMode masterMode)
{
    // Shift 7-bit address left by 1 and OR master mode in LSB
    myI2c->DR = ((uint8_t) address << 1) | (uint8_t) masterMode;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverTx(const uint8_t data)
{
    myI2c->DR = data;

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverRx(bool isLastByte)
{
    if (isLastByte)
    {
        BIT_CLEAR(myI2c->CR1, Register::Cr1::BIT_ACKNOWLEDGE_ENABLE);
    }
    else
    {
        BIT_SET(myI2c->CR1, Register::Cr1::BIT_ACKNOWLEDGE_ENABLE);
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverRead(uint8_t& byte)
{
    byte = (uint8_t) (myI2c->DR);

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverInterruptEnable(const Interrupt interrupt,
                                               const bool enable)
{
    if (enable)
    {
        BIT_SET(myI2c->CR2, interruptMap[interrupt]);
    }
    else
    {
        BIT_CLEAR(myI2c->CR2, interruptMap[interrupt]);
    }

    return ERROR_NONE;
}

//------------------------------------------------------------------------------
I2c::Error I2cStm32f4xx::driverReset()
{
    BIT_SET(myI2c->CR1, Register::Cr1::BIT_SOFTWARE_RESET);
    BIT_CLEAR(myI2c->CR1, Register::Cr1::BIT_SOFTWARE_RESET);

    return ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Interrupt service routines
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern "C" void I2C1_EV_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C1);

    switch (event)
    {
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                       I2c::INTERRUPT_MASTER_START_TRANSMITTED);

            break;
        }
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                    I2c::INTERRUPT_MASTER_TRANSMITTER_SELECTED);

            break;
        }
        case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
        {
            objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                       I2c::INTERRUPT_MASTER_RECEIVER_SELECTED);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
        {
            if (IS_BIT_CLEAR(I2C1->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                              I2c::INTERRUPT_BYTE_TRANSMITTING);
            }

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            if (IS_BIT_CLEAR(I2C1->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                               I2c::INTERRUPT_BYTE_TRANSMITTED);
            }

            break;
        }
        case I2C_EVENT_MASTER_BYTE_RECEIVED:
        {
            if (IS_BIT_CLEAR(I2C1->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                                  I2c::INTERRUPT_BYTE_RECEIVED);
            }

            break;
        }
    }
}

//------------------------------------------------------------------------------
extern "C" void I2C1_ER_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C1);

    if (I2C_GetITStatus(I2C1, I2C_IT_AF) != RESET)
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
        objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                                  I2c::INTERRUPT_NACK_RECEIVED);
    }

    if (I2C_GetITStatus(I2C1, I2C_IT_BERR) != RESET)
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
        objectMap[I2cStm32f4xx::ID_1]->interruptHandler(
                                                  I2c::INTERRUPT_NACK_RECEIVED);
    }

//    switch (event)
//    {
//        case I2C_EVENT_SLAVE_ACK_FAILURE:
//        {
//            objectMap[I2cStm32f4xx::ID_1]->
//                interruptHandler(I2c::INTERRUPT_NACK_RECEIVED);
//
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
}

//------------------------------------------------------------------------------
extern "C" void I2C2_EV_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C2);

    switch (event)
    {
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                       I2c::INTERRUPT_MASTER_START_TRANSMITTED);

            break;
        }
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                    I2c::INTERRUPT_MASTER_TRANSMITTER_SELECTED);

            break;
        }
        case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
        {
            objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                       I2c::INTERRUPT_MASTER_RECEIVER_SELECTED);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
        {
            if (IS_BIT_CLEAR(I2C2->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                              I2c::INTERRUPT_BYTE_TRANSMITTING);
            }

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            if (IS_BIT_CLEAR(I2C2->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                               I2c::INTERRUPT_BYTE_TRANSMITTED);
            }

            break;
        }
        case I2C_EVENT_MASTER_BYTE_RECEIVED:
        {
            if (IS_BIT_CLEAR(I2C2->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                                  I2c::INTERRUPT_BYTE_RECEIVED);
            }

            break;
        }
    }
}

//------------------------------------------------------------------------------
extern "C" void I2C2_ER_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C2);

    if (I2C_GetITStatus(I2C2, I2C_IT_AF) != RESET)
    {
        I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
        objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                                  I2c::INTERRUPT_NACK_RECEIVED);
    }

    if (I2C_GetITStatus(I2C2, I2C_IT_BERR) != RESET)
    {
        I2C_ClearITPendingBit(I2C2, I2C_IT_BERR);
        objectMap[I2cStm32f4xx::ID_2]->interruptHandler(
                                                  I2c::INTERRUPT_NACK_RECEIVED);
    }
}

//------------------------------------------------------------------------------
extern "C" void I2C3_EV_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C3);

    switch (event)
    {
        case I2C_EVENT_MASTER_MODE_SELECT:
        {
            objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                       I2c::INTERRUPT_MASTER_START_TRANSMITTED);

            break;
        }
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
        {
            objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                    I2c::INTERRUPT_MASTER_TRANSMITTER_SELECTED);

            break;
        }
        case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
        {
            objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                       I2c::INTERRUPT_MASTER_RECEIVER_SELECTED);

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
        {
            if (IS_BIT_CLEAR(I2C3->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                              I2c::INTERRUPT_BYTE_TRANSMITTING);
            }

            break;
        }
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
        {
            if (IS_BIT_CLEAR(I2C3->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                               I2c::INTERRUPT_BYTE_TRANSMITTED);
            }

            break;
        }
        case I2C_EVENT_MASTER_BYTE_RECEIVED:
        {
            if (IS_BIT_CLEAR(I2C3->CR1,
                             I2cStm32f4xx::Register::Cr1::BIT_STOP_GENERATION))
            {
                objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                                  I2c::INTERRUPT_BYTE_RECEIVED);
            }

            break;
        }
    }
}

//------------------------------------------------------------------------------
extern "C" void I2C3_ER_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent(I2C3);

    if (I2C_GetITStatus(I2C3, I2C_IT_AF) != RESET)
    {
        I2C_ClearITPendingBit(I2C3, I2C_IT_AF);
        objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                                  I2c::INTERRUPT_NACK_RECEIVED);
    }

    if (I2C_GetITStatus(I2C3, I2C_IT_BERR) != RESET)
    {
        I2C_ClearITPendingBit(I2C3, I2C_IT_BERR);
        objectMap[I2cStm32f4xx::ID_3]->interruptHandler(
                                                  I2c::INTERRUPT_NACK_RECEIVED);
    }
}
