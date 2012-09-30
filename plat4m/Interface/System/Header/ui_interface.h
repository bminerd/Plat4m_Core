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
 * @file ui_interface.h
 * @author Ben Minerd
 * @date 8/21/2012
 * @brief TODO Comment!
 */

#ifndef _UI_INTERFACE_H_
#define _UI_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <ui_driver.h>
#include <display_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

#ifdef  UI_INPUT_ID_LIMIT
#define UI_INPUT_COUNT (UI_INPUT_ID_LIMIT)
#else
#define UI_INPUT_COUNT (UI_INPUT_ID_COUNT)
#endif

#ifdef  UI_INPUT_EVENT_ID_LIMIT
#define UI_INPUT_EVENT_COUNT (UI_INPUT_EVENT_ID_LIMIT)
#else
#define UI_INPUT_EVENT_COUNT (UI_INPUT_EVENT_ID_COUNT)
#endif

#ifdef  UI_INPUT_CHANNEL_ID_LIMIT
#define UI_INPUT_CHANNEL_COUNT (UI_INPUT_CHANNEL_ID_LIMIT)
#else
#define UI_INPUT_CHANNEL_COUNT (UI_INPUT_CHANNEL_ID_COUNT)
#endif

#ifdef  UI_INPUT_BEHAVIOR_ID_LIMIT
#define UI_INPUT_BEHAVIOR_COUNT (UI_INPUT_BEHAVIOR_ID_LIMIT)
#else
#define UI_INPUT_BEHAVIOR_COUNT (UI_INPUT_BEHAVIOR_ID_COUNT)
#endif

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef enum _ui_input_error_e_
{
    UI_INPUT_ERROR_NONE,
    UI_INPUT_ERROR_INVALID_ID
} ui_input_error_e;

/**
 * TODO Comment!
 */
typedef enum _ui_menu_item_edit_e_
{
    UI_MENU_ITEM_EDIT_READ,
    UI_MENU_ITEM_EDIT_WRITE,
    UI_MENU_ITEM_EDIT_INCREASE,
    UI_MENU_ITEM_EDIT_DECREASE
} ui_menu_item_edit_e;

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef bool ui_input_is_active_f(void);

/**
 * TODO Comment!
 */
typedef struct _ui_input_driver_t_
{
    ui_input_id_e id;
    bool isEnabled;
    ui_input_is_active_f* isActive;
} ui_input_driver_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_sequence_state_t_
{
    const uint32_t minDurationMs;
    const uint32_t maxDurationMs;
    const bool isActive;
} ui_input_sequence_state_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_sequence_t_
{
    const ui_input_sequence_state_t* states;
    const int size;
} ui_input_sequence_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_event_driver_t_
{
    ui_input_event_id_e id;
    ui_input_sequence_t sequence;
} ui_input_event_driver_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_channel_source_list_t_
{
    ui_input_id_e* sources;
    int size;
} ui_input_channel_source_list_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_channel_driver_t_
{
    ui_input_channel_id_e id;
    ui_input_channel_source_list_t sourceList;
} ui_input_channel_driver_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_behavior_map_t_
{
    const bool map[UI_INPUT_BEHAVIOR_COUNT]
                  [UI_INPUT_CHANNEL_COUNT]
                  [UI_INPUT_EVENT_COUNT];
} ui_input_behavior_map_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_event_t_
{
    ui_input_event_id_e id;
    ui_input_channel_id_e channelId;
    uint32_t timeMs;
} ui_input_event_t;

/**
 * TODO Comment!
 */
typedef void ui_input_channel_event_handler_f(ui_input_event_t event);

struct _ui_menu_item_t_;

/**
 * TODO Comment!
 */
typedef void ui_menu_item_display_f(display_data_descriptor_t descriptor);

/**
 * TODO Comment!
 */
typedef void ui_menu_item_edit_f(ui_menu_item_edit_e edit);

/**
 * TODO Comment!
 */
typedef struct _ui_menu_item_t_
{
    ui_menu_item_display_f* display;
    procedure_f* enter;
    ui_menu_item_edit_f* edit;
    struct _ui_menu_item_t_* parent;
    struct _ui_menu_item_t_** children;
    int childCount;
} ui_menu_item_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
extern void uiInit(void);

/**
 * TODO Comment!
 */
extern bool uiAddInputDrivers(ui_input_driver_t drivers[], uint8_t size);

/**
 * TODO Comment!
 */
extern bool uiAddInputEventDrivers(ui_input_event_driver_t drivers[], int size);

/**
 * TODO Comment!
 */
extern bool uiAddInputChannelDrivers(ui_input_channel_driver_t drivers[],
                                     int size);

/**
 * TODO Comment!
 */
extern bool uiInputChannelAddEventHandler(ui_input_channel_id_e id,
                                          ui_input_channel_event_handler_f*
                                              handler);

/**
 * TODO Comment!
 */
extern ui_input_error_e uiInputIsEnabled(ui_input_id_e id,
                                         bool* isEnabled);

/**
 * TODO Comment!
 */
extern ui_input_error_e uiInputIsActive(ui_input_id_e id,
                                        bool* isActive);

/**
 * TODO Comment!
 */
extern ui_input_error_e uiInputChannelIsActive(ui_input_channel_id_e id,
                                               bool* isActive);

/**
 * TODO Comment!
 */
extern bool uiAddInputBehaviorMap(ui_input_behavior_map_t* map);

/**
 * TODO Comment!
 */
extern ui_input_error_e uiInputBehaviorSet(ui_input_behavior_id_e id);

/**
 * TODO Comment!
 */
extern bool uiAddMenu(const ui_menu_item_t* menu);

/**
 * TODO Comment!
 */
extern void uiMenuEnter(void);

/**
 * TODO Comment!
 */
extern void uiMenuExit(void);

/**
 * TODO Comment!
 */
extern void uiMenuItemEnter(void);

/**
 * TODO Comment!
 */
extern void uiMenuItemExit(void);

/**
 * TODO Comment!
 */
extern void uiMenuItemNext(void);

/**
 * TODO Comment!
 */
extern void uiMenuItemPrevious(void);

#endif // _UI_INTERFACE_H_
