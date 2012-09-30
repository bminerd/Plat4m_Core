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
extern bool displayIsEnabled(void)
{
    return isEnabled;
}

//------------------------------------------------------------------------------
extern void displaySetEnabled(bool enabled)
{
    displayDriverSetEnabled(enabled);
    isEnabled = enabled;
}

//------------------------------------------------------------------------------
extern display_error_e displayWriteData(display_data_descriptor_t descriptor,
                                        data_array_t data)
{
    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

    if (descriptor.sectionId >= DISPLAY_SECTION_ID_COUNT)
    {
        return DISPLAY_ERROR_INVALID_SECTION_ID;
    }

    if (descriptor.alignmentId >= DISPLAY_ALIGNMENT_ID_COUNT)
    {
        return DISPLAY_ERROR_INVALID_ALIGNMENT_ID;
    }

    if (!displayDriverWriteData(descriptor, data))
    {
        return DISPLAY_ERROR_COMM;
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern display_error_e displayWriteText(display_data_descriptor_t descriptor,
                                        display_font_id_e fontId,
                                        const char* text)
{
    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

    if (descriptor.sectionId >= DISPLAY_SECTION_ID_COUNT)
    {
        return DISPLAY_ERROR_INVALID_SECTION_ID;
    }

    if (descriptor.alignmentId >= DISPLAY_ALIGNMENT_ID_COUNT)
    {
        return DISPLAY_ERROR_INVALID_ALIGNMENT_ID;
    }

    if (fontId >= DISPLAY_FONT_ID_COUNT)
    {
        return DISPLAY_ERROR_INVALID_FONT_ID;
    }

    if (!displayDriverWriteText(descriptor, fontId, text))
    {
        return DISPLAY_ERROR_COMM;
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern display_error_e displayClear(void)
{
    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

    if (!displayDriverClear())
    {
        return DISPLAY_ERROR_COMM;
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern display_error_e displaySetBrightness(uint8_t brightness)
{
    if (!isEnabled)
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

    if (!displayDriverSetBrightness(brightness))
    {
        return DISPLAY_ERROR_COMM;
    }

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern display_error_e displaySetOn(bool on)
{
    if (!displayDriverSetOn(on))
    {
        return DISPLAY_ERROR_COMM;
    }

    return DISPLAY_ERROR_NONE;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void procedure(void)
{

}
