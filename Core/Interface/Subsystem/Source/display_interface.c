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
 * @brief Display interface layer.
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
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Display driver.
 */
static display_driver_t driver;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void displayInit(void)
{
    displayApplicationInit();
}

//------------------------------------------------------------------------------
display_error_e displaySetDriver(const display_driver_t* displayDriver)
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
display_error_e displayInitId(display_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = false;

    mutexInitMutex(&(info->mutex));

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
display_error_e displaySetEnabled(display_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DISPLAY_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    display_error_e error = driver.setEnabled(id->display, enabled);

    if (error == DISPLAY_ERROR_NONE)
    {
        info->isEnabled = enabled;
    }

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
display_error_e displayIsEnabled(display_id_t* id, bool* enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(enabled))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DISPLAY_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    *enabled = info->isEnabled;

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return DISPLAY_ERROR_NONE;
}

//------------------------------------------------------------------------------
display_error_e displayConfigure(display_id_t* id,
                                 const display_config_t* config)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(config))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DISPLAY_ERROR_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    display_error_e error = driver.configure(id->display, config);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
display_error_e displayWriteData(display_id_t* id,
                                 const display_data_definition_t* definition,
                                 const data_array_t* data)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id)         ||
        IS_NULL_POINTER(definition) ||
        IS_NULL_POINTER(data))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

    if ((definition->section == DISPLAY_SECTION_NONE) ||
        (definition->section >= DISPLAY_SECTION_COUNT))
    {
        return DISPLAY_ERROR_SECTION_INVALID;
    }

    if ((definition->alignment == DISPLAY_ALIGNMENT_NONE) ||
        (definition->alignment >= DISPLAY_ALIGNMENT_COUNT))
    {
        return DISPLAY_ERROR_ALIGNMENT_INVALID;
    }

    if ((definition->transition == DISPLAY_TRANSITION_NONE) ||
        (definition->transition >= DISPLAY_TRANSITION_COUNT))
    {
        return DISPLAY_ERROR_TRANSITION_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DISPLAY_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    display_error_e error = driver.writeData(id->display, definition, data);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
display_error_e displayWriteText(display_id_t* id,
                                 const display_data_definition_t* definition,
                                 const display_font_e font,
                                 const char* text)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id)         ||
        IS_NULL_POINTER(definition) ||
        IS_NULL_POINTER(text))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

    if ((definition->section == DISPLAY_SECTION_NONE) ||
        (definition->section >= DISPLAY_SECTION_COUNT))
    {
        return DISPLAY_ERROR_SECTION_INVALID;
    }

    if ((definition->alignment == DISPLAY_ALIGNMENT_NONE) ||
        (definition->alignment >= DISPLAY_ALIGNMENT_COUNT))
    {
        return DISPLAY_ERROR_ALIGNMENT_INVALID;
    }

    if ((definition->transition == DISPLAY_TRANSITION_NONE) ||
        (definition->transition >= DISPLAY_TRANSITION_COUNT))
    {
        return DISPLAY_ERROR_TRANSITION_INVALID;
    }

    if ((font == DISPLAY_FONT_NONE) || (font >= DISPLAY_FONT_COUNT))
    {
        return DISPLAY_ERROR_FONT_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DISPLAY_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    display_error_e error = driver.writeText(id->display,
                                             definition,
                                             font,
                                             text);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
display_error_e displayClear(display_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DISPLAY_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    display_error_e error = driver.clear(id->display);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

//------------------------------------------------------------------------------
display_error_e displaySetBrightness(display_id_t* id, const uint8_t brightness)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return DISPLAY_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    display_info_t* info = (display_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return DISPLAY_ERROR_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return DISPLAY_ERROR_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    // Lock mutex
    mutexSetLocked(&(info->mutex), true);

    display_error_e error = driver.setBrightness(id->display, brightness);

    // Unlock mutex
    mutexSetLocked(&(info->mutex), false);

    return error;
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/
