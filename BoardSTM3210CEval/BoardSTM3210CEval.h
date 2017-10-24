///
/// @file BoardSTM3210CEval.h
/// @author Ben Minerd
/// @date 12/24/2015
/// @brief BoardSTM3210CEval class.
///

#ifndef BOARD_STM3210C_EVAL_H
#define BOARD_STM3210C_EVAL_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Plat4m.h>
#include <Board.h>

#include <ProcessorSTM32F10xCL.h>
#include <GpioPinSTM32F10x.h>
#include <CanSTM32F10x.h>
#include <DisplaySTLCD.h>
#include <SpiSTM32F10x.h>

#include <GpioPin.h>
#include <Button.h>
#include <EnableLine.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

namespace Plat4m
{

class BoardSTM3210CEval : public Board
{
public:
    
    /*--------------------------------------------------------------------------
     * Public enumerations
     *------------------------------------------------------------------------*/
    
	enum ButtonId
	{
		BUTTON_ID_KEY = 0,
		BUTTON_ID_TAMPER,
		BUTTON_ID_WAKEUP
	};

	enum CanId
	{
		CAN_ID_1 = 0,
		CAN_ID_2
	};

	enum EnableLineId
	{
		// Note: Also called user button in manual
		ENABLE_LINE_ID_KEY_BUTTON = 0,
		ENABLE_LINE_ID_TAMPER_BUTTON,
		ENABLE_LINE_ID_WAKEUP_BUTTON
	};

    enum GpioPinId
    {
    	GPIO_PIN_ID_ANTI_TAMPER = 0,
    	GPIO_PIN_ID_CAN1_TX,
		GPIO_PIN_ID_CAN1_RX,
		GPIO_PIN_ID_CAN2_TX,
		GPIO_PIN_ID_CAN2_RX,
		GPIO_PIN_ID_LCD_CS,
		GPIO_PIN_ID_LCD_CLK,
		GPIO_PIN_ID_LCD_DI,
		GPIO_PIN_ID_LCD_DO,
        GPIO_PIN_ID_USER_BUTTON,
		GPIO_PIN_ID_USER_LED_1,
		GPIO_PIN_ID_USER_LED_2,
		GPIO_PIN_ID_USER_LED_3,
		GPIO_PIN_ID_USER_LED_4,
		GPIO_PIN_ID_WAKEUP
    };

    enum SpiId
    {
        SPI_ID_LCD = 0
    };

    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/
    
    BoardSTM3210CEval();
    
    /*--------------------------------------------------------------------------
	 * Public destructors
	 *------------------------------------------------------------------------*/

	~BoardSTM3210CEval();

    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/

    Button& getButton(const ButtonId buttonId);

    CanSTM32F10x& getCan(const CanId canId);

    DisplaySTLCD& getDisplay();

    EnableLine& getEnableLine(const EnableLineId enableLineId);

    GpioPinSTM32F10x& getGpioPin(const GpioPinId gpioPinId);
    
    ProcessorSTM32F10xCL& getProcessor();

    SpiSTM32F10x& getSpi(const SpiId spiId);

private:

    /*--------------------------------------------------------------------------
     * Private static data members
     *------------------------------------------------------------------------*/

    // Constants

    static const float myCoreVoltage;

    static const uint32_t myExternalClockFrequencyHz;

    static const GpioPin::Id myGpioPinIdMap[];

    // Variables

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/

    // Buttons
    Button* myKeyButton;
    Button* myTamperButton;
    Button* myWakeupButton;

    // Display
    DisplaySTLCD* myDisplay;

    // EnableLines
    EnableLine* myKeyButtonEnableLine;
    EnableLine* myTamperButtonEnableLine;
    EnableLine* myWakeupButtonEnableLine;
};

}; // namespace Plat4m

#endif // BOARD_STM3210C_EVAL_H
