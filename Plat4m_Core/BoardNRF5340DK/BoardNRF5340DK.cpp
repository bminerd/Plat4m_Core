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
// Copyright (c) 2022 Benjamin Minerd
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
/// @file BoardNRF5340DK.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief BoardNRF5340DK class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/BoardNRF5340DK/BoardNRF5340DK.h>
#include <Plat4m_Core/MemoryAllocator.h>

using namespace Plat4m;

//------------------------------------------------------------------------------
// Private static data member initialization
//------------------------------------------------------------------------------

const float BoardNRF5340DK::myProcessorCoreVoltage                 = 3.3;
const std::uint32_t BoardNRF5340DK::myProcessorExternalClockFrequencyHz =
                                                             32000000; // 32 MHz

const GpioPin::Id BoardNRF5340DK::myGpioPinIdMap[] =
{
    /// GPIO_PIN_ID_TXD
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_20},
    /// GPIO_PIN_ID_RXD
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_22},
    /// GPIO_PIN_ID_CTS
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_21},
    /// GPIO_PIN_ID_RTS
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_19},
    /// GPIO_PIN_ID_LED_1
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_28},
    /// GPIO_PIN_ID_LED_2
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_29},
    /// GPIO_PIN_ID_LED_3
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_30},
    /// GPIO_PIN_ID_LED_4
    {GpioPortNRF5340::ID_P0, GpioPinNRF5340::ID_31},
};

const UartNRF5340::Id BoardNRF5340DK::myUartIdMap[] =
{
    UartNRF5340::ID_2 /// UART_ID_VCOM
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardNRF5340DK::BoardNRF5340DK() :
    Board(),
    myGpioPorts(),
	myGpioPins(),
	myProcessor(0),
    myUart(0)
{
}

//------------------------------------------------------------------------------
// Public destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
BoardNRF5340DK::~BoardNRF5340DK()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GpioPortNRF5340& BoardNRF5340DK::getGpioPort(
                                               const GpioPortNRF5340::Id portId)
{
    if (isNullPointer(myGpioPorts[portId]))
    {
        myGpioPorts[portId] =
                             MemoryAllocator::allocate<GpioPortNRF5340>(portId);
    }

    return (*(myGpioPorts[portId]));
}

//------------------------------------------------------------------------------
GpioPinNRF5340& BoardNRF5340DK::getGpioPin(const GpioPinId gpioPinId)
{
    GpioPortNRF5340& gpioPort =
          getGpioPort((GpioPortNRF5340::Id) (myGpioPinIdMap[gpioPinId].portId));

    if (isNullPointer(myGpioPins[gpioPinId]))
    {
        GpioPinNRF5340::Id id =
                         (GpioPinNRF5340::Id) (myGpioPinIdMap[gpioPinId].pinId);

        myGpioPins[gpioPinId] =
                        MemoryAllocator::allocate<GpioPinNRF5340>(gpioPort, id);
    }

    return (*(myGpioPins[gpioPinId]));
}

//------------------------------------------------------------------------------
ProcessorNRF5340& BoardNRF5340DK::getProcessor()
{
    if (isNullPointer(myProcessor))
    {
        myProcessor =
            MemoryAllocator::allocate<ProcessorNRF5340>(myProcessorCoreVoltage);
    }

    return (*myProcessor);
}

//------------------------------------------------------------------------------
ProcessorNRF5340& BoardNRF5340DK::getProcessor(
                                         const ProcessorNRF5340::Config& config)
{
    if (isNullPointer(myProcessor))
    {
        myProcessor =
             MemoryAllocator::allocate<ProcessorNRF5340>(myProcessorCoreVoltage,
                                                         config);
    }

    return (*myProcessor);
}

//------------------------------------------------------------------------------
UartNRF5340& BoardNRF5340DK::getUart(const UartId uartId,
                                      const bool isHardwareFlowControlEnabled)
{
    if (isNullPointer(myUart))
    {
        if (isHardwareFlowControlEnabled)
        {
            myUart =
                MemoryAllocator::allocate<UartNRF5340>(
                                                   myUartIdMap[uartId],
                                                   getGpioPin(GPIO_PIN_ID_TXD),
                                                   getGpioPin(GPIO_PIN_ID_RXD),
                                                   getGpioPin(GPIO_PIN_ID_CTS),
                                                   getGpioPin(GPIO_PIN_ID_RTS));
        }
        else
        {
            myUart =
                MemoryAllocator::allocate<UartNRF5340>(
                                                   myUartIdMap[uartId],
                                                   getGpioPin(GPIO_PIN_ID_TXD),
                                                   getGpioPin(GPIO_PIN_ID_RXD));
        }
    }

    return (*myUart);
}
