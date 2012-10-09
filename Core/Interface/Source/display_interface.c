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
 * @file display_interface.c
 * @author Ben Minerd
 * @date 8/9/2012
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <display_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Display driver.
 */
static display_driver_t driver;

/**
 * @brief Flag that indicates if the Display has interfaces has been enabled.
 */
static bool isEnabled = false;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Procedure that updates the display.
 */
static void procedure(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void displayInit(void)
{
    displayDriverInit();
}

//------------------------------------------------------------------------------
extern display_error_e displaySetDriver(const display_driver_t* displayDriver)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(displayDriver)              ||
        IS_NULL_POINTER(displayDriver->setEnabled)  ||
        IS_NULL_POINTER(displayDriver->writeData)   ||
        IS_NULL_POINTER(displayDriver->writeText)   ||
        IS_NULL_POINTER(displayDriver->clear)       ||
        IS_NULL_POINTER(displayDriver->setBrightness))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    driver = *displayDriver;

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern display_error_e displayIsEnabled(bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(enabled))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    *enabled = isEnabled;

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern display_error_e displaySetEnabled(const bool enabled)
{
#ifdef PLAT4M_DEBUG

    // Errors?

#endif // PLAT4M_DEBUG

    isEnabled = enabled;

    return driver.setEnabled(enabled);
}

//------------------------------------------------------------------------------
extern display_error_e displayWriteData(
                                    const display_data_definition_t* definition,
                                    const data_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

    if (IS_NULL_POINTER(definition) || IS_NULL_POINTER(data))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

    if (definition->section >= DISPLAY_SECTION_COUNT)
    {
        return DISPLAY_ERROR_SECTION_INVALID;
    }

    if (definition->alignment >= DISPLAY_ALIGNMENT_COUNT)
    {
        return DISPLAY_ERROR_ALIGNMENT_INVALID;
    }

#endif // PLAT4M_DEBUG

    return driver.writeData(definition, data);
}

//------------------------------------------------------------------------------
extern display_error_e displayWriteText(
                                    const display_data_definition_t* definition,
                                    const display_font_e font,
                                    const char* text)
{
#ifdef PLAT4M_DEBUG

    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

    if (IS_NULL_POINTER(definition) || IS_NULL_POINTER(text))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

    if (definition->section >= DISPLAY_SECTION_COUNT)
    {
        return DISPLAY_ERROR_SECTION_INVALID;
    }

    if (definition->alignment >= DISPLAY_ALIGNMENT_COUNT)
    {
        return DISPLAY_ERROR_ALIGNMENT_INVALID;
    }

    if (font >= DISPLAY_FONT_COUNT)
    {
        return DISPLAY_ERROR_FONT_INVALID;
    }

#endif // PLAT4M_DEBUG

    return driver.writeText(definition, font, text);
}

//------------------------------------------------------------------------------
extern display_error_e displayClear(void)
{
#ifdef PLAT4M_DEBUG

    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    return driver.clear();
}

//------------------------------------------------------------------------------
extern display_error_e displaySetBrightness(const uint8_t brightness)
{
#ifdef PLAT4M_DEBUG

    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    return driver.setBrightness(brightness);
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void procedure(void)
{

}
