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
/// @file AdcSTM32F10x.cpp
/// @author Ben Minerd
/// @date 4/10/2013
/// @brief AdcSTM32F10x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <AdcSTM32F10x.h>
#include <Processor.h>
#include <CallbackMethod.h>

using Plat4m::AdcSTM32F10x;
using Plat4m::ProcessorSTM32F10x;
using Plat4m::Module;
using Plat4m::InterruptSTM32F10x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

// This table is local static because it needs to be accessed by c-style ISRs
// (bottom of this file)
// TODO:
static InterruptSTM32F10x* interruptObjectMap[3];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F10x::Peripheral AdcSTM32F10x::myPeripheralMap[] =
{
    ProcessorSTM32F10x::PERIPHERAL_ADC_1, /// ID_1
    ProcessorSTM32F10x::PERIPHERAL_ADC_2, /// ID_2
    ProcessorSTM32F10x::PERIPHERAL_ADC_3  /// ID_3
};

const InterruptSTM32F10x::Id AdcSTM32F10x::myInterruptIdMap[] =
{
    InterruptSTM32F10x::ID_ADC_1,   /// ID_1
    InterruptSTM32F10x::ID_ADC_1_2, /// ID_2
    InterruptSTM32F10x::ID_ADC_3    /// ID_3
};

// Variables

ADC_TypeDef* AdcSTM32F10x::myAdcMap[] =
{
    ADC1,   /// ADC_1
    ADC2,   /// ADC_2
    ADC3    /// ADC_3
};

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F10x::AdcSTM32F10x(const Id id) :
    myId(id),
    myAdc(myAdcMap[id]),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &AdcSTM32F10x::interruptHandler)),
    myDmaChannel(0),
    myConfig(),
    myRegularSequenceConfig(),
    myInjectedSequenceConfig(),
    myChannelConfigs(),
    myLevelsToVolts(Processor::getCoreVoltageV() / 4095.0),
    myRegularSequenceLength(0),
    myRegularValues(),
    myCurrentRegularValueIndex(),
    myRegularChannelToValueMap(),
    myInjectedChannelToValueMap(),
    myEndOfRegularConversionInterruptCallback(0),
    myEndOfInjectedConversionInterruptCallback(0),
    myAnalogWatchdogInterruptCallback(0),
    myLastConversionType(CONVERSION_TYPE_REGULAR)
{
    if (isValidPointer(interruptObjectMap[myId]))
    {
        // Trying to instantiate this peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F10x::~AdcSTM32F10x()
{
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ADC_TypeDef* AdcSTM32F10x::getAdc()
{
    return myAdc;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setConfig(const Config& config)
{
    setAlignment(config.alignment);
    setDmaEnabled(config.dmaEnabled);
    setScanModeEnabled(config.scanModeEnabled);

    privateSetEnabled(true);

    myConfig = config;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setRegularSequenceConfig(const RegularSequenceConfig& config)
{
    uint32_t size = config.sequenceChannels.getSize();
    setRegularSequenceLength(size);

    for (uint32_t i = 0; i < size; i++)
    {
        setRegularSequenceConversionChannel(i + 1,
                                            config.sequenceChannels[i]);
        myRegularChannelToValueMap[(config.sequenceChannels[i])] = i;
    }

    if (config.externalTriggerEnabled)
    {
        setRegularExternalTriggerEnabled(config.externalTriggerEnabled);
        setRegularExternalTrigger(config.externalTrigger);

        setInterruptEventEnabled(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION,
                                 true);
        myInterrupt.setEnabled(true);
    }

    myRegularSequenceConfig = config;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setInjectedSequenceConfig(
                                           const InjectedSequenceConfig& config)
{
    uint32_t size = config.sequenceChannels.getSize();

    if (config.discontinuousModeEnabled)
    {

        setInjectedSequenceLength(config.discontinuousSequenceLength);
    }
    else
    {
        setInjectedSequenceLength(size);
    }

    for (uint32_t i = 0; i < size; i++)
    {
        setInjectedSequenceConversionChannel(i + (4 - size) + 1,
                                             config.sequenceChannels[i]);
        myInjectedChannelToValueMap[(config.sequenceChannels[i])] = i;
    }

    if (config.externalTriggerEnabled)
    {
        setInjectedExternalTriggerEnabled(config.externalTriggerEnabled);
        setInjectedExternalTrigger(config.externalTrigger);

        setInterruptEventEnabled(INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION,
                                 true);
        myInterrupt.setEnabled(true);
    }

    myInjectedSequenceConfig = config;
}

//------------------------------------------------------------------------------
AdcSTM32F10x::Config AdcSTM32F10x::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setChannelConfig(const ChannelId channelId,
                                    const ChannelConfig& channelConfig)
{
    setChannelSampleTime(channelId, channelConfig.sampleTime);

    myChannelConfigs[channelId] = channelConfig;
}

//------------------------------------------------------------------------------
AdcSTM32F10x::ChannelConfig AdcSTM32F10x::getChannelConfig(
                                                const ChannelId channelId) const
{
    return (myChannelConfigs[channelId]);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setDmaChannel(DmaChannelSTM32F10x& dmaChannel)
{
    // TODO Channel / peripheral error checking?

    myDmaChannel = &dmaChannel;

    DmaChannelSTM32F10x::Config dmaChannelConfig;
    dmaChannelConfig.dataTransferDirection =
              DmaChannelSTM32F10x::DATA_TRANSFER_DIRECTION_READ_FROM_PERIPHERAL;
    dmaChannelConfig.circularModeEnabled            = true,
    dmaChannelConfig.peripheralIncrementModeEnabled = false;
    dmaChannelConfig.memoryIncrementModeEnabled     = true;
    dmaChannelConfig.peripheralSize =
                                   DmaChannelSTM32F10x::PERIPHERAL_SIZE_16_BITS;
    dmaChannelConfig.memorySize      = DmaChannelSTM32F10x::MEMORY_SIZE_16_BITS;
    dmaChannelConfig.priorityLevel   = DmaChannelSTM32F10x::PRIORITY_LEVEL_HIGH;
    dmaChannelConfig.memoryToMemoryModeEnabled      = false;
    dmaChannelConfig.nDataToTransfer   =
                             myRegularSequenceConfig.sequenceChannels.getSize();
    dmaChannelConfig.peripheralAddress = (uint32_t) &(myAdc->DR);
    dmaChannelConfig.memoryAddress     = (uint32_t) &(myRegularValues[0]);

    myDmaChannel->setConfig(dmaChannelConfig);
}

//------------------------------------------------------------------------------
AdcSTM32F10x::Error AdcSTM32F10x::readChannelVoltageV(const ChannelId channelId,
                                                      VoltageV& voltageV)
{
    uint8_t valueIndex;

    if (myLastConversionType == CONVERSION_TYPE_REGULAR)
    {
        valueIndex = myRegularChannelToValueMap[channelId];
        voltageV = ((VoltageV) myRegularValues[valueIndex]) * myLevelsToVolts;
    }
    else if (myLastConversionType == CONVERSION_TYPE_INJECTED)
    {
        valueIndex = myInjectedChannelToValueMap[channelId];
        voltageV = ((VoltageV) getInjectedData(valueIndex)) * myLevelsToVolts;
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Plat4m::VoltageV AdcSTM32F10x::readChannelVoltageVFast(
                                                      const ChannelId channelId)
{
    uint8_t valueIndex;
    VoltageV voltageV;

    if (myLastConversionType == CONVERSION_TYPE_REGULAR)
    {
        valueIndex = myRegularChannelToValueMap[channelId];
        voltageV = ((VoltageV) myRegularValues[valueIndex]) * myLevelsToVolts;
    }
    else if (myLastConversionType == CONVERSION_TYPE_INJECTED)
    {
        valueIndex = myInjectedChannelToValueMap[channelId];
        voltageV = ((VoltageV) getInjectedData(valueIndex)) * myLevelsToVolts;
    }

    return voltageV;
}

//------------------------------------------------------------------------------
AdcSTM32F10x::Error AdcSTM32F10x::readAllVoltages()
{
    while (myCurrentRegularValueIndex < myRegularSequenceLength)
    {
        startRegularConversion();

        while (!isConversionComplete())
        {
            // Do nothing
        }

        myLastConversionType = CONVERSION_TYPE_REGULAR;
        myRegularValues[myCurrentRegularValueIndex] = getRegularData();
//        setBits(myAdc->SR, ADC_SR_EOC);
        myCurrentRegularValueIndex++;
    }

    myCurrentRegularValueIndex = 0;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setEndOfRegularConversionInterruptCallback(
                              EndOfRegularConversionInterruptCallback& callback)
{
    myEndOfRegularConversionInterruptCallback = &callback;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setEndOfInjectedConversionInterruptCallback(
                             EndOfInjectedConversionInterruptCallback& callback)
{
    myEndOfInjectedConversionInterruptCallback = &callback;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setAnalogWatchdogInterruptCallback(
                                      AnalogWatchdogInterruptCallback& callback)
{
    myAnalogWatchdogInterruptCallback = &callback;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setDualMode(const DualMode dualMode)
{
    clearAndSetBits(myAdc->CR1, ADC_CR1_DUALMOD, ((uint32_t) dualMode) << 16);
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error AdcSTM32F10x::driverSetEnabled(const bool enabled)
{
    clearAndSetBits(RCC->CFGR, RCC_CFGR_ADCPRE, RCC_CFGR_ADCPRE_DIV6);

    ProcessorSTM32F10x::setPeripheralClockEnabled(myPeripheralMap[myId],
                                                  enabled);

//    privateSetEnabled(enabled);
    
    if (enabled)
    {
        privateSetEnabled(true);

        // Perform calibration
        setBits(myAdc->CR2, ADC_CR2_RSTCAL);
        setBits(myAdc->CR2, ADC_CR2_CAL);

        while (areBitsSet(myAdc->CR2, ADC_CR2_CAL))
        {
            // Do nothing
        }

        privateSetEnabled(false);
    }

    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION) ||
        isInterruptEventEnabled(INTERRUPT_EVENT_ANALOG_WATCHDOG)           ||
        isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION))
    {
        myInterrupt.setEnabled(enabled);
    }

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AdcSTM32F10x::privateSetEnabled(const bool enabled)
{
    setBitsSet(myAdc->CR2, ADC_CR2_ADON, enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setScanModeEnabled(const bool enabled)
{
    setBitsSet(myAdc->CR1, ADC_CR1_SCAN, (bool) enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setAlignment(const Alignment alignment)
{
    setBitsSet(myAdc->CR2, ADC_CR2_ALIGN, (bool) alignment);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setRegularSequenceLength(const uint32_t sequenceLength)
{
    clearAndSetBits(myAdc->SQR1, ADC_SQR1_L, (sequenceLength - 1) << 20);
    myRegularSequenceLength = sequenceLength;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setRegularSequenceConversionChannel(
                                                      const uint32_t conversion,
                                                      const ChannelId channelId)
{
    if (conversion <= 6)
    {
        uint8_t offset = ((conversion - 1) * 5);
        clearAndSetBits(myAdc->SQR3,
                        ((uint32_t) 0x1F) << offset,
                        ((uint32_t) channelId) << offset);
    }
    else if (conversion <= 12)
    {
        uint8_t offset = ((conversion - 1) - 6) * 6;
        clearAndSetBits(myAdc->SQR2,
                        ((uint32_t) 0x1F) << offset,
                        ((uint32_t) channelId) << offset);
    }
    else
    {
        uint8_t offset = ((conversion - 1) - 12) * 6;
        clearAndSetBits(myAdc->SQR1,
                        ((uint32_t) 0x1F) << offset,
                        ((uint32_t) channelId) << offset);
    }
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setInjectedSequenceLength(const uint32_t sequenceLength)
{
    // TODO: Injected sequence length can only be <= 4
    clearAndSetBits(myAdc->JSQR, ADC_JSQR_JL, (sequenceLength - 1) << 20);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setInjectedSequenceConversionChannel(
                                                      const uint32_t conversion,
                                                      const ChannelId channelId)
{
    uint8_t offset = ((conversion - 1) * 5);
    clearAndSetBits(myAdc->JSQR,
                    ((uint32_t) 0x1F) << offset,
                    ((uint32_t) channelId) << offset);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setRegularExternalTriggerEnabled(const bool enabled)
{
    setBitsSet(myAdc->CR2, ADC_CR2_EXTTRIG, enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setRegularExternalTrigger(
                            const RegularExternalTrigger regularExternalTrigger)
{
    clearAndSetBits(myAdc->CR2,
                    ADC_CR2_EXTSEL,
                    ((uint32_t) regularExternalTrigger) << 17);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setInjectedExternalTriggerEnabled(const bool enabled)
{
    setBitsSet(myAdc->CR2, ADC_CR2_JEXTTRIG, enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setInjectedExternalTrigger(
                          const InjectedExternalTrigger injectedExternalTrigger)
{
    clearAndSetBits(myAdc->CR2,
                    ADC_CR2_JEXTSEL,
                    ((uint32_t) injectedExternalTrigger) << 12);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setDmaEnabled(const bool enabled)
{
    setBitsSet(myAdc->CR2, ADC_CR2_DMA, enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                            const bool enabled)
{
    setBitsSet(myAdc->CR1, ((uint32_t) 0x1) << (5 + interruptEvent), enabled);
}

//------------------------------------------------------------------------------
bool AdcSTM32F10x::isInterruptEventEnabled(const InterruptEvent interruptEvent)
{
    return (areBitsSet(myAdc->CR1, ((uint32_t) 0x1) << (5 + interruptEvent)));
}

//------------------------------------------------------------------------------
bool AdcSTM32F10x::isInterruptEventPending(const InterruptEvent interruptEvent)
{
    uint32_t shiftIndex = 0;

    switch (interruptEvent)
    {
        case INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION:
        {
            shiftIndex = 1;

            break;
        }
        case INTERRUPT_EVENT_ANALOG_WATCHDOG:
        {
            shiftIndex = 0;

            break;
        }
        case INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION:
        {
            shiftIndex = 2;

            break;
        }
    }

    return (areBitsSet(myAdc->SR, ((uint32_t) 0x1) << shiftIndex));
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setChannelSampleTime(const ChannelId channelId,
                                        const SampleTime sampleTime)
{
    if (channelId <= CHANNEL_ID_9)
    {
        uint8_t offset = (channelId * 3);
        clearAndSetBits(myAdc->SMPR2,
                        ((uint32_t) 0x7) << offset,
                        ((uint32_t) sampleTime) << offset);
    }
    else
    {
        uint8_t offset = (channelId - 10) * 3;
        clearAndSetBits(myAdc->SMPR1,
                        ((uint32_t) 0x7) << offset,
                        ((uint32_t) sampleTime) << offset);
    }
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::startRegularConversion()
{
    setBits(myAdc->CR2, ADC_CR2_SWSTART);
}

//------------------------------------------------------------------------------
bool AdcSTM32F10x::isConversionComplete()
{
    return (areBitsSet(myAdc->SR, ADC_SR_EOC));
}

//------------------------------------------------------------------------------
uint16_t AdcSTM32F10x::getRegularData()
{
    return (myAdc->DR);
}

//------------------------------------------------------------------------------
uint16_t AdcSTM32F10x::getInjectedData(const uint32_t conversionIndex)
{
    uint16_t value;

    switch (conversionIndex)
    {
        case 0:
        {
            value = myAdc->JDR1;

            break;
        }
        case 1:
        {
            value = myAdc->JDR2;

            break;
        }
        case 2:
        {
            value = myAdc->JDR3;

            break;
        }
        case 3:
        {
            value = myAdc->JDR4;

            break;
        }
        default:
        {
            break;
        }
    }

    return value;
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::setInjectedDiscontinuousModeEnabled(const bool enabled)
{
    setBitsSet(myAdc->CR1, ADC_CR1_JDISCEN, enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::endOfRegularConversionInterruptEventHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION))
    {
        myLastConversionType = CONVERSION_TYPE_REGULAR;

        myRegularValues[myCurrentRegularValueIndex] = getRegularData();
        myCurrentRegularValueIndex++;
    }

    clearBits(myAdc->SR, ADC_SR_EOC);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::endOfInjectedConversionInterruptEventHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION))
    {
        myLastConversionType = CONVERSION_TYPE_INJECTED;

        if (isValidPointer(myEndOfInjectedConversionInterruptCallback))
        {
            myEndOfInjectedConversionInterruptCallback->call();
        }
    }

    clearBits(myAdc->SR, ADC_SR_JEOC);
}

//------------------------------------------------------------------------------
void AdcSTM32F10x::analogWatchdogInterruptEventHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_ANALOG_WATCHDOG))
    {
        if (isValidPointer(myAnalogWatchdogInterruptCallback))
        {
            myAnalogWatchdogInterruptCallback->call();
        }
    }

    clearBits(myAdc->SR, ADC_SR_AWD);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AdcSTM32F10x::interruptHandler()
{
    // End of regular conversion interrupt
    if (isInterruptEventPending(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION))
    {
        endOfRegularConversionInterruptEventHandler();
    }
    // End of injected conversion interrupt
    else if (isInterruptEventPending(
                                    INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION))
    {
        endOfInjectedConversionInterruptEventHandler();
    }
    // Analog watchdog 1 interrupt
    else if (isInterruptEventPending(INTERRUPT_EVENT_ANALOG_WATCHDOG))
    {
        analogWatchdogInterruptEventHandler();
    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void ADC1_2_IRQHandler(void)
{
    //
    // ADC1 interrupt
    //
    // Pseudocode:
    // if (areAnyAdc1InterruptsEnabled() && areAnyAdc1InterruptsActive())
    //
    if (((ADC1->CR1 & 0xE0) != 0) && ((ADC1->SR & 0x7) != 0))
    {
        interruptObjectMap[AdcSTM32F10x::ID_1]->handler();
    }

    //
    // ADC2 interrupt
    //
    // Pseudocode:
    // if (areAnyAdc2InterruptsEnabled() && areAnyAdc2InterruptsActive())
    //
    if (((ADC2->CR1 & 0xE0) != 0) && ((ADC2->SR & 0x7) != 0))
    {
        interruptObjectMap[AdcSTM32F10x::ID_2]->handler();
    }
}

//------------------------------------------------------------------------------
extern "C" void ADC3_IRQHandler(void)
{
    interruptObjectMap[AdcSTM32F10x::ID_3]->handler();
}
