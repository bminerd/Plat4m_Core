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
 * @file ui_interface.c
 * @author Ben Minerd
 * @date 8/21/2012
 * @brief TODO Comment!
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <ui_interface.h>
#include <system_interface.h>
#include <display_interface.h>

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
#define UI_PROCEDURE_DELAY_MS (10)

/*------------------------------------------------------------------------------
 * Enumerations
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Types
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
typedef struct _ui_input_state_t_
{
    uint32_t timeMs;
    bool isActive;
} ui_input_state_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_event_map_entry_t_
{
    const ui_input_event_definition_t* definition;
    bool isEnabled;
    unsigned int eventsSinceHandled;
} ui_input_event_map_entry_t;

/**
 * @brief UI input wrapper type.
 */
typedef struct _ui_input_wrapper_t_
{
    ui_input_t input;
    bool isEnabled;
} ui_input_wrapper_t;

/**
 * @brief UI input channel wrapper type.
 */
typedef struct _ui_input_channel_wrapper_t_
{
    ui_input_channel_t channel;
    bool isEnabled;
    ui_input_event_map_entry_t eventMap[UI_INPUT_EVENT_COUNT];
    ui_input_state_t stateLog[UI_INPUT_STATE_LOG_COUNT];
} ui_input_channel_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief Array of UI inputMap.
 */
static ui_input_wrapper_t inputMap[UI_INPUT_COUNT];

/**
 * @brief Array of UI event definitions.
 */
static ui_input_event_definition_t eventDefinitionMap[UI_INPUT_EVENT_COUNT];

/**
 * @brief Array of UI input channelMap.
 */
static ui_input_channel_wrapper_t channelMap[UI_INPUT_CHANNEL_COUNT];

/**
 * TODO Comment!
 */
static ui_input_behavior_map_t behaviorMap;

/**
 * TODO Comment!
 */
static ui_input_behavior_e curBehavior;

/**
 * TODO Comment!
 */
static const ui_menu_item_t* mainMenuItem;

/**
 * TODO Comment!
 */
static const ui_menu_item_t* curMenuItem;

/**
 * TODO Comment!
 */
static const ui_menu_item_t* lastMenuItem;

/**
 * TODO Comment!
 */
static int curChildIndex = 0;

/*------------------------------------------------------------------------------
 * Local function declarations
 *----------------------------------------------------------------------------*/

/**
 * @brief Adds the given input to the map.
 * @param input Input to use.
 * @return UI error.
 */
static ui_error_e addInput(const ui_input_t* input);

/**
 * @brief Adds the given input event definition to the map.
 * @param definition Event definition to use.
 * @return UI error.
 */
static ui_error_e addInputEventDefinition(
                                const ui_input_event_definition_t* definition);

/**
 * @brief Adds the given input channel to the map.
 * @param channel Channel to use.
 * @return UI error.
 */
static ui_error_e addInputChannel(const ui_input_channel_t* channel);

/**
 * TODO Comment!
 */
static void procedure(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
extern void uiInit(void)
{
    int i, j;

    for (i = 0; i < UI_INPUT_COUNT; i++)
    {
        inputMap[i].input.input     = UI_INPUT_NONE;
        inputMap[i].input.isActive  = NULL_POINTER;
        inputMap[i].isEnabled       = false;
    }

    for (i = 0; i < UI_INPUT_EVENT_COUNT; i++)
    {
        eventDefinitionMap[i].event             = UI_INPUT_EVENT_NONE;
        eventDefinitionMap[i].sequence.states   = NULL_POINTER;
        eventDefinitionMap[i].sequence.size     = 0;
    }

    for (i = 0; i < UI_INPUT_CHANNEL_COUNT; i++)
    {
        channelMap[i].channel.channel               = UI_INPUT_CHANNEL_NONE;
        channelMap[i].channel.sourceList.sources    = NULL_POINTER;
        channelMap[i].channel.sourceList.size       = 0;
        channelMap[i].channel.handler               = NULL_POINTER;
        channelMap[i].isEnabled                     = false;

        for (j = 0; j < UI_INPUT_EVENT_COUNT; j++)
        {
            channelMap[i].eventMap[j].definition          = NULL_POINTER;
            channelMap[i].eventMap[j].isEnabled           = false;
            channelMap[i].eventMap[j].eventsSinceHandled  = 0;
        }

        for (j = 0; j < UI_INPUT_STATE_LOG_COUNT; j++)
        {
            channelMap[i].stateLog[j].timeMs      = 0;
            channelMap[i].stateLog[j].isActive    = false;
        }
    }

    system_procedure_t uiProcedure =
    {
        .procedure      = SYSTEM_PROCEDURE_UI,
        .callback       = procedure,
        .timeDelayMs    = UI_PROCEDURE_DELAY_MS,
        .priority       = 1
    };

    systemAddProcedure(&uiProcedure);
    systemProcedureSetEnabled(SYSTEM_PROCEDURE_UI, true);

    uiApplicationInit();
}

//------------------------------------------------------------------------------
extern ui_error_e uiAddInputs(const ui_input_t inputs[],
                              const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > UI_INPUT_COUNT)
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        ui_error_e error = addInput(&(inputs[i]));

#ifdef PLAT4M_DEBUG

        if (error != UI_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiAddInputEventDefinitions(
                                const ui_input_event_definition_t definitions[],
                                const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > UI_INPUT_EVENT_COUNT)
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        ui_error_e error = addInputEventDefinition(&(definitions[i]));

#ifdef PLAT4M_DEBUG

        if (error != UI_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiAddInputChannels(const ui_input_channel_t channels[],
                                     const unsigned int size)
{
#ifdef PLAT4M_DEBUG

    if (size > UI_INPUT_CHANNEL_COUNT)
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;

    for (i = 0; i < size; i++)
    {
        ui_error_e error = addInputChannel(&(channels[i]));

#ifdef PLAT4M_DEBUG

        if (error != UI_ERROR_NONE)
        {
            return error;
        }

#endif // PLAT4M_DEBUG

    }

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiInputSetEnabled(const ui_input_e input, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((input == UI_INPUT_NONE) || (input >= UI_INPUT_COUNT))
    {
        return UI_ERROR_INPUT_INVALID;
    }

#endif // PLAT4M_DEBUG

    inputMap[input].isEnabled = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiInputIsEnabled(const ui_input_e input, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isEnabled))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

    if ((input == UI_INPUT_NONE) || (input >= UI_INPUT_COUNT))
    {
        return UI_ERROR_INPUT_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = inputMap[input].isEnabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiInputIsActive(const ui_input_e input, bool* isActive)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isActive))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

    if ((input == UI_INPUT_NONE) || (input >= UI_INPUT_COUNT))
    {
        return UI_ERROR_INPUT_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isActive = inputMap[input].input.isActive();

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiInputChannelSetEnabled(const ui_input_channel_e channel,
                                           const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if ((channel == UI_INPUT_CHANNEL_NONE) ||
        (channel >= UI_INPUT_CHANNEL_COUNT))
    {
        return UI_ERROR_INPUT_CHANNEL_INVALID;
    }

#endif // PLAT4M_DEBUG

    channelMap[channel].isEnabled = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiInputChannelIsEnabled(const ui_input_channel_e channel,
                                          bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if ((channel == UI_INPUT_CHANNEL_NONE) ||
        (channel >= UI_INPUT_CHANNEL_COUNT))
    {
        return UI_ERROR_INPUT_CHANNEL_INVALID;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = channelMap[channel].isEnabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiInputChannelIsActive(const ui_input_channel_e channel,
                                         bool* isActive)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(isActive))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

    if ((channel == UI_INPUT_CHANNEL_NONE) ||
        (channel >= UI_INPUT_CHANNEL_COUNT))
    {
        return UI_ERROR_INPUT_CHANNEL_INVALID;
    }

#endif // PLAT4M_DEBUG

    int i;
    *isActive = true;

    for (i = 0; i < channelMap[channel].channel.sourceList.size; i++)
    {
        if (!inputMap[channelMap[channel].channel.sourceList.sources[i]].
                isEnabled)
        {
            channelMap[channel].isEnabled = false;

            break;
        }

        // Check if input is currently active
        if (!inputMap[channelMap[channel].channel.sourceList.sources[i]].
                input.isActive())
        {
            *isActive = false;

            break;
        }
    }

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiSetInputBehaviorMap(const ui_input_behavior_map_t* map)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(map))
    {
        return false;
    }

#endif // PLAT4M_DEBUG

    behaviorMap = *map;

    uiSetInputBehavior((ui_input_behavior_e) 0);

    return true;
}

//------------------------------------------------------------------------------
extern ui_error_e uiSetInputBehavior(const ui_input_behavior_e behavior)
{
#ifdef PLAT4M_DEBUG

    if ((behavior == UI_INPUT_BEHAVIOR_NONE) ||
        (behavior >= UI_INPUT_BEHAVIOR_COUNT))
    {
        return UI_ERROR_INPUT_BEHAVIOR_INVALID;
    }

#endif

    int i, j;

    curBehavior = behavior;

    for (i = 0; i < UI_INPUT_CHANNEL_COUNT; i++)
    {
        for (j = 0; j < UI_INPUT_EVENT_COUNT; j++)
        {
            channelMap[i].eventMap[j].isEnabled =
                behaviorMap.map[curBehavior][i][j];
        }
    }

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_error_e uiSetMenu(const ui_menu_item_t* menu)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(menu))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    mainMenuItem = menu;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern void uiMenuEnter(void)
{
    display_data_definition_t definition;
    definition.section    = DISPLAY_SECTION_ALL;
    definition.alignment  = DISPLAY_ALIGNMENT_CENTER;
    definition.transition = DISPLAY_TRANSITION_NONE;

    displayWriteText(&definition, DISPLAY_FONT_ARIAL_AUTO, "Menu");
    systemDelayMs(500);

    if (!IS_NULL_POINTER(mainMenuItem))
    {
        curMenuItem = mainMenuItem->children[0];
        lastMenuItem = NULL_POINTER;
        curChildIndex = -1;
        uiMenuItemNext();
    }
}

//------------------------------------------------------------------------------
extern void uiMenuExit(void)
{
    display_data_definition_t definition;
    definition.section    = DISPLAY_SECTION_ALL;
    definition.alignment  = DISPLAY_ALIGNMENT_CENTER;
    definition.transition = UI_DISPLAY_TRANSITION_PARENT;

    displayWriteText(&definition, DISPLAY_FONT_ARIAL_AUTO, "Menu");
    systemDelayMs(500);
    systemStateTransition(SYSTEM_STATE_ON);
}

//------------------------------------------------------------------------------
extern void uiMenuItemEnter(void)
{
    if (!IS_NULL_POINTER(curMenuItem->enter))
    {
        curMenuItem->enter();
    }

    // Save current menu item
    lastMenuItem = curMenuItem;

    // If current menu item has no children, exit to the parent
    if (IS_NULL_POINTER(curMenuItem->children))
    {

        if (!IS_NULL_POINTER(curMenuItem->edit))
        {
            curMenuItem->edit(UI_MENU_ITEM_EDIT_WRITE);
        }

        curMenuItem = curMenuItem->parent;

        // If current menu item is the root menu item, exit the entire menu (because
        // we just exited from the top level menu)
        if (curMenuItem == mainMenuItem)
        {
            uiMenuExit();

            return ;
        }

        int i;

        for (i = 0; i < curMenuItem->parent->childCount; i++)
        {
            if (curMenuItem->parent->children[i] == curMenuItem)
            {
                curChildIndex = i - 1;

                break;
            }
        }
    }
    // Else advance to current menu item's children
    else
    {
        curMenuItem = curMenuItem->children[0];
        curChildIndex = -1;
    }

    if (!IS_NULL_POINTER(curMenuItem->edit))
    {
        curMenuItem->edit(UI_MENU_ITEM_EDIT_READ);
    }
    else
    {
        uiMenuItemNext();
    }
}

//------------------------------------------------------------------------------
extern void uiMenuItemExit(void)
{

}

//------------------------------------------------------------------------------
extern void uiMenuItemNext(void)
{
    display_data_definition_t definition;
    definition.section = DISPLAY_SECTION_ALL;
    definition.alignment = DISPLAY_ALIGNMENT_CENTER;

    // Default transition is sibling --> next sibling
    definition.transition = UI_DISPLAY_TRANSITION_NEXT_SIBLING;

    if (IS_NULL_POINTER(lastMenuItem))
    {
        definition.transition = UI_DISPLAY_TRANSITION_CHILD;
    }
    // If last menu item was current item's parent, perform parent --> child
    // transition
    else if (lastMenuItem == curMenuItem->parent)
    {
        definition.transition = UI_DISPLAY_TRANSITION_CHILD;
    }
    // Else compare last menu item with current menu item's children
    else
    {
        int i;

        for (i = 0; i < curMenuItem->childCount; i++)
        {
            // If last menu item is one of current menu item's children, perform
            // child --> parent transition
            if (lastMenuItem == curMenuItem->children[i])
            {
                definition.transition = UI_DISPLAY_TRANSITION_PARENT;

                break;
            }
        }
    }

    // Save current menu item
    lastMenuItem = curMenuItem;

    curChildIndex++;

    if (curChildIndex >= curMenuItem->parent->childCount)
    {
        curChildIndex = 0;
    }

    curMenuItem = curMenuItem->parent->children[curChildIndex];

    if (!IS_NULL_POINTER(curMenuItem->edit))
    {
        curMenuItem->edit(UI_MENU_ITEM_EDIT_INCREASE);
    }
    else
    {
        curMenuItem->display(&definition);
    }
}

//------------------------------------------------------------------------------
extern void uiMenuItemPrevious(void)
{
    lastMenuItem = curMenuItem;

    curChildIndex--;

    if (curChildIndex < 0)
    {
        curChildIndex = curMenuItem->parent->childCount - 1;
    }

    curMenuItem = curMenuItem->parent->children[curChildIndex];

    display_data_definition_t definition;
    definition.section = DISPLAY_SECTION_ALL;
    definition.alignment = DISPLAY_ALIGNMENT_CENTER;
    definition.transition = UI_DISPLAY_TRANSITION_PREV_SIBLING;

    if (!IS_NULL_POINTER(curMenuItem->edit))
    {
        curMenuItem->edit(UI_MENU_ITEM_EDIT_DECREASE);
    }
    else
    {
        curMenuItem->display(&definition);
    }
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static ui_error_e addInput(const ui_input_t* input)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(input))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

    if (input->input >= UI_INPUT_COUNT)
    {
        return UI_ERROR_INPUT_INVALID;
    }

#endif // PLAT4M_DEBUG

    inputMap[input->input].input = *input;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
static ui_error_e addInputEventDefinition(
                                const ui_input_event_definition_t* definition)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(definition))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

    if (definition->event >= UI_INPUT_EVENT_COUNT)
    {
        return UI_ERROR_INPUT_EVENT_INVALID;
    }

#endif // PLAT4M_DEBUG

    eventDefinitionMap[definition->event] = *definition;

    int i;

    for (i = 0; i < UI_INPUT_CHANNEL_COUNT; i++)
    {
        channelMap[i].eventMap[definition->event].definition =
            &eventDefinitionMap[definition->event];
    }

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
static ui_error_e addInputChannel(const ui_input_channel_t* channel)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(channel))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

    if (channel->channel >= UI_INPUT_CHANNEL_COUNT)
    {
        return UI_ERROR_INPUT_CHANNEL_INVALID;
    }

#endif // PLAT4M_DEBUG

    channelMap[channel->channel].channel = *channel;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
static void procedure(void)
{
    int i, j, k;
    uint32_t timeMs = systemTimeMsGet();
    bool isActive = false;
    ui_input_state_t state;
    ui_input_event_t event;

    for (i = 0; i < UI_INPUT_CHANNEL_COUNT; i++)
    {
        if (channelMap[i].isEnabled)
        {
            for (j = 0; j < channelMap[i].channel.sourceList.size; j++)
            {
                if (!inputMap[channelMap[i].channel.sourceList.sources[j]].
                        isEnabled)
                {
                    channelMap[i].isEnabled = false;

                    break;
                }

                // Check if input is currently active
                if (!inputMap[channelMap[i].channel.sourceList.sources[j]].
                        input.isActive())
                {
                    isActive = false;

                    break;
                }
            }

            if (j == channelMap[i].channel.sourceList.size)
            {
                isActive = true;
            }

            if (isActive != channelMap[i].stateLog[0].isActive)
            {
                // Push all old events back one slot
                for (j = UI_INPUT_STATE_LOG_COUNT - 1; j > 0; j--)
                {
                    channelMap[i].stateLog[j] =
                        channelMap[i].stateLog[j - 1];
                }

                state.timeMs = timeMs;
                state.isActive = isActive;
                channelMap[i].stateLog[0] = state;

                for (j = 0; j < UI_INPUT_EVENT_COUNT; j++)
                {
                    channelMap[i].eventMap[j].eventsSinceHandled++;
                }
            }

            // Attempt to match patterns in state log
            for (j = 0; j < UI_INPUT_EVENT_COUNT; j++)
            {
                if (channelMap[i].eventMap[j].isEnabled &&
                    (channelMap[i].eventMap[j].eventsSinceHandled >=
                        eventDefinitionMap[j].sequence.size))
                {
                    int logIndex = 0;

                    for (k = eventDefinitionMap[j].sequence.size - 1; k >= 0; k--)
                    {
                        // If levels don't match, exit loop
                        if (eventDefinitionMap[j].sequence.states[k].isActive !=
                                channelMap[i].stateLog[logIndex].isActive)
                        {
                            break;
                        }

                        // Most recent event
                        if (logIndex == 0)
                        {
                            // If event hasn't been active for minimum time,
                            // exit the loop (the most recent event is
                            // compared against system time)
                            if ((channelMap[i].stateLog[logIndex].timeMs +
                                    eventDefinitionMap[j].sequence.states[k].minDurationMs) >
                                        timeMs)
                            {
                                break;
                            }
                        }
                        // Older events
                        else
                        {
                            // If event hasn't been active for minimum time,
                            // exit the loop (older events are compared
                            // against previous)
                            if (((channelMap[i].stateLog[logIndex].timeMs +
                                     eventDefinitionMap[j].sequence.states[k].minDurationMs) >
                                  channelMap[i].stateLog[logIndex - 1].timeMs)          ||
                                 (((channelMap[i].stateLog[logIndex].timeMs +
                                         eventDefinitionMap[j].sequence.states[k].maxDurationMs) <=
                                    channelMap[i].stateLog[logIndex - 1].timeMs) &&
                                 (eventDefinitionMap[j].sequence.states[k].maxDurationMs != 0)))
                            {
                                break;
                            }
                        }

                        logIndex++;
                    }

                    // If reached last state of event definition, the log
                    // pattern matches the event --> call handler
                    if (k == -1)
                    {
                        event.event     = (ui_input_event_e) j;
                        event.channel   = (ui_input_channel_e) i;
                        event.timeMs    = timeMs;
                        channelMap[i].channel.handler(&event);
                        channelMap[i].eventMap[j].eventsSinceHandled = 0;
                    }
                }
            }
        }
    }
}
