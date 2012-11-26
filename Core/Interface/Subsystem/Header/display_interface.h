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
 * @brief Display interface layer.
 */

#ifndef _DISPLAY_INTERFACE_H_
#define _DISPLAY_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <display_application.h>
#include <mutex_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of display errors.
 */
typedef enum _display_error_e_
{
    DISPLAY_ERROR_NONE,
    DISPLAY_ERROR_PARAMETER_INVALID,
    DISPLAY_ERROR_SECTION_INVALID,
    DISPLAY_ERROR_ALIGNMENT_INVALID,
    DISPLAY_ERROR_TRANSITION_INVALID,
    DISPLAY_ERROR_FONT_INVALID,
    DISPLAY_ERROR_NOT_INITIALIZED,
    DISPLAY_ERROR_NOT_ENABLED,
    DISPLAY_ERROR_COMMUNICATION
} display_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Display type.
 */
typedef void display_t;

/**
 * @brief Display configuration type.
 */
typedef void display_config_t;

/**
 * @brief Display info type.
 */
typedef struct _display_info_t_
{
    bool isInitialized;
    bool isEnabled;
    mutex_t mutex;
} display_info_t;

/**
 * @brief Display ID type.
 */
typedef struct _display_id_t_
{
    const display_t* display;
    const generic_data_t private[sizeof(display_info_t) /
                                 sizeof(generic_data_t)];
} display_id_t;

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
 * @brief Function type that sets the given display enabled or disabled.
 * @param display Display to access.
 * @param enabled Flag that indicates if the display should be enabled or
 * disabled.
 * @return Display error.
 */
typedef display_error_e display_driver_set_enabled_f(const display_t* display,
                                                     const bool enabled);

/**
 * @brief Function type that configures the given display.
 * @param id Display to access.
 * @param config Display config.
 * @return Display error.
 */
typedef display_error_e display_driver_configure_f(
                                                const display_t* display,
                                                const display_config_t* config);

/**
 * @brief Function type that writes data to the given display.
 * @param id Display to access.
 * @param definition Display data definition.
 * @param data Display data to write.
 * @return Display error.
 */
typedef display_error_e display_driver_write_data_f(
                                    const display_t* display,
                                    const display_data_definition_t* definition,
                                    const data_array_t* data);

/**
 * @brief Function type that writes text to the given display.
 * @param id Display to access.
 * @param definition Display data definition.
 * @param font Font ID.
 * @param text Text string to write.
 * @return Display error.
 */
typedef display_error_e display_driver_write_text_f(
                                    const display_t* display,
                                    const display_data_definition_t* definition,
                                    const display_font_e font,
                                    const char* text);

/**
 * @brief Function type that clears the given display.
 * @param id Display to access.
 * @return Display error.
 */
typedef display_error_e display_driver_clear_f(const display_t* display);

/**
 * @brief Function type that sets the given display brightness.
 * @param id Display to access.
 * @param brightness Brightness value.
 * @return Display error.
 */
typedef display_error_e display_driver_set_brightness_f(
                                                    const display_t* display,
                                                    const uint8_t brightness);

/**
 * @brief Display driver type.
 */
typedef struct _display_driver_t_
{
    display_driver_set_enabled_f* setEnabled;
    display_driver_configure_f* configure;
    display_driver_write_data_f* writeData;
    display_driver_write_text_f* writeText;
    display_driver_clear_f* clear;
    display_driver_set_brightness_f* setBrightness;
} display_driver_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the Display interface layer.
 */
void displayInit(void);

/**
 * @brief Sets the display driver.
 * @param driver Display driver.
 * @return Display error.
 */
display_error_e displaySetDriver(const display_driver_t* displayDriver);

/**
 * @brief Adds the given display.
 * @param display Display to add.
 * @return Display error.
 */
display_error_e displayInitId(display_id_t* id);

/**
 * @brief Sets the display enabled or disabled.
 * @param id Display to access.
 * @param enabled Flag that indicates if the display should be enabled or
 * disabled.
 * @return Display error.
 */
display_error_e displaySetEnabled(display_id_t* id, const bool enabled);

/**
 * @brief Checks to see if the display is enabled or disabled.
 * @param id Display to access.
 * @param enabled Flag that indicates if the display is enabled or disabled.
 * @return Display error.
 */
display_error_e displayIsEnabled(display_id_t* id, bool* enabled);

/**
 * @brief Configures the given display.
 * @param id Display to access.
 * @param config Display configuration.
 * @return Display error.
 */
display_error_e displayConfigure(display_id_t* id,
                                 const display_config_t* config);

/**
 * @brief Writes the given data to the display.
 * @param id Display to access.
 * @param definition Display data definition.
 * @param data Display data to write.
 * @return Display error.
 */
display_error_e displayWriteData(display_id_t* id,
                                 const display_data_definition_t* definition,
                                 const data_array_t* data);

/**
 * @brief Writes the given text to the display.
 * @param id Display to access.
 * @param definition Display data definition.
 * @param font Font ID.
 * @param text Text string to write.
 * @return Display error.
 */
display_error_e displayWriteText(display_id_t* id,
                                 const display_data_definition_t* definition,
                                 const display_font_e font,
                                 const char* text);

/**
 * @brief Clears the display.
 * @param id Display to access.
 * @return Display error.
 */
display_error_e displayClear(display_id_t* id);

/**
 * @brief Sets the display brightness.
 * @param id Display to access.
 * @param brightness Brightness value.
 * @return Display error.
 */
display_error_e displaySetBrightness(display_id_t* id,
                                     const uint8_t brightness);

#endif // _DISPLAY_INTERFACE_H_
