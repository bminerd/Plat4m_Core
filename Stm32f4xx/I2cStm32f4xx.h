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
 * @file I2cStm32f4xx.h
 * @author Ben Minerd
 * @date 3/25/13
 * @brief I2cStm32f4xx class.
 */

#ifndef _I2C_STM32F4XX_H_
#define _I2C_STM32F4XX_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <I2c.h>
#include <GpioPinStm32f4xx.h>

#include <stm32f4xx.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

class I2cStm32f4xx : public I2c
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/

    /**
     * @brief Enumeration of I2Cs.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3
    };

    struct Register
    {
        struct Cr1
        {
            enum Bit
            {
                BIT_SOFTWARE_RESET                  = (uint16_t) 0x8000,
                // Bit 14: Reserved
                BIT_SMBUS_ALERT                     = (uint16_t) 0x2000,
                BIT_PACKET_ERROR_CHECKING           = (uint16_t) 0x1000,
                BIT_ACKNOWLEDGE_PEC_POSITION        = (uint16_t) 0x0800,
                BIT_ACKNOWLEDGE_ENABLE              = (uint16_t) 0x0400,
                BIT_STOP_GENERATION                 = (uint16_t) 0x0200,
                BIT_START_GENERATION                = (uint16_t) 0x0100,
                BIT_CLOCK_STRETCHING_DISABLE        = (uint16_t) 0x0080,
                BIT_GENERAL_CALL_ENABLE             = (uint16_t) 0x0040,
                BIT_PEC_ENABLE                      = (uint16_t) 0x0020,
                BIT_ARP_ENABLE                      = (uint16_t) 0x0010,
                BIT_SMBUS_TYPE                      = (uint16_t) 0x0008,
                // Bit 2: Reserved
                BIT_SMBUS_MODE                      = (uint16_t) 0x0002,
                BIT_PERIPHERAL_ENABLE               = (uint16_t) 0x0001
            };
        };

        struct Cr2
        {
            enum Bit
            {
                // Bits 15-13: Reserved
                BIT_DMA_LAST_TRANSFER               = (uint16_t) 0x1000,
                BIT_DMA_REQUESTS_ENABLE             = (uint16_t) 0x0800,
                BIT_BUFFER_INTERRUPT_ENABLE         = (uint16_t) 0x0400,
                BIT_EVENT_INTERRUPT_ENABLE          = (uint16_t) 0x0200,
                BIT_ERROR_INTERRUPT_ENABLE          = (uint16_t) 0x0100
                // Bits 7-6: Reserved
                // Bits 5-0: Peripheral clock frequency
            };
        };

        struct Sr1
        {
            enum Bit
            {
                BIT_SMBUS_ALERT                     = (uint16_t) 0x8000,
                BIT_TIMEOUT                         = (uint16_t) 0x4000,
                // Bit 13: Reserved
                BIT_PEC_ERROR                       = (uint16_t) 0x1000,
                BIT_OVERRUN_UNDERRUN                = (uint16_t) 0x0800,
                BIT_ACKNOWLEDGE_FAILURE             = (uint16_t) 0x0400,
                BIT_ARBITRATION_LOST                = (uint16_t) 0x0200,
                BIT_BUS_ERROR                       = (uint16_t) 0x0100,
                BIT_TRANSMIT_EMPTY                  = (uint16_t) 0x0080,
                BIT_RECEIVE_NOT_EMPTY               = (uint16_t) 0x0040,
                // Bit 5: Reserved
                BIT_STOP_DETECTION                  = (uint16_t) 0x0010,
                BIT_10_BIT_HEADER_SENT              = (uint16_t) 0x0008,
                BIT_BYTE_TRANSFER_FINISHED          = (uint16_t) 0x0004,
                BIT_ADDRESS_SENT_MATCHED            = (uint16_t) 0x0002,
                BIT_START_BIT                       = (uint16_t) 0x0001
            };
        };

        struct Sr2
        {
            enum Bit
            {
                // Bits 15-8: Packet error checking register
                BIT_DUAL_FLAG                       = (uint16_t) 0x0080,
                BIT_SMBUS_HOST_READER               = (uint16_t) 0x0040,
                BIT_SMBUS_DEVICE_DEFAULT_ADDRESS    = (uint16_t) 0x0020,
                BIT_GENERAL_CALL_ADDRESS            = (uint16_t) 0x0010,
                // Bit 3: Reserved
                BIT_TRANSMITTER_RECEIVER            = (uint16_t) 0x0004,
                BIT_BUS_BUSY                        = (uint16_t) 0x0002,
                BIT_MASTER_SLAVE                    = (uint16_t) 0x0001
            };
        };

        struct Ccr
        {
            enum Bit
            {
                BIT_MASTER_MODE_SELECTION           = (uint16_t) 0x8000,
                BIT_FAST_MODE_DUTY_CYCLE            = (uint16_t) 0x4000,
                // Bits 13-12: Reserved
                // Bits 11-0: Clock control value
            };
        };
    };
    
    /*--------------------------------------------------------------------------
     * Public static methods
     *------------------------------------------------------------------------*/
    
    static I2cStm32f4xx& get(const Id id,
                             GpioPinStm32f4xx& sclGpioPin,
                             GpioPinStm32f4xx& sdaGpioPin);

private:

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    const Id myId;

    I2C_TypeDef* myI2c;

    GpioPinStm32f4xx& mySclGpioPin;

    GpioPinStm32f4xx& mySdaGpioPin;

    /*--------------------------------------------------------------------------
    * Private constructors and destructors
     *------------------------------------------------------------------------*/

    I2cStm32f4xx(const Id id,
                 GpioPinStm32f4xx& sclGpioPin,
                 GpioPinStm32f4xx& sdaGpioPin);
    
    /*--------------------------------------------------------------------------
     * Private implemented methods
     *------------------------------------------------------------------------*/

    Error driverEnable(const bool enable);

    Error driverConfigure(const Config& config);

    Error driverMasterStart();

    Error driverMasterStop();

    Error driverMasterTxAddress(const uint8_t address, const MasterMode mode);

    Error driverTx(const uint8_t byte);

    Error driverRx(bool isLastByte);

    Error driverRead(uint8_t& byte);

    Error driverInterruptEnable(const Interrupt interrupt, const bool enable);

    Error driverReset();
};

#endif // _I2C_STM32F4XX_H_
