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
/// @file BoardNRF5340DK.h
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief BoardNRF5340DK class.
///

#ifndef PLAT4M_BOARD_NRF5340_PDK_H
#define PLAT4M_BOARD_NRF5340_PDK_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <cstdint>

#include <Plat4m_Core/Board.h>
#include <Plat4m_Core/NRF5340/ProcessorNRF5340.h>
#include <Plat4m_Core/NRF5340/GpioPortNRF5340.h>
#include <Plat4m_Core/NRF5340/GpioPinNRF5340.h>
#include <Plat4m_Core/NRF5340/UartNRF5340.h>
#include <Plat4m_Core/GpioPin.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class BoardNRF5340DK : public Board
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    enum GpioPinId
    {
		GPIO_PIN_ID_TXD = 0,
        GPIO_PIN_ID_RXD,
        GPIO_PIN_ID_CTS,
        GPIO_PIN_ID_RTS,
        GPIO_PIN_ID_LED_1,
        GPIO_PIN_ID_LED_2,
        GPIO_PIN_ID_LED_3,
        GPIO_PIN_ID_LED_4
    };

    enum UartId
    {
        UART_ID_VCOM = 0
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    BoardNRF5340DK();
    
    //--------------------------------------------------------------------------
    // Public destructors
    //--------------------------------------------------------------------------

	~BoardNRF5340DK();

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    GpioPortNRF5340& getGpioPort(const GpioPortNRF5340::Id portId);

    GpioPinNRF5340& getGpioPin(const GpioPinId gpioPinId);
    
    ProcessorNRF5340& getProcessor();

    ProcessorNRF5340& getProcessor(const ProcessorNRF5340::Config& config);

    UartNRF5340& getUart(const UartId uartId,
                         const bool isHardwareFlowControlEnabled = false);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const float myProcessorCoreVoltage;

    static const std::uint32_t myProcessorExternalClockFrequencyHz;

    static const GpioPin::Id myGpioPinIdMap[];

    static const UartNRF5340::Id myUartIdMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    GpioPortNRF5340* myGpioPorts[2];

    GpioPinNRF5340* myGpioPins[8];

    ProcessorNRF5340* myProcessor;

    UartNRF5340* myUart;
};

}; // namespace Plat4m

#endif // PLAT4M_BOARD_NRF5340_PDK_H
