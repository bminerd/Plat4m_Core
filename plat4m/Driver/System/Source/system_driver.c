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
 * @file system_driver.c
 * @author Ben Minerd
 * @date 3/18/12
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_interface.h>
#include <system_driver.h>

#include <gpio_interface.h>
#include <uart_interface.h>
#include <interrupt_interface.h>
#include <adc_interface.h>
#include <i2c_interface.h>
#include <display_interface.h>
#include <ui_interface.h>
#include <icons.h>
#include <setting_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define LED_PROCEDURE_DELAY_MS      (0)

/**
 * TODO Comment!
 */
#define DISPLAY_PROCEDURE_DELAY_MS  (0)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static void led1Procedure(void);

/**
 * TODO Comment!
 */
static void uart1RxCallback(uint8_t data);

/**
 * TODO Comment!
 */
static void buttonKeyEventHandler(ui_input_event_t event);

/**
 * TODO Comment!
 */
static void buttonTamperEventHandler(ui_input_event_t event);

/**
 * TODO Comment!
 */
static void buttonWakeupEventHandler(ui_input_event_t event);

/**
 * TODO Comment!
 */
static void buttonsKeyTamperEventHandler(ui_input_event_t event);

/**
 * TODO Comment!
 */
static void buttonsTamperWakeupEventHandler(ui_input_event_t event);

/**
 * TODO Comment!
 */
static void displayProcedure(void);

/**
 * TODO Comment!
 */
static void stateOffEnter(void);

/**
 * TODO Comment!
 */
static void stateOffExit(void);

/**
 * TODO Comment!
 */
static void stateOnEnter(void);

/**
 * TODO Comment!
 */
static void stateOnExit(void);

/**
 * TODO Comment!
 */
static void stateMenuEnter(void);

/**
 * TODO Comment!
 */
static void stateMenuExit(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void systemDriverInit(void)
{
    // plat4m
    //
    // Initialize system driver.
    // Add drivers to SPI interface.
    //
    // Example:
    //
    //      system_procedure_t procedures[] =
    //      {
    //          // LED1 procedure
    //          {
    //              .id         = SYSTEM_PROCEDURE_ID_LED1,
    //              .enabled    = true,
    //              ...
    //          },
    //          // LED2 procedure
    //          {
    //              .id         = SYSTEM_PROCEDURE_ID_LED2,
    //              .enabled    = true,
    //              ...
    //          }
    //          ...
    //      };
    //
    //      systemProcedureAddDrivers(procedures, ARRAY_SIZE(procedures));

    system_procedure_driver_t procedures[] =
    {
        // LED procedure
        {
            .id             = SYSTEM_PROCEDURE_ID_LED,
            .callback       = led1Procedure,
            .timeDelayMs    = LED_PROCEDURE_DELAY_MS
        },
        // Display procedure
        {
            .id             = SYSTEM_PROCEDURE_ID_DISPLAY,
            .callback       = displayProcedure,
            .timeDelayMs    = DISPLAY_PROCEDURE_DELAY_MS
        }
    };

    system_state_driver_t states[] =
    {
        // Off state
        {
            .id         = SYSTEM_STATE_ID_OFF,
            .enter      = stateOffEnter,
            .exit       = stateOffExit
        },
        // On state
        {
            .id         = SYSTEM_STATE_ID_ON,
            .enter      = stateOnEnter,
            .exit       = stateOnExit
        },
        // Menu state
        {
            .id         = SYSTEM_STATE_ID_MENU,
            .enter      = stateMenuEnter,
            .exit       = stateMenuExit
        }
    };

    systemAddProcedureDrivers(procedures, ARRAY_SIZE(procedures));
    systemAddStateDrivers(states, ARRAY_SIZE(states));

    // Component inits
    interruptInit();
    interruptAddHandler(INTERRUPT_ID_SYS_TICK, systemTimeMsHandler);

    gpioInit();

    uartInit();
    uartSetRxCallback(UART_ID_COM_PORT, uart1RxCallback);

    adcInit();

    i2cInit();

    // Subsystem inits
    displayInit();
    displaySetEnabled(true);

    // System inits
    uiInit();
    uiInputChannelAddEventHandler(UI_INPUT_CHANNEL_ID_BUTTON_KEY,
                                  buttonKeyEventHandler);
    uiInputChannelAddEventHandler(UI_INPUT_CHANNEL_ID_BUTTON_TAMPER,
                                  buttonTamperEventHandler);
    uiInputChannelAddEventHandler(UI_INPUT_CHANNEL_ID_BUTTON_WAKEUP,
                                  buttonWakeupEventHandler);
    uiInputChannelAddEventHandler(UI_INPUT_CHANNEL_ID_BUTTONS_KEY_TAMPER,
                                  buttonsKeyTamperEventHandler);
    uiInputChannelAddEventHandler(UI_INPUT_CHANNEL_ID_BUTTONS_TAMPER_WAKEUP,
                                  buttonsTamperWakeupEventHandler);

    settingInit();
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void led1Procedure(void)
{
    static uint32_t blinkTimeMs = 0;

    switch (systemStateGet())
    {
        case SYSTEM_STATE_ID_OFF:
        {
            if (systemTimeMsGet() >= blinkTimeMs)
            {
                gpioToggleLevel(GPIO_ID_LED_1);
                blinkTimeMs += 1000;
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void uart1RxCallback(uint8_t data)
{
    switch (systemStateGet())
    {
        case SYSTEM_STATE_ID_OFF:
        {
            break;
        }
        case SYSTEM_STATE_ID_ON:
        {
            byte_array_t dataStr = BYTE_ARRAY_STRING("\nplat4m> ");

            if (data == '\r')
            {
                uartTx(UART_ID_COM_PORT, &dataStr);
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
void buttonKeyEventHandler(ui_input_event_t event)
{
    switch (event.id)
    {
        case UI_INPUT_EVENT_ID_BUTTON_PRESS:
        {
            gpioSetLevel(GPIO_ID_LED_2, GPIO_LEVEL_HIGH);

            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_ON:
                {
                    display_data_descriptor_t descriptor;
                    descriptor.sectionId = DISPLAY_SECTION_ID_CENTER;
                    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;
                    descriptor.transitionId = DISPLAY_TRANSITION_ID_NONE;

                    displayWriteText(descriptor,
                                     DISPLAY_FONT_ID_ARIAL_14PT,
                                     "RNG");

                    break;
                }
                case SYSTEM_STATE_ID_MENU:
                {
                    uiMenuItemNext();

                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        case UI_INPUT_EVENT_ID_BUTTON_RELEASE:
        {
            gpioSetLevel(GPIO_ID_LED_2, GPIO_LEVEL_LOW);

            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_ON:
                {
                    display_data_descriptor_t descriptor;
                    descriptor.sectionId = DISPLAY_SECTION_ID_CENTER;
                    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_RIGHT;
                    descriptor.transitionId = DISPLAY_TRANSITION_ID_NONE;

                    char str[12];
                    int range = rand() % 15000;
                    str[sprintf(str, "%u", range)] = 0;

                    displayWriteText(descriptor,
                                     DISPLAY_FONT_ID_ARIAL_14PT,
                                     str);

                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        case UI_INPUT_EVENT_ID_BUTTON_HOLD:
        {
            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_MENU:
                {
                    bool isActive = true;

                    while (isActive)
                    {
                        uiMenuItemNext();
                        uiInputChannelIsActive(UI_INPUT_CHANNEL_ID_BUTTON_KEY,
                                               &isActive);
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void buttonTamperEventHandler(ui_input_event_t event)
{
    switch (event.id)
    {
        case UI_INPUT_EVENT_ID_BUTTON_PRESS:
        {
            gpioSetLevel(GPIO_ID_LED_3, GPIO_LEVEL_HIGH);

            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_MENU:
                {
                    uiMenuItemEnter();

                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        case UI_INPUT_EVENT_ID_BUTTON_RELEASE:
        {
            gpioSetLevel(GPIO_ID_LED_3, GPIO_LEVEL_LOW);

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void buttonWakeupEventHandler(ui_input_event_t event)
{
    switch (event.id)
    {
        case UI_INPUT_EVENT_ID_BUTTON_PRESS:
        {
            gpioSetLevel(GPIO_ID_LED_4, GPIO_LEVEL_HIGH);

            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_MENU:
                {
                    uiMenuItemPrevious();

                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        case UI_INPUT_EVENT_ID_BUTTON_RELEASE:
        {
            gpioSetLevel(GPIO_ID_LED_4, GPIO_LEVEL_LOW);

            break;
        }
        case UI_INPUT_EVENT_ID_BUTTON_HOLD:
        {
            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_MENU:
                {
                    bool isActive = true;

                    while (isActive)
                    {
                        uiMenuItemPrevious();
                        uiInputChannelIsActive(UI_INPUT_CHANNEL_ID_BUTTON_WAKEUP,
                                               &isActive);
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        case UI_INPUT_EVENT_ID_BUTTON_DOUBLE_TAP:
        {
            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_ON:
                {
                    display_data_descriptor_t descriptor;
                    descriptor.sectionId = DISPLAY_SECTION_ID_CENTER;
                    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;
                    descriptor.transitionId = DISPLAY_TRANSITION_ID_NONE;

                    displayWriteText(descriptor,
                                     DISPLAY_FONT_ID_ARIAL_14PT,
                                     "PEW");

                    break;
                }
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void buttonsKeyTamperEventHandler(ui_input_event_t event)
{
    switch (event.id)
    {
        case UI_INPUT_EVENT_ID_BUTTON_HOLD:
        {
            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_ON:
                {
                    systemStateTransition(SYSTEM_STATE_ID_MENU);

                    break;
                }
                case SYSTEM_STATE_ID_MENU:
                {
                    systemStateTransition(SYSTEM_STATE_ID_ON);

                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void buttonsTamperWakeupEventHandler(ui_input_event_t event)
{
    switch (event.id)
    {
        case UI_INPUT_EVENT_ID_BUTTON_HOLD:
        {
            switch (systemStateGet())
            {
                case SYSTEM_STATE_ID_OFF:
                {
                    systemStateTransition(SYSTEM_STATE_ID_ON);

                    break;
                }
                default:
                {
                    systemStateTransition(SYSTEM_STATE_ID_OFF);

                    break;
                }
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void displayProcedure(void)
{
    static uint8_t lastBrightness = 0;
    static uint32_t sleepTimeMs = 0;
    static uint32_t counterTimeMs = 0;
    static uint32_t counter = 0;
    uint8_t brightness = 0;
    float voltage = 0.0f;

    switch (systemStateGet())
    {
        case SYSTEM_STATE_ID_OFF:
        {
            if (displayIsEnabled() && (sleepTimeMs == 0))
            {
                sleepTimeMs = systemTimeMsGet() + 2000;
            }

            if ((sleepTimeMs != 0) && (systemTimeMsGet() >= sleepTimeMs))
            {
                displaySetEnabled(false);
                sleepTimeMs = 0;
            }

            break;
        }
        case SYSTEM_STATE_ID_ON:
        {
            if (displayIsEnabled())
            {
                adcRead(ADC_ID_1, &voltage);
                brightness = (uint8_t) (255.0 * (voltage / 3.3f));

                if (brightness != lastBrightness)
                {
                    displaySetBrightness(brightness);
                    lastBrightness = brightness;
                }

                display_data_descriptor_t descriptor;
                descriptor.sectionId = DISPLAY_SECTION_ID_ALL;
                descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;
                descriptor.transitionId = DISPLAY_TRANSITION_ID_NONE;

                if (systemTimeMsGet() >= counterTimeMs)
                {
                    int minutes = (counter / 60000) % 60;
                    int seconds = (counter / 1000) % 60;
                    int secondHundredths = (counter / 10) % 100;

                    char str[12];
                    str[sprintf(str,
                                "%02u:%02u:%02u",
                                minutes,
                                seconds,
                                secondHundredths)] = 0;
                    displayWriteText(descriptor,
                                     DISPLAY_FONT_ID_ARIAL_12PT,
                                     str);

                    counter += 10;

                    if (counter >= 36000000)
                    {
                        counter = 0;
                    }

                    counterTimeMs = systemTimeMsGet() + 10;
                }
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//------------------------------------------------------------------------------
static void stateOffEnter(void)
{
    uiInputBehaviorSet(UI_INPUT_BEHAVIOR_ID_OFF);

    gpioSetLevel(GPIO_ID_LED_2, GPIO_LEVEL_LOW);
    gpioSetLevel(GPIO_ID_LED_3, GPIO_LEVEL_LOW);
    gpioSetLevel(GPIO_ID_LED_4, GPIO_LEVEL_LOW);

    display_data_descriptor_t descriptor;
    descriptor.sectionId = DISPLAY_SECTION_ID_CENTER;
    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;
    descriptor.transitionId = DISPLAY_TRANSITION_ID_NONE;

    displayWriteText(descriptor, DISPLAY_FONT_ID_ARIAL_14PT, "OFF");
}

//------------------------------------------------------------------------------
static void stateOffExit(void)
{

}

//------------------------------------------------------------------------------
static void stateOnEnter(void)
{
    gpioSetLevel(GPIO_ID_LED_1, GPIO_LEVEL_HIGH);
    uiInputBehaviorSet(UI_INPUT_BEHAVIOR_ID_ON);
    displaySetEnabled(true);

    display_data_descriptor_t descriptor;
    descriptor.transitionId = DISPLAY_TRANSITION_ID_NONE;

    // Bolt icon
    descriptor.sectionId = DISPLAY_SECTION_ID_LEFT_TOP;
    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;

    data_array_t data;
    data.data.bytes = (uint8_t*) (bolt_icon.data);
    data.data.size = (bolt_icon.width * bolt_icon.height) / 8;

    displayWriteData(descriptor, data);

    // Batt icon
    descriptor.sectionId = DISPLAY_SECTION_ID_LEFT_BOTTOM;
    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;

    data.data.bytes = (uint8_t*) (batt_icon.data);
    data.data.size = (batt_icon.width * batt_icon.height) / 8;

    displayWriteData(descriptor, data);

    // Meters/yards
    descriptor.sectionId = DISPLAY_SECTION_ID_RIGHT_BOTTOM;
    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;

    setting_value_t value;
    settingValueGet(SETTING_ID_METERS_YARDS, &value);

    switch (value.uint8)
    {
        case RANGE_UNITS_METERS:
        {
            displayWriteText(descriptor,
                             DISPLAY_FONT_ID_ARIAL_7PT,
                             "M");

            break;
        }
        case RANGE_UNITS_YARDS:
        {
            displayWriteText(descriptor,
                             DISPLAY_FONT_ID_ARIAL_7PT,
                             "Y");

            break;
        }
        default:
        {
            break;
        }
    }

    descriptor.sectionId = DISPLAY_SECTION_ID_CENTER;
    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;

    displayWriteText(descriptor, DISPLAY_FONT_ID_ARIAL_14PT, "ON");
    systemDelayMs(500);
}

//------------------------------------------------------------------------------
static void stateOnExit(void)
{

}

//------------------------------------------------------------------------------
static void stateMenuEnter(void)
{
    uiMenuEnter();
}

//------------------------------------------------------------------------------
static void stateMenuExit(void)
{

}
