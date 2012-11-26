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
 * @brief UI interface layer.
 */

#ifndef _UI_INTERFACE_H_
#define _UI_INTERFACE_H_

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <system_types.h>

#include <ui_application.h>
#include <display_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/**
 * @brief Enumeration of UI errors.
 */
typedef enum _ui_error_e_
{
    UI_ERROR_NONE,
    UI_ERROR_PARAMETER_INVALID,
    UI_ERROR_INPUT_NOT_INITIALIZED,
    UI_ERROR_INPUT_NOT_ENABLED,
    UI_ERROR_INPUT_CHANNEL_NOT_INITIALIZED,
    UI_ERROR_INPUT_CHANNEL_NOT_ENABLED,
    UI_ERROR_INPUT_EVENT_NOT_INITIALIZED,
    UI_ERROR_INPUT_EVENT_NOT_ENABLED,
    UI_ERROR_STATE_NOT_INITIALIZED,
    UI_ERROR_STATE_NOT_ENABLED
} ui_error_e;

/**
 * @brief Enumeration of UI menu item transitions.
 */
typedef enum _ui_menu_item_transition_e_
{
    UI_MENU_ITEM_TRANSITION_CHILD,
    UI_MENU_ITEM_TRANSITION_PARENT,
    UI_MENU_ITEM_TRANSITION_NEXT_SIBLING,
    UI_MENU_ITEM_TRANSITION_PREV_SIBLING,

    // Do not place values below!
    UI_MENU_ITEM_TRANSITION_COUNT
} ui_menu_item_transition_e;

/**
 * @brief Enumeration of UI menu item edits.
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
 * @brief Function type that checks to see if a UI input is active.
 * @return True if the input is active, false if not.
 */
typedef bool ui_input_is_active_f(void);

/**
 * @brief UI input info type.
 */
typedef struct _ui_input_info_t_
{
    bool isInitialized;
    bool isEnabled;
} ui_input_info_t;

/**
 * @brief UI input type.
 */
typedef struct _ui_input_id_t_
{
    ui_input_is_active_f* isActive;
    const generic_data_t private[sizeof(ui_input_info_t) /
                                 sizeof(generic_data_t)];
} ui_input_id_t;

/**
 * @brief UI input sequence state type.
 */
typedef struct _ui_input_sequence_state_t_
{
    const time_ms_t minDurationMs;
    const time_ms_t maxDurationMs;
    const bool isActive;
} ui_input_sequence_state_t;

/**
 * @brief UI input sequence type.
 */
typedef struct _ui_input_sequence_t_
{
    const ui_input_sequence_state_t* states;
    unsigned int size;
} ui_input_sequence_t;

/**
 * @brief UI input event definition type.
 */
typedef struct _ui_input_event_info_t_
{
    bool isInitialized;
    bool isEnabled;
} ui_input_event_info_t;

typedef struct _ui_input_event_id_t_
{
    const ui_input_sequence_t sequence;
    const generic_data_t private[sizeof(ui_input_event_info_t) /
                                 sizeof(generic_data_t)];
} ui_input_event_id_t;

/**
 * @brief UI input event map entry type;
 */
typedef struct _ui_input_event_map_entry_t_
{
    ui_input_event_id_t* eventId;
    bool isEnabled;
    unsigned int eventsSinceHandled;
} ui_input_event_map_entry_t;

/**
 * @brief UI input event map type.
 */
typedef struct _ui_input_event_map_t_
{
    ui_input_event_map_entry_t* entries;
    int size;
} ui_input_event_map_t;

/**
 * @brief UI input state type.
 */
typedef struct _ui_input_state_t_
{
    uint32_t timeMs;
    bool isActive;
} ui_input_state_t;

/**
 * @brief UI input state log type.
 */
typedef struct _ui_input_state_log_t_
{
    ui_input_state_t* entries;
    int size;
} ui_input_state_log_t;

/**
 * @brief UI input channel source list type.
 */
typedef struct _ui_input_channel_source_list_t_
{
    ui_input_id_t** sources;
    unsigned int size;
} ui_input_channel_source_list_t;

/**
 * @brief Predefinition of UI input event type.
 */
struct _ui_input_event_t_;

/**
 * @brief Function type that is a handler for a UI input channel event.
 * @param event UI input event.
 */
typedef void ui_input_channel_event_handler_f(
                                        const struct _ui_input_event_t_* event);

/**
 * @brief UI input channel info type.
 */
typedef struct _ui_input_channel_info_t_
{
    bool isInitialized;
    bool isEnabled;
    ui_input_event_map_t eventMap;
    ui_input_state_log_t stateLog;
} ui_input_channel_info_t;

/**
 * @brief UI input channel ID type.
 */
typedef struct _ui_input_channel_id_t_
{
    ui_input_channel_source_list_t sourceList;
    ui_input_channel_event_handler_f* handler;
    const generic_data_t private[sizeof(ui_input_channel_info_t) /
                                 sizeof(generic_data_t)];
} ui_input_channel_id_t;

/**
 * @brief UI input event type.
 */
typedef struct _ui_input_event_t_
{
    ui_input_event_id_t* eventId;
    ui_input_channel_id_t* channelId;
    uint32_t timeMs;
} ui_input_event_t;

typedef struct _ui_input_channel_map_entry_t_
{
    ui_input_channel_id_t* channelId;
    ui_input_event_map_t eventMap;
} ui_input_channel_map_entry_t;

typedef struct _ui_input_channel_map_t_
{
    ui_input_channel_map_entry_t* entries;
    int size;
} ui_input_channel_map_t;

typedef procedure_f ui_state_enter_f;

typedef procedure_f ui_state_exit_f;

/**
 * @brief UI state info type.
 */
typedef struct _ui_state_info_t_
{
    bool isInitialized;
    bool isEnabled;
} ui_state_info_t;

/**
 * @brief UI state ID type.
 */
typedef struct _ui_state_id_t_
{
    ui_state_enter_f* enter;
    ui_state_exit_f* exit;
    const ui_input_channel_map_t channelMap;
    const generic_data_t private[sizeof(ui_state_info_t) /
                                 sizeof(generic_data_t)];
} ui_state_id_t;

/**
 * @brief Function type that displays a UI menu item.
 * @param definition Display data definition.
 */
typedef void ui_menu_item_display_f(const ui_menu_item_transition_e transition);

/**
 * @brief Function type that edits a UI menu item.
 * @param edit UI menu item edit option.
 */
typedef void ui_menu_item_edit_f(const ui_menu_item_edit_e edit);

/**
 * @brief Pre-definition of a UI menu item type.
 */
struct _ui_menu_item_t_;

/**
 * @brief UI menu item type.
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

/**
 * @brief Function type that enters a UI menu.
 */
typedef void ui_menu_enter_f(void);

/**
 * @brief Function type that exits a UI menu.
 */
typedef void ui_menu_exit_f(void);

/**
 * @brief UI menu type.
 */
typedef struct _ui_menu_t_
{
    ui_menu_enter_f* enter;
    ui_menu_exit_f* exit;
    const ui_menu_item_t* mainMenuItem;
} ui_menu_t;

/**
 * @brief Function type that sets the display.
 * @param id Display ID to set.
 */
typedef void ui_application_set_display_f(display_id_t* id);

/**
 * @brief UI application type.
 */
typedef struct _ui_application_t_
{
    ui_application_set_display_f* setDisplay;
} ui_application_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the UI interface layer.
 */
void uiInit(void);

/**
 * @brief Sets the application for the UI interface.
 * @param uiApplication UI application to set.
 * @return UI error.
 */
ui_error_e uiSetApplication(const ui_application_t* uiApplication);

/**
 * @brief Initializes the given UI input ID.
 * @param id UI Input to access.
 * @param enabled Flag that indicates if the UI input should be enabled or
 * disabled.
 * @return UI error.
 */
ui_error_e uiInputIdInit(ui_input_id_t* id, const bool enabled);

/**
 * @brief Sets the given UI input enabled or disabled.
 * @param input UI input to access.
 * @param enabled Flag that indicates if the UI input should be enabled or
 * disabled.
 * @return UI error.
 */
ui_error_e uiInputSetEnabled(ui_input_id_t* id, const bool enabled);

/**
 * @brief Checks to see if the given UI input is enabled.
 * @param input UI input to access.
 * @param isEnabled Flag that indicates if the UI input is enabled or disabled.
 * @return UI error.
 */
ui_error_e uiInputIsEnabled(ui_input_id_t* id, bool* isEnabled);

/**
 * @brief Checks to see if the given UI input is active.
 * @param input UI input to access.
 * @param isActive Flag that indicates if the UI input is active or inactive.
 * @return UI error.
 */
ui_error_e uiInputIsActive(ui_input_id_t* id, bool* isActive);

/**
 * @brief Initializes the given UI input event ID.
 * @param id UI Input event to access.
 * @return UI error.
 */
ui_error_e uiInputEventInitId(ui_input_event_id_t* id, const bool enabled);

/**
 * @brief Initializes the given UI input channel.
 * @param id UI Input channel to access.
 * @return UI error.
 */
ui_error_e uiInputChannelInit(ui_input_channel_id_t* id, const bool enabled);

/**
 * @brief Sets the given UI input channel enabled or disabled.
 * @param channel UI input channel to access.
 * @param enabled Flag that indicates if the UI input channel should be enabled
 * or disabled.
 * @return UI error.
 */
ui_error_e uiInputChannelSetEnabled(ui_input_channel_id_t* id,
                                    const bool enabled);

/**
 * @brief Checks to see if the given UI input channel is enabled or disabled.
 * @param channel UI input channel to access.
 * @param isEnabled Flag that indicates if the UI input channel is enabled or
 * disabled.
 * @return UI error.
 */
ui_error_e uiInputChannelIsEnabled(ui_input_channel_id_t* id, bool* isEnabled);

/**
 * @brief Checks to see if the given UI input channel is active.
 * @param channel UI input channel to access.
 * @param isActive Flag that indicates if the UI input channel is active or
 * inactive.
 * @return UI error.
 */
ui_error_e uiInputChannelIsActive(ui_input_channel_id_t* id, bool* isActive);

ui_error_e uiStateInitId(ui_state_id_t* id, const bool enabled);

ui_error_e uiStateSetEnabled(ui_state_id_t* id, const bool enabled);

ui_error_e uiStateIsEnabled(ui_state_id_t* id, bool* isEnabled);

ui_error_e uiStateTransition(ui_state_id_t* id);

/**
 * @brief Sets the display for the UI interface.
 * @param display Display to use.
 * @return UI error.
 */
ui_error_e uiSetDisplay(display_id_t* id);

/**
 * @brief Sets the UI menu.
 * @param menu UI menu to set.
 * @return UI error.
 */
ui_error_e uiSetMenu(const ui_menu_t* menu);

/**
 * @brief Enters the UI menu.
 */
void uiMenuEnter(void);

/**
 * @brief Exits the UI menu.
 */
void uiMenuExit(void);

/**
 * @brief Enters the current UI menu item.
 */
void uiMenuItemEnter(void);

/**
 * @brief Exits the current UI menu item.
 */
void uiMenuItemExit(void);

/**
 * @brief Proceeds to the next UI menu item.
 */
void uiMenuItemNext(void);

/**
 * @brief Returns to the previous UI menu item.
 */
void uiMenuItemPrevious(void);

#endif // _UI_INTERFACE_H_
