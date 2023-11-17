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
// Copyright (c) 2021-2023 Benjamin Minerd
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
/// @file TimerNRF5340.cpp
/// @author Ben Minerd
/// @date 1/6/2021
/// @brief TimerNRF5340 class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <limits>

#include <NRF5340/TimerNRF5340.h>
#include <Plat4m_Core/CallbackMethod.h>

using Plat4m::TimerNRF5340;
using Plat4m::Module;
using Plat4m::GpioPinNRF5340;
using Plat4m::InterruptNRF5340;
using Plat4m::ProcessorNRF5340;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptNRF5340* interruptObjectMap[5];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const InterruptNRF5340::Id TimerNRF5340::myInterruptIdMap[] =
{
    InterruptNRF5340::ID_TIMER0, /// ID_0
    InterruptNRF5340::ID_TIMER1, /// ID_1
    InterruptNRF5340::ID_TIMER2, /// ID_2
    InterruptNRF5340::ID_TIMER3, /// ID_3
    InterruptNRF5340::ID_TIMER4  /// ID_4
};

// Variables

NRF_TIMER_Type* TimerNRF5340::myTimerMap[] =
{
    NRF_TIMER0, /// TimerNRF5340::ID_0
    NRF_TIMER1, /// TimerNRF5340::ID_1
    NRF_TIMER2, /// TimerNRF5340::ID_2
    NRF_TIMER3, /// TimerNRF5340::ID_3
    NRF_TIMER4  /// TimerNRF5340::ID_4
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerNRF5340::TimerNRF5340(const Id id) :
    myId(id),
    myChannelCount(myId < ID_3 ? 4 : 6),
    myIsConfigured(false),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &TimerNRF5340::interruptHandler)),
    myCompareInterruptCallbacks(),
    myTimer(myTimerMap[id]),
    myInputClockFrequencyHz(0.0),
    myOutputClockFrequencyHz(0.0),
    myConfig()
{
    initialize();
}

//------------------------------------------------------------------------------
TimerNRF5340::TimerNRF5340(
                           const Id id,
                           CompareInterruptCallback& compareInterruptCallback) :
    myId(id),
    myChannelCount(myId < ID_3 ? 4 : 6),
    myIsConfigured(false),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &TimerNRF5340::interruptHandler)),
    myCompareInterruptCallbacks(),
    myTimer(myTimerMap[id]),
    myInputClockFrequencyHz(0.0),
    myOutputClockFrequencyHz(0.0),
    myConfig()
{
    myCompareInterruptCallbacks[CHANNEL_0] = &compareInterruptCallback;

    initialize();
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerNRF5340::~TimerNRF5340()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerNRF5340::Id TimerNRF5340::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
NRF_TIMER_Type* TimerNRF5340::getTimer()
{
    return myTimer;
}

//------------------------------------------------------------------------------
InterruptNRF5340& TimerNRF5340::getInterrupt()
{
    return myInterrupt;
}

//------------------------------------------------------------------------------
void TimerNRF5340::setCompareChannelInterruptCallback(
                             const Channel Channel,
                             CompareInterruptCallback& compareInterruptCallback)
{
    myCompareInterruptCallbacks[Channel] = &compareInterruptCallback;
}

//------------------------------------------------------------------------------
TimerNRF5340::Error TimerNRF5340::setConfig(const Config& config)
{
    triggerTask(TASK_STOP);

    uint32_t prescaler = 1;
    uint32_t prescalerExponent = 0;
    uint32_t period = 1;
    FrequencyHz outputClockFrequencyHz;

    // Timer input clock frequency is always 16 MHz per the datasheet
    myInputClockFrequencyHz = 16000000.0;

    switch (config.resolution)
    {
        case RESOLUTION_MIN:
        {
            // Maximize prescaler, minimize counter compare value

            myBitMode = BIT_MODE_8;

            prescaler =
                (uint32_t) round(myInputClockFrequencyHz / config.frequencyHz);

            prescalerExponent = (uint32_t) round(log(prescaler) / log(2));

            // All timers have a 4-bit prescaler register
            if (prescalerExponent > 15)
            {
                prescalerExponent = 15;
            }

            prescaler = pow(2, prescalerExponent);

            outputClockFrequencyHz =
                         myInputClockFrequencyHz / (float) (prescaler * period);

            float errorPercent =
                ((outputClockFrequencyHz -
                              config.frequencyHz) / config.frequencyHz) * 100.0;

            while (abs(errorPercent) > config.maxFrequencyErrorPercent)
            {
                if (errorPercent > 0.0)
                {
                    float ratio = (outputClockFrequencyHz / config.frequencyHz);
                    period = (uint32_t) round((float) period * ratio);
                }

                prescaler =
                    (uint32_t) round(myInputClockFrequencyHz /
                                         (config.frequencyHz * (float) period));

                prescalerExponent = (uint32_t) round(log(prescaler) / log(2));

                // All timers have a 4-bit prescaler register
                if (prescalerExponent > 15)
                {
                    prescalerExponent = 15;
                }

                prescaler = pow(2, prescalerExponent);

                outputClockFrequencyHz =
                         myInputClockFrequencyHz / (float) (prescaler * period);

                errorPercent =
                    ((outputClockFrequencyHz -
                            config.frequencyHz) / config.frequencyHz) * 100.0;
            }

            break;
        }
        case RESOLUTION_MAX:
        {
            // Minimize prescaler, maximize counter compare value

            myBitMode = BIT_MODE_32;

            period =
                 (uint32_t) round(myInputClockFrequencyHz / config.frequencyHz);

            if (period > numeric_limits<uint32_t>::max())
            {
                period = numeric_limits<uint32_t>::max();
            }

            outputClockFrequencyHz =
                       (myInputClockFrequencyHz / (float) (prescaler * period));

            float errorPercent =
                ((outputClockFrequencyHz -
                              config.frequencyHz) / config.frequencyHz) * 100.0;

            if (abs(errorPercent) > config.maxFrequencyErrorPercent)
            {
                float ratio = (outputClockFrequencyHz / config.frequencyHz);
                prescaler = (uint32_t) ceil(((float) pow(2, prescalerExponent)) * ratio);
            }

            while (abs(errorPercent) > config.maxFrequencyErrorPercent)
            {
                prescalerExponent++;
                prescaler = pow(2, prescalerExponent);

                period =
                    (uint32_t) round(myInputClockFrequencyHz /
                                              (prescaler * config.frequencyHz));

                if (period > numeric_limits<uint32_t>::max())
                {
                    period = numeric_limits<uint32_t>::max();
                }

                outputClockFrequencyHz =
                       (myInputClockFrequencyHz / (float) (prescaler * period));

                errorPercent =
                    ((outputClockFrequencyHz -
                              config.frequencyHz) / config.frequencyHz) * 100.0;
            }

            break;
        }
        case RESOLUTION_LEVELS:
        {
            // Optimize BitMode?
            myBitMode = BIT_MODE_32;

            prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                         (config.frequencyHz *
                                          (float) config.resolutionLevels));

            prescalerExponent = (uint32_t) round(log(prescaler) / log(2));

            // All timers have a 16-bit prescaler register
            if (prescalerExponent > 15)
            {
                prescalerExponent = 15;
            }

            prescaler = pow(2, prescalerExponent);

            outputClockFrequencyHz = (myInputClockFrequencyHz /
                                      (float) (prescaler * period));

            float errorPercent = ((outputClockFrequencyHz -
                                   config.frequencyHz)  /
                                  config.frequencyHz)           *
                                 100.0;

            while (abs(errorPercent) > config.maxFrequencyErrorPercent)
            {
                if (errorPercent > 0.0)
                {
                    float ratio = (outputClockFrequencyHz / config.frequencyHz);
                    period = (uint32_t) round((float) period * ratio);
                }

                prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                             (config.frequencyHz *
                                              (float) period));

                prescalerExponent = (uint32_t) round(log(prescaler) / log(2));

                if (prescalerExponent > 15)
                {
                    prescalerExponent = 15;
                }

                prescaler = pow(2, prescalerExponent);

                outputClockFrequencyHz = (myInputClockFrequencyHz /
                                          (float) (prescaler * period));

                errorPercent = ((outputClockFrequencyHz - config.frequencyHz) /
                                config.frequencyHz) *
                                100.0;
            }

            break;
        }
    }

    myOutputClockFrequencyHz = outputClockFrequencyHz;

    setMode(config.mode);
    setPrescaler(prescalerExponent);
    setBitMode(myBitMode);
    setChannelCaptureCompareValue(CHANNEL_0, period);
    setInterruptEventEnabled(INTERRUPT_EVENT_COMPARE_0, true);

    triggerTask(TASK_START);

    myConfig = config;

    myIsConfigured = true;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void TimerNRF5340::setChannelCaptureCompareValue(const Channel channel,
                                                  const uint32_t value)
{
    myTimer->CC[channel] = value;
}

//------------------------------------------------------------------------------
uint32_t TimerNRF5340::getChannelCaptureCompareValue(const Channel channel)
{
    return (myTimer->CC[channel]);
}

//------------------------------------------------------------------------------
// Private virtual methods overridden for Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error TimerNRF5340::driverSetEnabled(const bool enabled)
{
    if (enabled)
    {
        InterruptNRF5340::Config config;
        config.priority = 7;

        myInterrupt.configure(config);

        if (myIsConfigured)
        {
            triggerTask(TASK_START);
        }
    }
    else
    {
        triggerTask(TASK_STOP);
    }

    myInterrupt.setEnabled(enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimerNRF5340::initialize()
{
    if (isValidPointer(interruptObjectMap[myId]))
    {
        // Instantiating same peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;
}

//------------------------------------------------------------------------------
void TimerNRF5340::setMode(const Mode mode)
{
    myTimer->MODE = (uint32_t) mode;
}

//------------------------------------------------------------------------------
void TimerNRF5340::setPrescaler(const uint32_t prescaler)
{
    myTimer->PRESCALER = prescaler;
}

//------------------------------------------------------------------------------
void TimerNRF5340::setBitMode(const BitMode bitMode)
{
    myTimer->BITMODE = (uint32_t) bitMode;
}

//------------------------------------------------------------------------------
void TimerNRF5340::setInterruptEventEnabled(
                                            const InterruptEvent interruptEvent,
                                            const bool enabled)
{
    if (enabled)
    {
        setBitSet(myTimer->INTENSET, interruptEvent + 16, enabled);
    }
    else
    {
        setBitSet(myTimer->INTENCLR, interruptEvent + 16, enabled);
    }
}

//------------------------------------------------------------------------------
bool TimerNRF5340::isInterruptEventEnabled(const InterruptEvent interruptEvent)
{
    return isBitSet(myTimer->INTENSET, interruptEvent + 16);
}

//------------------------------------------------------------------------------
bool TimerNRF5340::isInterruptEventPending(const InterruptEvent interruptEvent)
{
    return (myTimer->EVENTS_COMPARE[interruptEvent] != 0);
}

//------------------------------------------------------------------------------
void TimerNRF5340::clearPendingInterruptEvent(
                                            const InterruptEvent interruptEvent)
{
    myTimer->EVENTS_COMPARE[interruptEvent] = 0;
}

//------------------------------------------------------------------------------
void TimerNRF5340::triggerTask(const Task task)
{
    switch (task)
    {
        case TASK_START:
        {
            myTimer->TASKS_START = 1;

            break;
        }
        case TASK_STOP:
        {
            myTimer->TASKS_STOP = 1;

            break;
        }
        case TASK_COUNT:
        {
            myTimer->TASKS_COUNT = 1;

            break;
        }
        case TASK_CLEAR:
        {
            myTimer->TASKS_CLEAR = 1;
            
            break;
        }
        case TASK_CAPTURE_0:
        {
            myTimer->TASKS_CAPTURE[0] = 1;
            
            break;
        }
        case TASK_CAPTURE_1:
        {
            myTimer->TASKS_CAPTURE[1] = 1;
            
            break;
        }
        case TASK_CAPTURE_2:
        {
            myTimer->TASKS_CAPTURE[2] = 1;
            
            break;
        }
        case TASK_CAPTURE_3:
        {
            myTimer->TASKS_CAPTURE[3] = 1;
            
            break;
        }
        case TASK_CAPTURE_4:
        {
            myTimer->TASKS_CAPTURE[4] = 1;
            
            break;
        }
        case TASK_CAPTURE_5:
        {
            myTimer->TASKS_CAPTURE[5] = 1;
            
            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void TimerNRF5340::interruptHandler()
{
    int i = 0;

    while (i < myChannelCount)
    {
        InterruptEvent interruptEvent = (InterruptEvent) i;

        if (isInterruptEventPending(interruptEvent))
        {
            if (isInterruptEventEnabled(interruptEvent))
            {
                myCompareInterruptCallbacks[i]->call();
            }
            
            clearPendingInterruptEvent(interruptEvent);

            triggerTask(TASK_CLEAR); // TODO: Connect this via PPI
        }

        i++;
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void TIMER0_IRQHandler(void)
{
    interruptObjectMap[TimerNRF5340::ID_0]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIMER1_IRQHandler(void)
{
    interruptObjectMap[TimerNRF5340::ID_1]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIMER2_IRQHandler(void)
{
    interruptObjectMap[TimerNRF5340::ID_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIMER3_IRQHandler(void)
{
    interruptObjectMap[TimerNRF5340::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIMER4_IRQHandler(void)
{
    interruptObjectMap[TimerNRF5340::ID_4]->handler();
}
