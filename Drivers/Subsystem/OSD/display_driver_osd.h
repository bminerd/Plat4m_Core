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
 * @file display_driver_osd.h
 * @author Ben Minerd
 * @date 8/9/2012
 * @brief Display driver layer for the OSD OLED display.
 */

#ifndef _DISPLAY_DRIVER_OSD_H_
#define _DISPLAY_DRIVER_OSD_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <i2c_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * @brief Display I2C address.
 * TODO Should this be here?
 */
#define DISPLAY_OSD_I2C_ADDRESS 0x78

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of display sections.
 */
typedef enum _display_section_e_
{
    // Do not place values above!
    DISPLAY_SECTION_NONE = -1,

    DISPLAY_SECTION_ALL,
    DISPLAY_SECTION_CENTER,
    DISPLAY_SECTION_CENTER_TOP,
    DISPLAY_SECTION_CENTER_BOTTOM,
    DISPLAY_SECTION_LEFT_TOP,
    DISPLAY_SECTION_LEFT_BOTTOM,
    DISPLAY_SECTION_RIGHT_TOP,
    DISPLAY_SECTION_RIGHT_BOTTOM,

    // Do not place values below!
    DISPLAY_SECTION_COUNT
} display_section_e;

/**
 * @brief Enumeration of display alignments.
 */
typedef enum _display_alignment_e_
{
    // Do not place values above!
    DISPLAY_ALIGNMENT_NONE = -1,

    DISPLAY_ALIGNMENT_LEFT,
    DISPLAY_ALIGNMENT_CENTER,
    DISPLAY_ALIGNMENT_RIGHT,

    // Do not place values below!
    DISPLAY_ALIGNMENT_COUNT
} display_alignment_e;

/**
 * @brief Enumeration of display fonts.
 */
typedef enum _display_font_e_
{
    // Do not place values above!
    DISPLAY_FONT_NONE = -1,

    DISPLAY_FONT_ARIAL_14PT,
    DISPLAY_FONT_ARIAL_12PT,
    DISPLAY_FONT_ARIAL_7PT,
    DISPLAY_FONT_ARIAL_AUTO,

    // Do not place values below!
    DISPLAY_FONT_COUNT
} display_font_e;

/**
 * @brief Enumeration of display transitions.
 */
typedef enum _display_transition_e
{
    // Do not place values above!
    DISPLAY_TRANSITION_NONE = -1,

    DISPLAY_TRANSITION_INSTANT,
    DISPLAY_TRANSITION_SLIDE_UP,
    DISPLAY_TRANSITION_SLIDE_UP_CHANGED,
    DISPLAY_TRANSITION_SLIDE_DOWN,
    DISPLAY_TRANSITION_SLIDE_DOWN_CHANGED,
    DISPLAY_TRANSITION_SLIDE_LEFT,
    DISPLAY_TRANSITION_SLIDE_RIGHT,

    // Do not place values below!
    DISPLAY_TRANSITION_COUNT
} display_transition_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * @brief Display OSD info type.
 */
typedef struct _display_osd_info_t_
{
    int a; // TODO Add info
} display_osd_info_t;

/**
 * @brief Display OSD type.
 */
typedef struct _display_osd_t_
{
    i2c_device_id_t* i2cDeviceId;
    generic_data_t private[sizeof(display_osd_info_t)];
} display_osd_t;

/**
 * @brief Display configuration type.
 */
typedef struct _display_osd_config_t_
{
    int a; // TODO Add configuration
} display_osd_config_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the display driver layer.
 */
void displayDriverOsdInit(void);

#endif // _DISPLAY_DRIVER_OSD_H_
