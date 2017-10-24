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
/// @file AdcSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 4/10/13
/// @brief AdcSTM32F4xx class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AdcSTM32F4xx.h>
#include <Processor.h>
#include <CallbackMethod.h>

using Plat4m::AdcSTM32F4xx;
using Plat4m::Module;
using Plat4m::ProcessorSTM32F4xx;
using Plat4m::InterruptSTM32F4xx;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static AdcSTM32F4xx* interruptObjectMap[3][16];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F4xx::Peripheral AdcSTM32F4xx::myPeripheralMap[] =
{
    ProcessorSTM32F4xx::PERIPHERAL_ADC_1, /// ID_1
    ProcessorSTM32F4xx::PERIPHERAL_ADC_2, /// ID_2
    ProcessorSTM32F4xx::PERIPHERAL_ADC_3  /// ID_3
};

const uint32_t AdcSTM32F4xx::myResolutionMap[] =
{
    ADC_CR1_RES_1 | ADC_CR1_RES_0, /// RESOLUTION_6_BIT
    ADC_CR1_RES_1,                 /// RESOLUTION_8_BIT
    ADC_CR1_RES_0,                 /// RESOLUTION_10_BIT
    0                              /// RESOLUTION_12_BIT
};

const float AdcSTM32F4xx::myResolutionMaxValueMap[] =
{
    64.0,   /// RESOLUTION_6_BIT
    256.0,  /// RESOLUTION_8_BIT
    1024.0, /// RESOLUTION_10_BIT
    4096.0  /// RESOLUTION_12_BIT
};

// Variables

/**
 * @brief ADC map.
 */
ADC_TypeDef* AdcSTM32F4xx::myAdcMap[] =
{
    ADC1, /// ID_1
    ADC2, /// ID_2
    ADC3  /// ID_3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F4xx::AdcSTM32F4xx(const Id id) :
    myId(id),
    myAdc(myAdcMap[id]),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &AdcSTM32F4xx::interruptHandler)),
    myDmaStream(0),
    myConfig(),
    myChannelConfigs(),
    myLevelsToVolts(0.0),
    myValues(),
    myCurrentValueIndex(0),
    myChannelToValueMap()
{
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F4xx::~AdcSTM32F4xx()
{
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error AdcSTM32F4xx::driverSetEnabled(const bool enabled)
{
//    RCC_APB2PeriphClockCmd(clockMap[myId], (FunctionalState) enable);
//    ADC_Cmd(myAdc, (FunctionalState) enable);

    ProcessorSTM32F4xx::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);
    
    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void ADC_IRQHandler(void)
{
//    if (((ADC1->ISR & ADC1->IER & 0x7FF)) != 0)
//    {
//        interruptObjectMap[AdcSTM32F30x::ID_1]->handler();
//    }
//    else if (((ADC2->ISR & ADC2->IER & 0x7FF)) != 0)
//    {
//        interruptObjectMap[AdcSTM32F30x::ID_2]->handler();
//    }
}
