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
/// @file TimerSTM32F30x.h
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief TimerSTM32F30x class header file.
///

#ifndef TIMER_STM32F30X_H
#define TIMER_STM32F30X_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Module.h>
#include <ErrorTemplate.h>
#include <Callback.h>
#include <GpioPinSTM32F30x.h>
#include <InterruptSTM32F30x.h>
#include <ProcessorSTM32F30x.h>

#include <stdint.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class TimerSTM32F30x : public Module
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE = 0
    };

    /**
     * @brief Enumeration of timers.
     */
    enum Id
    {
        ID_1 = 0,
        ID_2,
        ID_3,
        ID_4,
        ID_6,
        ID_7,
        ID_8,
        ID_15,
        ID_16,
        ID_17,
        ID_20
    };

    /**
     * @brief Enumeration of timer channels.
     */
    enum ChannelId
    {
        CHANNEL_ID_1 = 0,
        CHANNEL_ID_2,
        CHANNEL_ID_3,
        CHANNEL_ID_4
    };

    enum ComplementaryChannelId
    {
        COMPLEMENTARY_CHANNEL_ID_1 = 0,
        COMPLEMENTARY_CHANNEL_ID_2,
        COMPLEMENTARY_CHANNEL_ID_3
    };

    enum Mode
    {
        MODE_COUNTING_UP = 0,
        MODE_COUNTING_DOWN,
        MODE_COUNTING_UP_DOWN,
        MODE_INPUT_CAPTURE,
        MODE_OUTPUT_COMPARE,
        MODE_PWM_INPUT,
        MODE_PWM_OUTPUT,
        MODE_PULSE
    };

    enum CaptureCompareMode
    {
        CAPTURE_COMPARE_MODE_OUTPUT     = 0,
        CAPTURE_COMPARE_MODE_INPUT_TI_1 = 1,
        CAPTURE_COMPARE_MODE_INPUT_TI_2 = 2,
        CAPTURE_COMPARE_MODE_INPUT_TRC  = 3
    };

    enum OutputCompareMode
    {
        OUTPUT_COMPARE_MODE_FROZEN                   = 0,
        OUTPUT_COMPARE_MODE_ACTIVE_ON_MATCH          = 1,
        OUTPUT_COMPARE_MODE_INACTIVE_ON_MATCH        = 2,
        OUTPUT_COMPARE_MODE_TOGGLE                   = 3,
        OUTPUT_COMPARE_MODE_FORCE_INACTIVE           = 4,
        OUTPUT_COMPARE_MODE_FORCE_ACTIVE             = 5,
        OUTPUT_COMPARE_MODE_PWM_MODE_1               = 6,
        OUTPUT_COMPARE_MODE_PWM_MODE_2               = 7,
        OUTPUT_COMPARE_MODE_RETRIGGERABLE_OPM_MODE_1 = 8,
        OUTPUT_COMPARE_MODE_RETRIGGERABLE_OPM_MODE_2 = 9,
        OUTPUT_COMPARE_MODE_COMBINED_PWM_MODE_1      = 12,
        OUTPUT_COMPARE_MODE_COMBINED_PWM_MODE_2      = 13,
        OUTPUT_COMPARE_MODE_ASYMMETRIC_PWM_MODE_1    = 14,
        OUTPUT_COMPARE_MODE_ASYMMETRIC_PWM_MODE_2    = 15
    };

    enum Polarity
    {
        POLARITY_HIGH = 0,
        POLARITY_LOW  = 1
    };

    enum Resolution
    {
        RESOLUTION_MIN = 0,
        RESOLUTION_MAX,
        RESOLUTION_LEVELS
    };

    enum Alignment
    {
        ALIGNMENT_EDGE           = 0,
        ALIGNMENT_CENTER_DOWN    = 1,
        ALIGNMENT_CENTER_UP      = 2,
        ALIGNMENT_CENTER_UP_DOWN = 3
    };

    enum Direction
    {
        DIRECTION_UP   = 0,
        DIRECTION_DOWN = 1
    };

    enum Interrupt
    {
        INTERRUPT_TIM_1_BRK_TIM_15 = 0,
        INTERRUPT_TIM_1_UP_TIM_16,
        INTERRUPT_TIM_1_TRG_COM_TIM_17,
        INTERRUPT_TIM_1_CC,
        INTERRUPT_TIM_2,
        INTERRUPT_TIM_3,
        INTERRUPT_TIM_4,
        INTERRUPT_TIM_6_DAC,
        INTERRUPT_TIM_7,
        INTERRUPT_TIM_8_BRK,
        INTERRUPT_TIM_8_UP,
        INTERRUPT_TIM_8_TRG_COM,
        INTERRUPT_TIM_8_CC,
        INTERRUPT_TIM_20_BRK,
        INTERRUPT_TIM_20_UP,
        INTERRUPT_TIM_20_TRG_COM,
        INTERRUPT_TIM_20_CC
//        INTERRUPT_UPDATE,
//        INTERRUPT_BREAK,
//        INTERRUPT_CAPTURE_COMPARE,
//        INTERRUPT_TRIGGER,
//        INTERRUPT_COMMUTATION
    };

    typedef ErrorTemplate<ErrorCode> Error;

    typedef Callback<> UpdateInterruptCallback;

    typedef Callback<> BreakInterruptCallback;

    typedef Callback<> CaptureCompareInterruptCallback;

    typedef Callback<> TriggerInterruptCallback;

    typedef Callback<> CommutationInterruptCallback;

    struct Config
    {
        Mode mode;
        float frequencyHz;
        Resolution resolution;
        float maxFrequencyErrorPercent;
        uint32_t resolutionLevels;
        Direction direction;
        Alignment alignment;
        bool autoReloadPreloadEnabled;
    };

    struct OutputCompareConfig
    {
        OutputCompareMode mode;
        bool preloadEnabled;
        Polarity polarity;
        uint32_t value;
    };

    struct OutputCompareComplementaryConfig
    {
        Polarity polarity;
    };

    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------

    TimerSTM32F30x(const Id id);

    TimerSTM32F30x(const Id id,
                   UpdateInterruptCallback& updateInterruptCallback);

    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    Id getId() const;

    TIM_TypeDef* getTimer();

    Error setConfig(const Config& config);

    uint32_t getPeriod() const;

    void setChannelCaptureCompareEnabled(const ChannelId channelId,
                                         const bool enabled);

    void setComplementaryChannelCaptureCompareEnabled(
                            const ComplementaryChannelId complementaryChannelId,
                            const bool enabled);

    void setChannelCaptureCompareMode(
                                   const ChannelId channelId,
                                   const CaptureCompareMode captureCompareMode);

    void setChannelOutputCompareMode(const ChannelId channelId,
                                     const OutputCompareMode outputCompareMode);

    void setChannelOutputComparePreloadEnabled(const ChannelId channelId,
                                               const bool enabled);

    void setChannelPolarity(const ChannelId channelId, const Polarity polarity);

    void setComplementaryChannelPolarity(
                            const ComplementaryChannelId complementaryChannelId,
                            const Polarity polarity);

    void setChannelCaptureCompareValue(const ChannelId channelId,
                                       const uint32_t value);

    void generateCaptureCompareControlUpdate();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F30x::Peripheral myPeripheralMap[];

    static const GpioSTM32F30x::AlternateFunction myAlternateFunctionMap[];

    static const uint16_t myCounterModeMap[];

    static const uint16_t myPolarityMap[];

    static const InterruptSTM32F30x::Id myInterruptIdMap[];

    static const GpioSTM32F30x::OutputSpeed myDefaultGpioSpeed;

    // Variables

    static TIM_TypeDef* myTimerMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;
    
    TIM_TypeDef* myTimer;
    
    GpioPinSTM32F30x* myGpioPin;
    
    InterruptSTM32F30x myUpdateInterrupt;

    float myInputClockFrequencyHz;
    
    float myOutputClockFrequencyHz;
    
    uint32_t myPeriod;
    
    Config myConfig;

    UpdateInterruptCallback* myUpdateInterruptCallback;

    BreakInterruptCallback* myBreakInterruptCallback;

    CaptureCompareInterruptCallback* myCaptureCompareInterruptCallback;

    TriggerInterruptCallback* myTriggerInterruptCallback;

    CommutationInterruptCallback* myCommutationInterruptCallback;

    //--------------------------------------------------------------------------
    // Private static methods
    //--------------------------------------------------------------------------

    static void tim1BrkTim15InterruptHandler();

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void privateSetEnabled(const bool enabled);

    inline void setPrescaler(const uint32_t prescaler);

    inline void setClockDivision(const uint32_t clockDivision);

    inline void setPeriod(const uint32_t period);

    inline void setDirection(const Direction Direction);

    inline void setAlignment(const Alignment alignment);

    inline void setAutoReloadPreloadEnabled(const bool enabled);

    inline void setMainOutputEnabled(const bool enabled);

    inline void setCaptureComparePreloadEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------

    void initialize();

    void updateInterruptHandler();

    void breakInterruptHandler();

    void captureCompareInterruptHandler();

    void triggerInterruptHandler();

    void commutationInterruptHandler();
};

}; // namespace Plat4m

#endif // TIMER_STM32F30X_H
