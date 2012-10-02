/*------------------------------------------------------------------------------
 *                  __                           ___
 *                 || |             __          //  |
 *       _______   || |   _______  || |__      //   |    _____  ___
 *      ||  ___ \  || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      || |  || | || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |__|| | || | || |__|| | || |     // /_|| |_  || | || | || |
 *      ||  ____/  || |  \\____  | || |__  //_____   _| || | || | || |
 *      || |       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *      || |
 *      ||_|
 *
 * Copyright (c) 2012 Ben Minerd. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *----------------------------------------------------------------------------*/

/**
 * @file gpio_driver_atmega328p.c
 * @author Ben Minerd
 * @date 2/3/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <gpio_interface.h>
#include <gpio_driver_atmega328p.h>

#include <avr/io.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief GPIO driver port map.
 */
static uint8_t directionMap[][] =
{
	/// GPIO_DRIVER_PORT_B
	{
		DDB0,	/// GPIO_PIN_0
		DDB1,	/// GPIO_PIN_1
		DDB2, 	/// GPIO_PIN_2
		DDB3, 	/// GPIO_PIN_3
		DDB4, 	/// GPIO_PIN_4
		DDB5, 	/// GPIO_PIN_5
		DDB6, 	/// GPIO_PIN_6
		DDB7, 	/// GPIO_PIN_7
	},
	/// GPIO_DRIVER_PORT_B
	{
		DDC0,	/// GPIO_PIN_0
		DDC1, 	/// GPIO_PIN_1
		DDC2, 	/// GPIO_PIN_2
		DDC3, 	/// GPIO_PIN_3
		DDC4, 	/// GPIO_PIN_4
		DDC5, 	/// GPIO_PIN_5
		DDC6, 	/// GPIO_PIN_6
		DDC7, 	/// GPIO_PIN_7
	},
	/// GPIO_DRIVER_PORT_B
	{
		DDD0,	/// GPIO_PIN_0
		DDD1, 	/// GPIO_PIN_1
		DDD2, 	/// GPIO_PIN_2
		DDD3, 	/// GPIO_PIN_3
		DDD4, 	/// GPIO_PIN_4
		DDD5, 	/// GPIO_PIN_5
		DDD6, 	/// GPIO_PIN_6
		DDD7, 	/// GPIO_PIN_7
	}
};

/**
 * @brief GPIO driver port map.
 */
static uint8_t portMap[][] =
{
	/// GPIO_DRIVER_PORT_B
	{
		PORTB0,	/// GPIO_PIN_0
		PORTB1, /// GPIO_PIN_1
		PORTB2, /// GPIO_PIN_2
		PORTB3, /// GPIO_PIN_3
		PORTB4, /// GPIO_PIN_4
		PORTB5, /// GPIO_PIN_5
		PORTB6, /// GPIO_PIN_6
		PORTB7, /// GPIO_PIN_7
	},
	/// GPIO_DRIVER_PORT_B
	{
		PORTC0,	/// GPIO_PIN_0
		PORTC1, /// GPIO_PIN_1
		PORTC2, /// GPIO_PIN_2
		PORTC3, /// GPIO_PIN_3
		PORTC4, /// GPIO_PIN_4
		PORTC5, /// GPIO_PIN_5
		PORTC6, /// GPIO_PIN_6
		PORTC7, /// GPIO_PIN_7
	},
	/// GPIO_DRIVER_PORT_B
	{
		PORTD0,	/// GPIO_PIN_0
		PORTD1, /// GPIO_PIN_1
		PORTD2, /// GPIO_PIN_2
		PORTD3, /// GPIO_PIN_3
		PORTD4, /// GPIO_PIN_4
		PORTD5, /// GPIO_PIN_5
		PORTD6, /// GPIO_PIN_6
		PORTD7, /// GPIO_PIN_7
	}
};

/**
 * @brief GPIO driver port map.
 */
static uint8_t pinMap[][] =
{
	/// GPIO_DRIVER_PORT_B
	{
		PINB0,	/// GPIO_PIN_0
		PINB1, 	/// GPIO_PIN_1
		PINB2, 	/// GPIO_PIN_2
		PINB3, 	/// GPIO_PIN_3
		PINB4, 	/// GPIO_PIN_4
		PINB5, 	/// GPIO_PIN_5
		PINB6, 	/// GPIO_PIN_6
		PINB7 	/// GPIO_PIN_7
	},
	/// GPIO_DRIVER_PORT_B
	{
		PINC0,	/// GPIO_PIN_0
		PINC1, 	/// GPIO_PIN_1
		PINC2, 	/// GPIO_PIN_2
		PINC3, 	/// GPIO_PIN_3
		PINC4, 	/// GPIO_PIN_4
		PINC5, 	/// GPIO_PIN_5
		PINC6, 	/// GPIO_PIN_6
		PINC7 	/// GPIO_PIN_7
	},
	/// GPIO_DRIVER_PORT_B
	{
		PIND0,	/// GPIO_PIN_0
		PIND1, 	/// GPIO_PIN_1
		PIND2, 	/// GPIO_PIN_2
		PIND3, 	/// GPIO_PIN_3
		PIND4, 	/// GPIO_PIN_4
		PIND5, 	/// GPIO_PIN_5
		PIND6, 	/// GPIO_PIN_6
		PIND7 	/// GPIO_PIN_7
	}
};

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

// plat4m
//
// Declare local GPIO driver functions here.
//
// Example:
//
//      void ledSetEnabled(bool setEnabled);
//      void ledSetLevel(gpio_level_e level);
//      ...

/**
 * @brief Sets enabled the given GPIO port and pin.
 * @param hardware GPIO hardware to access.
 * @param enabled Flag for enabled/disabled.
 * @return true if the function was successful, false if not.
 */
static void setEnabled(gpio_hardware_t* hardware, bool enabled);

/**
 * @brief Sets the mode for the given GPIO port and pin.
 * @param hardware GPIO hardware to access.
 * @return true if the function was successful, false if not.
 */
static void configure(gpio_hardware_t* hardware, gpio_config_t* config);

/**
 * @brief Sets the level for the given GPIO port and pin.
 * @param hardware GPIO hardware to access.
 * @return true if the function was successful, false if not.
 */
static void setLevel(gpio_hardware_t* hardware, gpio_level_e level);

/**
 * @brief Gets the level for the given GPIO output port and pin.
 * @param hardware GPIO hardware to access.
 * @return true if the function was successful, false if not.
 */
static void getLevel(gpio_hardware_t* hardware, gpio_level_e* level);

/**
 * @brief Reads the level for the given GPIO input port and pin.
 * @param hardware GPIO hardware to access.
 * @return true if the function was successful, false if not.
 */
static void readLevel(gpio_hardware_t* hardware, gpio_level_e* level);

/**
 * @brief Toggles the level for the given GPIO output port and pin.
 * @param gpio GPIO to access.
 * @return true if the function was successful, false if not.
 */
static void toggleLevel(gpio_hardware_t* hardware);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void gpioDriverInit(void)
{
    // plat4m
    //
    // Initialize GPIO driver.
    // Add drivers to GPIO interface.
    
    static gpio_driver_t driver =
    {
        .setEnabled     = setEnabled,
        .configure      = configure,
        .setLevel       = setLevel,
        .getLevel       = getLevel,
        .readLevel      = readLevel,
        .toggleLevel    = toggleLevel
    };
    
    gpioSetDriver(&driver);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void setEnabled(gpio_hardware_t* hardware, bool enabled)
{

}

//------------------------------------------------------------------------------
static void configure(gpio_hardware_t* hardware, gpio_config_t* config)
{
	switch (config->mode)
	{
		case GPIO_MODE_OUTPUT:
		{
			directionMap[hardware->portId][hardware->pinId] = 1;

			break;
		}
		case GPIO_MODE_INPUT:
		{
			directionMap[hardware->portId][hardware->pinId] = 0;

			switch (config->resistor)
			{
				case GPIO_RESISTOR_NONE:
				{
					portMap[hardware->portId][hardware->pinId] = 0;

					break;
				}
				case GPIO_RESISTOR_PULL_UP:
				{
					portMap[hardware->portId][hardware->pinId] = 1;

					break;
				}
			}

			break;
		}
	}
}

//------------------------------------------------------------------------------
static void setLevel(gpio_hardware_t* hardware, gpio_level_e level)
{
    portMap[hardware->portId][hardware->pinId] = level;
}

//------------------------------------------------------------------------------
static void getLevel(gpio_hardware_t* hardware, gpio_level_e* level)
{
    *level = (gpio_level_e) pinMap[hardware->portId][hardware->pinId];
}

//------------------------------------------------------------------------------
static void readLevel(gpio_hardware_t* hardware, gpio_level_e* level)
{
	*level = (gpio_level_e) pinMap[hardware->portId][hardware->pinId];
}

//------------------------------------------------------------------------------
static void toggleLevel(gpio_hardware_t* hardware)
{
    pinMap[hardware->portId][hardware->pinId] = 1;
}
