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
 * @file ui_driver.h
 * @author Ben Minerd
 * @date 8/21/2012
 * @brief TODO Comment!
 */

#ifndef _UI_DRIVER_H_
#define _UI_DRIVER_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>
#include <display_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define UI_INPUT_STATE_LOG_COUNT (4u)

/**
 * TODO Comment!
 */
#define UI_DISPLAY_TRANSITION_ID_NEXT_SIBLING \
        (DISPLAY_TRANSITION_ID_SLIDE_UP)

/**
 * TODO Comment!
 */
#define UI_DISPLAY_TRANSITION_ID_PREV_SIBLING \
        (DISPLAY_TRANSITION_ID_SLIDE_DOWN)

/**
 * TODO Comment!
 */
#define UI_DISPLAY_TRANSITION_ID_CHILD \
        (DISPLAY_TRANSITION_ID_SLIDE_LEFT)

/**
 * TODO Comment!
 */
#define UI_DISPLAY_TRANSITION_ID_PARENT \
        (DISPLAY_TRANSITION_ID_SLIDE_RIGHT)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _ui_input_id_e_
{
    UI_INPUT_ID_BUTTON_KEY = 0,
    UI_INPUT_ID_BUTTON_TAMPER,
    UI_INPUT_ID_BUTTON_WAKEUP,

    // Do not place values below!
    UI_INPUT_ID_COUNT
} ui_input_id_e;

/**
 * TODO Comment!
 */
typedef enum _ui_input_event_id_e_
{
    UI_INPUT_EVENT_ID_BUTTON_PRESS = 0,
    UI_INPUT_EVENT_ID_BUTTON_RELEASE,
    UI_INPUT_EVENT_ID_BUTTON_HOLD,
    UI_INPUT_EVENT_ID_BUTTON_SINGLE_TAP,
    UI_INPUT_EVENT_ID_BUTTON_DOUBLE_TAP,

    // Do not place values below!
    UI_INPUT_EVENT_ID_COUNT
} ui_input_event_id_e;

/**
 * TODO Comment!
 */
typedef enum _ui_input_channel_id_e_
{
    UI_INPUT_CHANNEL_ID_BUTTON_KEY = 0,
    UI_INPUT_CHANNEL_ID_BUTTON_TAMPER,
    UI_INPUT_CHANNEL_ID_BUTTON_WAKEUP,
    UI_INPUT_CHANNEL_ID_BUTTONS_KEY_TAMPER,
    UI_INPUT_CHANNEL_ID_BUTTONS_TAMPER_WAKEUP,

    // Do not place values below!
    UI_INPUT_CHANNEL_ID_COUNT,
} ui_input_channel_id_e;

/**
 * TODO Comment!
 */
typedef enum _ui_input_behavior_id_e_
{
    UI_INPUT_BEHAVIOR_ID_OFF = 0,
    UI_INPUT_BEHAVIOR_ID_ON,

    // Do not place values below!
    UI_INPUT_BEHAVIOR_ID_COUNT
} ui_input_behavior_id_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void uiDriverInit(void);

#endif // _UI_DRIVER_H_
