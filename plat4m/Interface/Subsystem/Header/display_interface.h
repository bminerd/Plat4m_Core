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
    DISPLAY_ERROR_NOT_ENABLED,
    DISPLAY_ERROR_COMM,
    DISPLAY_ERROR_INVALID_SECTION_ID,
    DISPLAY_ERROR_INVALID_ALIGNMENT_ID,
    DISPLAY_ERROR_INVALID_FONT_ID
} display_error_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef struct _display_data_descriptor_t_
{
    display_section_id_e sectionId;
    display_alignment_id_e alignmentId;
    display_transition_id_e transitionId;
} display_data_descriptor_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void displayInit(void);

/**
 * TODO Comment!
 */
extern bool displayIsEnabled(void);

/**
 * TODO Comment!
 */
extern void displaySetEnabled(bool enabled);

/**
 * TODO Comment!
 */
extern display_error_e displayWriteData(display_data_descriptor_t descriptor,
                                        data_array_t data);

/**
 * TODO Comment!
 */
extern display_error_e displayWriteText(display_data_descriptor_t descriptor,
                                        display_font_id_e fontId,
                                        const char* text);

/**
 * TODO Comment!
 */
extern display_error_e displayClear(void);

/**
 * TODO Comment!
 */
extern display_error_e displaySetBrightness(uint8_t brightness);

/**
 * TODO Comment!
 */
extern display_error_e displaySetOn(bool on);

#endif // _DISPLAY_INTERFACE_H_
