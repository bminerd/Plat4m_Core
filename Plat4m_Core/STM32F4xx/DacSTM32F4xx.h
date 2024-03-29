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
// Copyright (c) 2013-2023 Benjamin Minerd
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
/// @file DacSTM32F4xx.h
/// @author Ben Minerd
/// @date 4/11/2013
/// @brief DacSTM32F4xx class header file.
///

#ifndef PLAT4M_DAC_STM32F4XX_H
#define PLAT4M_DAC_STM32F4XX_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <stm32f4xx.h>
#include <stm32f4xx_dac.h>
#include <stm32f4xx_rcc.h>

#include <Plat4m_Core/Dac.h>
#include <Plat4m_Core/Module.h>
#include <Plat4m_Core/STM32F4xx/GpioPinSTM32F4xx.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class DacSTM32F4xx : public Dac
{
public:

    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------

    /**
     * @brief Enumeration of DAC channels.
     */
    enum ChannelId
    {
        CHANNEL_ID_1 = 0,
        CHANNEL_ID_2
    };

    //--------------------------------------------------------------------------
    // Public static methods
    //--------------------------------------------------------------------------

    static DacSTM32F4xx& get(const ChannelId channelId,
                             GpioPinSTM32F4xx& gpioPin);

private:
    
    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    const ChannelId myChannelId;

    GpioPinSTM32F4xx& myGpioPin;

    //--------------------------------------------------------------------------
    // Private constructors
    //--------------------------------------------------------------------------

    DacSTM32F4xx(const ChannelId channelId, GpioPinSTM32F4xx& gpioPin);
    
    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Module
    //--------------------------------------------------------------------------

    virtual Module::Error driverEnable(const bool enable) override;

    //--------------------------------------------------------------------------
    // Private virtual methods overridden for Dac
    //--------------------------------------------------------------------------

    virtual Error driverConfigure(const Config& config) override;

    virtual Error driverSetVoltage(const float voltage) override;
};

}; // namespace Plat4m

#endif // PLAT4M_DAC_STM32F4XX_H
