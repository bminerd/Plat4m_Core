///
/// @file BoardSTM3210CEval.cpp
/// @author Ben Minerd
/// @date 12/24/2015
/// @brief BoardSTM3210CEval class.
///

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <BoardSTM3210CEval.h>

using Plat4m::BoardSTM3210CEval;
using Plat4m::Button;
using Plat4m::CanSTM32F10x;
using Plat4m::EnableLine;
using Plat4m::DisplaySTLCD;
using Plat4m::GpioPin;
using Plat4m::GpioSTM32F10x;
using Plat4m::GpioPinSTM32F10x;
using Plat4m::ProcessorSTM32F10xCL;
using Plat4m::SpiSTM32F10x;

/*------------------------------------------------------------------------------
 * Private static data members
 *----------------------------------------------------------------------------*/

// Constants

const float BoardSTM3210CEval::myCoreVoltage = 3.3f;

// 25 MHz
const uint32_t BoardSTM3210CEval::myExternalClockFrequencyHz = 25000000;

const GpioPin::Id BoardSTM3210CEval::myGpioPinIdMap[] =
{
	/// GPIO_PIN_ID_ANTI_TAMPER
	{GpioSTM32F10x::PORT_ID_C, GpioSTM32F10x::PIN_ID_13},

	/// GPIO_PIN_ID_CAN1_TX
	{GpioSTM32F10x::PORT_ID_D, GpioSTM32F10x::PIN_ID_1},

	/// GPIO_PIN_ID_CAN1_RX
	{GpioSTM32F10x::PORT_ID_D, GpioSTM32F10x::PIN_ID_0},

	/// GPIO_PIN_ID_CAN2_TX
	{GpioSTM32F10x::PORT_ID_B, GpioSTM32F10x::PIN_ID_6},

	/// GPIO_PIN_ID_CAN2_RX
	{GpioSTM32F10x::PORT_ID_B, GpioSTM32F10x::PIN_ID_5},

    /// GPIO_PIN_ID_LCD_CS
    {GpioSTM32F10x::PORT_ID_B, GpioSTM32F10x::PIN_ID_2},

    /// GPIO_PIN_ID_LCD_CLK
    {GpioSTM32F10x::PORT_ID_C, GpioSTM32F10x::PIN_ID_10},

    /// GPIO_PIN_ID_LCD_DI
    {GpioSTM32F10x::PORT_ID_C, GpioSTM32F10x::PIN_ID_12},

    /// GPIO_PIN_ID_LCD_DO
    {GpioSTM32F10x::PORT_ID_C, GpioSTM32F10x::PIN_ID_11},

    /// GPIO_PIN_ID_USER_BUTTON
    {GpioSTM32F10x::PORT_ID_B, GpioSTM32F10x::PIN_ID_9},

	/// GPIO_PIN_ID_USER_LED_1
	{GpioSTM32F10x::PORT_ID_D, GpioSTM32F10x::PIN_ID_7},

	/// GPIO_PIN_ID_USER_LED_2
	{GpioSTM32F10x::PORT_ID_D, GpioSTM32F10x::PIN_ID_13},

	/// GPIO_PIN_ID_USER_LED_3
	{GpioSTM32F10x::PORT_ID_D, GpioSTM32F10x::PIN_ID_3},

	/// GPIO_PIN_ID_USER_LED_4
	{GpioSTM32F10x::PORT_ID_D, GpioSTM32F10x::PIN_ID_4},

	/// GPIO_PIN_ID_WAKEUP
	{GpioSTM32F10x::PORT_ID_A, GpioSTM32F10x::PIN_ID_0},
};

// Variables


/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
BoardSTM3210CEval::BoardSTM3210CEval() :
    Board(),
    myKeyButton(0),
    myTamperButton(0),
    myWakeupButton(0),
    myDisplay(0),
    myKeyButtonEnableLine(0),
    myTamperButtonEnableLine(0),
    myWakeupButtonEnableLine(0)
{
}

/*------------------------------------------------------------------------------
 * Public destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
BoardSTM3210CEval::~BoardSTM3210CEval()
{
	// Buttons

	if (isValidPointer(myKeyButton))
	{
		delete myKeyButton;
	}

	if (isValidPointer(myTamperButton))
	{
		delete myTamperButton;
	}

	if (isValidPointer(myWakeupButton))
	{
		delete myWakeupButton;
	}

	// EnableLines

	if (isValidPointer(myKeyButtonEnableLine))
	{
		delete myKeyButtonEnableLine;
	}

	if (isValidPointer(myTamperButtonEnableLine))
	{
		delete myTamperButtonEnableLine;
	}

	if (isValidPointer(myWakeupButtonEnableLine))
	{
		delete myWakeupButtonEnableLine;
	}
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Button& BoardSTM3210CEval::getButton(const ButtonId buttonId)
{
    Button* button = 0;

    switch (buttonId)
    {
        case BUTTON_ID_KEY:
        {
            if (isNullPointer(myKeyButton))
            {
                myKeyButton =
                           new Button(getEnableLine(ENABLE_LINE_ID_KEY_BUTTON));
            }

            button = myKeyButton;

            break;
        }
        case BUTTON_ID_TAMPER:
        {
            if (isNullPointer(myTamperButton))
            {
                myTamperButton =
                        new Button(getEnableLine(ENABLE_LINE_ID_TAMPER_BUTTON));
            }

            button = myTamperButton;

            break;
        }
        case BUTTON_ID_WAKEUP:
        {
            if (isNullPointer(myWakeupButton))
            {
                myWakeupButton =
                        new Button(getEnableLine(ENABLE_LINE_ID_WAKEUP_BUTTON));
            }

            button = myWakeupButton;

            break;
        }
    }

    return (*button);
}

//------------------------------------------------------------------------------
CanSTM32F10x& BoardSTM3210CEval::getCan(const CanId canId)
{
	CanSTM32F10x* can = 0;

	switch (canId)
	{
		case CAN_ID_1:
		{
			can = &(CanSTM32F10x::get(CanSTM32F10x::ID_1,
									  getGpioPin(GPIO_PIN_ID_CAN1_TX),
									  getGpioPin(GPIO_PIN_ID_CAN1_RX),
									  CanSTM32F10x::GPIO_REMAP_2));

			break;
		}
		case CAN_ID_2:
		{
			can = &(CanSTM32F10x::get(CanSTM32F10x::ID_2,
									  getGpioPin(GPIO_PIN_ID_CAN2_TX),
									  getGpioPin(GPIO_PIN_ID_CAN2_RX),
									  CanSTM32F10x::GPIO_REMAP_1));

			break;
		}
	}

	return (*can);
}

//------------------------------------------------------------------------------
DisplaySTLCD& BoardSTM3210CEval::getDisplay()
{
    if (isNullPointer(myDisplay))
    {
        myDisplay = new DisplaySTLCD(getGpioPin(GPIO_PIN_ID_LCD_CS),
                                     getSpi(SPI_ID_LCD));
    }

    return (*myDisplay);
}

//------------------------------------------------------------------------------
EnableLine& BoardSTM3210CEval::getEnableLine(const EnableLineId enableLineId)
{
	EnableLine* enableLine = 0;

	switch (enableLineId)
	{
		case ENABLE_LINE_ID_KEY_BUTTON:
		{
			if (isNullPointer(myKeyButtonEnableLine))
			{
				myKeyButtonEnableLine =
							 new EnableLine(EnableLine::MODE_INPUT,
							 			    EnableLine::ACTIVE_LEVEL_LOW,
							 			    getGpioPin(GPIO_PIN_ID_USER_BUTTON),
										    true);
			}

			enableLine = myKeyButtonEnableLine;

			break;
		}
		case ENABLE_LINE_ID_TAMPER_BUTTON:
		{
			if (isNullPointer(myTamperButtonEnableLine))
			{
				myTamperButtonEnableLine =
							 new EnableLine(EnableLine::MODE_INPUT,
											EnableLine::ACTIVE_LEVEL_LOW,
											getGpioPin(GPIO_PIN_ID_ANTI_TAMPER),
											true);
			}

			enableLine = myTamperButtonEnableLine;

			break;
		}
		case ENABLE_LINE_ID_WAKEUP_BUTTON:
		{
			if (isNullPointer(myWakeupButtonEnableLine))
			{
				myWakeupButtonEnableLine =
								  new EnableLine(EnableLine::MODE_INPUT,
												 EnableLine::ACTIVE_LEVEL_HIGH,
												 getGpioPin(GPIO_PIN_ID_WAKEUP),
												 true);
			}

			enableLine = myWakeupButtonEnableLine;

			break;
		}
	}

    return (*enableLine);
}

//------------------------------------------------------------------------------
GpioPinSTM32F10x& BoardSTM3210CEval::getGpioPin(const GpioPinId gpioPinId)
{
    return GpioPinSTM32F10x::get(myGpioPinIdMap[gpioPinId]);
}

//------------------------------------------------------------------------------
ProcessorSTM32F10xCL& BoardSTM3210CEval::getProcessor()
{
    return ProcessorSTM32F10xCL::get(myCoreVoltage, myExternalClockFrequencyHz);
}

//------------------------------------------------------------------------------
SpiSTM32F10x& BoardSTM3210CEval::getSpi(const SpiId spiId)
{
    SpiSTM32F10x* spi = 0;

    switch (spiId)
    {
        case SPI_ID_LCD:
        {
            spi = &(SpiSTM32F10x::get(SpiSTM32F10x::ID_3,
                                      Spi::TRANSMISSION_MODE_RX_TX,
                                      getGpioPin(GPIO_PIN_ID_LCD_CLK),
                                      getGpioPin(GPIO_PIN_ID_LCD_DO),
                                      getGpioPin(GPIO_PIN_ID_LCD_DI),
                                      SpiSTM32F10x::GPIO_REMAP_1));

            break;
        }
    }

    return (*spi);
}
