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
    UI_ERROR_INPUT_INVALID,
    UI_ERROR_INPUT_EVENT_INVALID,
    UI_ERROR_INPUT_CHANNEL_INVALID,
    UI_ERROR_INPUT_BEHAVIOR_INVALID
} ui_error_e;

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
 * @brief UI input type.
 */
typedef struct _ui_input_t_
{
    ui_input_e input;
    const ui_input_is_active_f* isActive;
} ui_input_t;

/**
 * @brief UI input sequence state type.
 */
typedef struct _ui_input_sequence_state_t_
{
    const uint32_t minDurationMs;
    const uint32_t maxDurationMs;
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
 * @brief UI input event type.
 */
typedef struct _ui_input_event_t_
{
    ui_input_event_e event;
    ui_input_channel_e channel;
    uint32_t timeMs;
} ui_input_event_t;

/**
 * @brief UI input event definition type.
 */
typedef struct _ui_input_event_definition_t_
{
    ui_input_event_e event;
    ui_input_sequence_t sequence;
} ui_input_event_definition_t;

/**
 * @brief UI input channel source list type.
 */
typedef struct _ui_input_channel_source_list_t_
{
    ui_input_e* sources;
    unsigned int size;
} ui_input_channel_source_list_t;

/**
 * @brief Function type that is a handler for a UI input channel event.
 * @param event UI input event.
 */
typedef void ui_input_channel_event_handler_f(const ui_input_event_t* event);

/**
 * @brief UI input channel definition type.
 */
typedef struct _ui_input_channel_t_
{
    ui_input_channel_e channel;
    ui_input_channel_source_list_t sourceList;
    const ui_input_channel_event_handler_f* handler;
} ui_input_channel_t;

/**
 * @brief UI input behavior map type.
 */
typedef struct _ui_input_behavior_map_t_
{
    bool map[UI_INPUT_BEHAVIOR_COUNT]
            [UI_INPUT_CHANNEL_COUNT]
            [UI_INPUT_EVENT_COUNT];
} ui_input_behavior_map_t;

/**
 * @brief Function type that displays a UI menu item.
 * @param definition Display data definition.
 */
typedef void ui_menu_item_display_f(
                                const display_data_definition_t* definition);

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
    const ui_menu_item_display_f* display;
    const procedure_f* enter;
    const ui_menu_item_edit_f* edit;
    struct _ui_menu_item_t_* parent;
    struct _ui_menu_item_t_** children;
    int childCount;
} ui_menu_item_t;

/*------------------------------------------------------------------------------
 * Global function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Initializes the UI interface layer.
 */
extern void uiInit(void);

/**
 * @brief Adds the given UI inputs.
 * @param inputs Inputs to add.
 * @param size Number of inputs to add.
 * @return UI error.
 */
extern ui_error_e uiAddInputs(const ui_input_t inputs[],
                              const unsigned int size);

/**
 * @brief Adds the given UI input event definitions.
 * @param definitions Event definitions to add.
 * @param size Number of event definitions to add.
 * @return UI error.
 */
extern ui_error_e uiAddInputEventDefinitions(
                                const ui_input_event_definition_t definitions[],
                                const unsigned int size);

/**
 * @brief Adds the given UI input channels.
 * @param channels Channels to add.
 * @param size Number of channels to add.
 * @return UI error.
 */
extern ui_error_e uiAddInputChannels(const ui_input_channel_t channels[],
                                     const unsigned int size);

/**
 * @brief Sets the given UI input enabled or disabled.
 * @param input UI input to access.
 * @param enabled Flag that indicates if the UI input should be enabled or
 * disabled.
 * @return UI error.
 */
extern ui_error_e uiInputSetEnabled(const ui_input_e input, const bool enabled);

/**
 * @brief Checks to see if the given UI input is enabled.
 * @param input UI input to access.
 * @param isEnabled Flag that indicates if the UI input is enabled or disabled.
 * @return UI error.
 */
extern ui_error_e uiInputIsEnabled(const ui_input_e input, bool* isEnabled);

/**
 * @brief Checks to see if the given UI input is active.
 * @param input UI input to access.
 * @param isActive Flag that indicates if the UI input is active or inactive.
 * @return UI error.
 */
extern ui_error_e uiInputIsActive(const ui_input_e input, bool* isActive);

/**
 * @brief Sets the given UI input channel enabled or disabled.
 * @param channel UI input channel to access.
 * @param enabled Flag that indicates if the UI input channel should be enabled
 * or disabled.
 * @return UI error.
 */
extern ui_error_e uiInputChannelSetEnabled(const ui_input_channel_e channel,
                                           const bool enabled);

/**
 * @brief Checks to see if the given UI input channel is enabled or disabled.
 * @param channel UI input channel to access.
 * @param isEnabled Flag that indicates if the UI input channel is enabled or
 * disabled.
 * @return UI error.
 */
extern ui_error_e uiInputChannelIsEnabled(const ui_input_channel_e channel,
                                          bool* isEnabled);

/**
 * @brief Checks to see if the given UI input channel is active.
 * @param channel UI input channel to access.
 * @param isActive Flag that indicates if the UI input channel is active or
 * inactive.
 * @return UI error.
 */
extern ui_error_e uiInputChannelIsActive(const ui_input_channel_e channel,
                                         bool* isActive);

/**
 * @brief Sets the UI input behavior map.
 * @param map UI input behavior map to set.
 * @return UI error.
 */
extern ui_error_e uiSetInputBehaviorMap(const ui_input_behavior_map_t* map);

/**
 * @brief Sets the UI input behavior.
 * @param behavior UI input behavior to set.
 * @return UI error.
 */
extern ui_error_e uiSetInputBehavior(const ui_input_behavior_e behavior);

/**
 * @brief Sets the UI menu.
 * @param menu UI menu to set.
 * @note Menu parameter must point to a static variable!
 * @return UI error.
 */
extern ui_error_e uiSetMenu(const ui_menu_item_t* menu);

/**
 * @brief Enters the UI menu.
 */
extern void uiMenuEnter(void);

/**
 * @brief Exits the UI menu.
 */
extern void uiMenuExit(void);

/**
 * @brief Enters the current UI menu item.
 */
extern void uiMenuItemEnter(void);

/**
 * @brief Exits the current UI menu item.
 */
extern void uiMenuItemExit(void);

/**
 * @brief Proceeds to the next UI menu item.
 */
extern void uiMenuItemNext(void);

/**
 * @brief Returns to the previous UI menu item.
 */
extern void uiMenuItemPrevious(void);

#endif // _UI_INTERFACE_H_
