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
 * @file display_osd.h
 * @author Ben Minerd
 * @date 8/9/2012
 * @brief TODO Comment!
 */

#ifndef _DISPLAY_DRIVER_H_
#define _DISPLAY_DRIVER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _display_section_id_e_
{
    DISPLAY_SECTION_ID_ALL = 0,
    DISPLAY_SECTION_ID_CENTER,
    DISPLAY_SECTION_ID_CENTER_TOP,
    DISPLAY_SECTION_ID_CENTER_BOTTOM,
    DISPLAY_SECTION_ID_LEFT_TOP,
    DISPLAY_SECTION_ID_LEFT_BOTTOM,
    DISPLAY_SECTION_ID_RIGHT_TOP,
    DISPLAY_SECTION_ID_RIGHT_BOTTOM,

    // Do not place values below!
    DISPLAY_SECTION_ID_COUNT
} display_section_id_e;

/**
 * TODO Comment!
 */
typedef enum _display_alignment_id_e_
{
    DISPLAY_ALIGNMENT_ID_LEFT = 0,
    DISPLAY_ALIGNMENT_ID_CENTER,
    DISPLAY_ALIGNMENT_ID_RIGHT,

    // Do not place values below!
    DISPLAY_ALIGNMENT_ID_COUNT
} display_alignment_id_e;

/**
 * TODO Comment!
 */
typedef enum _display_font_id_e_
{
    DISPLAY_FONT_ID_ARIAL_14PT = 0,
    DISPLAY_FONT_ID_ARIAL_12PT,
    DISPLAY_FONT_ID_ARIAL_7PT,
    DISPLAY_FONT_ID_ARIAL_AUTO,

    // Do not place values below!
    DISPLAY_FONT_ID_COUNT
} display_font_id_e;

/**
 * TODO Comment!
 */
typedef enum _display_transition_id_e
{
    DISPLAY_TRANSITION_ID_NONE = 0,
    DISPLAY_TRANSITION_ID_SLIDE_UP,
    DISPLAY_TRANSITION_ID_SLIDE_UP_CHANGED,
    DISPLAY_TRANSITION_ID_SLIDE_DOWN,
    DISPLAY_TRANSITION_ID_SLIDE_DOWN_CHANGED,
    DISPLAY_TRANSITION_ID_SLIDE_LEFT,
    DISPLAY_TRANSITION_ID_SLIDE_RIGHT,

    // Do not place values below!
    DISPLAY_TRANSITION_ID_COUNT
} display_transition_id_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
struct _display_data_descriptor_t_;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void displayDriverInit(void);

/**
 * TODO Comment!
 */
extern bool displayDriverSetEnabled(bool enabled);

/**
 * TODO Comment!
 */
extern bool displayDriverWriteData(struct _display_data_descriptor_t_
                                       descriptor,
                                   data_array_t data);

/**
 * TODO Comment!
 */
extern bool displayDriverWriteText(struct _display_data_descriptor_t_
                                       descriptor,
                                   display_font_id_e fontId,
                                   const char text[]);

/**
 * TODO Comment!
 */
extern bool displayDriverClear(void);

/**
 * TODO Comment!
 */
extern bool displayDriverSetBrightness(uint8_t brightness);

/**
 * TODO Comment!
 */
extern bool displayDriverSetOn(bool on);

#endif // _DISPLAY_DRIVER_H_
