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
/// @file TimerSTM32F30x.h
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief TimerSTM32F30x class header file.
///

#ifndef PLAT4M_TIMER_STM32F30X_H
#define PLAT4M_TIMER_STM32F30X_H

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
    };

    enum InterruptEvent
    {
        INTERRUPT_EVENT_UPDATE = 0,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_1,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_2,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_3,
        INTERRUPT_EVENT_CAPTURE_COMPARE_CHANNEL_4,
        INTERRUPT_EVENT_COMMUTATION,
        INTERRUPT_EVENT_TRIGGER,
        INTERRUPT_EVENT_BREAK,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_1,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_2,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_3,
        INTERRUPT_EVENT_CAPTURE_COMPARE_OVERCAPTURE_CHANNEL_4
    };

    enum MasterMode
    {
        MASTER_MODE_RESET = 0,
        MASTER_MODE_ENABLE,
        MASTER_MODE_UPDATE,
        MASTER_MODE_COMPARE_PULSE,
        MASTER_MODE_COMPARE_OC1REF,
        MASTER_MODE_COMPARE_OC2REF,
        MASTER_MODE_COMPARE_OC3REF,
        MASTER_MODE_COMPARE_OC4REF
    };

    enum SlaveMode
    {
        SLAVE_MODE_DISABLED = 0,
        SLAVE_MODE_ENCODER_1,
        SLAVE_MODE_ENCODER_2,
        SLAVE_MODE_ENCODER_3,
        SLAVE_MODE_RESET,
        SLAVE_MODE_GATED,
        SLAVE_MODE_TRIGGER,
        SLAVE_MODE_EXTERNAL_CLOCK_1,
        SLAVE_MODE_RESET_AND_TRIGGER
    };

    enum InputSelection
    {
        INPUT_SELECTION_CH_1_TO_TI_1 = 0,
        INPUT_SELECTION_CH_1_2_3_TO_TI_1
    };

    enum CaptureCompareSelection
    {
        CAPTURE_COMPARE_SELECTION_OUTPUT = 0,
        CAPTURE_COMPARE_SELECTION_INPUT_IC_1_MAPPED_ON_TI_1,
        CAPTURE_COMPARE_SELECTION_INPUT_IC_1_MAPPED_ON_TI_2,
        CAPTURE_COMPARE_SELECTION_INPUT_IC_1_MAPPED_ON_TRC
    };

    enum InputCaptureSelection
    {
        INPUT_CAPTURE_SELECTION_IC_1_MAPPED_ON_TI_1 = 1,
        INPUT_CAPTURE_SELECTION_IC_1_MAPPED_ON_TI_2,
        INPUT_CAPTURE_SELECTION_IC_1_MAPPED_ON_TRC
    };

    enum TriggerSelection
    {
        TRIGGER_SELECTION_INTERNAL_TRIGGER_0      = 0,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_15       = 0,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_1        = 0,
        TRIGGER_SELECTION_TIM_20_FROM_TIM_1       = 0,
        TRIGGER_SELECTION_INTERNAL_TRIGGER_1      = 1,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_2        = 1,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_2        = 1,
        TRIGGER_SELECTION_TIM_20_FROM_TIM_8       = 1,
        TRIGGER_SELECTION_INTERNAL_TRIGGER_2      = 2,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_3        = 2,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_4        = 2,
        TRIGGER_SELECTION_TIM_20_FROM_TIM_4       = 2,
        TRIGGER_SELECTION_INTERNAL_TRIGGER_3      = 3,
        TRIGGER_SELECTION_TIM_1_FROM_TIM_4_TIM_17 = 3,
        TRIGGER_SELECTION_TIM_8_FROM_TIM_3        = 3,
        TRIGGER_SELECTION_TIM_20_FROM_TIM_15      = 3,
        TRIGGER_SELECTION_TI1_EDGE_DETECTOR,
        TRIGGER_SELECTION_FILTERED_TIMER_INPUT_1,
        TRIGGER_SELECTION_FILTERED_TIMER_INPUT_2,
        TRIGGER_SELECTION_EXTERNAL_TRIGGER_INPUT
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
        FrequencyHz frequencyHz;
        Resolution resolution;
        RealNumber maxFrequencyErrorPercent;
        uint32_t resolutionLevels;
        Direction direction;
        Alignment alignment;
        bool autoReloadPreloadEnabled;
        uint8_t repetitionCount;
    };

    struct InputCaptureConfig
    {
        InputCaptureSelection selection;
    };

    struct OutputCompareConfig
    {
        OutputCompareMode mode;
        Polarity polarity;
        bool preloadEnabled;
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

    InterruptSTM32F30x* getGlobalInterrupt();

    InterruptSTM32F30x* getUpdateInterrupt();

    InterruptSTM32F30x* getBreakInterrupt();

    InterruptSTM32F30x* getCaptureCompareInterrupt();

    InterruptSTM32F30x* getTriggerCommutationInterrupt();

    void setUpdateInterruptCallback(
                              UpdateInterruptCallback& updateInterruptCallback);

    void setBreakInterruptCallback(
                                BreakInterruptCallback& breakInterruptCallback);

    void setChannelCaptureCompareInterruptCallback(
              const ChannelId channelId,
              CaptureCompareInterruptCallback& captureCompareInterruptCallback);

    void setTriggerInterruptCallback(
                            TriggerInterruptCallback& triggerInterruptCallback);

    void setCommutationInterruptCallback(
                    CommutationInterruptCallback& commutationInterruptCallback);

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

    void generateUpdateEvent();

    void generateCaptureCompareControlUpdate();

    void setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                  const bool enabled);

    void setCounterEnabled(const bool enabled);

    void setMasterMode(const MasterMode masterMode);

    void setSlaveMode(const SlaveMode slaveMode);

    void setInputSelection(const InputSelection inputSelection);

    void setChannelCaptureCompareSelection(
                         const ChannelId channelId,
                         const CaptureCompareSelection captureCompareSelection);

    void setChannelOutputCompareConfig(
                                 const ChannelId channelId,
                                 const OutputCompareConfig outputCompareConfig);

    void setChannelInputCaptureConfig(
                                   const ChannelId channelId,
                                   const InputCaptureConfig inputCaptureConfig);

    void setPrescaler(const uint32_t prescaler);

    void setClockDivision(const uint32_t clockDivision);

    inline void setPeriod(const uint32_t period);

    void setTriggerSelection(const TriggerSelection triggerSelection);

    uint32_t getChannelCaptureCompareValue(const ChannelId channelId);

    void setRepetitionCount(const uint8_t repetitionCount);

    uint32_t getCounterValue();

    void setMainOutputEnabled(const bool enabled);

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    // Constants

    static const ProcessorSTM32F30x::Peripheral myPeripheralMap[];

    static const GpioPinSTM32F30x::AlternateFunction myAlternateFunctionMap[];

    static const uint16_t myCounterModeMap[];

    static const uint16_t myPolarityMap[];

    static const InterruptSTM32F30x::Id myInterruptIdMap[];

    static const uint32_t myInterruptEventMap[];

    // Variables

    static TIM_TypeDef* myTimerMap[];

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------
    
    const Id myId;

    UpdateInterruptCallback* myUpdateInterruptCallback;

    BreakInterruptCallback* myBreakInterruptCallback;

    CaptureCompareInterruptCallback* myCaptureCompareInterruptCallbacks[4];

    TriggerInterruptCallback* myTriggerInterruptCallback;

    CommutationInterruptCallback* myCommutationInterruptCallback;

    TIM_TypeDef* myTimer;

    InterruptSTM32F30x* myGlobalInterrupt;

    InterruptSTM32F30x* myBreakInterrupt;

    InterruptSTM32F30x* myUpdateInterrupt;

    InterruptSTM32F30x* myCaptureCompareInterrupt;

    InterruptSTM32F30x* myTriggerCommutationInterrupt;

    FrequencyHz myInputClockFrequencyHz;

    FrequencyHz myOutputClockFrequencyHz;

    uint32_t myPeriod;

    Config myConfig;

    //--------------------------------------------------------------------------
    // Private methods implemented from Module
    //--------------------------------------------------------------------------
    
    Module::Error driverSetEnabled(const bool enabled);

    //--------------------------------------------------------------------------
    // Private inline methods
    //--------------------------------------------------------------------------

    inline void initialize();

    inline void setDirection(const Direction Direction);

    inline void setAlignment(const Alignment alignment);

    inline void setAutoReloadPreloadEnabled(const bool enabled);

    inline bool isInterruptEventEnabled(const InterruptEvent interruptEvent);

    inline bool isInterruptEventPending(const InterruptEvent interruptEvent);

    inline void clearPendingInterruptEvent(const InterruptEvent interruptEvent);

    inline void setCaptureComparePreloadEnabled(const bool enabled);

    inline void basicTimerInterruptHandler();

    inline void generalPurposeTimerInterruptHandler();

    inline void captureCompareInterruptHandler();

    inline void triggerCommutationInterruptHandler();

    inline void updateInterruptEventHandler();

    inline void captureCompareChannel1InterruptEventHandler();

    inline void captureCompareChannel2InterruptEventHandler();

    inline void captureCompareChannel3InterruptEventHandler();

    inline void captureCompareChannel4InterruptEventHandler();

    inline void commutationInterruptEventHandler();

    inline void triggerInterruptEventHandler();

    inline void breakInterruptEventHandler();

    inline void captureCompareChannel1OvercaptureInterruptEventHandler();

    inline void captureCompareChannel2OvercaptureInterruptEventHandler();

    inline void captureCompareChannel3OvercaptureInterruptEventHandler();

    inline void captureCompareChannel4OvercaptureInterruptEventHandler();
};

}; // namespace Plat4m

#endif // PLAT4M_TIMER_STM32F30X_H
