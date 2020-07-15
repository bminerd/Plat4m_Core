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
/// @file AdcSTM32F30x.cpp
/// @author Ben Minerd
/// @date 7/12/2016
/// @brief AdcSTM32F30x class source file.
///

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Plat4m_Core/STM32F30x/AdcSTM32F30x.h>
#include <Plat4m_Core/Processor.h>
#include <Plat4m_Core/CallbackMethod.h>

// TODO Create a way to delay without System include
#include <Plat4m_Core/System.h>

using Plat4m::AdcSTM32F30x;
using Plat4m::Module;
using Plat4m::ProcessorSTM32F30x;
using Plat4m::InterruptSTM32F30x;

//------------------------------------------------------------------------------
// Local variables
//------------------------------------------------------------------------------

// This table is local static because it needs to be accessed by c-style ISRs
// (bottom of this file)
// TODO:
static InterruptSTM32F30x* interruptObjectMap[4];

//------------------------------------------------------------------------------
// Private static data members
//------------------------------------------------------------------------------

// Constants

const ProcessorSTM32F30x::Peripheral AdcSTM32F30x::myPeripheralMap[] =
{
    ProcessorSTM32F30x::PERIPHERAL_ADC_1_2, /// ID_1
    ProcessorSTM32F30x::PERIPHERAL_ADC_1_2, /// ID_2
    ProcessorSTM32F30x::PERIPHERAL_ADC_3_4, /// ID_3
    ProcessorSTM32F30x::PERIPHERAL_ADC_3_4  /// ID_4
};

const InterruptSTM32F30x::Id AdcSTM32F30x::myInterruptIdMap[] =
{
    InterruptSTM32F30x::ID_ADC_1_2, /// ID_1
    InterruptSTM32F30x::ID_ADC_1_2, /// ID_2
    InterruptSTM32F30x::ID_ADC_3,   /// ID_3
    InterruptSTM32F30x::ID_ADC_4    /// ID_4
};

const uint32_t AdcSTM32F30x::myResolutionMap[] =
{
    ADC_CFGR_RES_1 | ADC_CFGR_RES_0, /// RESOLUTION_6_BIT
    ADC_CFGR_RES_1,                  /// RESOLUTION_8_BIT
    ADC_CFGR_RES_0,                  /// RESOLUTION_10_BIT
    0                                /// RESOLUTION_12_BIT
};

const float AdcSTM32F30x::myResolutionMaxValueMap[] =
{
    63.0,   /// RESOLUTION_6_BIT
    255.0,  /// RESOLUTION_8_BIT
    1023.0, /// RESOLUTION_10_BIT
    4095.0  /// RESOLUTION_12_BIT
};

// Variables

ADC_TypeDef* AdcSTM32F30x::myAdcMap[] =
{
    ADC1, /// ADC_1
    ADC2, /// ADC_2
    ADC3, /// ADC_3
    ADC4  /// ADC_4
};

AdcSTM32F30x::CommonConfig AdcSTM32F30x::myAdc1And2CommonConfig;
AdcSTM32F30x::CommonConfig AdcSTM32F30x::myAdc3And4CommonConfig;

//------------------------------------------------------------------------------
// Public constructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F30x::AdcSTM32F30x(const Id id) :
    myId(id),
    myAdc(myAdcMap[myId]),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &AdcSTM32F30x::interruptHandler)),
    myDmaChannel(0),
    myConfig(),
    myChannelConfigs(),
    myLevelsToVolts(0.0),
    myRegularValues(),
    myCurrentValueIndex(0),
    myRegularChannelToValueMap(),
    myInjectedChannelToValueMap(),
    myEndOfRegularConversionInterruptCallback(0),
    myEndOfInjectedConversionInterruptCallbacks(),
    myEndOfInjectedSequenceInterruptCallback(0),
    myAnalogWatchdogInterruptCallback(0),
    myInjectedSequenceLength(0),
    myCurrentInjectedConversionIndex(0),
    myOffsetValueCount(0),
    myChannelToOffsetValueIndexMap(),
    myIsOffsetCalibrationRunningMap(),
    myOffsetCalibrationTotalNSamplesMap(),
    myOffsetCalibrationCurrentNSamplesMap(),
    myOffsetValueSumMap(),
	myInjectedValueMap(),
	myChannelConversionTypeMap(),
	myMasterAdc(0),
	mySlaveAdc(0)
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
AdcSTM32F30x::AdcSTM32F30x(const Id id, AdcSTM32F30x& masterAdc) :
    myId(id),
    myAdc(myAdcMap[myId]),
    myInterrupt(myInterruptIdMap[myId],
                createCallback(this, &AdcSTM32F30x::interruptHandler)),
    myDmaChannel(0),
    myConfig(),
    myChannelConfigs(),
    myLevelsToVolts(0.0),
    myRegularValues(),
	myRegularSequenceLength(0),
    myCurrentValueIndex(0),
    myRegularChannelToValueMap(),
    myInjectedChannelToValueMap(),
    myEndOfRegularConversionInterruptCallback(0),
    myEndOfInjectedConversionInterruptCallbacks(),
    myEndOfInjectedSequenceInterruptCallback(0),
    myAnalogWatchdogInterruptCallback(0),
    myInjectedSequenceLength(0),
    myCurrentInjectedConversionIndex(0),
    myOffsetValueCount(0),
    myChannelToOffsetValueIndexMap(),
    myIsOffsetCalibrationRunningMap(),
    myOffsetCalibrationTotalNSamplesMap(),
    myOffsetCalibrationCurrentNSamplesMap(),
    myOffsetValueSumMap(),
	myInjectedValueMap(),
	myValueSumMap(),
	myChannelConversionTypeMap(),
	myMasterAdc(&masterAdc),
	mySlaveAdc(0)
{
    if (isValidPointer(interruptObjectMap[myId]))
    {
        // Trying to instantiate this peripheral twice, lockup
        while (true)
        {
        }
    }

    interruptObjectMap[myId] = &myInterrupt;

    myMasterAdc->mySlaveAdc = this;
}

//------------------------------------------------------------------------------
// Public virtual destructors
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
AdcSTM32F30x::~AdcSTM32F30x()
{
}

//------------------------------------------------------------------------------
// Public static methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AdcSTM32F30x::setCommonConfig(const CommonId id, const CommonConfig config)
{
	if (id == COMMON_ID_1_2)
	{
	    clearBits(RCC->CFGR2, RCC_CFGR2_ADCPRE12);
	    setBits(RCC->CFGR2, RCC_CFGR2_ADCPRE12_DIV1);

		if (config.clockMode <= CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_256)
		{
			setBits(RCC->CFGR2, ((uint32_t) config.clockMode) << 4);
		}

		ProcessorSTM32F30x::setPeripheralClockEnabled(
										 ProcessorSTM32F30x::PERIPHERAL_ADC_1_2,
										 true);

		if ((config.clockMode >= CLOCK_MODE_SYNCHRONOUS_HCLK_DIVIDE_BY_1) &&
			(config.clockMode <= CLOCK_MODE_SYNCHRONOUS_HCLK_DIVIDE_BY_4))
		{
			clearAndSetBits(ADC1_2->CCR,
							ADC12_CCR_CKMODE,
							(((uint32_t) config.clockMode) - 11) << 16);
			clearBits(RCC->CFGR2, RCC_CFGR2_ADCPRE12);
		}

		if (config.dualMode <=
					   DUAL_MODE_COMBINED_INTERLEAVED_AND_INJECTED_SIMULTANEOUS)
		{
			clearAndSetBits(ADC1_2->CCR,
							ADC12_CCR_MULTI,
							(uint32_t) config.dualMode);
		}
		else
		{
			clearAndSetBits(ADC1_2->CCR,
							ADC12_CCR_MULTI,
							((uint32_t) config.dualMode) + 1);
		}

		clearBits(ADC1_2->CCR, ADC12_CCR_VBATEN);

		if (config.vBatEnabled)
		{
			setBits(ADC1_2->CCR, ADC12_CCR_VBATEN);
		}

		clearBits(ADC1_2->CCR, ADC12_CCR_TSEN);

		if (config.temperatureSensorEnabled)
		{
			setBits(ADC1_2->CCR, ADC12_CCR_TSEN);
		}

		clearBits(ADC1_2->CCR, ADC12_CCR_VREFEN);

		if (config.vRefintEnabled)
		{
			setBits(ADC1_2->CCR, ADC12_CCR_VREFEN);
		}

		myAdc1And2CommonConfig = config;
	}
	else if (id == COMMON_ID_3_4)
	{
		clearBits(RCC->CFGR2, RCC_CFGR2_ADCPRE34);

		if (config.clockMode <= CLOCK_MODE_ASYNCHRONOUS_DIVIDE_BY_256)
		{
			setBits(RCC->CFGR2, ((uint32_t) config.clockMode) << 9);
		}

		ProcessorSTM32F30x::setPeripheralClockEnabled(
										 ProcessorSTM32F30x::PERIPHERAL_ADC_3_4,
										 true);

		if ((config.clockMode >= CLOCK_MODE_SYNCHRONOUS_HCLK_DIVIDE_BY_1) &&
			(config.clockMode <= CLOCK_MODE_SYNCHRONOUS_HCLK_DIVIDE_BY_4))
		{
			clearAndSetBits(ADC3_4->CCR,
							ADC34_CCR_CKMODE,
							(((uint32_t) config.clockMode) - 11) << 16);
		}

		clearAndSetBits(ADC3_4->CCR,
						ADC34_CCR_MULTI,
						(uint32_t) config.dualMode);

		myAdc3And4CommonConfig = config;
	}
}

//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ADC_TypeDef* AdcSTM32F30x::getAdc()
{
    return myAdc;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setConfig(const Config& config)
{
    setResolution(config.resolution);
    setAlignment(config.alignment);
    setDmaEnabled(config.dmaEnabled);

    if (config.dmaEnabled)
    {
        setDmaMode(config.dmaMode);
    }

    InterruptSTM32F30x::Config interruptConfig;
    interruptConfig.priority = 6;
//    interruptConfig.priority = 5;
    myInterrupt.configure(interruptConfig);

    myConfig = config;

    myLevelsToVolts = Processor::getCoreVoltageV() /
                      myResolutionMaxValueMap[config.resolution];
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setRegularSequenceConfig(const RegularSequenceConfig& config)
{
    uint32_t size = config.sequenceChannels.getSize();
    setRegularSequenceLength(size);

    for (uint32_t i = 0; i < size; i++)
    {
        setRegularSequenceConversionChannel(i + 1,
                                            config.sequenceChannels[i]);
        myRegularChannelToValueMap[(config.sequenceChannels[i])] = i;
        myChannelConversionTypeMap[(config.sequenceChannels[i])] =
        												CONVERSION_TYPE_REGULAR;
    }

    if (config.externalTriggerMode != EXTERNAL_TRIGGER_MODE_DISABLED)
    {
        setRegularExternalTriggerMode(config.externalTriggerMode);
        setRegularExternalTrigger(config.externalTrigger);

        setInterruptEventEnabled(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION,
                                 true);
        myInterrupt.setEnabled(true);
    }

    myRegularSequenceConfig = config;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setInjectedSequenceConfig(
                                           const InjectedSequenceConfig& config)
{
    uint32_t jsqrValue = 0;

    uint32_t size = config.sequenceChannels.getSize();

    setInjectedDiscontinuousModeEnabled(config.discontinuousModeEnabled);

    jsqrValue |= getInjectedSequenceLengthBits(size);

    for (uint32_t i = 0; i < size; i++)
    {
        jsqrValue |= getInjectedSequenceConversionChannelBits(
                                                    i + 1,
                                                    config.sequenceChannels[i]);
        myInjectedChannelToValueMap[(config.sequenceChannels[i])] = i;
        myChannelConversionTypeMap[(config.sequenceChannels[i])] =
        											   CONVERSION_TYPE_INJECTED;
    }

    for (uint32_t i = size; i < 4; i++)
    {
        jsqrValue |= getInjectedSequenceConversionChannelBits(
                                                    i + 1,
                                                    CHANNEL_ID_1);
    }

    if (config.externalTriggerMode != EXTERNAL_TRIGGER_MODE_DISABLED)
    {
        jsqrValue |= getInjectedExternalTriggerModeBits(
                                                    config.externalTriggerMode);
        jsqrValue |= getInjectedExternalTriggerBits(config.externalTrigger);

//        myInterrupt.setEnabled(true);
    }

    // TODO: This will need to be looked at if both interrupts are needed
    if (config.discontinuousModeEnabled)
    {
    	setInterruptEventEnabled(INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION,
    							 true);
    }
    else
    {
    	setInterruptEventEnabled(INTERRUPT_EVENT_END_OF_INJECTED_SEQUENCE,
    							 true);
    }

    myInterrupt.setEnabled(true);

    privateSetEnabled(false);

    myAdc->JSQR = jsqrValue;

	privateSetEnabled(true);

	// Block until ready
	while (!isReady())
	{
	}

    myInjectedSequenceConfig = config;
}

//------------------------------------------------------------------------------
AdcSTM32F30x::Config AdcSTM32F30x::getConfig() const
{
    return myConfig;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setChannelConfig(const ChannelId channelId,
                                    const ChannelConfig& channelConfig)
{
    setChannelSampleTime(channelId, channelConfig.sampleTime);

    myChannelConfigs[channelId] = channelConfig;
}

//------------------------------------------------------------------------------
AdcSTM32F30x::ChannelConfig AdcSTM32F30x::getChannelConfig(
                                                const ChannelId channelId) const
{
    return myChannelConfigs[channelId];
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setDmaChannel(DmaChannelSTM32F30x& dmaChannel)
{
    // TODO Channel / peripheral error checking?

    myDmaChannel = &dmaChannel;

    DmaChannelSTM32F30x::Config dmaChannelConfig;
    dmaChannelConfig.dataTransferDirection =
              DmaChannelSTM32F30x::DATA_TRANSFER_DIRECTION_READ_FROM_PERIPHERAL;
    dmaChannelConfig.circularModeEnabled            = true,
    dmaChannelConfig.peripheralIncrementModeEnabled = false;
    dmaChannelConfig.memoryIncrementModeEnabled     = true;
    dmaChannelConfig.peripheralSize =
                                   DmaChannelSTM32F30x::PERIPHERAL_SIZE_16_BITS;
    dmaChannelConfig.memorySize      = DmaChannelSTM32F30x::MEMORY_SIZE_16_BITS;
    dmaChannelConfig.priorityLevel   = DmaChannelSTM32F30x::PRIORITY_LEVEL_HIGH;
    dmaChannelConfig.memoryToMemoryModeEnabled      = false;
    dmaChannelConfig.nDataToTransfer      =
                             myRegularSequenceConfig.sequenceChannels.getSize();
    dmaChannelConfig.peripheralAddress              = (uint32_t) &(myAdc->DR);
    dmaChannelConfig.memoryAddress           = (uint32_t) &(myRegularValues[0]);

    myDmaChannel->setConfig(dmaChannelConfig);
}

//------------------------------------------------------------------------------
AdcSTM32F30x::Error AdcSTM32F30x::readChannelVoltageV(const ChannelId channelId,
                                                      VoltageV& voltageV)
{
    uint8_t valueIndex;

    if (myChannelConversionTypeMap[channelId] == CONVERSION_TYPE_REGULAR)
    {
        valueIndex = myRegularChannelToValueMap[channelId];
        voltageV = ((VoltageV) myRegularValues[valueIndex]) * myLevelsToVolts;
    }
    else if (myChannelConversionTypeMap[channelId] == CONVERSION_TYPE_INJECTED)
    {
        valueIndex = myInjectedChannelToValueMap[channelId];
        voltageV =
        	      ((VoltageV) myInjectedValueMap[valueIndex]) * myLevelsToVolts;
    }

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
Plat4m::VoltageV AdcSTM32F30x::readChannelVoltageVFast(
                                                      const ChannelId channelId)
{
    uint8_t valueIndex;
    VoltageV voltageV;

    if (myChannelConversionTypeMap[channelId] == CONVERSION_TYPE_REGULAR)
    {
        valueIndex = myRegularChannelToValueMap[channelId];
        voltageV = ((VoltageV) myRegularValues[valueIndex]) * myLevelsToVolts;
    }
    else if (myChannelConversionTypeMap[channelId] == CONVERSION_TYPE_INJECTED)
    {
        valueIndex = myInjectedChannelToValueMap[channelId];
        voltageV =
        	      ((VoltageV) myInjectedValueMap[valueIndex]) * myLevelsToVolts;
    }

    return voltageV;
}

//------------------------------------------------------------------------------
AdcSTM32F30x::Error AdcSTM32F30x::readAllVoltages()
{
    startRegularConversion();

    while (!isSequenceComplete())
    {
        while (!isConversionComplete())
        {
            // Do nothing
        }

        myRegularValues[myCurrentValueIndex] = getRegularData();
        myCurrentValueIndex++;
    }

    setBits(myAdc->ISR, ADC_ISR_EOS);
    myCurrentValueIndex = 0;

    return Error(ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setEndOfRegularConversionInterruptCallback(
                              EndOfRegularConversionInterruptCallback& callback)
{
    myEndOfRegularConversionInterruptCallback = &callback;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setEndOfRegularSequenceInterruptCallback(
                                EndOfRegularSequenceInterruptCallback& callback)
{
    myEndOfRegularSequenceInterruptCallback = &callback;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setEndOfInjectedConversionInterruptCallback(
                             const uint8_t conversionIndex,
                             EndOfInjectedConversionInterruptCallback& callback)
{
    myEndOfInjectedConversionInterruptCallbacks[conversionIndex] = &callback;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setEndOfInjectedSequenceInterruptCallback(
                               EndOfInjectedSequenceInterruptCallback& callback)
{
    myEndOfInjectedSequenceInterruptCallback = &callback;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setAnalogWatchdogInterruptCallback(
                                      AnalogWatchdogInterruptCallback& callback)
{
    myAnalogWatchdogInterruptCallback = &callback;
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::setChannelOffsetValue(const ChannelId channelId,
                                         const uint16_t offsetValue)
{
    if (myOffsetValueCount >= 4)
    {
        return false;
    }

    switch (myOffsetValueCount)
    {
        case 0:
        {
            setBits(myAdc->OFR1, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR1,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId) << 26);
            clearAndSetBits(myAdc->OFR1, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        case 1:
        {
            setBits(myAdc->OFR2, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR2,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId) << 26);
            clearAndSetBits(myAdc->OFR2, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        case 2:
        {
            setBits(myAdc->OFR3, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR3,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId) << 26);
            clearAndSetBits(myAdc->OFR3, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        case 3:
        {
            setBits(myAdc->OFR4, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR4,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId) << 26);
            clearAndSetBits(myAdc->OFR4, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        default:
        {
            break;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::setChannelOffsetVoltage(const ChannelId channelId,
                                           const VoltageV offsetVoltageV)
{
    uint16_t offsetValue =
                       (uint16_t) round(offsetVoltageV * (1.0/myLevelsToVolts));

    return setChannelOffsetValue(channelId, offsetValue);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::startChannelOffsetCalibration(const ChannelId channelId,
                                                 const uint32_t nSamples)
{
    myChannelToOffsetValueIndexMap[channelId] = myOffsetValueCount;
    myOffsetCalibrationTotalNSamplesMap[myOffsetValueCount] = nSamples;
    myOffsetCalibrationCurrentNSamplesMap[myOffsetValueCount] = 0;
    myOffsetValueSumMap[myOffsetValueCount] = 0;

    setOffsetChannel(myOffsetValueCount, channelId);

    myIsOffsetCalibrationRunningMap[myOffsetValueCount] = true;

    myOffsetValueCount++;
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isChannelOffsetCalibrationComplete(const ChannelId channelId)
{
    return (!(myIsOffsetCalibrationRunningMap[
                                 (myChannelToOffsetValueIndexMap[channelId])]));
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::startRegularConversion()
{
	if (isValidPointer(myMasterAdc))
	{
		myMasterAdc->startRegularConversion();
	}
	else
	{
		setBits(myAdc->CR, ADC_CR_ADSTART);
	}
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isRegularConversionStarted()
{
	if (isValidPointer(myMasterAdc))
	{
		return (myMasterAdc->isRegularConversionStarted());
	}

	// else
	return (areBitsSet(myAdc->CR, ADC_CR_ADSTART));
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::stopRegularConversion()
{
	if (isValidPointer(myMasterAdc))
	{
		myMasterAdc->stopRegularConversion();
	}
	else
	{
		setBits(myAdc->CR, ADC_CR_ADSTP);

		while (areBitsSet(myAdc->CR, ADC_CR_ADSTP))
		{
			// Do nothing
		}
	}
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isRegularConversionStopComplete()
{
	if (isValidPointer(myMasterAdc))
	{
		return (myMasterAdc->isRegularConversionStopComplete());
	}

	// else
	return (areBitsSet(myAdc->CR, ADC_CR_ADSTP));
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::startInjectedConversion()
{
	if (isValidPointer(myMasterAdc))
	{
		myMasterAdc->startInjectedConversion();
	}
	else
	{
		setBits(myAdc->CR, ADC_CR_JADSTART);
	}
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isInjectedConversionStarted()
{
	return (areBitsSet(myAdc->CR, ADC_CR_JADSTART));
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::stopInjectedConversion()
{
	setBits(myAdc->CR, ADC_CR_JADSTP);

	while (areBitsSet(myAdc->CR, ADC_CR_JADSTP))
	{
		// Do nothing
	}
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isInjectedConversionStopComplete()
{
	return (areBitsSet(myAdc->CR, ADC_CR_JADSTP));
}

//------------------------------------------------------------------------------
// Private methods implemented from Module
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Module::Error AdcSTM32F30x::driverSetEnabled(const bool enabled)
{
    // TODO: Come up with shared user scheme

	if (enabled)
	{
		clearBits(myAdc->CR, ADC_CR_ADVREGEN);
		setBits(myAdc->CR, ADC_CR_ADVREGEN_0);

		uint32_t counter = 1000000;

		System::delayTimeMs(10);

		// Perform calibration

		clearBits(myAdc->CR, ADC_CR_ADCALDIF);
		setBits(myAdc->CR, ADC_CR_ADCAL);

		// Block until calibration is complete
		while (areBitsSet(myAdc->CR, ADC_CR_ADCAL))
		{
		}

		uint16_t calFact = myAdc->CALFACT;

		counter = 100;

		while (counter--)
		{
			// Do nothing
		}

		privateSetEnabled(true);

		// Block until ready
		while (!isReady())
		{
		}
	}
	else
	{
		privateSetEnabled(false);
	}

    return Module::Error(Module::ERROR_CODE_NONE);
}

//------------------------------------------------------------------------------
// Private inline methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AdcSTM32F30x::privateSetEnabled(const bool enabled)
{
    if (enabled)
    {
        setBits(myAdc->CR, ADC_CR_ADEN);
    }
    else
    {
        setBits(myAdc->CR, ADC_CR_ADDIS);
    }
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setResolution(const Resolution resolution)
{
    clearAndSetBits(myAdc->CFGR,
                    ADC_CFGR_RES,
                    ((uint32_t) myResolutionMap[resolution]) << 3);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setAlignment(const Alignment alignment)
{
    clearAndSetBits(myAdc->CFGR, ADC_CFGR_ALIGN, (uint32_t) alignment);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setRegularSequenceLength(const uint32_t sequenceLength)
{
    clearAndSetBits(myAdc->SQR1, 0xF, sequenceLength - 1);
    myRegularSequenceLength = sequenceLength;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setRegularSequenceConversionChannel(
                                                      const uint32_t conversion,
                                                      const ChannelId channelId)
{
    if (conversion <= 4)
    {
        uint8_t offset = ((conversion - 1) * 6) + 6;
        clearAndSetBits(myAdc->SQR1,
                        ((uint32_t) 0x1F) << offset,
                        ((uint32_t) (channelId + 1)) << offset);
    }
    else if (conversion <= 9)
    {
        uint8_t offset = ((conversion - 1) - 4) * 6;
        clearAndSetBits(myAdc->SQR2,
                        ((uint32_t) 0x1F) << offset,
                        ((uint32_t) (channelId + 1)) << offset);
    }
    else if (conversion <= 14)
    {
        uint8_t offset = ((conversion - 1) - 9) * 6;
        clearAndSetBits(myAdc->SQR3,
                        ((uint32_t) 0x1F) << offset,
                        ((uint32_t) (channelId + 1)) << offset);
    }
    else
    {
        uint8_t offset = ((conversion - 1) - 14) * 6;
        clearAndSetBits(myAdc->SQR4,
                        ((uint32_t) 0x1F) << offset,
                        ((uint32_t) (channelId + 1)) << offset);
    }
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setInjectedDiscontinuousModeEnabled(const bool enabled)
{
    setBitsSet(myAdc->CFGR, ADC_CFGR_JDISCEN, enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setInjectedSequenceLength(const uint32_t sequenceLength)
{
    clearAndSetBits(myAdc->JSQR, ADC_JSQR_JL, sequenceLength - 1);

    myInjectedSequenceLength = sequenceLength;
}

//------------------------------------------------------------------------------
uint32_t AdcSTM32F30x::getInjectedSequenceLengthBits(
                                                  const uint32_t sequenceLength)
{
    myInjectedSequenceLength = sequenceLength;

    return ((uint32_t) (sequenceLength - 1));
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setInjectedSequenceConversionChannel(
                                                      const uint32_t conversion,
                                                      const ChannelId channelId)
{
    uint8_t offset = ((conversion - 1) * 6) + 8;
    clearAndSetBits(myAdc->JSQR,
                    ((uint32_t) 0x1F) << offset,
                    ((uint32_t) (channelId + 1)) << offset);
}

//------------------------------------------------------------------------------
uint32_t AdcSTM32F30x::getInjectedSequenceConversionChannelBits(
                                                      const uint32_t conversion,
                                                      const ChannelId channelId)
{
    uint8_t offset = ((conversion - 1) * 6) + 8;

    return (((uint32_t) (channelId + 1)) << offset);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setRegularExternalTriggerMode(
                                  const ExternalTriggerMode externalTriggerMode)
{
    clearAndSetBits(myAdc->CFGR,
                    ADC_CFGR_EXTEN,
                    ((uint32_t) externalTriggerMode) << 10);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setRegularExternalTrigger(
                            const RegularExternalTrigger regularExternalTrigger)
{
    clearAndSetBits(myAdc->CFGR,
                    ADC_CFGR_EXTSEL,
                    ((uint32_t) regularExternalTrigger) << 6);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setInjectedExternalTriggerMode(
                                  const ExternalTriggerMode externalTriggerMode)
{
    clearAndSetBits(myAdc->JSQR,
                    ADC_JSQR_JEXTEN,
                    ((uint32_t) externalTriggerMode) << 6);
}

//------------------------------------------------------------------------------
uint32_t AdcSTM32F30x::getInjectedExternalTriggerModeBits(
                                  const ExternalTriggerMode externalTriggerMode)
{
    return (((uint32_t) externalTriggerMode) << 6);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setInjectedExternalTrigger(
                          const InjectedExternalTrigger injectedExternalTrigger)
{
    clearAndSetBits(myAdc->JSQR,
                    ADC_JSQR_JEXTSEL,
                    ((uint32_t) injectedExternalTrigger) << 2);
}

//------------------------------------------------------------------------------
uint32_t AdcSTM32F30x::getInjectedExternalTriggerBits(
                          const InjectedExternalTrigger injectedExternalTrigger)
{
    return (((uint32_t) injectedExternalTrigger) << 2);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setDmaEnabled(const bool enabled)
{
    setBitsSet(myAdc->CFGR, ADC_CFGR_DMAEN, enabled);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setDmaMode(const DmaMode dmaMode)
{
    clearAndSetBits(myAdc->CFGR,
                    ADC_CFGR_DMACFG,
                    ((uint32_t) dmaMode) << 1);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setInterruptEventEnabled(const InterruptEvent interruptEvent,
                                            const bool enabled)
{
    setBitsSet(myAdc->IER, ((uint32_t) 0x1) << interruptEvent, enabled);
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isInterruptEventEnabled(const InterruptEvent interruptEvent)
{
    return areBitsSet(myAdc->IER, ((uint32_t) 0x1) << interruptEvent);
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isInterruptEventPending(const InterruptEvent interruptEvent)
{
    return (areBitsSet(myAdc->ISR, ((uint32_t) 0x1) << interruptEvent));
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setChannelSampleTime(const ChannelId channelId,
                                        const SampleTime sampleTime)
{
    if (channelId <= CHANNEL_ID_9)
    {
        uint8_t offset = (channelId * 3) + 3;
        clearAndSetBits(myAdc->SMPR1,
                        ((uint32_t) 0x7) << offset,
                        ((uint32_t) sampleTime) << offset);
    }
    else
    {
        uint8_t offset = (channelId - 9) * 3;
        clearAndSetBits(myAdc->SMPR2,
                        ((uint32_t) 0x7) << offset,
                        ((uint32_t) sampleTime) << offset);
    }
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isReady()
{
    return (areBitsSet(myAdc->ISR, ADC_ISR_ADRD));
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isConversionComplete()
{
    return (areBitsSet(myAdc->ISR, ADC_ISR_EOC));
}

//------------------------------------------------------------------------------
bool AdcSTM32F30x::isSequenceComplete()
{
    return (areBitsSet(myAdc->ISR, ADC_ISR_EOS));
}

//------------------------------------------------------------------------------
int16_t AdcSTM32F30x::getRegularData()
{
    return (myAdc->DR);
}

//------------------------------------------------------------------------------
int16_t AdcSTM32F30x::getInjectedData(const uint32_t conversionIndex)
{
    int16_t data;

    switch (conversionIndex)
    {
        case 0:
        {
            data = (*((int16_t*) &(myAdc->JDR1)));

            break;
        }
        case 1:
        {
            data = (*((int16_t*) &(myAdc->JDR2)));

            break;
        }
        case 2:
        {
            data = (*((int16_t*) &(myAdc->JDR3)));

            break;
        }
        case 3:
        {
            data = (*((int16_t*) &(myAdc->JDR4)));

            break;
        }
        default:
        {
            break;
        }
    }

    return data;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setOffsetValue(const uint32_t offsetIndex,
                                  const uint16_t offsetValue)
{
	bool wasRegularConversionStarted = isRegularConversionStarted();
	bool wasInjectedConversionStarted = isInjectedConversionStarted();

	if (wasRegularConversionStarted)
	{
		stopRegularConversion();
	}

	if (wasInjectedConversionStarted)
	{
		stopInjectedConversion();
	}

    switch (offsetIndex)
    {
        case 0:
        {
        	setBits(myAdc->OFR1, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR1, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        case 1:
        {
        	setBits(myAdc->OFR2, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR2, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        case 2:
        {
        	setBits(myAdc->OFR3, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR3, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        case 3:
        {
        	setBits(myAdc->OFR4, ADC_OFR1_OFFSET1_EN);
            clearAndSetBits(myAdc->OFR4, ADC_OFR1_OFFSET1, offsetValue);

            break;
        }
        default:
        {
            break;
        }
    }

	if (wasRegularConversionStarted)
	{
		startRegularConversion();
	}

	if (wasInjectedConversionStarted)
	{
		startInjectedConversion();
	}
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::setOffsetChannel(const uint32_t offsetIndex,
	     	 	 	 	 	 	    const ChannelId channelId)
{
	bool wasRegularConversionStarted = isRegularConversionStarted();
	bool wasInjectedConversionStarted = isInjectedConversionStarted();

	if (wasRegularConversionStarted)
	{
		stopRegularConversion();
	}

	if (wasInjectedConversionStarted)
	{
		stopInjectedConversion();
	}

    switch (offsetIndex)
    {
        case 0:
        {
            clearAndSetBits(myAdc->OFR1,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId + 1) << 26);

            break;
        }
        case 1:
        {
            clearAndSetBits(myAdc->OFR2,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId + 1) << 26);

            break;
        }
        case 2:
        {
            clearAndSetBits(myAdc->OFR3,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId + 1) << 26);

            break;
        }
        case 3:
        {
            clearAndSetBits(myAdc->OFR4,
                            ADC_OFR1_OFFSET1_CH,
                            ((uint32_t) channelId + 1) << 26);

            break;
        }
        default:
        {
            break;
        }
    }

	if (wasRegularConversionStarted)
	{
		startRegularConversion();
	}

	if (wasInjectedConversionStarted)
	{
		startInjectedConversion();
	}
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::readyInterruptEventHandler()
{
    // Do nothing
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::endOfSamplingInterruptEventHandler()
{
    setBits(myAdc->ISR, ADC_ISR_EOSMP);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::endOfRegularConversionInterruptEventHandler()
{
    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION))
    {
        myRegularValues[myCurrentValueIndex] = getRegularData();
        myCurrentValueIndex++;

        if (myCurrentValueIndex == myRegularSequenceLength)
        {
        	myCurrentValueIndex = 0;
        }

        if (isValidPointer(myEndOfRegularConversionInterruptCallback))
        {
            myEndOfRegularConversionInterruptCallback->call();
        }
    }
    else
    {
    	setBits(myAdc->ISR, ADC_ISR_EOC);
    }
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::endOfRegularSequenceInterruptEventHandler()
{
	if (isValidPointer(myEndOfRegularSequenceInterruptCallback))
	{
		myEndOfRegularSequenceInterruptCallback->call();
	}

    setBits(myAdc->ISR, ADC_ISR_EOS);
    myCurrentValueIndex = 0;
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::overrunInterruptEventHandler()
{
    setBits(myAdc->ISR, ADC_ISR_OVR);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::endOfInjectedConversionInterruptEventHandler()
{
	uint8_t index = myCurrentInjectedConversionIndex;

	if (myIsOffsetCalibrationRunningMap[index])
	{
		if (isValidPointer(myEndOfInjectedConversionInterruptCallbacks[index]))
		{
			myEndOfInjectedConversionInterruptCallbacks[index]->call();
		}

		myOffsetValueSumMap[index] += getInjectedData(index);

		myOffsetCalibrationCurrentNSamplesMap[index]++;

		uint32_t currentNSamples = myOffsetCalibrationCurrentNSamplesMap[index];
		uint32_t totalNSamples   = myOffsetCalibrationTotalNSamplesMap[index];

		if (currentNSamples >= totalNSamples)
		{
			RealNumber valueSum = (RealNumber) (myOffsetValueSumMap[index]);
			RealNumber nSamples =
						   	   (RealNumber)
							   	   (myOffsetCalibrationTotalNSamplesMap[index]);

			uint16_t offsetValue = (uint16_t) round(valueSum / nSamples);
			setOffsetValue(index, offsetValue);

			myIsOffsetCalibrationRunningMap[index] = false;

			if (isValidPointer(myMasterAdc)			  &&
				!(myIsOffsetCalibrationRunningMap[0]) &&
				!(myIsOffsetCalibrationRunningMap[1]) &&
				!(myIsOffsetCalibrationRunningMap[2]) &&
				!(myIsOffsetCalibrationRunningMap[3]))
			{
				setInterruptEventEnabled(
								 	 INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION,
									 false);
			}
		}
	}
	else
	{
		myInjectedValueMap[index] = getInjectedData(index);

		if (isValidPointer(mySlaveAdc) &&
		    !mySlaveAdc->isInterruptEventEnabled(
		    						INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION))
		{
			mySlaveAdc->myInjectedValueMap[index] =
											 mySlaveAdc->getInjectedData(index);
		}

		if (isValidPointer(myEndOfInjectedConversionInterruptCallbacks[index]))
		{
			myEndOfInjectedConversionInterruptCallbacks[index]->call();
		}
	}

    myCurrentInjectedConversionIndex++;

    if (myCurrentInjectedConversionIndex > (myInjectedSequenceLength - 1))
    {
        myCurrentInjectedConversionIndex = 0;
    }
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::endOfInjectedSequenceInterruptEventHandler()
{
	for (uint8_t index = 0; index < myInjectedSequenceLength; index++)
	{
		if (myIsOffsetCalibrationRunningMap[index])
		{
			myOffsetValueSumMap[index] += getInjectedData(index);

			myOffsetCalibrationCurrentNSamplesMap[index]++;

			uint32_t currentNSamples =
								   myOffsetCalibrationCurrentNSamplesMap[index];
			uint32_t totalNSamples   =
									 myOffsetCalibrationTotalNSamplesMap[index];

			if (currentNSamples >= totalNSamples)
			{
				RealNumber valueSum = (RealNumber) (myOffsetValueSumMap[index]);
				RealNumber nSamples =
							   (RealNumber)
								   (myOffsetCalibrationTotalNSamplesMap[index]);

				uint16_t offsetValue = (uint16_t) round(valueSum / nSamples);
				setOffsetValue(index, offsetValue);

				myIsOffsetCalibrationRunningMap[index] = false;

				if (isValidPointer(myMasterAdc)			  &&
					!(myIsOffsetCalibrationRunningMap[0]) &&
					!(myIsOffsetCalibrationRunningMap[1]) &&
					!(myIsOffsetCalibrationRunningMap[2]) &&
					!(myIsOffsetCalibrationRunningMap[3]))
				{
					setInterruptEventEnabled(
									 INTERRUPT_EVENT_END_OF_INJECTED_SEQUENCE,
									 false);
				}
			}
		}
		else
		{
			myInjectedValueMap[index] = getInjectedData(index);

			if (isValidPointer(mySlaveAdc) &&
				!(mySlaveAdc->isInterruptEventEnabled(
									 INTERRUPT_EVENT_END_OF_INJECTED_SEQUENCE)))
			{
				mySlaveAdc->myInjectedValueMap[index] =
											 mySlaveAdc->getInjectedData(index);
			}
		}
	}

	if (isValidPointer(myEndOfInjectedSequenceInterruptCallback))
	{
		myEndOfInjectedSequenceInterruptCallback->call();
	}

    setBits(myAdc->ISR, ADC_ISR_JEOS);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::analogWatchdog1InterruptEventHandler()
{
    setBits(myAdc->ISR, ADC_ISR_AWD1);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::analogWatchdog2InterruptEventHandler()
{
    setBits(myAdc->ISR, ADC_ISR_AWD2);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::analogWatchdog3InterruptEventHandler()
{
    setBits(myAdc->ISR, ADC_ISR_AWD3);
}

//------------------------------------------------------------------------------
void AdcSTM32F30x::injectedContextQueueOverflowInterruptEventHandler()
{
    setBits(myAdc->ISR, ADC_ISR_JQOVF);
}

//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void AdcSTM32F30x::interruptHandler()
{
    // End of injected conversion interrupt
    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION) &&
    	isInterruptEventPending(INTERRUPT_EVENT_END_OF_INJECTED_CONVERSION))
    {
        endOfInjectedConversionInterruptEventHandler();
    }

    // End of injected sequence interrupt
    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_INJECTED_SEQUENCE) &&
    	isInterruptEventPending(INTERRUPT_EVENT_END_OF_INJECTED_SEQUENCE))
    {
        endOfInjectedSequenceInterruptEventHandler();
    }

    // End of regular conversion interrupt
    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION) &&
        isInterruptEventPending(INTERRUPT_EVENT_END_OF_REGULAR_CONVERSION))
    {
        endOfRegularConversionInterruptEventHandler();
    }

    // End of regular sequence interrupt
    if (isInterruptEventEnabled(INTERRUPT_EVENT_END_OF_REGULAR_SEQUENCE) &&
    	isInterruptEventPending(INTERRUPT_EVENT_END_OF_REGULAR_SEQUENCE))
    {
        endOfRegularSequenceInterruptEventHandler();
    }

//    // Overrun interrupt
//    if (isInterruptEventPending(INTERRUPT_EVENT_OVERRUN))
//    {
//        overrunInterruptEventHandler();
//    }
//
//    // Analog watchdog 1 interrupt
//    if (isInterruptEventPending(INTERRUPT_EVENT_ANALOG_WATCHDOG_1))
//    {
//        analogWatchdog1InterruptEventHandler();
//    }
//
//    // Analog watchdog 2 interrupt
//    if (isInterruptEventPending(INTERRUPT_EVENT_ANALOG_WATCHDOG_2))
//    {
//        analogWatchdog2InterruptEventHandler();
//    }
//
//    // Analog watchdog 3 interrupt
//    if (isInterruptEventPending(INTERRUPT_EVENT_ANALOG_WATCHDOG_3))
//    {
//        analogWatchdog3InterruptEventHandler();
//    }
//
//    // Injected context queue overflow interrupt
//    if (isInterruptEventPending(
//                               INTERRUPT_EVENT_INJECTED_CONTEXT_QUEUE_OVERFLOW))
//    {
//        injectedContextQueueOverflowInterruptEventHandler();
//    }
//
//    // Ready interrupt
//    if (isInterruptEventPending(INTERRUPT_EVENT_ADC_READY))
//    {
//        readyInterruptEventHandler();
//    }
//
//    // End of sampling interrupt
//    if (isInterruptEventPending(INTERRUPT_EVENT_END_OF_SAMPLING))
//    {
//        endOfSamplingInterruptEventHandler();
//    }
}

//------------------------------------------------------------------------------
// Interrupt service routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern "C" void ADC1_2_IRQHandler(void)
{
	uint32_t isr1 = ADC1->ISR;
	uint32_t ier1 = ADC1->IER;

	uint32_t isr2 = ADC2->ISR;
	uint32_t ier2 = ADC2->IER;

    if (((isr1 & ier1 & 0x7FF)) != 0)
    {
        interruptObjectMap[AdcSTM32F30x::ID_1]->handler();
    }

    if (((isr2 & ier2 & 0x7FF)) != 0)
    {
        interruptObjectMap[AdcSTM32F30x::ID_2]->handler();
    }
}

//------------------------------------------------------------------------------
extern "C" void ADC3_IRQHandler(void)
{
    interruptObjectMap[AdcSTM32F30x::ID_3]->handler();
}

//------------------------------------------------------------------------------
extern "C" void ADC4_IRQHandler(void)
{
    interruptObjectMap[AdcSTM32F30x::ID_4]->handler();
}
