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
 * @brief UI interface layer.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <ui_interface.h>
#include <ui_application.h>

#include <system_interface.h>
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

///**
// * TODO Comment!
// */
//typedef struct _ui_input_event_map_entry_t_
//{
//    const ui_input_event_definition_t* definition;
//    bool isEnabled;
//    unsigned int eventsSinceHandled;
//} ui_input_event_map_entry_t;

///**
// * @brief UI input wrapper type.
// */
//typedef struct _ui_input_wrapper_t_
//{
//    ui_input_t input;
//    bool isEnabled;
//} ui_input_wrapper_t;

///**
// * @brief UI input channel wrapper type.
// */
//typedef struct _ui_input_channel_wrapper_t_
//{
//    ui_input_channel_t channel;
//    bool isEnabled;
//    ui_input_event_map_entry_t eventMap[UI_INPUT_EVENT_COUNT];
//    ui_input_state_t stateLog[UI_INPUT_STATE_LOG_COUNT];
//} ui_input_channel_wrapper_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * @brief UI application.
 */
static ui_application_t application;

static ui_state_id_t* currentStateId;

/**
 * TODO Comment!
 */
static const ui_menu_t* mainMenu;

/**
 * TODO Comment!
 */
static const ui_menu_item_t* currentMenuItem;

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
 * @brief UI procedure.
 */
static void procedure(void);

/*------------------------------------------------------------------------------
 * Global function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void uiInit(void)
{
    system_procedure_t uiProcedure =
    {
        .id             = SYSTEM_PROCEDURE_ID_UI,
        .callback       = procedure,
        .timeDelayMs    = UI_PROCEDURE_DELAY_MS,
        .priority       = 1
    };

    systemAddProcedure(&uiProcedure);
    systemProcedureSetEnabled(SYSTEM_PROCEDURE_ID_UI, true);

    uiApplicationInit();
}

//------------------------------------------------------------------------------
ui_error_e uiSetApplication(const ui_application_t* uiApplication)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(uiApplication)  ||
        IS_NULL_POINTER(uiApplication->setDisplay))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    application = *uiApplication;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputIdInit(ui_input_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(id->isActive))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_info_t* info = (ui_input_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputSetEnabled(ui_input_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_info_t* info = (ui_input_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_INPUT_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    info->isEnabled = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputIsEnabled(ui_input_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_info_t* info = (ui_input_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_INPUT_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = info->isEnabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputIsActive(ui_input_id_t* id, bool* isActive)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isActive))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_info_t* info = (ui_input_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_INPUT_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return UI_ERROR_INPUT_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    *isActive = id->isActive();

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputEventInitId(ui_input_event_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id)                     ||
        IS_NULL_POINTER(id->sequence.states)    ||
        (id->sequence.size == 0))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_event_info_t* info = (ui_input_event_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputChannelInit(ui_input_channel_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_event_info_t* info = (ui_input_event_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputChannelSetEnabled(ui_input_channel_id_t* id,
                                    const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_event_info_t* info = (ui_input_event_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_INPUT_CHANNEL_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    info->isEnabled = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputChannelIsEnabled(ui_input_channel_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_event_info_t* info = (ui_input_event_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_INPUT_CHANNEL_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = info->isEnabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiInputChannelIsActive(ui_input_channel_id_t* id, bool* isActive)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isActive))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_input_event_info_t* info = (ui_input_event_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_INPUT_CHANNEL_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return UI_ERROR_INPUT_CHANNEL_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    int i;
    *isActive = true;

    for (i = 0; i < id->sourceList.size; i++)
    {
        // Check if input is currently active
        if (!(id->sourceList.sources[i].isActive()))
        {
            // If any source is inactive, the channel is inactive
            *isActive = false;

            break;
        }
    }

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiStateInitId(ui_state_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id)         ||
        IS_NULL_POINTER(id->enter)  ||
        IS_NULL_POINTER(id->exit)   ||
        IS_NULL_POINTER(id->channelMap.entries))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_state_info_t* info = (ui_state_info_t*) id->private;

    info->isInitialized = true;
    info->isEnabled     = enabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiStateSetEnabled(ui_state_id_t* id, const bool enabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_state_info_t* info = (ui_state_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_STATE_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiStateIsEnabled(ui_state_id_t* id, bool* isEnabled)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id) || IS_NULL_POINTER(isEnabled))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_state_info_t* info = (ui_state_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_STATE_NOT_INITIALIZED;
    }

#endif // PLAT4M_DEBUG

    *isEnabled = info->isEnabled;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiStateTransition(ui_state_id_t* id)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(id))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    ui_state_info_t* info = (ui_state_info_t*) id->private;

#ifdef PLAT4M_DEBUG

    if (!(info->isInitialized))
    {
        return UI_ERROR_STATE_NOT_INITIALIZED;
    }

    if (!(info->isEnabled))
    {
        return UI_ERROR_STATE_NOT_ENABLED;
    }

#endif // PLAT4M_DEBUG

    currentStateId->exit();
    currentStateId = id;
    currentStateId->enter();

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiSetDisplay(display_id_t* display)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(display))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    application.setDisplay(display);

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
ui_error_e uiSetMenu(const ui_menu_t* menu)
{
#ifdef PLAT4M_DEBUG

    if (IS_NULL_POINTER(menu))
    {
        return UI_ERROR_PARAMETER_INVALID;
    }

#endif // PLAT4M_DEBUG

    mainMenu = menu;

    return UI_ERROR_NONE;
}

//------------------------------------------------------------------------------
void uiMenuEnter(void)
{
//    display_data_definition_t definition;
//    definition.section    = DISPLAY_SECTION_ALL;
//    definition.alignment  = DISPLAY_ALIGNMENT_CENTER;
//    definition.transition = DISPLAY_TRANSITION_INSTANT;
//
//    displayWriteText(mainDisplay,
//                     &definition,
//                     DISPLAY_FONT_ARIAL_AUTO,
//                     "Menu");
//    systemDelayMs(500);

    mainMenu->enter();

    if (!IS_NULL_POINTER(mainMenu))
    {
        currentMenuItem = mainMenu->mainMenuItem->children[0];
        lastMenuItem = NULL_POINTER;
        curChildIndex = -1;
        uiMenuItemNext();
    }
}

//------------------------------------------------------------------------------
void uiMenuExit(void)
{
//    display_data_definition_t definition;
//    definition.section    = DISPLAY_SECTION_ALL;
//    definition.alignment  = DISPLAY_ALIGNMENT_CENTER;
//    definition.transition = UI_DISPLAY_TRANSITION_PARENT;
//
//    displayWriteText(mainDisplay,
//                     &definition,
//                     DISPLAY_FONT_ARIAL_AUTO,
//                     "Menu");
//    systemDelayMs(500);
//    systemStateTransition(SYSTEM_STATE_ID_ON);

    mainMenu->exit();
}

//------------------------------------------------------------------------------
void uiMenuItemEnter(void)
{
    if (!IS_NULL_POINTER(currentMenuItem->enter))
    {
        currentMenuItem->enter();
    }

    // Save current menu item
    lastMenuItem = currentMenuItem;

    // If current menu item has no children, exit to the parent
    if (IS_NULL_POINTER(currentMenuItem->children))
    {

        if (!IS_NULL_POINTER(currentMenuItem->edit))
        {
            currentMenuItem->edit(UI_MENU_ITEM_EDIT_WRITE);
        }

        currentMenuItem = currentMenuItem->parent;

        // If current menu item is the root menu item, exit the entire menu (because
        // we just exited from the top level menu)
        if (currentMenuItem == (mainMenu->mainMenuItem))
        {
            uiMenuExit();

            return;
        }

        int i;

        for (i = 0; i < currentMenuItem->parent->childCount; i++)
        {
            if (currentMenuItem->parent->children[i] == currentMenuItem)
            {
                curChildIndex = i - 1;

                break;
            }
        }
    }
    // Else advance to current menu item's children
    else
    {
        currentMenuItem = currentMenuItem->children[0];
        curChildIndex = -1;
    }

    if (!IS_NULL_POINTER(currentMenuItem->edit))
    {
        currentMenuItem->edit(UI_MENU_ITEM_EDIT_READ);
    }
    else
    {
        uiMenuItemNext();
    }
}

//------------------------------------------------------------------------------
void uiMenuItemExit(void)
{

}

//------------------------------------------------------------------------------
void uiMenuItemNext(void)
{
    // Default transition is sibling --> next sibling
    ui_menu_item_transition_e transition = UI_MENU_ITEM_TRANSITION_NEXT_SIBLING;

    // If last menu item was current item's parent, perform parent --> child
    // transition
    if (IS_NULL_POINTER(lastMenuItem) || (lastMenuItem == currentMenuItem->parent))
    {
        transition = UI_MENU_ITEM_TRANSITION_CHILD;
    }
    // Else compare last menu item with current menu item's children
    else
    {
        int i;

        for (i = 0; i < currentMenuItem->childCount; i++)
        {
            // If last menu item is one of current menu item's children, perform
            // child --> parent transition
            if (lastMenuItem == currentMenuItem->children[i])
            {
                transition = UI_MENU_ITEM_TRANSITION_PARENT;

                break;
            }
        }
    }

    // Save current menu item
    lastMenuItem = currentMenuItem;

    curChildIndex++;

    if (curChildIndex >= currentMenuItem->parent->childCount)
    {
        curChildIndex = 0;
    }

    currentMenuItem = currentMenuItem->parent->children[curChildIndex];

    if (!IS_NULL_POINTER(currentMenuItem->edit))
    {
        currentMenuItem->edit(UI_MENU_ITEM_EDIT_INCREASE);
    }
    else
    {
        currentMenuItem->display(transition);
    }
}

//------------------------------------------------------------------------------
void uiMenuItemPrevious(void)
{
    lastMenuItem = currentMenuItem;

    curChildIndex--;

    if (curChildIndex < 0)
    {
        curChildIndex = currentMenuItem->parent->childCount - 1;
    }

    currentMenuItem = currentMenuItem->parent->children[curChildIndex];

    if (!IS_NULL_POINTER(currentMenuItem->edit))
    {
        currentMenuItem->edit(UI_MENU_ITEM_EDIT_DECREASE);
    }
    else
    {
        currentMenuItem->display(UI_MENU_ITEM_TRANSITION_PREV_SIBLING);
    }
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static void procedure(void)
{
    int i, j, k;
    uint32_t timeMs = systemTimeMsGet();

    for (i = 0; i < currentStateId->channelMap.size; i++)
    {
        ui_input_event_map_t* channelEventMap =
            (currentStateId->channelMap.entries[i].eventMap);
        ui_input_channel_id_t* channelId =
            currentStateId->channelMap.entries[i].channelId;
        ui_input_channel_info_t* channelInfo =
            (ui_input_channel_info_t*) channelId->private;

        if (channelInfo->isEnabled)
        {
            bool isActive = false;

            for (j = 0; j < channelId->sourceList.size; j++)
            {
                ui_input_id_t* inputId = &(channelId->sourceList.sources[j]);
//                ui_input_info_t* inputInfo = (ui_input_info_t*) inputId.private;

                // Check if input is currently active
                if (!(inputId->isActive()))
                {
                    isActive = false;

                    break;
                }
            }

            // If we made it through all of the sources without finding an
            // inactive one, the channel is active
            if (j == channelId->sourceList.size)
            {
                isActive = true;
            }

            if (isActive != channelInfo->stateLog.entries[0].isActive)
            {
                // Push all old events back one slot
                for (j = channelInfo->stateLog.size - 1; j > 0; j--)
                {
                    channelInfo->stateLog.entries[j] =
                        channelInfo->stateLog.entries[j - 1];
                }

                ui_input_state_t state =
                {
                    .timeMs = timeMs,
                    .isActive = isActive
                };

                channelInfo->stateLog.entries[0] = state;

                for (j = 0; j < channelEventMap->size; j++)
                {
                    channelEventMap->entries[j].eventsSinceHandled++;
                }
            }

            // Attempt to match patterns in state log
            for (j = 0; j < channelEventMap->size; j++)
            {
                ui_input_event_map_entry_t* eventMapEntry =
                    &(channelEventMap->entries[j]);
                const ui_input_sequence_t* sequence =
                    &(eventMapEntry->eventId->sequence);
                ui_input_state_log_t* stateLog = &(channelInfo->stateLog);

                if (eventMapEntry->isEnabled &&
                    (eventMapEntry->eventsSinceHandled >= sequence->size))
                {
                    int logIndex = 0;

                    for (k = sequence->size - 1; k >= 0; k--)
                    {
                        // If levels don't match, exit loop
                        if (sequence->states[k].isActive !=
                                stateLog->entries[logIndex].isActive)
                        {
                            break;
                        }

                        // Most recent event
                        if (logIndex == 0)
                        {
                            // If event hasn't been active for minimum time,
                            // exit the loop (the most recent event is
                            // compared against system time)
                            if ((stateLog->entries[logIndex].timeMs +
                                    sequence->states[k].minDurationMs) >
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
                            if (((stateLog->entries[logIndex].timeMs +
                                     sequence->states[k].minDurationMs) >
                                  stateLog->entries[logIndex - 1].timeMs)   ||
                                 (((stateLog->entries[logIndex].timeMs +
                                        sequence->states[k].maxDurationMs) <=
                                    stateLog->entries[logIndex - 1].timeMs) &&
                                 (sequence->states[k].maxDurationMs != 0)))
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
                        ui_input_event_t event =
                        {
                            .eventId    = eventMapEntry->eventId,
                            .channelId  = channelId,
                            .timeMs     = timeMs
                        };

                        channelId->handler(&event);
                        eventMapEntry->eventsSinceHandled = 0;
                    }
                }
            }
        }
    }
}
