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
/// @file TimerSTM32F4xx.cpp
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief TimerSTM32F4xx class.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <TimerSTM32F4xx.h>
#include <CallbackMethod.h>

#include <STM32F4xx_tim.h>

using Plat4m::TimerSTM32F4xx;
using Plat4m::Module;
using Plat4m::InterruptSTM32F4xx;
using Plat4m::ProcessorSTM32F4xx;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

static InterruptSTM32F4xx* interruptObjectMap[17];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

const ProcessorSTM32F4xx::Peripheral TimerSTM32F4xx::myPeripheralMap[] =
{
    ProcessorSTM32F4xx::PERIPHERAL_TIM_1,  /// ID_1
    ProcessorSTM32F4xx::PERIPHERAL_TIM_2,  /// ID_2
    ProcessorSTM32F4xx::PERIPHERAL_TIM_3,  /// ID_3
    ProcessorSTM32F4xx::PERIPHERAL_TIM_4,  /// ID_4
	ProcessorSTM32F4xx::PERIPHERAL_TIM_5,  /// ID_5
    ProcessorSTM32F4xx::PERIPHERAL_TIM_6,  /// ID_6
    ProcessorSTM32F4xx::PERIPHERAL_TIM_7,  /// ID_7
    ProcessorSTM32F4xx::PERIPHERAL_TIM_8,  /// ID_8
	ProcessorSTM32F4xx::PERIPHERAL_TIM_9,  /// ID_9
	ProcessorSTM32F4xx::PERIPHERAL_TIM_10, /// ID_10
	ProcessorSTM32F4xx::PERIPHERAL_TIM_11, /// ID_11
	ProcessorSTM32F4xx::PERIPHERAL_TIM_12, /// ID_12
	ProcessorSTM32F4xx::PERIPHERAL_TIM_13, /// ID_13
	ProcessorSTM32F4xx::PERIPHERAL_TIM_14  /// ID_14
};

const InterruptSTM32F4xx::Id TimerSTM32F4xx::myInterruptIdMap[] =
{
	InterruptSTM32F4xx::ID_TIM_1_BRK_TIM_9,
	InterruptSTM32F4xx::ID_TIM_1_UP_TIM_10,
	InterruptSTM32F4xx::ID_TIM_1_TRG_COM_TIM_11,
	InterruptSTM32F4xx::ID_TIM_1_CC,
	InterruptSTM32F4xx::ID_TIM_2,
	InterruptSTM32F4xx::ID_TIM_3,
	InterruptSTM32F4xx::ID_TIM_4,
	InterruptSTM32F4xx::ID_TIM_8_BRK_TIM_12,
	InterruptSTM32F4xx::ID_TIM_8_UP_TIM_13,
	InterruptSTM32F4xx::ID_TIM_8_TRG_COM_TIM_14,
	InterruptSTM32F4xx::ID_TIM_8_CC
};

TIM_TypeDef* TimerSTM32F4xx::myTimerMap[] =
{
    TIM1, /// TimerSTM32F4xx::ID_1
    TIM2, /// TimerSTM32F4xx::ID_2
    TIM3, /// TimerSTM32F4xx::ID_3
    TIM4, /// TimerSTM32F4xx::ID_4
    TIM6, /// TimerSTM32F4xx::ID_6
    TIM7, /// TimerSTM32F4xx::ID_7
    TIM8  /// TimerSTM32F4xx::ID_8
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F4xx::TimerSTM32F4xx(const Id id) :
    myId(id),
    myTimer(myTimerMap[id]),
    myGpioPin(0),
    myUpdateInterrupt(0)
{
    initialize();
}

//------------------------------------------------------------------------------
TimerSTM32F4xx::TimerSTM32F4xx(
                             const Id id,
                             UpdateInterruptCallback& updateInterruptCallback) :
    myId(id),
    myTimer(myTimerMap[id]),
    myGpioPin(0),
    myUpdateInterrupt(0),
    myUpdateInterruptCallback(&updateInterruptCallback)
{
    // Timers 6 and 7 do not have a related GPIO pin
    if ((myId == ID_6) && (myId == ID_7))
    {
        // Lockup
        while (true)
        {
        }
    }

    initialize();
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
TimerSTM32F4xx::Id TimerSTM32F4xx::getId() const
{
    return myId;
}

//------------------------------------------------------------------------------
TIM_TypeDef* TimerSTM32F4xx::getTimer()
{
    return myTimer;
}

//------------------------------------------------------------------------------
TimerSTM32F4xx::Error TimerSTM32F4xx::setConfig(const Config& config)
{
    // TODO: Incorporate clock division (for reeeeally slow timers)

    uint32_t prescaler = 1;
    uint32_t period = 1;
    float outputClockFrequencyHz;

    myInputClockFrequencyHz =
        ProcessorSTM32F4xx::getPeripheralInputClockFrequencyHz(
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

            while (abs(errorPercent) > config.maxFrequencyErrorPercent)
            {
                if (errorPercent > 0.0)
                {
                    float ratio = (outputClockFrequencyHz / config.frequencyHz);
                    prescaler = (uint32_t) round((float) prescaler * ratio);
                }

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
        setCaptureComparePreloadEnabled(true);

        // TODO Make this config parameter
//        setBits(myTimer->BDTR, 0x82);
        setBits(myTimer->BDTR, 0x7F);
    }

    setBits(myTimer->EGR, TIM_EGR_UG);

    myConfig = config;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
uint32_t TimerSTM32F4xx::getPeriod() const
{
    return myPeriod;
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setChannelCaptureCompareEnabled(const ChannelId channelId,
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
void TimerSTM32F4xx::setComplementaryChannelCaptureCompareEnabled(
                            const ComplementaryChannelId complementaryChannelId,
                            const bool enabled)
{
    // TODO Handle channels > 3

    uint32_t offset = (complementaryChannelId * 4) + 2;

    setBitsSet(myTimer->CCER, ((uint32_t) 0x1) << offset, enabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setChannelCaptureCompareMode(
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
void TimerSTM32F4xx::setChannelOutputCompareMode(
                                      const ChannelId channelId,
                                      const OutputCompareMode outputCompareMode)
{
    switch (channelId)
    {
        case CHANNEL_ID_1:
        {
            clearAndSetBits(myTimer->CCMR1,
            				TIM_CCMR1_OC1M,
							((uint32_t) outputCompareMode) << 4);

            break;
        }
        case CHANNEL_ID_2:
        {
            clearAndSetBits(myTimer->CCMR1,
            				TIM_CCMR1_OC2M,
							((uint32_t) outputCompareMode) << 12);

            break;
        }
        case CHANNEL_ID_3:
        {
            clearAndSetBits(myTimer->CCMR2,
            				TIM_CCMR2_OC3M,
							((uint32_t) outputCompareMode) << 4);

            break;
        }
        case CHANNEL_ID_4:
        {
            clearAndSetBits(myTimer->CCMR2,
            				TIM_CCMR2_OC4M,
							((uint32_t) outputCompareMode) << 12);

            break;
        }
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setChannelOutputComparePreloadEnabled(
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
void TimerSTM32F4xx::setChannelPolarity(const ChannelId channelId,
                                        const Polarity polarity)
{
    // TODO Handle channels > 3

    uint32_t offset = (channelId * 4) + 1;

    clearAndSetBits(myTimer->CCER,
                    ((uint32_t) 0x1) << offset,
                    ((uint32_t) polarity) << offset);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setComplementaryChannelPolarity(
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
void TimerSTM32F4xx::setChannelCaptureCompareValue(const ChannelId channelId,
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
void TimerSTM32F4xx::generateCaptureCompareControlUpdate()
{
    setBits(myTimer->EGR, TIM_EGR_COMG);
}

//------------------------------------------------------------------------------
// Private static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimerSTM32F4xx::tim1BrkTim15InterruptHandler()
{

}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error TimerSTM32F4xx::driverSetEnabled(const bool enabled)
{
    if (isValidPointer(myGpioPin))
    {
        myGpioPin->setEnabled(enabled);

        if (enabled)
        {
            // Generic GpioPin configuration
            GpioPin::Config gpioConfig;
            gpioConfig.mode     = GpioPin::MODE_ALTERNATE_FUNCTION;
//            gpioConfig.resistor = GpioPin::RESISTOR_NONE;
            gpioConfig.resistor = GpioPin::RESISTOR_PULL_DOWN;

            myGpioPin->configure(gpioConfig);
        }
    }

    ProcessorSTM32F4xx::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

    privateSetEnabled(enabled);

    // TODO: Clean this up
    setBitsSet(myTimer->DIER, TIM_DIER_UIE, enabled);

    myUpdateInterrupt->setEnabled(enabled);

    InterruptSTM32F4xx::Config config;
    config.priority = 2;

    myUpdateInterrupt->configure(config);

    // TODO: For now
    setBitsSet(myTimer->CR2, TIM_CR2_MMS_1, enabled);

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimerSTM32F4xx::privateSetEnabled(const bool enabled)
{
    setBitsSet(myTimer->CR1, TIM_CR1_CEN, enabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setPrescaler(const uint32_t prescaler)
{
    myTimer->PSC = prescaler - 1;
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setClockDivision(const uint32_t clockDivision)
{
    // Empty for now
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setPeriod(const uint32_t period)
{
    myTimer->ARR = period - 1;
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setDirection(const Direction direction)
{
    clearAndSetBits(myTimer->CR1,
                    TIM_CR1_DIR,
                    ((uint32_t) direction) << 4);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setAlignment(const Alignment alignment)
{
    clearAndSetBits(myTimer->CR1,
                    TIM_CR1_CMS,
                    ((uint32_t) alignment) << 5);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setAutoReloadPreloadEnabled(const bool enabled)
{
    setBitsSet(myTimer->CR1, TIM_CR1_ARPE, enabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setMainOutputEnabled(const bool enabled)
{
    setBitsSet(myTimer->BDTR, TIM_BDTR_MOE, enabled);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::setCaptureComparePreloadEnabled(const bool enabled)
{
    setBitsSet(myTimer->CR2, TIM_CR2_CCPC, enabled);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void TimerSTM32F4xx::initialize()
{
    // TODO: This is temporary only!
    if (myId == ID_6)
    {
    	myUpdateInterrupt = new InterruptSTM32F4xx(
				 InterruptSTM32F4xx::ID_TIM_6_DAC,
    	         createCallback(this, &TimerSTM32F4xx::updateInterruptHandler));
        interruptObjectMap[7] = myUpdateInterrupt;
    }
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::updateInterruptHandler()
{
    myUpdateInterruptCallback->call();
    clearBits(myTimer->SR, TIM_SR_UIF);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::breakInterruptHandler()
{
    myBreakInterruptCallback->call();
    clearBits(myTimer->SR, TIM_SR_UIF);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::captureCompareInterruptHandler()
{
    myCaptureCompareInterruptCallback->call();
    clearBits(myTimer->SR, TIM_SR_UIF);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::triggerInterruptHandler()
{
    myTriggerInterruptCallback->call();
    clearBits(myTimer->SR, TIM_SR_UIF);
}

//------------------------------------------------------------------------------
void TimerSTM32F4xx::commutationInterruptHandler()
{
    myCommutationInterruptCallback->call();
    clearBits(myTimer->SR, TIM_SR_UIF);
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void TIM1_BRK_TIM15_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_1_BRK_TIM_15]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM1_UP_TIM16_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_1_UP_TIM_16]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
    interruptObjectMap[
                     TimerSTM32F4xx::INTERRUPT_TIM_1_TRG_COM_TIM_17]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM1_CC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_1_CC]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM2_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_2]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM3_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM4_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_4]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM6_DAC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_6_DAC]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM7_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_7]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_BRK_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_8_BRK]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_UP_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_8_UP]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_TRG_COM_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_8_TRG_COM]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM8_CC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_8_CC]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_BRK_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_20_BRK]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_UP_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_20_UP]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_TRG_COM_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_20_TRG_COM]->handler();
}

//------------------------------------------------------------------------------
extern "C" void TIM20_CC_IRQHandler(void)
{
    interruptObjectMap[TimerSTM32F4xx::INTERRUPT_TIM_20_CC]->handler();
}
