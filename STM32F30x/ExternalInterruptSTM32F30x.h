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
/// @file ExternalInterruptSTM32F30x.h
/// @author Ben Minerd
/// @date 2/3/2016
/// @brief ExternalInterruptSTM32F30x class header file.
///

#ifndef PLAT4M_EXTERNAL_INTERRUPT_STM32F30X_H
#define PLAT4M_EXTERNAL_INTERRUPT_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------
     
#include <stm32f30x.h>
#include <stm32f30x_exti.h>
#include <stm32f30x_rcc.h>

#include <Plat4m_Core/Plat4m.h>
#include <Plat4m_Core/ExternalInterrupt.h>
#include <Plat4m_Core/STM32F30x/GpioPinSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/GpioPortSTM32F30x.h>
#include <Plat4m_Core/STM32F30x/InterruptSTM32F30x.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class ExternalInterruptSTM32F30x : public ExternalInterrupt
{
public:
    
    //--------------------------------------------------------------------------
    // Public enumerations
    //--------------------------------------------------------------------------
    
    /**
     * @brief Enumeration of interrupts.
     */
    enum Id
    {
        ID_0 = 0,
        ID_1,
        ID_2,
        ID_3,
        ID_4,
        ID_5,
        ID_6,
        ID_7,
        ID_8,
        ID_9,
        ID_10,
        ID_11,
        ID_12,
        ID_13,
        ID_14,
        ID_15
    };
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    ExternalInterruptSTM32F30x(GpioPinSTM32F30x& gpioPin);
    
    //--------------------------------------------------------------------------
    // Public static inline methods
    //--------------------------------------------------------------------------

    static inline bool isInterruptActive(const Id id);

    static inline void clear(const Id id);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const InterruptSTM32F30x::Id myInterruptIdMap[];
    
    static const uint32_t myLineMap[];

    static const EXTITrigger_TypeDef myTriggerMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    const GpioPortSTM32F30x::Id myGpioPortId;
    
    InterruptSTM32F30x myInterrupt;
    
    //--------------------------------------------------------------------------
    // Private static methods
    //--------------------------------------------------------------------------

    static void exti5To9Interrupt();

    static void exti10To15Interrupt();

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverEnable(const bool enable);

    //--------------------------------------------------------------------------
    // Private methods implemented from ExternalInterrupt
    //--------------------------------------------------------------------------
    
    Error driverConfigure(const Config& config);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void interruptHandlerPrivate();
};

}; // namespace Plat4m

#endif // EXTERNAL_INTERRUPT_STM32F30X_H
