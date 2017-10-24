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
// Copyright (c) 2017 Benjamin Minerd
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
/// @file TimerSTM32F30x.cpp
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief TimerSTM32F30x class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <TimerSTM32F30x.h>
#include <CallbackMethod.h>

using Plat4m::TimerSTM32F30x;
using Plat4m::Module;
using Plat4m::GpioPinSTM32F30x;
using Plat4m::InterruptSTM32F30x;
using Plat4m::ProcessorSTM32F30x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F30x* interruptObjectMap[17];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ProcessorSTM32F30x::Peripheral TimerSTM32F30x::myPeripheralMap[] =
{
    ProcessorSTM32F30x::PERIPHERAL_TIM_1,  /// ID_1
    ProcessorSTM32F30x::PERIPHERAL_TIM_2,  /// ID_2
    ProcessorSTM32F30x::PERIPHERAL_TIM_3,  /// ID_3
    ProcessorSTM32F30x::PERIPHERAL_TIM_4,  /// ID_4
    ProcessorSTM32F30x::PERIPHERAL_TIM_6,  /// ID_6
    ProcessorSTM32F30x::PERIPHERAL_TIM_7,  /// ID_7
    ProcessorSTM32F30x::PERIPHERAL_TIM_8,  /// ID_8
    ProcessorSTM32F30x::PERIPHERAL_TIM_15, /// ID_15
    ProcessorSTM32F30x::PERIPHERAL_TIM_16, /// ID_16
    ProcessorSTM32F30x::PERIPHERAL_TIM_17, /// ID_17
    ProcessorSTM32F30x::PERIPHERAL_TIM_20  /// ID_20
};

const uint16_t TimerSTM32F30x::myCounterModeMap[] =
{
    TIM_CounterMode_Up,   /// MODE_COUNTING_UP
    TIM_CounterMode_Down, /// MODE_COUNTING_DOWN
    TIM_CounterMode_Up,   /// MODE_COUNTING_UP_DOWN
    TIM_CounterMode_Up,   /// MODE_INPUT_CAPTURE
    TIM_CounterMode_Up,   /// MODE_OUTPUT_COMPARE
    TIM_CounterMode_Up,   /// MODE_PWM_INPUT
    TIM_CounterMode_Up,   /// MODE_PWM_OUTPUT
    TIM_CounterMode_Up,   /// MODE_PULSE
};

const uint16_t TimerSTM32F30x::myPolarityMap[] =
{
    TIM_OCPolarity_High, /// POLARITY_HIGH
    TIM_OCPolarity_Low   /// POLARITY_LOW
};

const InterruptSTM32F30x::Id TimerSTM32F30x::myInterruptIdMap[] =
{
    InterruptSTM32F30x::ID_TIM_1_BRK_TIM_15,     /// INTERRUPT_TIM_1_BRK_TIM_15
    InterruptSTM32F30x::ID_TIM_1_UP_TIM_16,      /// INTERRUPT_TIM_1_UP_TIM_16
    InterruptSTM32F30x::ID_TIM_1_TRG_COM_TIM_17, /// INTERRUPT_TIM_1_TRG_COM_TIM_17
    InterruptSTM32F30x::ID_TIM_1_CC,             /// INTERRUPT_TIM_1_CC
    InterruptSTM32F30x::ID_TIM_2,                /// INTERRUPT_TIM_2
    InterruptSTM32F30x::ID_TIM_3,                /// INTERRUPT_TIM_3
    InterruptSTM32F30x::ID_TIM_4,                /// INTERRUPT_TIM_4
    InterruptSTM32F30x::ID_TIM_6_DAC,            /// INTERRUPT_TIM_6_DAC
    InterruptSTM32F30x::ID_TIM_7,                /// INTERRUPT_TIM_7
    InterruptSTM32F30x::ID_TIM_8_BRK,            /// INTERRUPT_TIM_8_BRK
    InterruptSTM32F30x::ID_TIM_8_UP,             /// INTERRUPT_TIM_8_UP
    InterruptSTM32F30x::ID_TIM_8_TRG_COM,        /// INTERRUPT_TIM_8_TRG_COM
    InterruptSTM32F30x::ID_TIM_8_CC,             /// INTERRUPT_TIM_8_CC
    InterruptSTM32F30x::ID_TIM_20_BRK,           /// INTERRUPT_TIM_20_BRK
    InterruptSTM32F30x::ID_TIM_20_UP,            /// INTERRUPT_TIM_20_UP
    InterruptSTM32F30x::ID_TIM_20_TRG_COM,       /// INTERRUPT_TIM_20_TRG_COM
    InterruptSTM32F30x::ID_TIM_20_CC             /// INTERRUPT_TIM_20_CC
};

const uint32_t TimerSTM32F30x::myInterruptEventMap[] =
{
    TIM_SR_UIF,   /// INTERRUPT_EVENT_UPDATE
    TIM_SR_CC1IF, /// INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1
    TIM_SR_CC2IF, /// INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2
    TIM_SR_CC3IF, /// INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3
    TIM_SR_CC4IF, /// INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4
    TIM_SR_COMIF, /// INTERRUPT_EVENT_COMMUTATION
    TIM_SR_TIF,   /// INTERRUPT_EVENT_TRIGGER
    TIM_SR_BIF,   /// INTERRUPT_EVENT_BREAK
    TIM_SR_CC1OF, /// INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1
    TIM_SR_CC2OF, /// INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2
    TIM_SR_CC3OF, /// INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3
    TIM_SR_CC4OF  /// INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4
};

TIM_TypeDef* TimerSTM32F30x::myTimerMap[] =
{
    TIM1,  /// TimerSTM32F30x::ID_1
    TIM2,  /// TimerSTM32F30x::ID_2
    TIM3,  /// TimerSTM32F30x::ID_3
    TIM4,  /// TimerSTM32F30x::ID_4
    TIM6,  /// TimerSTM32F30x::ID_6
    TIM7,  /// TimerSTM32F30x::ID_7
    TIM8,  /// TimerSTM32F30x::ID_8
    TIM15, /// TimerSTM32F30x::ID_15
    TIM16, /// TimerSTM32F30x::ID_16
    TIM17, /// TimerSTM32F30x::ID_17
    TIM20  /// TimerSTM32F30x::ID_20
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F30x::TimerSTM32F30x(const Id id) :
    myId(id),
    myUpdateInterruptCallback(0),
    myBreakInterruptCallback(0),
    myCaptureCompareInterruptCallbacks(),
    myTriggerInterruptCallback(0),
    myCommutationInterruptCallback(0),
    myTimer(myTimerMap[id]),
    myGlobalInterrupt(0),
    myUpdateInterrupt(0),
    myBreakInterrupt(0),
    myCaptureCompareInterrupt(0),
    myTriggerCommutationInterrupt(0),
    myInputClockFrequencyHz(0.0),
    myOutputClockFrequencyHz(0.0),
    myPeriod(0),
    myConfig()
{
    initialize();
}

//------------------------------------------------------------------------------
TimerSTM32F30x::TimerSTM32F30x(
                             const Id id,
                             UpdateInterruptCallback& updateInterruptCallback) :
    myId(id),
    myUpdateInterruptCallback(&updateInterruptCallback),
    myBreakInterruptCallback(0),
    myCaptureCompareInterruptCallbacks(),
    myTriggerInterruptCallback(0),
    myCommutationInterruptCallback(0),
    myTimer(myTimerMap[id]),
    myGlobalInterrupt(0),
    myUpdateInterrupt(0),
    myBreakInterrupt(0),
    myCaptureCompareInterrupt(0),
    myTriggerCommutationInterrupt(0),
    myInputClockFrequencyHz(0.0),
    myOutputClockFrequencyHz(0.0),
    myPeriod(0),
    myConfig()
{
    initialize();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F30x::Id TimerSTM32F30x::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
TIM_TypeDef* TimerSTM32F30x::getTimer()
{
    return myTimer;
}

//------------------------------------------------------------------------------
InterruptSTM32F30x* TimerSTM32F30x::getGlobalInterrupt()
{
	return myGlobalInterrupt;
}

//------------------------------------------------------------------------------
InterruptSTM32F30x* TimerSTM32F30x::getUpdateInterrupt()
{
	return myUpdateInterrupt;
}

//------------------------------------------------------------------------------
InterruptSTM32F30x* TimerSTM32F30x::getBreakInterrupt()
{
	return myBreakInterrupt;
}

//------------------------------------------------------------------------------
InterruptSTM32F30x* TimerSTM32F30x::getCaptureCompareInterrupt()
{
	return myCaptureCompareInterrupt;
}

//------------------------------------------------------------------------------
InterruptSTM32F30x* TimerSTM32F30x::getTriggerCommutationInterrupt()
{
	return myTriggerCommutationInterrupt;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setUpdateInterruptCallback(
                               UpdateInterruptCallback& updateInterruptCallback)
{
    myUpdateInterruptCallback = &updateInterruptCallback;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setBreakInterruptCallback(
                                 BreakInterruptCallback& breakInterruptCallback)
{
    myBreakInterruptCallback = &breakInterruptCallback;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelCaptureCompareInterruptCallback(
               const ChannelId channelId,
               CaptureCompareInterruptCallback& captureCompareInterruptCallback)
{
    myCaptureCompareInterruptCallbacks[channelId] =
                                               &captureCompareInterruptCallback;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setTriggerInterruptCallback(
                             TriggerInterruptCallback& triggerInterruptCallback)
{
    myTriggerInterruptCallback = &triggerInterruptCallback;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setCommutationInterruptCallback(
                     CommutationInterruptCallback& commutationInterruptCallback)
{
    myCommutationInterruptCallback = &commutationInterruptCallback;
}

//------------------------------------------------------------------------------
TimerSTM32F30x::Error TimerSTM32F30x::setConfig(const Config& config)
{
    // TODO: Incorporate clock division (for reeeeally slow timers)

    uint32_t prescaler = 1;
    uint32_t period = 1;
    float outputClockFrequencyHz;

    myInputClockFrequencyHz =
        ProcessorSTM32F30x::getPeripheralInputClockFrequencyHz(
                                                         myPeripheralMap[myId]);

    switch (config.resolution)
    {
        case RESOLUTION_MIN:
        {
            prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                         config.frequencyHz);

            // All timers have a 16-bit prescaler register
            if (prescaler > 65536)
            {
                prescaler = 65536;
            }

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

                // All timers have a 16-bit prescaler register
                if (prescaler > 65536)
                {
                    prescaler = 65536;
                }

                outputClockFrequencyHz = (myInputClockFrequencyHz /
                                          (float) (prescaler * period));

                errorPercent = ((outputClockFrequencyHz - config.frequencyHz) /
                                config.frequencyHz) *
                                100.0;
            }

            break;
        }
        case RESOLUTION_MAX:
        {
            period = (uint32_t) round(myInputClockFrequencyHz /
                                      config.frequencyHz);

            // All timers except TIM2 have a 16-bit counter register
            if ((myId != ID_2) && (period > 65536))
            {
                period = 65536;
            }

            outputClockFrequencyHz = (myInputClockFrequencyHz /
                                     (float) (prescaler * period));

            float errorPercent = ((outputClockFrequencyHz -
                                   config.frequencyHz) /
                                  config.frequencyHz) *
                                 100.0;

            if (abs(errorPercent) > config.maxFrequencyErrorPercent)
            {
                float ratio = (outputClockFrequencyHz / config.frequencyHz);
                prescaler = (uint32_t) ceil((float) prescaler * ratio);
            }

            while (abs(errorPercent) > config.maxFrequencyErrorPercent)
            {
                prescaler++;

                period = (uint32_t) round(myInputClockFrequencyHz /
                                          (prescaler * config.frequencyHz));

                // All timers except TIM2 have a 16-bit counter register
                if ((myId != ID_2) && (period > 65536))
                {
                    period = 65536;
                }

                outputClockFrequencyHz = (myInputClockFrequencyHz /
                                          (float) (prescaler * period));

                errorPercent = ((outputClockFrequencyHz - config.frequencyHz) /
                                config.frequencyHz) *
                               100.0;
            }

            break;
        }
        case RESOLUTION_LEVELS:
        {
            prescaler = (uint32_t) round(myInputClockFrequencyHz /
                                         (config.frequencyHz *
                                          (float) config.resolutionLevels));

            // All timers have a 16-bit prescaler register
            if (prescaler > 65536)
            {
                prescaler = 65536;
            }

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

                // All timers have a 16-bit counter register
                if (prescaler > 65536)
                {
                    prescaler = 65536;
                }

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
    myPeriod = period;

    setPrescaler(prescaler);
    setPeriod(period);
    setClockDivision(0);
    setDirection(config.direction);
    setAlignment(config.alignment);
    setAutoReloadPreloadEnabled(config.autoReloadPreloadEnabled);

    if (config.mode == MODE_PWM_OUTPUT)
    {
        setMainOutputEnabled(true);
//        setCaptureComparePreloadEnabled(true);

        // TODO Make this config parameter
//        setBits(myTimer->BDTR, 0x82);
//        setBits(myTimer->BDTR, 36);
        setBits(myTimer->BDTR, 72);
    }

    setRepetitionCount(config.repetitionCount);

    generateUpdateEvent();

    myConfig = config;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t TimerSTM32F30x::getPeriod() const
{
    return myPeriod;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelCaptureCompareEnabled(const ChannelId channelId,
                                                     const bool enabled)
{
    // TODO Handle channels > 4

    if (channelId < CHANNEL_ID_4)
    {
        uint32_t offset = (channelId * 4);

        setBitsSet(myTimer->CCER, ((uint32_t) 0x1) << offset, enabled);
    }
    else if (channelId == CHANNEL_ID_4)
    {
        setBitsSet(myTimer->CCER, ((uint32_t) 0x1) << 12, enabled);
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setComplementaryChannelCaptureCompareEnabled(
                            const ComplementaryChannelId complementaryChannelId,
                            const bool enabled)
{
    // TODO Handle channels > 3

    uint32_t offset = (complementaryChannelId * 4) + 2;

    setBitsSet(myTimer->CCER, ((uint32_t) 0x1) << offset, enabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelCaptureCompareMode(
                                    const ChannelId channelId,
                                    const CaptureCompareMode captureCompareMode)
{
    switch (channelId)
    {
        case CHANNEL_ID_1:
        {
            clearAndSetBits(myTimer->CCMR1,
                            TIM_CCMR1_CC1S,
                            (uint32_t) captureCompareMode);

            break;
        }
        case CHANNEL_ID_2:
        {
            clearAndSetBits(myTimer->CCMR1,
                            TIM_CCMR1_CC2S,
                            ((uint32_t) captureCompareMode) << 8);

            break;
        }
        case CHANNEL_ID_3:
        {
            clearAndSetBits(myTimer->CCMR2,
                            TIM_CCMR2_CC3S,
                            (uint32_t) captureCompareMode);

            break;
        }
        case CHANNEL_ID_4:
        {
            clearAndSetBits(myTimer->CCMR2,
                            TIM_CCMR2_CC4S,
                            ((uint32_t) captureCompareMode) << 8);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelOutputCompareMode(
                                      const ChannelId channelId,
                                      const OutputCompareMode outputCompareMode)
{
    switch (channelId)
    {
        case CHANNEL_ID_1:
        {
            clearBits(myTimer->CCMR1, TIM_CCMR1_OC1M);

            uint32_t mask;

            // Bits[2:0]
            mask = TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
            setBits(myTimer->CCMR1,
                    (((uint32_t) outputCompareMode) << 4) & mask);

            // Bits[3]
            mask = TIM_CCMR1_OC1M_3;
            setBits(myTimer->CCMR1,
                    (((uint32_t) outputCompareMode) << 13) & mask);

            break;
        }
        case CHANNEL_ID_2:
        {
            clearBits(myTimer->CCMR1, TIM_CCMR1_OC2M);

            uint32_t mask;

            // Bits[2:0]
            mask = TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
            setBits(myTimer->CCMR1,
                    (((uint32_t) outputCompareMode) << 12) & mask);

            // Bits[3]
            mask = TIM_CCMR1_OC1M_3;
            setBits(myTimer->CCMR1,
                    (((uint32_t) outputCompareMode) << 21) & mask);

            break;
        }
        case CHANNEL_ID_3:
        {
            clearBits(myTimer->CCMR2, TIM_CCMR2_OC3M);

            uint32_t mask;

            // Bits[2:0]
            mask = TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
            setBits(myTimer->CCMR2,
                    (((uint32_t) outputCompareMode) << 4) & mask);

            // Bits[3]
            mask = TIM_CCMR2_OC3M_3;
            setBits(myTimer->CCMR2,
                    (((uint32_t) outputCompareMode) << 13) & mask);

            break;
        }
        case CHANNEL_ID_4:
        {
            clearBits(myTimer->CCMR2, TIM_CCMR2_OC4M);

            uint32_t mask;

            // Bits[2:0]
            mask = TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
            setBits(myTimer->CCMR2,
                    (((uint32_t) outputCompareMode) << 12) & mask);

            // Bits[3]
            mask = TIM_CCMR2_OC4M_3;
            setBits(myTimer->CCMR2,
                    (((uint32_t) outputCompareMode) << 21) & mask);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelOutputComparePreloadEnabled(
                                                      const ChannelId channelId,
                                                      const bool enabled)
{
    switch (channelId)
    {
        case CHANNEL_ID_1:
        {
            setBitsSet(myTimer->CCMR1, TIM_CCMR1_OC1PE, enabled);

            break;
        }
        case CHANNEL_ID_2:
        {
            setBitsSet(myTimer->CCMR1, TIM_CCMR1_OC2PE, enabled);

            break;
        }
        case CHANNEL_ID_3:
        {
            setBitsSet(myTimer->CCMR2, TIM_CCMR2_OC3PE, enabled);

            break;
        }
        case CHANNEL_ID_4:
        {
            setBitsSet(myTimer->CCMR2, TIM_CCMR2_OC4PE, enabled);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelPolarity(const ChannelId channelId,
                                        const Polarity polarity)
{
    // TODO Handle channels > 3

    uint32_t offset = (channelId * 4) + 1;

    clearAndSetBits(myTimer->CCER,
                    ((uint32_t) 0x1) << offset,
                    ((uint32_t) polarity) << offset);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setComplementaryChannelPolarity(
                            const ComplementaryChannelId complementaryChannelId,
                            const Polarity polarity)
{
    // TODO Handle channels > 3

    uint32_t offset = (complementaryChannelId * 4) + 3;

    clearAndSetBits(myTimer->CCER,
                    ((uint32_t) 0x1) << offset,
                    ((uint32_t) polarity) << offset);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelCaptureCompareValue(const ChannelId channelId,
                                                   const uint32_t value)
{
    switch (channelId)
    {
        case CHANNEL_ID_1:
        {
            myTimer->CCR1 = value;

            break;
        }
        case CHANNEL_ID_2:
        {
            myTimer->CCR2 = value;

            break;
        }
        case CHANNEL_ID_3:
        {
            myTimer->CCR3 = value;

            break;
        }
        case CHANNEL_ID_4:
        {
            myTimer->CCR4 = value;

            break;
        }
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::generateUpdateEvent()
{
    setBits(myTimer->EGR, TIM_EGR_UG);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::generateCaptureCompareControlUpdate()
{
    setBits(myTimer->EGR, TIM_EGR_COMG);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setInterruptEventEnabled(
                                            const InterruptEvent interruptEvent,
                                            const bool enabled)
{
    setBitsSet(myTimer->DIER, ((uint32_t) 1) << interruptEvent, enabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setCounterEnabled(const bool enabled)
{
    setBitsSet(myTimer->CR1, TIM_CR1_CEN, enabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setMasterMode(const MasterMode masterMode)
{
    clearAndSetBits(myTimer->CR2, TIM_CR2_MMS, ((uint32_t) masterMode) << 4);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setSlaveMode(const SlaveMode slaveMode)
{
    clearBits(myTimer->SMCR, TIM_SMCR_SMS);

    if (slaveMode == SLAVE_MODE_RESET_AND_TRIGGER)
    {
        setBits(myTimer->SMCR, TIM_SMCR_SMS_3);
    }
    else
    {
        setBits(myTimer->SMCR, (uint32_t) slaveMode);
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setInputSelection(const InputSelection inputSelection)
{
    clearAndSetBits(myTimer->CR2,
                    TIM_CR2_TI1S,
                    ((uint32_t) inputSelection) << 7);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setMainOutputEnabled(const bool enabled)
{
    setBitsSet(myTimer->BDTR, TIM_BDTR_MOE, enabled);
}

// TODO: Put these methods in same order as header file

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelOutputCompareConfig(
                                  const ChannelId channelId,
                                  const OutputCompareConfig outputCompareConfig)
{
    // Configure as output compare
    setChannelCaptureCompareSelection(channelId,
                                      CAPTURE_COMPARE_SELECTION_OUTPUT);

    setChannelOutputCompareMode(channelId, outputCompareConfig.mode);
    setChannelPolarity(channelId, outputCompareConfig.polarity);
    setChannelOutputComparePreloadEnabled(channelId,
                                          outputCompareConfig.preloadEnabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setTriggerSelection(
                                        const TriggerSelection triggerSelection)
{
    clearAndSetBits(myTimer->SMCR,
                    TIM_SMCR_TS,
                    ((uint32_t) triggerSelection) << 4);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setRepetitionCount(const uint8_t repetitionCount)
{
    myTimer->RCR = (uint16_t) repetitionCount;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelInputCaptureConfig(
                                    const ChannelId channelId,
                                    const InputCaptureConfig inputCaptureConfig)
{
    setChannelCaptureCompareSelection(
                        channelId,
                        (CaptureCompareSelection) inputCaptureConfig.selection);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setChannelCaptureCompareSelection(
                          const ChannelId channelId,
                          const CaptureCompareSelection captureCompareSelection)
{
    switch (channelId)
    {
        case CHANNEL_ID_1:
        {
            clearAndSetBits(myTimer->CCMR1,
                            TIM_CCMR1_CC1S,
                            (uint32_t) captureCompareSelection);

            break;
        }
        case CHANNEL_ID_2:
        {
            clearAndSetBits(myTimer->CCMR1,
                            TIM_CCMR1_CC2S,
                            ((uint32_t) captureCompareSelection) << 8);

            break;
        }
        case CHANNEL_ID_3:
        {
            clearAndSetBits(myTimer->CCMR2,
                            TIM_CCMR2_CC3S,
                            (uint32_t) captureCompareSelection);

            break;
        }
        case CHANNEL_ID_4:
        {
            clearAndSetBits(myTimer->CCMR2,
                            TIM_CCMR2_CC4S,
                            ((uint32_t) captureCompareSelection) << 8);

            break;
        }
    }
}

//------------------------------------------------------------------------------
uint32_t TimerSTM32F30x::getChannelCaptureCompareValue(
                                                      const ChannelId channelId)
{
    uint32_t value;

    switch (channelId)
    {
        case CHANNEL_ID_1:
        {
            value = myTimer->CCR1;

            break;
        }
        case CHANNEL_ID_2:
        {
            value = myTimer->CCR2;

            break;
        }
        case CHANNEL_ID_3:
        {
            value = myTimer->CCR3;

            break;
        }
        case CHANNEL_ID_4:
        {
            value = myTimer->CCR4;

            break;
        }
    }

    return value;
}

//------------------------------------------------------------------------------
uint32_t TimerSTM32F30x::getCounterValue()
{
    return (myTimer->CNT);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error TimerSTM32F30x::driverSetEnabled(const bool enabled)
{

    ProcessorSTM32F30x::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    InterruptSTM32F30x::Config config;
    config.priority = 7;

    if (isValidPointer(myGlobalInterrupt))
    {
        myGlobalInterrupt->configure(config);
        myGlobalInterrupt->setEnabled(enabled);
    }

    if (isValidPointer(myBreakInterrupt))
    {
        myBreakInterrupt->configure(config);
        myBreakInterrupt->setEnabled(enabled);
    }

    if (isValidPointer(myUpdateInterrupt))
    {
//    	config.priority = 5;
        myUpdateInterrupt->configure(config);
        myUpdateInterrupt->setEnabled(enabled);
//        config.priority = 7;
    }

    if (isValidPointer(myCaptureCompareInterrupt))
    {
        myCaptureCompareInterrupt->configure(config);
        myCaptureCompareInterrupt->setEnabled(enabled);
    }

    if (isValidPointer(myTriggerCommutationInterrupt))
    {
        myTriggerCommutationInterrupt->configure(config);
        myTriggerCommutationInterrupt->setEnabled(enabled);
    }

    if (isValidPointer(myUpdateInterruptCallback))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_UPDATE, enabled);
    }

    if (isValidPointer(myBreakInterruptCallback))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_BREAK, enabled);
    }

    if (isValidPointer(myCaptureCompareInterruptCallbacks[CHANNEL_ID_1]))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1,
                                 enabled);
    }

    if (isValidPointer(myCaptureCompareInterruptCallbacks[CHANNEL_ID_2]))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2,
                                 enabled);
    }

    if (isValidPointer(myCaptureCompareInterruptCallbacks[CHANNEL_ID_3]))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3,
                                 enabled);
    }

    if (isValidPointer(myCaptureCompareInterruptCallbacks[CHANNEL_ID_4]))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4,
                                 enabled);
    }

    if (isValidPointer(myTriggerInterruptCallback))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_TRIGGER, enabled);
    }

    if (isValidPointer(myCommutationInterruptCallback))
    {
        setInterruptEventEnabled(INTERRUPT_EVENT_COMMUTATION, enabled);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimerSTM32F30x::setPrescaler(const uint32_t prescaler)
{
    myTimer->PSC = prescaler - 1;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setClockDivision(const uint32_t clockDivision)
{
    // Empty for now
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setPeriod(const uint32_t period)
{
    myTimer->ARR = period - 1;
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setDirection(const Direction direction)
{
    clearAndSetBits(myTimer->CR1,
                    TIM_CR1_DIR,
                    ((uint32_t) direction) << 4);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setAlignment(const Alignment alignment)
{
    clearAndSetBits(myTimer->CR1,
                    TIM_CR1_CMS,
                    ((uint32_t) alignment) << 5);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setAutoReloadPreloadEnabled(const bool enabled)
{
    setBitsSet(myTimer->CR1, TIM_CR1_ARPE, enabled);
}

//------------------------------------------------------------------------------
bool TimerSTM32F30x::isInterruptEventEnabled(
                                            const InterruptEvent interruptEvent)
{
    if (interruptEvent >= INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1)
    {
        return areBitsSet(myTimer->DIER, myInterruptEventMap[interruptEvent - 7]);
    }

    return areBitsSet(myTimer->DIER, myInterruptEventMap[interruptEvent]);
}

//------------------------------------------------------------------------------
bool TimerSTM32F30x::isInterruptEventPending(
                                            const InterruptEvent interruptEvent)
{
    return areBitsSet(myTimer->SR, myInterruptEventMap[interruptEvent]);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::clearPendingInterruptEvent(
                                            const InterruptEvent interruptEvent)
{
    clearBits(myTimer->SR, myInterruptEventMap[interruptEvent]);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::setCaptureComparePreloadEnabled(const bool enabled)
{
    setBitsSet(myTimer->CR2, TIM_CR2_CCPC, enabled);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimerSTM32F30x::initialize()
{
    switch (myId)
    {
        case ID_1:
        {
            myUpdateInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_1_UP_TIM_16,
                    createCallback(
                                 this,
                                 &TimerSTM32F30x::updateInterruptEventHandler));
            myBreakInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_1_BRK_TIM_15,
                    createCallback(
                                  this,
                                  &TimerSTM32F30x::breakInterruptEventHandler));
            myCaptureCompareInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_1_CC,
                    createCallback(
                              this,
                              &TimerSTM32F30x::captureCompareInterruptHandler));
            myTriggerCommutationInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_1_TRG_COM_TIM_17,
                    createCallback(
                          this,
                          &TimerSTM32F30x::triggerCommutationInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_1_UP_TIM_16]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_1_UP_TIM_16] = myUpdateInterrupt;
            interruptObjectMap[INTERRUPT_TIM_1_BRK_TIM_15] = myBreakInterrupt;
            interruptObjectMap[INTERRUPT_TIM_1_CC]  = myCaptureCompareInterrupt;
            interruptObjectMap[INTERRUPT_TIM_1_TRG_COM_TIM_17] =
                                                  myTriggerCommutationInterrupt;

            break;
        }
        case ID_2:
        {
            myGlobalInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_2,
                    createCallback(
                         this,
                         &TimerSTM32F30x::generalPurposeTimerInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_2]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_2] = myGlobalInterrupt;

            break;
        }
        case ID_3:
        {
            myGlobalInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_3,
                    createCallback(
                         this,
                         &TimerSTM32F30x::generalPurposeTimerInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_3]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_3] = myGlobalInterrupt;

            break;
        }
        case ID_4:
        {
            myGlobalInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_4,
                    createCallback(
                         this,
                         &TimerSTM32F30x::generalPurposeTimerInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_4]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_4] = myGlobalInterrupt;

            break;
        }
        case ID_6:
        {
            myGlobalInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_6_DAC,
                    createCallback(
                                  this,
                                  &TimerSTM32F30x::basicTimerInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_6_DAC]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_6_DAC] = myGlobalInterrupt;

            break;
        }
        case ID_7:
        {
            myGlobalInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_7,
                    createCallback(
                                  this,
                                  &TimerSTM32F30x::basicTimerInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_7]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_7] = myGlobalInterrupt;

            break;
        }
        case ID_8:
        {
            myUpdateInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_8_UP,
                    createCallback(
                                 this,
                                 &TimerSTM32F30x::updateInterruptEventHandler));
            myBreakInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_8_BRK,
                    createCallback(
                                  this,
                                  &TimerSTM32F30x::breakInterruptEventHandler));
            myCaptureCompareInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_8_CC,
                    createCallback(
                              this,
                              &TimerSTM32F30x::captureCompareInterruptHandler));
            myTriggerCommutationInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_8_TRG_COM,
                    createCallback(
                          this,
                          &TimerSTM32F30x::triggerCommutationInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_8_UP]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_8_UP] = myUpdateInterrupt;
            interruptObjectMap[INTERRUPT_TIM_8_BRK] = myBreakInterrupt;
            interruptObjectMap[INTERRUPT_TIM_8_CC] = myCaptureCompareInterrupt;
            interruptObjectMap[INTERRUPT_TIM_8_TRG_COM] =
                                                  myTriggerCommutationInterrupt;

            break;
        }
        case ID_15:
        {
            if (isNullPointer(interruptObjectMap[INTERRUPT_TIM_1_BRK_TIM_15]))
            {
                myGlobalInterrupt =
                    new InterruptSTM32F30x(
                        InterruptSTM32F30x::ID_TIM_1_BRK_TIM_15,
                        createCallback(
                                  this,
                                  &TimerSTM32F30x::basicTimerInterruptHandler));

                interruptObjectMap[INTERRUPT_TIM_1_BRK_TIM_15] =
                                                              myGlobalInterrupt;
            }
            else
            {
                myGlobalInterrupt =
                                 interruptObjectMap[INTERRUPT_TIM_1_BRK_TIM_15];
            }

            break;
        }
        case ID_16:
        {
            if (isNullPointer(interruptObjectMap[INTERRUPT_TIM_1_UP_TIM_16]))
            {
                myGlobalInterrupt =
                    new InterruptSTM32F30x(
                        InterruptSTM32F30x::ID_TIM_1_UP_TIM_16,
                        createCallback(
                                  this,
                                  &TimerSTM32F30x::basicTimerInterruptHandler));

                interruptObjectMap[INTERRUPT_TIM_1_UP_TIM_16] =
                                                              myGlobalInterrupt;
            }
            else
            {
                myGlobalInterrupt =
                                 interruptObjectMap[INTERRUPT_TIM_1_UP_TIM_16];
            }

            break;
        }
        case ID_17:
        {
            if (isNullPointer(
                            interruptObjectMap[INTERRUPT_TIM_1_TRG_COM_TIM_17]))
            {
                myGlobalInterrupt =
                    new InterruptSTM32F30x(
                        InterruptSTM32F30x::ID_TIM_1_TRG_COM_TIM_17,
                        createCallback(
                                  this,
                                  &TimerSTM32F30x::basicTimerInterruptHandler));

                interruptObjectMap[INTERRUPT_TIM_1_TRG_COM_TIM_17] =
                                                              myGlobalInterrupt;
            }
            else
            {
                myGlobalInterrupt =
                             interruptObjectMap[INTERRUPT_TIM_1_TRG_COM_TIM_17];
            }

            break;
        }
        case ID_20:
        {
            myUpdateInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_20_UP,
                    createCallback(
                                 this,
                                 &TimerSTM32F30x::updateInterruptEventHandler));
            myBreakInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_20_BRK,
                    createCallback(
                                  this,
                                  &TimerSTM32F30x::breakInterruptEventHandler));
            myCaptureCompareInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_20_CC,
                    createCallback(
                              this,
                              &TimerSTM32F30x::captureCompareInterruptHandler));
            myTriggerCommutationInterrupt =
                new InterruptSTM32F30x(
                    InterruptSTM32F30x::ID_TIM_20_TRG_COM,
                    createCallback(
                          this,
                          &TimerSTM32F30x::triggerCommutationInterruptHandler));

            if (isValidPointer(interruptObjectMap[INTERRUPT_TIM_20_UP]))
            {
                // Trying to instantiate peripheral twice, lock up
                while (true)
                {
                }
            }

            interruptObjectMap[INTERRUPT_TIM_20_UP] = myUpdateInterrupt;
            interruptObjectMap[INTERRUPT_TIM_20_BRK] = myBreakInterrupt;
            interruptObjectMap[INTERRUPT_TIM_20_CC] = myCaptureCompareInterrupt;
            interruptObjectMap[INTERRUPT_TIM_20_TRG_COM] =
                                                  myTriggerCommutationInterrupt;

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::basicTimerInterruptHandler()
{
    // Basic timers only have update interrupt events
    updateInterruptEventHandler();
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::generalPurposeTimerInterruptHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_UPDATE) &&
        isInterruptEventPending(INTERRUPT_EVENT_UPDATE))
    {
        updateInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                                   INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1) &&
             isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1))
    {
        captureCompareChannel1InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                                   INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2) &&
             isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2))
    {
        captureCompareChannel2InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                                   INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3) &&
             isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3))
    {
        captureCompareChannel3InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                                   INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4) &&
             isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4))
    {
        captureCompareChannel4InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(INTERRUPT_EVENT_TRIGGER) &&
             isInterruptEventPending(INTERRUPT_EVENT_TRIGGER))
    {
        triggerInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1))
    {
        captureCompareChannel1OvercaptureInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2))
    {
        captureCompareChannel2OvercaptureInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3))
    {
        captureCompareChannel3OvercaptureInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4))
    {
        captureCompareChannel4OvercaptureInterruptEventHandler();
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareInterruptHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1) &&
        isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1))
    {
        captureCompareChannel1InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                                   INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2) &&
             isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2))
    {
        captureCompareChannel2InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                                   INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3) &&
             isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3))
    {
        captureCompareChannel3InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                                   INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4) &&
             isInterruptEventPending(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4))
    {
        captureCompareChannel4InterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1))
    {
        captureCompareChannel1OvercaptureInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2))
    {
        captureCompareChannel2OvercaptureInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3))
    {
        captureCompareChannel3OvercaptureInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(
                       INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4) &&
             isInterruptEventPending(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4))
    {
        captureCompareChannel4OvercaptureInterruptEventHandler();
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::triggerCommutationInterruptHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_TRIGGER) &&
        isInterruptEventPending(INTERRUPT_EVENT_TRIGGER))
    {
        triggerInterruptEventHandler();
    }
    else if (isInterruptEventEnabled(INTERRUPT_EVENT_COMMUTATION) &&
             isInterruptEventPending(INTERRUPT_EVENT_COMMUTATION))
    {
        commutationInterruptEventHandler();
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::updateInterruptEventHandler()
{
    myUpdateInterruptCallback->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_UPDATE);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel1InterruptEventHandler()
{
    myCaptureCompareInterruptCallbacks[CHANNEL_ID_1]->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel2InterruptEventHandler()
{
    myCaptureCompareInterruptCallbacks[CHANNEL_ID_2]->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel3InterruptEventHandler()
{
    myCaptureCompareInterruptCallbacks[CHANNEL_ID_3]->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel4InterruptEventHandler()
{
    myCaptureCompareInterruptCallbacks[CHANNEL_ID_4]->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::commutationInterruptEventHandler()
{
    myCommutationInterruptCallback->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_COMMUTATION);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::triggerInterruptEventHandler()
{
    myTriggerInterruptCallback->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_TRIGGER);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::breakInterruptEventHandler()
{
    myBreakInterruptCallback->call();
    clearPendingInterruptEvent(INTERRUPT_EVENT_BREAK);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel1OvercaptureInterruptEventHandler()
{
    clearPendingInterruptEvent(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel2OvercaptureInterruptEventHandler()
{
    clearPendingInterruptEvent(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel3OvercaptureInterruptEventHandler()
{
    clearPendingInterruptEvent(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3);
}

//------------------------------------------------------------------------------
void TimerSTM32F30x::captureCompareChannel4OvercaptureInterruptEventHandler()
{
    clearPendingInterruptEvent(
                         INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4);
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void TIM1_BRK_TIM15_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_1_BRK_TIM_15]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM1_UP_TIM16_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_1_UP_TIM_16]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
    interruptObjectMap[
                     TimerSTM32F30x::INTERRUPT_TIM_1_TRG_COM_TIM_17]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM1_CC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_1_CC]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM2_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM3_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM4_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM6_DAC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_6_DAC]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM7_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_7]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_BRK_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_8_BRK]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_UP_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_8_UP]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_TRG_COM_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_8_TRG_COM]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_CC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_8_CC]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_BRK_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_20_BRK]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_UP_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_20_UP]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_TRG_COM_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_20_TRG_COM]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_CC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F30x::INTERRUPT_TIM_20_CC]->handler();
}
