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
 * @file display_interface.h
 * @author Ben Minerd
 * @date 8/9/2012
 * @brief TODO Comment!
 */

#ifndef _DISPLAY_INTERFACE_H_
#define _DISPLAY_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <display_driver.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _display_error_e_
{
    DISPLAY_ERROR_NONE,
    DISPLAY_ERROR_PARAMETER_INVALID,
    DISPLAY_ERROR_SECTION_INVALID,
    DISPLAY_ERROR_ALIGNMENT_INVALID,
    DISPLAY_ERROR_FONT_INVALID,
    DISPLAY_ERROR_NOT_ENABLED,
    DISPLAY_ERROR_COMMUNICATION
} display_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Display data definition type.
 */
typedef struct _display_data_definition_t_
{
    display_section_e section;
    display_alignment_e alignment;
    display_transition_e transition;
} display_data_definition_t;

/**
 * @brief Function type that sets the display enabled or disabled.
 * @param enabled Flag that indicates if the display should be enabled or
 * disabled.
 * @return Display error.
 */
typedef display_error_e display_driver_set_enabled_f(const bool enabled);

/**
 * @brief Function type that writes data to the display.
 * @param definition Display data definition.
 * @param data Display data to write.
 * @return Display error.
 */
typedef display_error_e display_driver_write_data_f(
                                    const display_data_definition_t* definition,
                                    const data_array_t* data);

/**
 * @brief Function type that writes text to the display.
 * @param definition Display data definition.
 * @param font Font ID.
 * @param text Text string to write.
 * @return Display error.
 */
typedef display_error_e display_driver_write_text_f(
                                    const display_data_definition_t* definition,
                                    const display_font_e font,
                                    const char* text);

/**
 * @brief Function type that clears the display.
 * @return Display error.
 */
typedef display_error_e display_driver_clear_f(void);

/**
 * @brief Function type that sets the display brightness.
 * @param brightness Brightness value.
 * @return Display error.
 */
typedef display_error_e display_driver_set_brightness_f(
                                                    const uint8_t brightness);

/**
 * @brief Display driver type.
 */
typedef struct _display_driver_t_
{
    const display_driver_set_enabled_f* setEnabled;
    const display_driver_write_data_f* writeData;
    const display_driver_write_text_f* writeText;
    const display_driver_clear_f* clear;
    const display_driver_set_brightness_f* setBrightness;
} display_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the Display interface layer.
 */
extern void displayInit(void);

/**
 * @brief Sets the display driver.
 * @param driver Display driver.
 * @return Display error.
 */
extern display_error_e displaySetDriver(const display_driver_t* displayDriver);

/**
 * @brief Sets the display enabled or disabled.
 * @param enabled Flag that indicates if the display should be enabled or
 * disabled.
 * @return Display error.
 */
extern display_error_e displaySetEnabled(const bool enabled);

/**
 * @brief Checks to see if the display is enabled or disabled.
 * @param enabled Flag that indicates if the display is enabled or disabled.
 * @return Display error.
 */
extern display_error_e displayIsEnabled(bool* enabled);

/**
 * @brief Writes the given data to the display.
 * @param definition Display data definition.
 * @param data Display data to write.
 * @return Display error.
 */
extern display_error_e displayWriteData(
                                    const display_data_definition_t* definition,
                                    const data_array_t* data);

/**
 * @brief Writes the given text to the display.
 * @param definition Display data definition.
 * @param font Font ID.
 * @param text Text string to write.
 * @return Display error.
 */
extern display_error_e displayWriteText(
                                    const display_data_definition_t* definition,
                                    const display_font_e font,
                                    const char* text);

/**
 * @brief Clears the display.
 * @return Display error.
 */
extern display_error_e displayClear(void);

/**
 * @brief Sets the display brightness.
 * @param brightness Brightness value.
 * @return Display error.
 */
extern display_error_e displaySetBrightness(const uint8_t brightness);

#endif // _DISPLAY_INTERFACE_H_
