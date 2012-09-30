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
    ui_input_event_id_e id;
    bool isEnabled;
    int eventsSinceHandled;
} ui_input_event_map_entry_t;

/**
 * TODO Comment!
 */
typedef struct _ui_input_channel_container_t_
{
    ui_input_channel_driver_t driver;
    bool isEnabled;
    ui_input_event_map_entry_t eventMap[UI_INPUT_EVENT_ID_COUNT];
    ui_input_state_t stateLog[UI_INPUT_STATE_LOG_COUNT];
    ui_input_channel_event_handler_f* handler;
} ui_input_channel_container_t;

/*------------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/

/**
 * TODO Comment!
 */
static ui_input_driver_t inputDrivers[UI_INPUT_COUNT];

/**
 * TODO Comment!
 */
static ui_input_event_driver_t eventDrivers[UI_INPUT_EVENT_ID_COUNT];

/**
 * TODO Comment!
 */
static ui_input_channel_container_t
    channelContainers[UI_INPUT_CHANNEL_ID_COUNT];

/**
 * TODO Comment!
 */
static ui_input_behavior_map_t* behaviorMap;

/**
 * TODO Comment!
 */
static ui_input_behavior_id_e behaviorId;

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
 * TODO Comment!
 */
static bool uiAddInputDriver(ui_input_driver_t* driver);

/**
 * TODO Comment!
 */
static bool uiAddInputEventDriver(ui_input_event_driver_t* driver);

/**
 * TODO Comment!
 */
static bool uiAddInputChannelDriver(ui_input_channel_driver_t* driver);

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
        inputDrivers[i].id          = (ui_input_id_e) i;
        inputDrivers[i].isEnabled   = false;
        inputDrivers[i].isActive    = NULL_POINTER;
    }

    for (i = 0; i < UI_INPUT_EVENT_ID_COUNT; i++)
    {
        eventDrivers[i].id               = (ui_input_event_id_e) i;
        eventDrivers[i].sequence.states  = NULL_POINTER;
    }

    for (i = 0; i < UI_INPUT_CHANNEL_COUNT; i++)
    {
        channelContainers[i].driver.id                  =
            (ui_input_channel_id_e) i;
        channelContainers[i].driver.sourceList.sources  = NULL_POINTER;
        channelContainers[i].driver.sourceList.size     = 0;
        channelContainers[i].isEnabled                  = false;

        for (j = 0; j < UI_INPUT_EVENT_ID_COUNT; j++)
        {
            channelContainers[i].eventMap[j].id =
                (ui_input_event_id_e) j;
            channelContainers[i].eventMap[j].isEnabled = false;
            channelContainers[i].eventMap[j].eventsSinceHandled = 0;
        }

        for (j = 0; j < UI_INPUT_STATE_LOG_COUNT; j++)
        {
            channelContainers[i].stateLog[j].timeMs    = 0;
            channelContainers[i].stateLog[j].isActive  = false;
        }

        channelContainers[i].handler = NULL_POINTER;
    }

    system_procedure_driver_t procedures[] =
    {
        // UI procedure
        {
            .id             = SYSTEM_PROCEDURE_UI,
            .callback       = procedure,
            .timeDelayMs    = UI_PROCEDURE_DELAY_MS,
            .priority       = 1
        }
    };

    systemAddProcedureDrivers(procedures, ARRAY_SIZE(procedures));

    uiDriverInit();
}

//------------------------------------------------------------------------------
extern bool uiAddInputDrivers(ui_input_driver_t drivers[], uint8_t size)
{
    int i;

    if (size > UI_INPUT_COUNT)
    {
        return false;
    }

    for (i = 0; i < size; i++)
    {
        if (!uiAddInputDriver(&drivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern bool uiAddInputEventDrivers(ui_input_event_driver_t drivers[], int size)
{
    int i;

    if (size > UI_INPUT_EVENT_ID_COUNT)
    {
        return false;
    }

    for (i = 0; i < size; i++)
    {
        if (!uiAddInputEventDriver(&drivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern bool uiAddInputChannelDrivers(ui_input_channel_driver_t drivers[],
                                     int size)
{
    int i;

    if (size > UI_INPUT_CHANNEL_COUNT)
    {
        return false;
    }

    for (i = 0; i < size; i++)
    {
        if (!uiAddInputChannelDriver(&drivers[i]))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
extern bool uiInputChannelAddEventHandler(ui_input_channel_id_e id,
                                          ui_input_channel_event_handler_f*
                                              handler)
{
    if ((id >= UI_INPUT_CHANNEL_ID_COUNT) ||
        IS_NULL_POINTER(handler))
    {
        return false;
    }

    channelContainers[id].isEnabled = true;
    channelContainers[id].handler   = handler;

    return true;
}

//------------------------------------------------------------------------------
extern ui_input_error_e uiInputIsEnabled(ui_input_id_e id, bool* isEnabled)
{
    if (id >= UI_INPUT_ID_COUNT)
    {
        return UI_INPUT_ERROR_INVALID_ID;
    }

    *isEnabled = inputDrivers[id].isEnabled;

    return UI_INPUT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_input_error_e uiInputIsActive(ui_input_id_e id, bool* isActive)
{
    if (id >= UI_INPUT_ID_COUNT)
    {
        return UI_INPUT_ERROR_INVALID_ID;
    }

    *isActive = inputDrivers[id].isActive();

    return UI_INPUT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern ui_input_error_e uiInputChannelIsActive(ui_input_channel_id_e id,
                                               bool* isActive)
{
    if (id >= UI_INPUT_CHANNEL_ID_COUNT)
    {
        return UI_INPUT_ERROR_INVALID_ID;
    }

    int i;
    *isActive = true;

    for (i = 0; i < channelContainers[id].driver.sourceList.size; i++)
    {
        if (!inputDrivers[channelContainers[id].driver.sourceList.sources[i]].isEnabled)
        {
            channelContainers[id].isEnabled = false;

            break;
        }

        // Check if input is currently active
        if (!inputDrivers[channelContainers[id].driver.sourceList.sources[i]].isActive())
        {
            *isActive = false;

            break;
        }
    }

    return UI_INPUT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern bool uiAddInputBehaviorMap(ui_input_behavior_map_t* map)
{
    if (IS_NULL_POINTER(map))
    {
        return false;
    }

    behaviorMap = map;

    uiInputBehaviorSet((ui_input_behavior_id_e) 0);

    return true;
}

//------------------------------------------------------------------------------
extern ui_input_error_e uiInputBehaviorSet(ui_input_behavior_id_e id)
{
    int i, j;

    if (id >= UI_INPUT_BEHAVIOR_ID_COUNT)
    {
        return UI_INPUT_ERROR_INVALID_ID;
    }

    behaviorId = id;

    for (i = 0; i < UI_INPUT_CHANNEL_COUNT; i++)
    {
        for (j = 0; j < UI_INPUT_EVENT_ID_COUNT; j++)
        {
            channelContainers[i].eventMap[j].isEnabled =
                behaviorMap->map[behaviorId][i][j];
        }
    }

    return UI_INPUT_ERROR_NONE;
}

//------------------------------------------------------------------------------
extern bool uiAddMenu(const ui_menu_item_t* menu)
{
    if (IS_NULL_POINTER(menu))
    {
        return false;
    }

    mainMenuItem = menu;

    return true;
}

//------------------------------------------------------------------------------
extern void uiMenuEnter(void)
{
    display_data_descriptor_t descriptor;
    descriptor.sectionId    = DISPLAY_SECTION_ID_ALL;
    descriptor.alignmentId  = DISPLAY_ALIGNMENT_ID_CENTER;
    descriptor.transitionId = DISPLAY_TRANSITION_ID_NONE;

    displayWriteText(descriptor, DISPLAY_FONT_ID_ARIAL_AUTO, "Menu");
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
    display_data_descriptor_t descriptor;
    descriptor.sectionId    = DISPLAY_SECTION_ID_ALL;
    descriptor.alignmentId  = DISPLAY_ALIGNMENT_ID_CENTER;
    descriptor.transitionId = UI_DISPLAY_TRANSITION_ID_PARENT;

    displayWriteText(descriptor, DISPLAY_FONT_ID_ARIAL_AUTO, "Menu");
    systemDelayMs(500);
    systemStateTransition(SYSTEM_STATE_ID_ON);
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

            return;
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
    display_data_descriptor_t descriptor;
    descriptor.sectionId = DISPLAY_SECTION_ID_ALL;
    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;

    // Default transition is sibling --> next sibling
    descriptor.transitionId = UI_DISPLAY_TRANSITION_ID_NEXT_SIBLING;

    if (IS_NULL_POINTER(lastMenuItem))
    {
        descriptor.transitionId = UI_DISPLAY_TRANSITION_ID_CHILD;
    }
    // If last menu item was current item's parent, perform parent --> child
    // transition
    else if (lastMenuItem == curMenuItem->parent)
    {
        descriptor.transitionId = UI_DISPLAY_TRANSITION_ID_CHILD;
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
                descriptor.transitionId =
                    UI_DISPLAY_TRANSITION_ID_PARENT;

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
        curMenuItem->display(descriptor);
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

    display_data_descriptor_t descriptor;
    descriptor.sectionId = DISPLAY_SECTION_ID_ALL;
    descriptor.alignmentId = DISPLAY_ALIGNMENT_ID_CENTER;
    descriptor.transitionId = UI_DISPLAY_TRANSITION_ID_PREV_SIBLING;

    if (!IS_NULL_POINTER(curMenuItem->edit))
    {
        curMenuItem->edit(UI_MENU_ITEM_EDIT_DECREASE);
    }
    else
    {
        curMenuItem->display(descriptor);
    }
}

/*------------------------------------------------------------------------------
 * Local function definitions
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
static bool uiAddInputDriver(ui_input_driver_t* driver)
{
    if (IS_NULL_POINTER(driver)             ||
        (driver->id >= UI_INPUT_ID_COUNT)   ||
        IS_NULL_POINTER(driver->isActive))
    {
        return false;
    }

    ADD_DRIVER(inputDrivers, driver);

    return true;
}

//------------------------------------------------------------------------------
static bool uiAddInputEventDriver(ui_input_event_driver_t* driver)
{
    if (IS_NULL_POINTER(driver)                     ||
        (driver->id >= UI_INPUT_EVENT_ID_COUNT)     ||
        IS_NULL_POINTER(driver->sequence.states)    ||
        (driver->sequence.size == 0))
    {
        return false;
    }

    ADD_DRIVER(eventDrivers, driver);

    return true;
}

//------------------------------------------------------------------------------
static bool uiAddInputChannelDriver(ui_input_channel_driver_t* driver)
{
    int i;

    if (IS_NULL_POINTER(driver)                     ||
        (driver->id >= UI_INPUT_CHANNEL_ID_COUNT)   ||
        IS_NULL_POINTER(driver->sourceList.sources) ||
        (driver->sourceList.size == 0))
    {
        return false;
    }

    ADD_CONTAINER(channelContainers, driver);

    for (i = 0; i < UI_INPUT_EVENT_ID_COUNT; i++)
    {
        channelContainers[driver->id].eventMap[i].isEnabled = true;
    }

    return true;
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
        if (channelContainers[i].isEnabled)
        {
            for (j = 0; j < channelContainers[i].driver.sourceList.size; j++)
            {
                if (!inputDrivers[channelContainers[i].driver.sourceList.sources[j]].isEnabled)
                {
                    channelContainers[i].isEnabled = false;

                    break;
                }

                // Check if input is currently active
                if (!inputDrivers[channelContainers[i].driver.sourceList.sources[j]].isActive())
                {
                    isActive = false;

                    break;
                }
            }

            if (j == channelContainers[i].driver.sourceList.size)
            {
                isActive = true;
            }

            if (isActive != channelContainers[i].stateLog[0].isActive)
            {
                // Push all old events back one slot
                for (j = UI_INPUT_STATE_LOG_COUNT - 1; j > 0; j--)
                {
                    channelContainers[i].stateLog[j] =
                        channelContainers[i].stateLog[j - 1];
                }

                state.timeMs = timeMs;
                state.isActive = isActive;
                channelContainers[i].stateLog[0] = state;

                for (j = 0; j < UI_INPUT_EVENT_ID_COUNT; j++)
                {
                    channelContainers[i].eventMap[j].eventsSinceHandled++;
                }
            }

            // Attempt to match patterns in state log
            for (j = 0; j < UI_INPUT_EVENT_ID_COUNT; j++)
            {
                if (channelContainers[i].eventMap[j].isEnabled &&
                    (channelContainers[i].eventMap[j].eventsSinceHandled >=
                        eventDrivers[j].sequence.size))
                {
                    int logIndex = 0;

                    for (k = eventDrivers[j].sequence.size - 1; k >= 0; k--)
                    {
                        // If levels don't match, exit loop
                        if (eventDrivers[j].sequence.states[k].isActive !=
                                channelContainers[i].stateLog[logIndex].isActive)
                        {
                            break;
                        }

                        // Most recent event
                        if (logIndex == 0)
                        {
                            // If event hasn't been active for minimum time,
                            // exit the loop (the most recent event is
                            // compared against system time)
                            if ((channelContainers[i].stateLog[logIndex].timeMs +
                                    eventDrivers[j].sequence.states[k].minDurationMs) >
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
                            if (((channelContainers[i].stateLog[logIndex].timeMs +
                                     eventDrivers[j].sequence.states[k].minDurationMs) >
                                  channelContainers[i].stateLog[logIndex - 1].timeMs)          ||
                                 (((channelContainers[i].stateLog[logIndex].timeMs +
                                         eventDrivers[j].sequence.states[k].maxDurationMs) <=
                                    channelContainers[i].stateLog[logIndex - 1].timeMs) &&
                                 (eventDrivers[j].sequence.states[k].maxDurationMs != 0)))
                            {
                                break;
                            }
                        }

                        logIndex++;
                    }

                    // If reached last state of event descriptor, the log
                    // pattern matches the event --> call handler
                    if (k == -1)
                    {
                        event.id = (ui_input_event_id_e) j;
                        event.channelId = (ui_input_channel_id_e) i;
                        event.timeMs = timeMs;
                        channelContainers[i].handler(event);
                        channelContainers[i].eventMap[j].eventsSinceHandled = 0;
                    }
                }
            }
        }
    }
}
